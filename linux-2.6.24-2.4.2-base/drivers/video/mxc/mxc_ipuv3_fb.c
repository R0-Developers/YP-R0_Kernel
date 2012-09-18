/*
 * Copyright 2004-2008 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*!
 * @defgroup Framebuffer Framebuffer Driver for SDC and ADC.
 */

/*!
 * @file mxcfb.c
 *
 * @brief MXC Frame buffer driver for SDC
 *
 * @ingroup Framebuffer
 */

/*!
 * Include files
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/fb.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/dma-mapping.h>
#include <linux/clk.h>
#include <linux/console.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/uaccess.h>
#include <asm/arch/ipu.h>
#include <asm/arch/mxcfb.h>

/*
 * Driver name
 */
#define MXCFB_NAME      "mxc_sdc_fb"
/*!
 * Structure containing the MXC specific framebuffer information.
 */
struct mxcfb_info {
	int blank;
	ipu_channel_t ipu_ch;
	int ipu_di;
	bool overlay;
	uint32_t ipu_ch_irq;
	uint32_t cur_ipu_buf;

	u32 pseudo_palette[16];

	struct semaphore flip_sem;
	struct completion vsync_complete;
};

struct mxcfb_alloc_list {
	struct list_head list;
	dma_addr_t phy_addr;
	void *cpu_addr;
	u32 size;
};

typedef enum {
	BOTH_ON,
	SRC_ON,
	TGT_ON,
	BOTH_OFF
} swap_mode_t;

static char *fb_mode;
#if defined(CONFIG_MACH_MX37_BIGBANG)
static unsigned long default_bpp = 24;
#elif defined(CONFIG_MACH_MX37_APOLLO)
static unsigned long default_bpp = 24;
#else
static unsigned long default_bpp = 16;
#endif
#ifdef CONFIG_FB_MXC_INTERNAL_MEM
static struct clk *iram_clk;
#endif
LIST_HEAD(fb_alloc_list);
static struct fb_info *mxcfb_info[3];

static uint32_t bpp_to_pixfmt(struct fb_info *fbi)
{
	uint32_t pixfmt = 0;

	if (fbi->var.nonstd)
		return fbi->var.nonstd;

	switch (fbi->var.bits_per_pixel) {
	case 18:
		pixfmt = IPU_PIX_FMT_RGB666;
		break;
	case 24:
		pixfmt = IPU_PIX_FMT_BGR24;
		break;
	case 32:
		pixfmt = IPU_PIX_FMT_BGR32;
		break;
	case 16:
		pixfmt = IPU_PIX_FMT_RGB565;
		break;
	}
	return pixfmt;
}

static irqreturn_t mxcfb_irq_handler(int irq, void *dev_id);
static int mxcfb_blank(int blank, struct fb_info *info);
static int mxcfb_map_video_memory(struct fb_info *fbi, bool use_internal_ram);
static int mxcfb_unmap_video_memory(struct fb_info *fbi);

extern void (*lcd_cabc_on)(void);
extern void (*lcd_cabc_off)(void);

/*
 * Set fixed framebuffer parameters based on variable settings.
 *
 * @param       info     framebuffer information pointer
 */
static int mxcfb_set_fix(struct fb_info *info)
{
	struct fb_fix_screeninfo *fix = &info->fix;
	struct fb_var_screeninfo *var = &info->var;

	fix->line_length = var->xres_virtual * var->bits_per_pixel / 8;

	fix->type = FB_TYPE_PACKED_PIXELS;
	fix->accel = FB_ACCEL_NONE;
	fix->visual = FB_VISUAL_TRUECOLOR;
	fix->xpanstep = 1;
	fix->ypanstep = 1;

	return 0;
}

static int _setup_disp_channel1(struct fb_info *fbi)
{
	ipu_channel_params_t params;
	struct mxcfb_info *mxc_fbi = (struct mxcfb_info *)fbi->par;

	memset(&params, 0, sizeof(params));
	params.mem_dp_bg_sync.di = mxc_fbi->ipu_di;

	/*
	 * Assuming interlaced means yuv output,below setting also
	 * valid for mem_dc_sync. FG should have the same vmode as BG.
	 */
	if (mxc_fbi->ipu_ch == MEM_FG_SYNC) {
		struct mxcfb_info *mxc_fbi_tmp;
		int i;

		for (i = 0; i < num_registered_fb; i++) {
			mxc_fbi_tmp = (struct mxcfb_info *)
				(registered_fb[i]->par);
			if (mxc_fbi_tmp->ipu_ch == MEM_BG_SYNC) {
				fbi->var.vmode =
				registered_fb[i]->var.vmode;
				break;
			}
		}
	}
	if (fbi->var.vmode & FB_VMODE_INTERLACED) {
		params.mem_dp_bg_sync.interlaced = true;
		params.mem_dp_bg_sync.out_pixel_fmt =
			IPU_PIX_FMT_YUV444;
	} else {
#if defined(CONFIG_MACH_MX37_BIGBANG)
		params.mem_dp_bg_sync.out_pixel_fmt =
			IPU_PIX_FMT_RGB666;
#elif defined(CONFIG_MACH_MX37_APOLLO)
		params.mem_dp_bg_sync.out_pixel_fmt =
			IPU_PIX_FMT_RGB24;
#else
		params.mem_dp_bg_sync.out_pixel_fmt =
			IPU_PIX_FMT_RGB666;
#endif
	}
	params.mem_dp_bg_sync.in_pixel_fmt = bpp_to_pixfmt(fbi);

	ipu_init_channel(mxc_fbi->ipu_ch, &params);

	return 0;
}

static int _setup_disp_channel2(struct fb_info *fbi)
{
	int retval = 0;
	struct mxcfb_info *mxc_fbi = (struct mxcfb_info *)fbi->par;

	mxc_fbi->cur_ipu_buf = 1;
	sema_init(&mxc_fbi->flip_sem, 1);
	fbi->var.xoffset = fbi->var.yoffset = 0;

	retval = ipu_init_channel_buffer(mxc_fbi->ipu_ch, IPU_INPUT_BUFFER,
					 bpp_to_pixfmt(fbi),
					 fbi->var.xres, fbi->var.yres,
					 fbi->fix.line_length,
					 IPU_ROTATE_NONE,
					 fbi->fix.smem_start +
					 (fbi->fix.line_length * fbi->var.yres),
					 fbi->fix.smem_start,
					 0, 0);
	if (retval) {
		dev_err(fbi->device,
			"ipu_init_channel_buffer error %d\n", retval);
	}

	return retval;
}

static void fill_black_color(struct fb_info *fbi)
{
	int i;

	if (fbi->var.nonstd) {
		switch(fbi->var.nonstd) {
		case IPU_PIX_FMT_UYVY:
			{
			uint16_t *tmp = (uint16_t *)fbi->screen_base;
			for (i=0; i < fbi->fix.smem_len/2; i++, tmp++)
				*tmp = 0x0080;
			break;
			}
		default:
			break;
		}
	} else
		memset(fbi->screen_base, 0, fbi->fix.smem_len);
}

/*
 * Set framebuffer parameters and change the operating mode.
 *
 * @param       info     framebuffer information pointer
 */
static int mxcfb_set_par(struct fb_info *fbi)
{
	int retval = 0;
	bool use_iram = false;
	u32 mem_len;
	ipu_di_signal_cfg_t sig_cfg;
	struct mxcfb_info *mxc_fbi = (struct mxcfb_info *)fbi->par;

	dev_dbg(fbi->device, "Reconfiguring framebuffer\n");

	ipu_disable_irq(mxc_fbi->ipu_ch_irq);
	ipu_disable_channel(mxc_fbi->ipu_ch, true);
	ipu_uninit_channel(mxc_fbi->ipu_ch);
	ipu_clear_irq(mxc_fbi->ipu_ch_irq);
	mxcfb_set_fix(fbi);

	if (fbi->var.yres_virtual == fbi->var.yres)
		fbi->var.yres_virtual = fbi->var.yres * 2;

	mem_len = fbi->var.yres_virtual * fbi->fix.line_length;
	if (mem_len > fbi->fix.smem_len) {
		if (fbi->fix.smem_start)
			mxcfb_unmap_video_memory(fbi);

#ifdef CONFIG_FB_MXC_INTERNAL_MEM
		if (mxc_fbi->ipu_ch == MEM_BG_SYNC)
			use_iram = true;
#endif
		if (mxcfb_map_video_memory(fbi, use_iram) < 0)
			return -ENOMEM;
	}

	if (mxc_fbi->blank != FB_BLANK_UNBLANK) {
		fill_black_color(fbi);
		return retval;
	}

	_setup_disp_channel1(fbi);

	if (!mxc_fbi->overlay) {
		uint32_t out_pixel_fmt;

		memset(&sig_cfg, 0, sizeof(sig_cfg));
		if (fbi->var.vmode & FB_VMODE_INTERLACED) {
			sig_cfg.interlaced = true;
			out_pixel_fmt = IPU_PIX_FMT_YUV444;
		} else
#if defined(CONFIG_MACH_MX37_BIGBANG)
			out_pixel_fmt = IPU_PIX_FMT_RGB666;
#elif defined(CONFIG_MACH_MX37_APOLLO)
			out_pixel_fmt = IPU_PIX_FMT_RGB24;
#else
			out_pixel_fmt = IPU_PIX_FMT_RGB666;
#endif
		if (fbi->var.vmode & FB_VMODE_ODD_FLD_FIRST) /* PAL */
			sig_cfg.odd_field_first = true;
		if (fbi->var.sync & FB_SYNC_EXT)
			sig_cfg.ext_clk = true;
		if (fbi->var.sync & FB_SYNC_HOR_HIGH_ACT)
			sig_cfg.Hsync_pol = true;
		if (fbi->var.sync & FB_SYNC_VERT_HIGH_ACT)
			sig_cfg.Vsync_pol = true;
		if (fbi->var.sync & FB_SYNC_CLK_INVERT)
			sig_cfg.clk_pol = true;
		if (fbi->var.sync & FB_SYNC_DATA_INVERT)
			sig_cfg.data_pol = true;
		if (fbi->var.sync & FB_SYNC_OE_ACT_HIGH)
			sig_cfg.enable_pol = true;
		if (fbi->var.sync & FB_SYNC_CLK_IDLE_EN)
			sig_cfg.clkidle_en = true;

		dev_dbg(fbi->device, "pixclock = %ul Hz\n",
			(u32) (PICOS2KHZ(fbi->var.pixclock) * 1000UL));

		if (ipu_init_sync_panel(mxc_fbi->ipu_di,
					(PICOS2KHZ(fbi->var.pixclock)) * 1000UL,
					fbi->var.xres, fbi->var.yres,
					out_pixel_fmt,
					fbi->var.left_margin,
					fbi->var.hsync_len,
					fbi->var.right_margin,
					fbi->var.upper_margin,
					fbi->var.vsync_len,
					fbi->var.lower_margin,
					320, sig_cfg) != 0) {
			dev_err(fbi->device,
				"mxcfb: Error initializing panel.\n");
			return -EINVAL;
		}

		fbi->mode =
		    (struct fb_videomode *)fb_match_mode(&fbi->var,
							 &fbi->modelist);
		ipu_disp_set_window_pos(mxc_fbi->ipu_ch, 0, 0);
	}

	retval = _setup_disp_channel2(fbi);
	if (retval)
		return retval;

	ipu_enable_channel(mxc_fbi->ipu_ch);

	return retval;
}

static int _swap_channels(struct fb_info *fbi,
	struct fb_info *fbi_to, bool both_on)
{
	int retval, tmp;
	ipu_channel_t old_ch;
	struct mxcfb_info *mxc_fbi_from = (struct mxcfb_info *)fbi->par;
	struct mxcfb_info *mxc_fbi_to = (struct mxcfb_info *)fbi_to->par;

	if (both_on) {
		ipu_disable_channel(mxc_fbi_to->ipu_ch, true);
		ipu_uninit_channel(mxc_fbi_to->ipu_ch);
	}

	/*switch the mxc fbi parameters*/
	old_ch = mxc_fbi_from->ipu_ch;
	mxc_fbi_from->ipu_ch = mxc_fbi_to->ipu_ch;
	mxc_fbi_to->ipu_ch = old_ch;
	tmp = mxc_fbi_from->ipu_ch_irq;
	mxc_fbi_from->ipu_ch_irq = mxc_fbi_to->ipu_ch_irq;
	mxc_fbi_to->ipu_ch_irq = tmp;

	_setup_disp_channel1(fbi);
	retval = _setup_disp_channel2(fbi);
	if (retval)
		return retval;

	/* switch between dp and dc, disable old dmac, enable new dmac*/
	retval = ipu_swap_channel(old_ch, mxc_fbi_from->ipu_ch);
	ipu_uninit_channel(old_ch);

	if (both_on) {
		_setup_disp_channel1(fbi_to);
		retval = _setup_disp_channel2(fbi_to);
		if (retval)
			return retval;
		ipu_enable_channel(mxc_fbi_to->ipu_ch);
	}

	return retval;
}

static int swap_channels(struct fb_info *fbi)
{
	int i;
	swap_mode_t swap_mode;
	ipu_channel_t ch_to;
	struct mxcfb_info *mxc_fbi_from = (struct mxcfb_info *)fbi->par;
	struct fb_info *fbi_to = NULL;
	struct mxcfb_info *mxc_fbi_to;

	/*what's the target channel?*/
	if (mxc_fbi_from->ipu_ch == MEM_BG_SYNC)
		ch_to = MEM_DC_SYNC;
	else
		ch_to = MEM_BG_SYNC;

	for (i = 0; i < num_registered_fb; i++) {
		mxc_fbi_to =
			(struct mxcfb_info *)mxcfb_info[i]->par;
		if (mxc_fbi_to->ipu_ch == ch_to) {
			fbi_to = mxcfb_info[i];
			break;
		}
	}
	if (fbi_to == NULL)
		return -1;

	if (mxc_fbi_from->blank == FB_BLANK_UNBLANK) {
		if (mxc_fbi_to->blank == FB_BLANK_UNBLANK)
			swap_mode = BOTH_ON;
		else
			swap_mode = SRC_ON;
	} else {
		if (mxc_fbi_to->blank == FB_BLANK_UNBLANK)
			swap_mode = TGT_ON;
		else
			swap_mode = BOTH_OFF;
	}

	/* tvout di-1: for DC use UYVY, for DP use RGB*/
	if (mxc_fbi_from->ipu_di == 1 && ch_to == MEM_DC_SYNC) {
		fbi->var.bits_per_pixel = 16;
		fbi->var.nonstd = IPU_PIX_FMT_UYVY;
	} else if (mxc_fbi_from->ipu_di == 1 && ch_to == MEM_BG_SYNC)
		fbi->var.nonstd = 0;
	else if (mxc_fbi_from->ipu_di == 0 && ch_to == MEM_DC_SYNC)
		fbi_to->var.nonstd = 0;
	else if (mxc_fbi_from->ipu_di == 0 && ch_to == MEM_BG_SYNC) {
		fbi->var.bits_per_pixel = 16;
		fbi->var.nonstd = IPU_PIX_FMT_UYVY;
	}

	switch (swap_mode) {
	case BOTH_ON:
		/*disable target->switch src->enable target*/
		/*_swap_channels(fbi, fbi_to, true);//not support well*/
		break;
	case SRC_ON:
		/*just switch src*/
		_swap_channels(fbi, fbi_to, false);
		break;
	case TGT_ON:
		/*just switch target*/
		_swap_channels(fbi_to, fbi, false);
		break;
	case BOTH_OFF:
		/*switch directly, no more need to do*/
		mxc_fbi_to->ipu_ch = mxc_fbi_from->ipu_ch;
		mxc_fbi_from->ipu_ch = ch_to;
		i = mxc_fbi_from->ipu_ch_irq;
		mxc_fbi_from->ipu_ch_irq = mxc_fbi_to->ipu_ch_irq;
		mxc_fbi_to->ipu_ch_irq = i;
		break;
	default:
		break;
	}

	return 0;
}

/*
 * Check framebuffer variable parameters and adjust to valid values.
 *
 * @param       var      framebuffer variable parameters
 *
 * @param       info     framebuffer information pointer
 */
static int mxcfb_check_var(struct fb_var_screeninfo *var, struct fb_info *info)
{
	u32 vtotal;
	u32 htotal;
	struct mxcfb_info *mxc_fbi = (struct mxcfb_info *)info->par;

	if (var->xres_virtual < var->xres)
		var->xres_virtual = var->xres;
	if (var->yres_virtual < var->yres)
		var->yres_virtual = var->yres;

#ifdef CONFIG_FB_MXC_INTERNAL_MEM
	if ((info->fix.smem_start == FB_RAM_BASE_ADDR) &&
	    ((var->yres_virtual * var->xres_virtual * var->bits_per_pixel / 8) >
	     FB_RAM_SIZE))
		return -EINVAL;
#endif

	if ((var->bits_per_pixel != 32) && (var->bits_per_pixel != 24) &&
	    (var->bits_per_pixel != 16))
		var->bits_per_pixel = default_bpp;

	if (mxc_fbi->ipu_ch == MEM_DC_SYNC && mxc_fbi->ipu_di == 1) {
		var->bits_per_pixel = 16;
		var->nonstd = IPU_PIX_FMT_UYVY;
	}

	switch (var->bits_per_pixel) {
	case 16:
		var->red.length = 5;
		var->red.offset = 11;
		var->red.msb_right = 0;

		var->green.length = 6;
		var->green.offset = 5;
		var->green.msb_right = 0;

		var->blue.length = 5;
		var->blue.offset = 0;
		var->blue.msb_right = 0;

		var->transp.length = 0;
		var->transp.offset = 0;
		var->transp.msb_right = 0;
		break;
	case 24:
		var->red.length = 8;
		var->red.offset = 16;
		var->red.msb_right = 0;

		var->green.length = 8;
		var->green.offset = 8;
		var->green.msb_right = 0;

		var->blue.length = 8;
		var->blue.offset = 0;
		var->blue.msb_right = 0;

		var->transp.length = 0;
		var->transp.offset = 0;
		var->transp.msb_right = 0;
		break;
	case 32:
		var->red.length = 8;
		var->red.offset = 16;
		var->red.msb_right = 0;

		var->green.length = 8;
		var->green.offset = 8;
		var->green.msb_right = 0;

		var->blue.length = 8;
		var->blue.offset = 0;
		var->blue.msb_right = 0;

		var->transp.length = 8;
		var->transp.offset = 24;
		var->transp.msb_right = 0;
		break;
	}

	if (var->pixclock < 1000) {
		htotal = var->xres + var->right_margin + var->hsync_len +
		    var->left_margin;
		vtotal = var->yres + var->lower_margin + var->vsync_len +
		    var->upper_margin;
		var->pixclock = (vtotal * htotal * 6UL) / 100UL;
		var->pixclock = KHZ2PICOS(var->pixclock);
		dev_dbg(info->device,
			"pixclock set for 60Hz refresh = %u ps\n",
			var->pixclock);
	}

	var->height = -1;
	var->width = -1;
	var->grayscale = 0;

	return 0;
}

static inline u_int _chan_to_field(u_int chan, struct fb_bitfield *bf)
{
	chan &= 0xffff;
	chan >>= 16 - bf->length;
	return chan << bf->offset;
}

static int mxcfb_setcolreg(u_int regno, u_int red, u_int green, u_int blue,
			   u_int trans, struct fb_info *fbi)
{
	unsigned int val;
	int ret = 1;

	/*
	 * If greyscale is true, then we convert the RGB value
	 * to greyscale no matter what visual we are using.
	 */
	if (fbi->var.grayscale)
		red = green = blue = (19595 * red + 38470 * green +
				      7471 * blue) >> 16;
	switch (fbi->fix.visual) {
	case FB_VISUAL_TRUECOLOR:
		/*
		 * 16-bit True Colour.  We encode the RGB value
		 * according to the RGB bitfield information.
		 */
		if (regno < 16) {
			u32 *pal = fbi->pseudo_palette;

			val = _chan_to_field(red, &fbi->var.red);
			val |= _chan_to_field(green, &fbi->var.green);
			val |= _chan_to_field(blue, &fbi->var.blue);

			pal[regno] = val;
			ret = 0;
		}
		break;

	case FB_VISUAL_STATIC_PSEUDOCOLOR:
	case FB_VISUAL_PSEUDOCOLOR:
		break;
	}

	return ret;
}

/*
 * Function to handle custom ioctls for MXC framebuffer.
 *
 * @param       inode   inode struct
 *
 * @param       file    file struct
 *
 * @param       cmd     Ioctl command to handle
 *
 * @param       arg     User pointer to command arguments
 *
 * @param       fbi     framebuffer information pointer
 */
static int mxcfb_ioctl(struct fb_info *fbi, unsigned int cmd, unsigned long arg)
{
	int retval = 0;
	int __user *argp = (void __user *)arg;
	struct mxcfb_info *mxc_fbi = (struct mxcfb_info *)fbi->par;

	switch (cmd) {
	case MXCFB_SET_GBL_ALPHA:
		{
			struct mxcfb_gbl_alpha ga;
			if (copy_from_user(&ga, (void *)arg, sizeof(ga))) {
				retval = -EFAULT;
				break;
			}
			retval =
			    ipu_disp_set_global_alpha(MEM_BG_SYNC,
						      (bool) ga.enable,
						      ga.alpha);
			dev_dbg(fbi->device, "Set global alpha to %d\n",
				ga.alpha);
			break;
		}
	case MXCFB_SET_CLR_KEY:
		{
			struct mxcfb_color_key key;
			if (copy_from_user(&key, (void *)arg, sizeof(key))) {
				retval = -EFAULT;
				break;
			}
			retval = ipu_disp_set_color_key(MEM_BG_SYNC, key.enable,
							key.color_key);
			dev_dbg(fbi->device, "Set color key to 0x%08X\n",
				key.color_key);
			break;
		}
	case MXCFB_WAIT_FOR_VSYNC:
		{
			if (mxc_fbi->blank != FB_BLANK_UNBLANK)
				break;

			down(&mxc_fbi->flip_sem);
			init_completion(&mxc_fbi->vsync_complete);

			if (ipu_request_irq(mxc_fbi->ipu_ch_irq, mxcfb_irq_handler, 0,
						MXCFB_NAME, fbi) != 0) {
				dev_err(fbi->device,
					"Error registering irq handler for VSYNC.\n");
				retval = -EBUSY;
				break;
			}

			retval = wait_for_completion_interruptible_timeout(
				&mxc_fbi->vsync_complete, 1 * HZ);
			ipu_free_irq(mxc_fbi->ipu_ch_irq, fbi);
			if (retval == 0) {
				dev_err(fbi->device,
					"MXCFB_WAIT_FOR_VSYNC: timeout %d\n",
					retval);
				retval = -ETIME;
			} else if (retval > 0) {
				retval = 0;
			}
			break;
		}
	case FBIO_ALLOC:
		{
			int size;
			struct mxcfb_alloc_list *mem;

			mem = kzalloc(sizeof(*mem), GFP_KERNEL);
			if (mem == NULL)
				return -ENOMEM;

			if (get_user(size, argp))
				return -EFAULT;

			mem->size = PAGE_ALIGN(size);

			mem->cpu_addr = dma_alloc_coherent(fbi->device, size,
							   &mem->phy_addr,
							   GFP_DMA);
			if (mem->cpu_addr == NULL) {
				kfree(mem);
				return -ENOMEM;
			}

			list_add(&mem->list, &fb_alloc_list);

			dev_dbg(fbi->device, "allocated %d bytes @ 0x%08X\n",
				mem->size, mem->phy_addr);

			if (put_user(mem->phy_addr, argp))
				return -EFAULT;

			break;
		}
	case FBIO_FREE:
		{
			unsigned long offset;
			struct mxcfb_alloc_list *mem;

			if (get_user(offset, argp))
				return -EFAULT;

			retval = -EINVAL;
			list_for_each_entry(mem, &fb_alloc_list, list) {
				if (mem->phy_addr == offset) {
					list_del(&mem->list);
					dma_free_coherent(fbi->device,
							  mem->size,
							  mem->cpu_addr,
							  mem->phy_addr);
					kfree(mem);
					retval = 0;
					break;
				}
			}

			break;
		}
	case MXCFB_SET_OVERLAY_POS:
		{
			struct mxcfb_pos pos;
			if (copy_from_user(&pos, (void *)arg, sizeof(pos))) {
				retval = -EFAULT;
				break;
			}
			retval = ipu_disp_set_window_pos(mxc_fbi->ipu_ch,
							 pos.x, pos.y);
			break;
		}
	case MXCFB_GET_FB_IPU_CHAN:
		{
			struct mxcfb_info *mxc_fbi =
				(struct mxcfb_info *)fbi->par;

			if (put_user(mxc_fbi->ipu_ch, argp))
				return -EFAULT;

			break;
		}
	case MXCFB_GET_FB_BLANK_STATE:
		{
			struct mxcfb_info *mxc_fbi =
				(struct mxcfb_info *)fbi->par;

			if (put_user(mxc_fbi->blank, argp))
				return -EFAULT;

			break;
		}
	case MXCFB_CABC_ON:
		{
			lcd_cabc_on();
		}
		break;
	case MXCFB_CABC_OFF:
		{
			lcd_cabc_off();
		}
		break;
	default:
		retval = -EINVAL;
	}
	return retval;
}

/*
 * mxcfb_blank():
 *      Blank the display.
 */
static int mxcfb_blank(int blank, struct fb_info *info)
{
	struct mxcfb_info *mxc_fbi = (struct mxcfb_info *)info->par;

	dev_dbg(info->device, "blank = %d\n", blank);

	if (mxc_fbi->blank == blank)
		return 0;

	mxc_fbi->blank = blank;

	switch (blank) {
	case FB_BLANK_POWERDOWN:
	case FB_BLANK_VSYNC_SUSPEND:
	case FB_BLANK_HSYNC_SUSPEND:
	case FB_BLANK_NORMAL:
		ipu_disable_channel(mxc_fbi->ipu_ch, true);
		ipu_uninit_channel(mxc_fbi->ipu_ch);
		break;
	case FB_BLANK_UNBLANK:
		mxcfb_set_par(info);
		break;
	}
	return 0;
}

/*
 * Pan or Wrap the Display
 *
 * This call looks only at xoffset, yoffset and the FB_VMODE_YWRAP flag
 *
 * @param               var     Variable screen buffer information
 * @param               info    Framebuffer information pointer
 */
static int
mxcfb_pan_display(struct fb_var_screeninfo *var, struct fb_info *info)
{
	struct mxcfb_info *mxc_fbi = (struct mxcfb_info *)info->par;
	u_int y_bottom;
	unsigned long base;

	if (var->xoffset > 0) {
		dev_dbg(info->device, "x panning not supported\n");
		return -EINVAL;
	}

	if ((info->var.xoffset == var->xoffset) &&
	    (info->var.yoffset == var->yoffset))
		return 0;	/* No change, do nothing */

	y_bottom = var->yoffset;

	if (!(var->vmode & FB_VMODE_YWRAP))
		y_bottom += var->yres;

	if (y_bottom > info->var.yres_virtual)
		return -EINVAL;

	base = (var->yoffset * var->xres_virtual + var->xoffset);
	base *= (var->bits_per_pixel) / 8;
	base += info->fix.smem_start;

	dev_dbg(info->device, "Updating SDC BG buf %d address=0x%08lX\n",
		mxc_fbi->cur_ipu_buf, base);

	mxc_fbi->cur_ipu_buf = !mxc_fbi->cur_ipu_buf;
	if (ipu_update_channel_buffer(mxc_fbi->ipu_ch, IPU_INPUT_BUFFER,
				      mxc_fbi->cur_ipu_buf, base) == 0) {
		ipu_select_buffer(mxc_fbi->ipu_ch, IPU_INPUT_BUFFER,
				  mxc_fbi->cur_ipu_buf);
	} else {
		dev_err(info->device,
			"Error updating SDC buf %d to address=0x%08lX\n",
			mxc_fbi->cur_ipu_buf, base);
	}

	dev_dbg(info->device, "Update complete\n");

	info->var.xoffset = var->xoffset;
	info->var.yoffset = var->yoffset;

	if (var->vmode & FB_VMODE_YWRAP)
		info->var.vmode |= FB_VMODE_YWRAP;
	else
		info->var.vmode &= ~FB_VMODE_YWRAP;

	return 0;
}

/*
 * Function to handle custom mmap for MXC framebuffer.
 *
 * @param       fbi     framebuffer information pointer
 *
 * @param       vma     Pointer to vm_area_struct
 */
static int mxcfb_mmap(struct fb_info *fbi, struct vm_area_struct *vma)
{
	bool found = false;
	u32 len;
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
	struct mxcfb_alloc_list *mem;

	if (offset < fbi->fix.smem_len) {
		/* mapping framebuffer memory */
		len = fbi->fix.smem_len - offset;
		vma->vm_pgoff = (fbi->fix.smem_start + offset) >> PAGE_SHIFT;
	} else {
		list_for_each_entry(mem, &fb_alloc_list, list) {
			if (offset == mem->phy_addr) {
				found = true;
				len = mem->size;
				break;
			}
		}
		if (!found)
			return -EINVAL;
	}

	len = PAGE_ALIGN(len);
	if (vma->vm_end - vma->vm_start > len)
		return -EINVAL;

	/* make buffers write-thru cacheable */
	vma->vm_page_prot = __pgprot(pgprot_val(vma->vm_page_prot) &
				     ~L_PTE_BUFFERABLE);

	vma->vm_flags |= VM_IO | VM_RESERVED;

	if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
			    vma->vm_end - vma->vm_start, vma->vm_page_prot)) {
		dev_dbg(fbi->device, "mmap remap_pfn_range failed\n");
		return -ENOBUFS;
	}

	return 0;
}

/*!
 * This structure contains the pointers to the control functions that are
 * invoked by the core framebuffer driver to perform operations like
 * blitting, rectangle filling, copy regions and cursor definition.
 */
static struct fb_ops mxcfb_ops = {
	.owner = THIS_MODULE,
	.fb_set_par = mxcfb_set_par,
	.fb_check_var = mxcfb_check_var,
	.fb_setcolreg = mxcfb_setcolreg,
	.fb_pan_display = mxcfb_pan_display,
	.fb_ioctl = mxcfb_ioctl,
	.fb_mmap = mxcfb_mmap,
	.fb_fillrect = cfb_fillrect,
	.fb_copyarea = cfb_copyarea,
	.fb_imageblit = cfb_imageblit,
	.fb_blank = mxcfb_blank,
};

static irqreturn_t mxcfb_irq_handler(int irq, void *dev_id)
{
	struct fb_info *fbi = dev_id;
	struct mxcfb_info *mxc_fbi = fbi->par;

	complete(&mxc_fbi->vsync_complete);
	up(&mxc_fbi->flip_sem);
	ipu_disable_irq(irq);
	return IRQ_HANDLED;
}

/*
 * Suspends the framebuffer and blanks the screen. Power management support
 */
static int mxcfb_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct fb_info *fbi = platform_get_drvdata(pdev);
	struct mxcfb_info *mxc_fbi = (struct mxcfb_info *)fbi->par;
	int saved_blank;
#ifdef CONFIG_FB_MXC_LOW_PWR_DISPLAY
	void *fbmem;
#endif

	acquire_console_sem();
	fb_set_suspend(fbi, 1);
	saved_blank = mxc_fbi->blank;
	mxcfb_blank(FB_BLANK_POWERDOWN, fbi);
	mxc_fbi->blank = saved_blank;
	release_console_sem();

	return 0;
}

/*
 * Resumes the framebuffer and unblanks the screen. Power management support
 */
static int mxcfb_resume(struct platform_device *pdev)
{
	struct fb_info *fbi = platform_get_drvdata(pdev);
	struct mxcfb_info *mxc_fbi = (struct mxcfb_info *)fbi->par;
	int saved_blank;

	acquire_console_sem();
	saved_blank = mxc_fbi->blank;
	mxc_fbi->blank = FB_BLANK_POWERDOWN;
	mxcfb_blank(saved_blank, fbi);
	fb_set_suspend(fbi, 0);
	release_console_sem();

	return 0;
}

/*
 * Main framebuffer functions
 */

/*!
 * Allocates the DRAM memory for the frame buffer.      This buffer is remapped
 * into a non-cached, non-buffered, memory region to allow palette and pixel
 * writes to occur without flushing the cache.  Once this area is remapped,
 * all virtual memory access to the video memory should occur at the new region.
 *
 * @param       fbi     framebuffer information pointer
 *
 * @param       use_internal_ram flag on whether to use internal RAM for memory
 *
 * @return      Error code indicating success or failure
 */
static int mxcfb_map_video_memory(struct fb_info *fbi, bool use_internal_ram)
{
	int retval = 0;

#ifdef CONFIG_FB_MXC_INTERNAL_MEM
	if (use_internal_ram) {
		fbi->fix.smem_len = FB_RAM_SIZE;
		fbi->fix.smem_start = FB_RAM_BASE_ADDR;
		if (fbi->fix.smem_len <
		    (fbi->var.yres_virtual * fbi->fix.line_length)) {
			dev_err(fbi->device,
				"Not enough internal RAM for fb config\n");
			retval = -EINVAL;
			goto err0;
		}

		if (request_mem_region(fbi->fix.smem_start, fbi->fix.smem_len,
				       fbi->device->driver->name) == NULL) {
			dev_err(fbi->device,
				"Unable to request internal RAM\n");
			retval = -ENOMEM;
			goto err0;
		}

		fbi->screen_base = ioremap(fbi->fix.smem_start,
					   fbi->fix.smem_len);
		if (!fbi->screen_base) {
			dev_err(fbi->device,
				"Unable to map fb memory to virtual address\n");
			release_mem_region(fbi->fix.smem_start,
					   fbi->fix.smem_len);
			retval = -EIO;
			goto err0;
		}

		iram_clk = clk_get(NULL, "iram_clk");
		clk_enable(iram_clk);
	} else
#endif
	{
		fbi->fix.smem_len = fbi->var.yres_virtual *
		    fbi->fix.line_length;
		fbi->screen_base =
		    dma_alloc_writecombine(fbi->device,
					   fbi->fix.smem_len,
					   (dma_addr_t *) &fbi->fix.smem_start,
					   GFP_DMA);

		if (fbi->screen_base == 0) {
			dev_err(fbi->device,
				"Unable to allocate framebuffer memory\n");
			retval = -EBUSY;
			goto err0;
		}
	}

	dev_dbg(fbi->device, "allocated fb @ paddr=0x%08X, size=%d.\n",
		(uint32_t) fbi->fix.smem_start, fbi->fix.smem_len);

	fbi->screen_size = fbi->fix.smem_len;

	/* Clear the screen */
	memset((char *)fbi->screen_base, 0, fbi->fix.smem_len);

	return 0;

err0:
	fbi->fix.smem_len = 0;
	fbi->fix.smem_start = 0;
	fbi->screen_base = NULL;
	return retval;
}

/*!
 * De-allocates the DRAM memory for the frame buffer.
 *
 * @param       fbi     framebuffer information pointer
 *
 * @return      Error code indicating success or failure
 */
static int mxcfb_unmap_video_memory(struct fb_info *fbi)
{
#ifdef CONFIG_FB_MXC_INTERNAL_MEM
	if (fbi->fix.smem_start == FB_RAM_BASE_ADDR) {
		iounmap(fbi->screen_base);
		release_mem_region(fbi->fix.smem_start, fbi->fix.smem_len);
		fbi->fix.smem_start = 0;
		fbi->fix.smem_len = 0;
		clk_disable(iram_clk);
	} else
#endif
	{
		dma_free_writecombine(fbi->device, fbi->fix.smem_len,
				      fbi->screen_base, fbi->fix.smem_start);
	}
	fbi->screen_base = 0;
	fbi->fix.smem_start = 0;
	fbi->fix.smem_len = 0;
	return 0;
}

/*!
 * Initializes the framebuffer information pointer. After allocating
 * sufficient memory for the framebuffer structure, the fields are
 * filled with custom information passed in from the configurable
 * structures.  This includes information such as bits per pixel,
 * color maps, screen width/height and RGBA offsets.
 *
 * @return      Framebuffer structure initialized with our information
 */
static struct fb_info *mxcfb_init_fbinfo(struct device *dev, struct fb_ops *ops)
{
	struct fb_info *fbi;
	struct mxcfb_info *mxcfbi;

	/*
	 * Allocate sufficient memory for the fb structure
	 */
	fbi = framebuffer_alloc(sizeof(struct mxcfb_info), dev);
	if (!fbi)
		return NULL;

	mxcfbi = (struct mxcfb_info *)fbi->par;

	fbi->var.activate = FB_ACTIVATE_NOW;

	fbi->fbops = ops;
	fbi->flags = FBINFO_FLAG_DEFAULT;
	fbi->pseudo_palette = mxcfbi->pseudo_palette;

	/*
	 * Allocate colormap
	 */
	fb_alloc_cmap(&fbi->cmap, 16, 0);

	return fbi;
}

static ssize_t show_disp_chan(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	struct fb_info *info = dev_get_drvdata(dev);
	struct mxcfb_info *mxcfbi = (struct mxcfb_info *)info->par;

	if (mxcfbi->ipu_ch == MEM_BG_SYNC)
		return sprintf(buf, "2-layer-fb-bg\n");
	else if (mxcfbi->ipu_ch == MEM_FG_SYNC)
		return sprintf(buf, "2-layer-fb-fg\n");
	else if (mxcfbi->ipu_ch == MEM_DC_SYNC)
		return sprintf(buf, "1-layer-fb\n");
	else
		return sprintf(buf, "err: no display chan\n");
}

static ssize_t swap_disp_chan(struct device *dev,
		struct device_attribute *attr,
		 const char *buf, size_t count)
{
	struct fb_info *info = dev_get_drvdata(dev);
	struct mxcfb_info *mxcfbi = (struct mxcfb_info *)info->par;
	struct mxcfb_info *fg_mxcfbi = NULL;

	/*swap only happen between DP-BG and DC, while DP-FG disable*/
	if (((mxcfbi->ipu_ch == MEM_BG_SYNC) &&
		(strstr(buf, "1-layer-fb") != NULL)) ||
		((mxcfbi->ipu_ch == MEM_DC_SYNC) &&
		(strstr(buf, "2-layer-fb-bg") != NULL))) {
		int i;

		for (i = 0; i < num_registered_fb; i++) {
			fg_mxcfbi =
				(struct mxcfb_info *)mxcfb_info[i]->par;
			if (fg_mxcfbi->ipu_ch == MEM_FG_SYNC)
				break;
			else
				fg_mxcfbi = NULL;
		}
		if (!fg_mxcfbi ||
			fg_mxcfbi->blank == FB_BLANK_UNBLANK) {
			dev_err(dev,
				"Can not switch whilefb2(fb-fg) is on.\n");
			return count;
		}

		if (swap_channels(info) < 0)
			dev_err(dev, "Swap display channel failed.\n");
	}

	return count;
}
DEVICE_ATTR(fsl_disp_property, 644, show_disp_chan, swap_disp_chan);


/*!
 * Probe routine for the framebuffer driver. It is called during the
 * driver binding process.      The following functions are performed in
 * this routine: Framebuffer initialization, Memory allocation and
 * mapping, Framebuffer registration, IPU initialization.
 *
 * @return      Appropriate error code to the kernel common code
 */
static int mxcfb_probe(struct platform_device *pdev)
{
	struct fb_info *fbi;
	struct mxcfb_info *mxcfbi;
	int ret = 0;

	/*
	 * Initialize FB structures
	 */
	fbi = mxcfb_init_fbinfo(&pdev->dev, &mxcfb_ops);
	if (!fbi) {
		ret = -ENOMEM;
		goto err0;
	}
	mxcfbi = (struct mxcfb_info *)fbi->par;

	if (pdev->id == 0) {
		mxcfbi->ipu_ch_irq = IPU_IRQ_BG_SYNC_EOF;
		mxcfbi->ipu_ch = MEM_BG_SYNC;
		mxcfbi->ipu_di = pdev->id;
		ipu_disp_set_global_alpha(MEM_BG_SYNC, true, 0x80);
		ipu_disp_set_color_key(MEM_BG_SYNC, false, 0);
		mxcfbi->blank = FB_BLANK_UNBLANK;

		strcpy(fbi->fix.id, "DISP3 BG");

		fbi->var.xres = 240;
		fbi->var.yres = 320;
		fbi->var.bits_per_pixel = 18;
	} else if (pdev->id == 1) {
		mxcfbi->ipu_ch_irq = IPU_IRQ_DC_SYNC_EOF;
		mxcfbi->ipu_ch = MEM_DC_SYNC;
		mxcfbi->ipu_di = pdev->id;
		mxcfbi->blank = FB_BLANK_POWERDOWN;

		strcpy(fbi->fix.id, "DISP3 BG - DI1");

		fbi->var.xres = 720;
		fbi->var.yres = 576;
		fbi->var.bits_per_pixel = 16;
	} else if (pdev->id == 2) {	/* Overlay */
		mxcfbi->ipu_ch_irq = IPU_IRQ_FG_SYNC_EOF;
		mxcfbi->ipu_ch = MEM_FG_SYNC;
		mxcfbi->ipu_di = -1;
		mxcfbi->overlay = true;
		mxcfbi->blank = FB_BLANK_POWERDOWN;

		strcpy(fbi->fix.id, "DISP3 FG");

		fbi->var.xres = 720;
		fbi->var.yres = 576;
		fbi->var.bits_per_pixel = 16;
	}

	mxcfb_info[pdev->id] = fbi;

	/* Default Y virtual size is 2x panel size */
#ifndef CONFIG_FB_MXC_INTERNAL_MEM
	fbi->var.xres_virtual = fbi->var.xres;
	fbi->var.yres_virtual = fbi->var.yres * 2;
#endif
	mxcfb_set_fix(fbi);

	/* alocate fb first */
	if (mxcfb_map_video_memory(fbi, 0) < 0)
		return -ENOMEM;

	/* Need dummy values until real panel is configured */
	fbi->var.xres = 240;
	fbi->var.yres = 320;
	fbi->var.bits_per_pixel = 0;

	mxcfb_check_var(&fbi->var, fbi);

	/* Default Y virtual size is 2x panel size */
#ifndef CONFIG_FB_MXC_INTERNAL_MEM
	fbi->var.yres_virtual = fbi->var.yres * 2;
#endif

	mxcfb_set_fix(fbi);

	ret = register_framebuffer(fbi);
	if (ret < 0)
		goto err1;

	platform_set_drvdata(pdev, fbi);

	device_create_file(fbi->dev, &dev_attr_fsl_disp_property);

	dev_err(&pdev->dev, "fb registered, using mode %s\n", fb_mode);
	return 0;

err1:
	fb_dealloc_cmap(&fbi->cmap);
	framebuffer_release(fbi);
err0:
	return ret;
}

static int mxcfb_remove(struct platform_device *pdev)
{
	struct fb_info *fbi = platform_get_drvdata(pdev);
	struct mxcfb_info *mxc_fbi = fbi->par;

	if (!fbi)
		return 0;

	mxcfb_blank(FB_BLANK_POWERDOWN, fbi);
	mxcfb_unmap_video_memory(fbi);

	if (&fbi->cmap)
		fb_dealloc_cmap(&fbi->cmap);

	unregister_framebuffer(fbi);
	framebuffer_release(fbi);
	return 0;
}

/*!
 * This structure contains pointers to the power management callback functions.
 */
static struct platform_driver mxcfb_driver = {
	.driver = {
		   .name = MXCFB_NAME,
		   },
	.probe = mxcfb_probe,
	.remove = mxcfb_remove,
	.suspend = mxcfb_suspend,
	.resume = mxcfb_resume,
};

/*
 * Parse user specified options (`video=trident:')
 * example:
 * 	video=trident:800x600,bpp=16,noaccel
 */
int mxcfb_setup(char *options)
{
	char *opt;
	if (!options || !*options)
		return 0;
	while ((opt = strsep(&options, ",")) != NULL) {
		if (!*opt)
			continue;
		if (!strncmp(opt, "bpp=", 4))
			default_bpp = simple_strtoul(opt + 4, NULL, 0);
		else
			fb_mode = opt;
	}
	return 0;
}

/*!
 * Main entry function for the framebuffer. The function registers the power
 * management callback functions with the kernel and also registers the MXCFB
 * callback functions with the core Linux framebuffer driver \b fbmem.c
 *
 * @return      Error code indicating success or failure
 */
int __init mxcfb_init(void)
{
	int ret = 0;
#ifndef MODULE
	char *option = NULL;
#endif

#ifndef MODULE
	if (fb_get_options("mxcfb", &option))
		return -ENODEV;
	mxcfb_setup(option);
#endif

	ret = platform_driver_register(&mxcfb_driver);
	return ret;
}

void mxcfb_exit(void)
{
	platform_driver_unregister(&mxcfb_driver);
}

module_init(mxcfb_init);
module_exit(mxcfb_exit);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("MXC framebuffer driver");
MODULE_LICENSE("GPL");
MODULE_SUPPORTED_DEVICE("fb");
