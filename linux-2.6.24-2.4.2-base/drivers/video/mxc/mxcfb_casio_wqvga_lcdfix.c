/*
 * Copyright 2007-2008 Freescale Semiconductor, Inc. All Rights Reserved.
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
 * @file mxcfb_casio_wqvga.c
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
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/fb.h>
#include <linux/init.h>
#include <linux/regulator/regulator.h>
#include <linux/spi/spi.h>
#include <asm/arch/mxcfb.h>
#include <asm/mach-types.h>
#include <asm/arch/gpio.h>

extern void lcd_24bpp_ioinit(void);
extern void casio_display_en(void);
extern void casio_display_disable(void);
static void lcd_poweron(void);
static void lcd_poweroff(void);
extern void spi_write_26_test(struct spi_device *spi);
static void (*lcd_reset) (void);

static struct regulator *io_reg;
static struct regulator *core_reg;
static struct spi_device *lcd_spi;


static struct fb_videomode video_modes[] = {
	{
	 /* 240x400 @ 60 Hz */
	 //"Casio-wqvga", 60, 240, 400, 132057, 2, 4, 4, 5, 10, 2,
	 "Casio-wqvga", 85, 240, 400, 107611, 2, 4, 4, 5, 10, 2,
	 FB_SYNC_CLK_INVERT,
	FB_VMODE_NONINTERLACED,
	 0,},
};


static void lcd_init_fb(struct fb_info *info)
{
	struct fb_var_screeninfo var;

	memset(&var, 0, sizeof(var));

	fb_videomode_to_var(&var, &video_modes[0]);

	if (machine_is_mx31_3ds()) {
		var.upper_margin = 0;
		var.left_margin = 0;
	}

	var.activate = FB_ACTIVATE_ALL;
	var.yres_virtual = var.yres * 2;

	acquire_console_sem();
	info->flags |= FBINFO_MISC_USEREVENT;
	fb_set_var(info, &var);
	info->flags &= ~FBINFO_MISC_USEREVENT;
	release_console_sem();
}

static int lcd_fb_event(struct notifier_block *nb, unsigned long val, void *v)
{
	struct fb_event *event = v;

	if (strcmp(event->info->fix.id, "DISP3 BG")) {
		return 0;
	}

	switch (val) {
	case FB_EVENT_FB_REGISTERED:
		lcd_init_fb(event->info);
		lcd_poweron();
		break;
	case FB_EVENT_BLANK:
		if ((event->info->var.xres != 240) ||
		    (event->info->var.yres != 400)) {
			break;
		}
		if (*((int *)event->data) == FB_BLANK_UNBLANK) {
			lcd_poweron();
		} else {
			lcd_poweroff();
		}
		break;
	}
	return 0;
}

static struct notifier_block nb = {
	.notifier_call = lcd_fb_event,
};


static int lcd_spi_setup(struct spi_device *spi)
{
	spi->mode = SPI_MODE_3;
	spi->bits_per_word = 26;
        return spi_setup(spi);
}


/*!
 * This function is called whenever the SPI slave device is detected.
 *
 * @param	spi	the SPI slave device
 *
 * @return 	Returns 0 on SUCCESS and error on FAILURE.
 */
static int __devinit lcd_spi_probe(struct spi_device *spi)
{
	int i;
	struct mxc_lcd_platform_data *plat = spi->dev.platform_data;
	lcd_spi = spi;
	
	lcd_24bpp_ioinit();
	
	if (plat) {
		io_reg = regulator_get(&spi->dev, plat->io_reg);
		if (!IS_ERR(io_reg)) {
			regulator_set_voltage(io_reg, 1800000);
			regulator_enable(io_reg);
		}
		core_reg = regulator_get(&spi->dev, plat->core_reg);
		if (!IS_ERR(core_reg)) {
			regulator_set_voltage(core_reg, 2800000);
			regulator_enable(core_reg);
		}

		lcd_reset = plat->reset;
		if (lcd_reset)
			lcd_reset();
	}
	
	lcd_spi_setup(lcd_spi);

	for (i = 0; i < num_registered_fb; i++) {
		if (strcmp(registered_fb[i]->fix.id, "DISP3 BG") == 0) {
			lcd_init_fb(registered_fb[i]);
			fb_show_logo(registered_fb[i], 0);
			lcd_poweron(); 
		}
	}

	fb_register_client(&nb);

	return 0;
}

static int __devexit lcd_spi_remove(struct spi_device *spi)
{
	fb_unregister_client(&nb);
	lcd_poweroff();
	regulator_put(io_reg, &spi->dev);
	regulator_put(core_reg, &spi->dev);

	return 0;
}

static int lcd_suspend(struct spi_device *spi, pm_message_t message)
{
	return 0;
}

static int lcd_resume(struct spi_device *spi)
{
	return 0;
}

/*!
 * spi driver structure for Casio-wqvga
 */
static struct spi_driver lcd_spi_dev_driver = {

	.driver = {
		   .name = "lcd_spi",
		   .owner = THIS_MODULE,
		   },
	.probe = lcd_spi_probe,
	.remove = __devexit_p(lcd_spi_remove),
	.suspend = lcd_suspend,
	.resume = lcd_resume,
};


static void spi_write_26(struct spi_device *spi, const u32 *buf, size_t len)
{
	u32 i;
	u32 *temp = (u32*)buf;
	u32 *send_buf;
	u32 *dat = kmalloc(sizeof(u32)*len, GFP_KERNEL);
	send_buf = dat;
	for(i=0; i<len; i++){
		*dat = ((*temp&0xff0000)<<2)|(*temp&0xffff);
		temp++;
		dat++;
	}
	spi_write_32(spi, send_buf, len);
	kfree(send_buf);
}



static void lcd_poweron(void)
{
	
	const u32 cmd1[] ={0x700070, 0x720040};
	const u32 cmd2[] ={
		0x700000, 0x720001,
		0x700001, 0x722433,  // clock falling active
		0x700002, 0x720000,	
		0x700003, 0x721020,
		0x700004, 0x727000,
		0x700005, 0x720010,
		0x700008, 0x720e02,
		0x70000a, 0x72050c,
		0x70000b, 0x720000,
		0x70000c, 0x727770,
		0x70000d, 0x720002,
		0x70000e, 0x720001,
		0x700013, 0x720213,
		0x700012, 0x72000a,
		0x700014, 0x72001e,
		0x700015, 0x723a00,
		0x700010, 0x720000,
		0x700011, 0x720004,
		0x700016, 0x720000,

/*
		0x700028, 0x72A703,
		0x700029, 0x720077,
		0x70002a, 0x728588,
		0x70002b, 0x7200CF,
		0x70002c, 0x72A703,
		0x70002d, 0x720077,
		0x70002e, 0x728588,
		0x70002f, 0x7200CF,
		0x700030, 0x72A703,
		0x700031, 0x720077,
		0x700032, 0x728588,
		0x700033, 0x7200CF,
		0x700034, 0x720660,
		0x700035, 0x720770,
		0x700036, 0x720880,
		0x700037, 0x721600,
		0x700038, 0x72000F,
		0x700039, 0x721604,
		0x70003a, 0x72030F,
		0x70003b, 0x721609,
		0x70003c, 0x72070F,
*/
		0x700028, 0x722c53,
		0x700029, 0x720045,
		0x70002a, 0x72c254,
		0x70002b, 0x720035,
		0x70002c, 0x722b33,
		0x70002d, 0x720046,
		0x70002e, 0x72b264,
		0x70002f, 0x720033,
		0x700030, 0x723a23,
		0x700031, 0x720045,
		0x700032, 0x72a354,
		0x700033, 0x720032,
		0x700034, 0x720440,
		0x700035, 0x720440,
		0x700036, 0x720440,
		0x700037, 0x721603,
		0x700038, 0x72020f,
		0x700039, 0x721602,
		0x70003a, 0x72010f,
		0x70003b, 0x721600,
		0x70003c, 0x72000f,
	};

	const u32 cmd3[] = {0x700012, 0x72002a};
	const u32 cmd4[] = {0x700012, 0x72004a};
	const u32 cmd5[] = {0x700012, 0x72304a};
	const u32 cmd6[] = {0x700012, 0x72314a};
	const u32 cmd7[] = {0x700012, 0x72334a};
	const u32 cmd8[] = {0x700012, 0x72374a};
	const u32 cmd9[] = {0x700012, 0x72376a};

	const u32 cmd10[] = {
		0x700040, 0x720000,
		0x700041, 0x7201b7,
		0x700042, 0x720000,
		0x700043, 0x720000,
		//0x700043, 0x7201b7,
		0x700044, 0x72ef00,
		0x700045, 0x720193,
		0x700046, 0x720004,
		//0x700045, 0x7201a3,
		//0x700046, 0x720014,
		0x700020, 0x720004,
		0x700021, 0x7200ef,
		
	};

	const u32 cmd11[]= {
		0x70000b, 0x720110,
		0x700022, 0x720000,
		0x700007, 0x720012,
	};
	//delay for 40ms or more
	const u32 cmd12[] = {
		
		0x700007, 0x720013,
		0x700022, 0x720000,

	};

	spi_write_26(lcd_spi, cmd1, ARRAY_SIZE(cmd1));
	udelay(10);
	spi_write_26(lcd_spi, cmd2, ARRAY_SIZE(cmd2));
	spi_write_26(lcd_spi, cmd3, ARRAY_SIZE(cmd3));
	msleep(15);
	spi_write_26(lcd_spi, cmd4, ARRAY_SIZE(cmd4));

	msleep(60); 
	spi_write_26(lcd_spi, cmd5, ARRAY_SIZE(cmd5));
	msleep(30);
	spi_write_26(lcd_spi, cmd6, ARRAY_SIZE(cmd6));
	msleep(60);
	spi_write_26(lcd_spi, cmd7, ARRAY_SIZE(cmd7));
	msleep(60);
	spi_write_26(lcd_spi, cmd8, ARRAY_SIZE(cmd8));
	msleep(30);
	spi_write_26(lcd_spi, cmd9, ARRAY_SIZE(cmd9));
	msleep(15);

	spi_write_26(lcd_spi, cmd10, ARRAY_SIZE(cmd10));
	spi_write_26(lcd_spi, cmd11, ARRAY_SIZE(cmd11));
	msleep(60);
	spi_write_26(lcd_spi, cmd12, ARRAY_SIZE(cmd12));  

	casio_display_en();
}
static void lcd_poweroff(void)
{
	const u32 cmd1[] = {
			0x70007, 0x720012,
			0x7000b, 0x720000,
		};
		const u32 cmd2[] = {
			0x70007, 0x720000,
				0x70012, 0x72174a,
		};
		const u32 cmd3[] = {0x70012, 0x72134a,};
		const u32 cmd4[] = {0x70012, 0x72114a,};	
		const u32 cmd5[] = {0x70012, 0x72104a,};	
		const u32 cmd6[] = {0x70012, 0x72004a,};	
		const u32 cmd7[] = {0x70012, 0x72000a,};	
		const u32 cmd8[] = {0x70010, 0x720001,};	
	
		spi_write_26(lcd_spi, cmd1, ARRAY_SIZE(cmd1));
		msleep(60);
		spi_write_26(lcd_spi, cmd2, ARRAY_SIZE(cmd2));
		msleep(30);
		spi_write_26(lcd_spi, cmd3, ARRAY_SIZE(cmd3));
		msleep(60);
		spi_write_26(lcd_spi, cmd4, ARRAY_SIZE(cmd4));
		msleep(60);
		spi_write_26(lcd_spi, cmd5, ARRAY_SIZE(cmd5));
		msleep(30);
		spi_write_26(lcd_spi, cmd6, ARRAY_SIZE(cmd6));
		msleep(10);
		spi_write_26(lcd_spi, cmd7, ARRAY_SIZE(cmd7));
		msleep(5);
		spi_write_26(lcd_spi, cmd8, ARRAY_SIZE(cmd8));
		msleep(100);	
		
		casio_display_disable();
}

static int __init casio_lcd_init(void)
{
	return spi_register_driver(&lcd_spi_dev_driver);
}

static void __exit casio_lcd_exit(void)
{
	spi_unregister_driver(&lcd_spi_dev_driver);
}

module_init(casio_lcd_init);
module_exit(casio_lcd_exit);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("Epson VGA LCD init driver");
MODULE_LICENSE("GPL");


