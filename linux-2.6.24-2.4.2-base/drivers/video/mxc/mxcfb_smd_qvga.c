/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*
* Bigbang project
* Copyright (c) 2009 VPS R&D Group, Samsung Electronics, Inc.
* All rights reserved.
*
* This software is the confidential and proprietary information
* of Samsung Electronics, Inc. ("Confidential Information").  You
* shall not disclose such Confidential Information and shall use
* it only in accordance with the terms of the license agreement
* you entered into with Samsung Electronics.
*/

/**
* @file		mxcfb_smd_qvga.c
* @brief	MXC Frame buffer driver for S6D04H0
*
* @author	Eung Chan Kim (eungchan.kim@samsung.com)
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
#if !defined(CONFIG_MACH_MX37_BIGBANG)
#include <linux/regulator/regulator.h>
#endif
#include <linux/spi/spi.h>
#include <asm/arch/mxcfb.h>
#include <asm/mach-types.h>
#include <asm/arch/gpio.h>

#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>
#include <linux/kernel_stat.h>
#include <../arch/arm/mach-mx37/iomux.h>

#define DBG(f, x...) \
	pr_debug(" [%s()]: " f, __func__, ## x)
#define NOTICE(f, x...) \
	pr_notice(" [%s()]: " f, __func__, ## x)
#define ERR(f, x...) \
	pr_err(" [%s()]: " f, __func__, ## x)
#define INFO(f, x...) \
	pr_info(" [%s()]: " f, __func__, ## x)

typedef enum {
	CABC_ON,
	CABC_OFF
} eCtrl_cabc_t;

//extern void afe_backlight_on(void);
//extern void afe_backlight_off(void);
static void lcd_poweron(void);
static void lcd_poweroff(void);
static void sleep_in_seq(void);
static void sleep_out_seq(void);
static int lcd_suspend(struct spi_device *spi, pm_message_t message);
static int lcd_resume(struct spi_device *spi);

static void (*lcd_reset) (void);
void (*lcd_cabc_on)(void);
void (*lcd_cabc_off)(void);

EXPORT_SYMBOL_GPL(lcd_cabc_on);
EXPORT_SYMBOL_GPL(lcd_cabc_off);

#define LCD_ESD_PIN		MX37_PIN_OWIRE_LINE
static int esd_work_resume(void);
static int esd_work_pause(void);
static void esd_workqueue_handler(struct work_struct* work);

DECLARE_DELAYED_WORK(lcd_esd_work, esd_workqueue_handler);
DEFINE_MUTEX(lcd_power_mutex);

#if !defined(CONFIG_MACH_MX37_BIGBANG)
static struct regulator *io_reg;
static struct regulator *core_reg;
#endif
static struct spi_device *lcd_spi;

static int lcdpoweron = -1;  // 0:off, 1:on, -1:boot-on
static long prev_irq_cnt = -3;

static void lcd_poweron_sync(void)
{
	esd_work_pause();	/* for safety only.. */
	mutex_lock(&lcd_power_mutex);

	lcd_poweron();

	mutex_unlock(&lcd_power_mutex);
	esd_work_resume();
}

static void lcd_poweroff_sync(void)
{
	esd_work_pause();
	mutex_lock(&lcd_power_mutex);

	lcd_poweroff();

	mutex_unlock(&lcd_power_mutex);
}

static void esd_workqueue_handler(struct work_struct* work)
{
	long irq_cnt;
	int irq;
	unsigned long flags;
	int interval_ms = 3000;	/* 3 sec duration */

	/* return if PWM is high */
	if (mxc_get_gpio_datain(LCD_ESD_PIN))
	{
		prev_irq_cnt = -3;
		goto ESD_WQ_RESET;
	}

	irq = IOMUX_TO_IRQ(LCD_ESD_PIN);
	spin_lock_irqsave(&irq_desc[irq].lock, flags);
	if (!irq_desc[irq].action)
	{
		spin_unlock_irqrestore(&irq_desc[irq].lock, flags);
		interval_ms = 10;
		goto ESD_WQ_RESET;
	}

	irq_cnt = kstat_cpu(0).irqs[irq];	/* CPU'0' */
	spin_unlock_irqrestore(&irq_desc[irq].lock, flags);

	if (prev_irq_cnt == -3)			/* start check */
	{
//		INFO("lcd_esd : pre-shock! 1\n");
		interval_ms = 200;			/* 200ms */
		prev_irq_cnt = -2;
	}
	else
	{
		if (prev_irq_cnt == -2)		/* irq event didn't occur for 200ms */
		{
//			INFO("lcd_esd : pre-shock! 2\n");
			interval_ms = 200;		/* 200ms */
			prev_irq_cnt = -1;
		}
		else
		{
			if (prev_irq_cnt == -1)		/* irq event didn't occur for 200ms */
			{
				INFO("lcd_esd : shock!\n");
				interval_ms = 1000;		/* 1sec */
				prev_irq_cnt = irq_cnt;
			}
			else
			{
				if (prev_irq_cnt == irq_cnt)	/* irq event didn't occur for 1 sec */
				{
					INFO("lcd_esd : recover!\n");
					mutex_lock(&lcd_power_mutex);
					lcd_poweroff();
					lcd_poweron();
					mutex_unlock(&lcd_power_mutex);
				}

				prev_irq_cnt = -3;
			}
		}
	}

ESD_WQ_RESET:
	schedule_delayed_work(&lcd_esd_work, msecs_to_jiffies(interval_ms));
	return;
}

static int esd_work_pause(void)
{
	cancel_delayed_work_sync(&lcd_esd_work);

	return 0;
}

static int esd_work_resume(void)
{
	prev_irq_cnt = -3;
	schedule_delayed_work(&lcd_esd_work, msecs_to_jiffies(3000));

	return 0;
}

static irqreturn_t esd_irq_handler(int irq, void* dev_id)
{
	return IRQ_HANDLED;
}

static int esd_irq_init(void)
{
	unsigned int irq = IOMUX_TO_IRQ(LCD_ESD_PIN);

	mxc_request_iomux(LCD_ESD_PIN, IOMUX_CONFIG_ALT4);
	mxc_iomux_set_pad(LCD_ESD_PIN, PAD_CTL_DRV_HIGH | PAD_CTL_HYS_ENABLE | PAD_CTL_PKE_NONE | PAD_CTL_SRE_FAST);
	mxc_set_gpio_direction(LCD_ESD_PIN, 1);

	set_irq_type(irq, IRQT_FALLING);

	if (request_irq(irq, esd_irq_handler, IRQF_DISABLED, "lcd_mon", esd_irq_handler))
	{
		ERR("lcd_esd detect irq handler init fail!\n");
		return -1;
	}

	mutex_init(&lcd_power_mutex);

	schedule_delayed_work(&lcd_esd_work, msecs_to_jiffies(3000));

	return 1;
}

static void esd_irq_deinit(void)
{
	cancel_delayed_work_sync(&lcd_esd_work);
	free_irq(IOMUX_TO_IRQ(LCD_ESD_PIN), esd_irq_handler);
}

static struct fb_videomode video_modes[] = {
	{
		"smd-qvga", 	/* LCD Identifier (optional) */
		90, 			/* refresh rate. (optional) */
		240,			/* x-res */
		320,			/* y-res */
		124319, 		/* pixel clock in ps = 10^12/(refr_rate*(left+right+hsync+x-res)*(upper+lower+vsync+y-res)) */
		8,				/* left margin */
		6,				/* right margin */
		6,				/* upper margin */
		6,				/* lower margin */
		12, 			/* hsync len */
		4,				/* vsync len */
		FB_SYNC_CLK_INVERT, 		/* sync */
		FB_VMODE_NONINTERLACED, 	/* vmode */
		0							/* flag */
	},
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
        //Remove Samsung Logo as soon as possible!
	lcd_poweroff_sync();
	lcd_poweron_sync();
}

static int lcd_fb_event(struct notifier_block *nb, unsigned long val, void *v)
{
	struct fb_event *event = v;

	if (strcmp(event->info->fix.id, "DISP3 BG"))
	{
		return 0;
	}

	switch (val)
	{
	case FB_EVENT_FB_REGISTERED:
		DBG("LCD_DEBUG : FB_EVENT_FB_REGISTERED!\n");
		lcd_init_fb(event->info);
		lcd_poweron();
		break;
	case FB_EVENT_BLANK:
		if ((event->info->var.xres != 240) || (event->info->var.yres != 320))
		{
			DBG("LCD_DEBUG : UNMATCHED RESOLUTION!\n");
			break;
		}
		if (*((int *)event->data) == FB_BLANK_UNBLANK)
		{
			DBG("LCD_DEBUG : FB UNBLANK!\n");
			lcd_poweron_sync();
		}
		else
		{
			DBG("LCD_DEBUG : FB BLANK(%d)!\n", *((int*)event->data));
			lcd_poweroff_sync();
		}
		break;
	case FB_EVENT_SUSPEND:
		DBG("LCD_DEBUG : FB SUSPEND!\n");
//		lcd_poweroff();
		break;
	case FB_EVENT_RESUME:
		DBG("LCD_DEBUG : FB RESUME!\n");
//		lcd_poweron();
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
	spi->bits_per_word = 9;
        return spi_setup(spi);
}


/*!
 * This function is called whenever the SPI slave device is detected.
 * @param	spi	the SPI slave device
 * @return 	Returns 0 on SUCCESS and error on FAILURE.
 */
static int __devinit lcd_spi_probe(struct spi_device *spi)
{
	int i;
	struct mxc_lcd_platform_data *plat = spi->dev.platform_data;
	lcd_spi = spi;

	if (plat) {
#if !defined(CONFIG_MACH_MX37_BIGBANG)
		io_reg = regulator_get(&spi->dev, plat->io_reg);
		if (!IS_ERR(io_reg))
		{
			regulator_set_voltage(io_reg, 1800000);
			regulator_enable(io_reg);
		}
		core_reg = regulator_get(&spi->dev, plat->core_reg);
		if (!IS_ERR(core_reg))
		{
			regulator_set_voltage(core_reg, 2800000);
			regulator_enable(core_reg);
		}
#endif
		lcd_reset = plat->reset;
//		if (lcd_reset)
//			lcd_reset();
	}

	lcd_spi_setup(lcd_spi);

	for (i = 0; i < num_registered_fb; i++) {
		if (strcmp(registered_fb[i]->fix.id, "DISP3 BG") == 0)
		{
//			lcd_poweron();
			lcd_init_fb(registered_fb[i]);
			fb_prepare_logo(registered_fb[i], 0);
			fb_show_logo(registered_fb[i], 0);
		}
	}

	fb_register_client(&nb);

	if(esd_irq_init() > 0)
	{
		DBG("lcd_esd_mon Started!\n");
	}
	else
	{
		DBG("lcd_esd_mon Failed!\n");
	}

	return 0;
}

static int __devexit lcd_spi_remove(struct spi_device *spi)
{
	fb_unregister_client(&nb);
	lcd_poweroff_sync();
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	regulator_put(io_reg, &spi->dev);
	regulator_put(core_reg, &spi->dev);
#endif

	esd_irq_deinit();
	return 0;
}

/*!
 * spi driver structure for smd-qvga
 */
#define PARAM_A0_BIT	0x100
static struct spi_driver lcd_spi_dev_driver = {

	.driver = {
		   .name = "lcd_spi",
		   .owner = THIS_MODULE,
		   },
	.probe = lcd_spi_probe,
	.remove = __devexit_p(lcd_spi_remove),
};

/*!
 * 3-wire 9-bit serial interface II
 */
static void spi_write_9bit_if2(struct spi_device *spi, const u8 *buf, size_t len)
{
	size_t i;
	u8 *tmp = (u8 *)buf;
	u16 *sendbuf;
	u16 *dat = kmalloc(sizeof(u16)*len, GFP_KERNEL);
	sendbuf = dat;

	*dat++ = (u16)*tmp++;
	for(i=1; i<len; i++)
	{
		*dat++ = PARAM_A0_BIT|(u16)*tmp++;
	}

	spi_write_16(spi, sendbuf, len);
	kfree(sendbuf);
}

/* 090813, Approval Rev000 */
void cabc_on_seq(void)
{
	const u8 WRCABC[] = {0x55,
							0x03,};

	spi_write_9bit_if2(lcd_spi, WRCABC, ARRAY_SIZE(WRCABC));
	msleep(10);
}

void cabc_off_seq(void)
{
	const u8 WRCABC[] = {0x55,
							0x00,};

	spi_write_9bit_if2(lcd_spi, WRCABC, ARRAY_SIZE(WRCABC));
	msleep(10);
}

static void cabc_seq(eCtrl_cabc_t mode)
{
	const u8 WRDISBV[] = {0x51,
							0xD9,};
	const u8 WRCTRLD[] = {0x53,
							0x2C,};
	const u8 WRCABC[] = {0x55,
							0x00,};
	const u8 WRCABCMB[] = {0x5E,
							0x03,};
	const u8 MIECTL1[] = {0xC0,
							0x80, 0x80, 0x20,};
	const u8 BCMODE[] = {0xC1,
							0x03,};
	const u8 WRBLCTL[] = {0xC3,
							0x7F, 0x3D,};

	spi_write_9bit_if2(lcd_spi, WRDISBV, ARRAY_SIZE(WRDISBV));
	spi_write_9bit_if2(lcd_spi, WRCTRLD, ARRAY_SIZE(WRCTRLD));
	spi_write_9bit_if2(lcd_spi, WRCABC, ARRAY_SIZE(WRCABC));
	spi_write_9bit_if2(lcd_spi, WRCABCMB, ARRAY_SIZE(WRCABCMB));
	spi_write_9bit_if2(lcd_spi, MIECTL1, ARRAY_SIZE(MIECTL1));
	spi_write_9bit_if2(lcd_spi, BCMODE, ARRAY_SIZE(BCMODE));
	spi_write_9bit_if2(lcd_spi, WRBLCTL, ARRAY_SIZE(WRBLCTL));

	if(mode == CABC_ON)
		cabc_on_seq();
	else if(mode == CABC_OFF)
		cabc_off_seq();
}

static void initialize_seq_1(void)
{
	const u8 PASSWD1[] = {0xF0,
							0x5A, 0x5A,};
	const u8 TEON[] = {0x35,};
	const u8 MADCTL[] = {0x36,
		/*	Page address order	: "0" Top -> Bottom,
			Column address order: "1" Right -> Left,
			Page/Column order	: "0" Normal mode,
			Line address order	: "0" Top -> Bottom,
			RGB/BGR order		: "1" BGR
		*/
							0x48,};
	const u8 COLMOD[] = {0x3A,
		/*	RGB 	: 18bit,
			Control : 18bit
		*/
							0x66,};
	const u8 DISCTL[] = {0xF2,
							0x28, 0x60, 0x7F, 0x05, 0x05,
							0x00, 0x00, 0x15, 0x48, 0x00,
							0x07, 0x01,};
	const u8 IFCTL[] = {0xF7,
		/*	GS = 0  : G1 -> G320
			SS = 0  : S1 -> S720
			DM = 01 : RGB interface mode
			RAM_ACCS = 1 : Memory
		*/
							0x00, 0x01, 0x00, 0x80};
	const u8 VCMCTL[] = {0xF5,
							0x00, 0x47, 0x67, 0x00, 0x00,
							0x00,};
	const u8 SRCCTL[] = {0xF6,
							0x01, 0x00, 0x07, 0x00, 0x01,
							0x0C, 0x03, 0x0C, 0x03,};
	const u8 PWRCTL[] = {0xF4,
							0x09, 0x00, 0x00, 0x00, 0x22,
							0x67, 0x04, 0x02, 0x2A, 0x62,
							0x03,};

	sleep_out_seq();

	spi_write_9bit_if2(lcd_spi, PASSWD1, ARRAY_SIZE(PASSWD1));
	spi_write_9bit_if2(lcd_spi, TEON, ARRAY_SIZE(TEON));
	spi_write_9bit_if2(lcd_spi, MADCTL, ARRAY_SIZE(MADCTL));
	spi_write_9bit_if2(lcd_spi, COLMOD, ARRAY_SIZE(COLMOD));

	cabc_seq(CABC_ON);		//CABC_OFF

	spi_write_9bit_if2(lcd_spi, DISCTL, ARRAY_SIZE(DISCTL));
	spi_write_9bit_if2(lcd_spi, IFCTL, ARRAY_SIZE(IFCTL));
	spi_write_9bit_if2(lcd_spi, VCMCTL, ARRAY_SIZE(VCMCTL));
	spi_write_9bit_if2(lcd_spi, SRCCTL, ARRAY_SIZE(SRCCTL));
	spi_write_9bit_if2(lcd_spi, PWRCTL, ARRAY_SIZE(PWRCTL));
}

static void initialize_seq_2(void)
{
	const u8 PASSWD1[] = {0xF0,
							0xA5, 0xA5,};

	spi_write_9bit_if2(lcd_spi, PASSWD1, ARRAY_SIZE(PASSWD1));
}

static void gamma_setting_seq(void)
{
	const u8 GAMMASEL1[] = {0xF9,
							0x04,};
	const u8 GAMMASEL2[] = {0xF9,
							0x02,};
	const u8 GAMMASEL3[] = {0xF9,
							0x01,};

	const u8 PGAMMACTL[] = {0xFA,
							0x0A, 0x08, 0x25, 0x24, 0x23,
							0x24, 0x17, 0x3E, 0x35, 0x1F,
							0x0A, 0x00, 0x00, 0x00, 0x00,
							0x00,};
	const u8 NGAMMACTL[] = {0xFB,
							0x00, 0x00, 0x25, 0x24, 0x23,
							0x24, 0x17, 0x3E, 0x35, 0x1F,
							0x0A, 0x00, 0x00, 0x00, 0x00,
							0x00,};

	spi_write_9bit_if2(lcd_spi, GAMMASEL1, ARRAY_SIZE(GAMMASEL1));
	spi_write_9bit_if2(lcd_spi, PGAMMACTL, ARRAY_SIZE(PGAMMACTL));
	spi_write_9bit_if2(lcd_spi, NGAMMACTL, ARRAY_SIZE(NGAMMACTL));
	spi_write_9bit_if2(lcd_spi, GAMMASEL2, ARRAY_SIZE(GAMMASEL2));
	spi_write_9bit_if2(lcd_spi, PGAMMACTL, ARRAY_SIZE(PGAMMACTL));
	spi_write_9bit_if2(lcd_spi, NGAMMACTL, ARRAY_SIZE(NGAMMACTL));
	spi_write_9bit_if2(lcd_spi, GAMMASEL3, ARRAY_SIZE(GAMMASEL3));
	spi_write_9bit_if2(lcd_spi, PGAMMACTL, ARRAY_SIZE(PGAMMACTL));
	spi_write_9bit_if2(lcd_spi, NGAMMACTL, ARRAY_SIZE(NGAMMACTL));
}

static void display_on_seq(void)
{
 	const u8 DISPON[] = {0x29,};

 	spi_write_9bit_if2(lcd_spi, DISPON, ARRAY_SIZE(DISPON));
	msleep(40);
}

static void display_off_seq(void)
{
 	const u8 DISPOFF[] = {0x28,};

 	spi_write_9bit_if2(lcd_spi, DISPOFF, ARRAY_SIZE(DISPOFF));
	msleep(40);
}

static void sleep_in_seq(void)
{
 	const u8 SLPIN[] = {0x10,};

 	spi_write_9bit_if2(lcd_spi, SLPIN, ARRAY_SIZE(SLPIN));
	msleep(120);
}

static void sleep_out_seq(void)
{
 	const u8 SLPOUT[] = {0x11,};

 	spi_write_9bit_if2(lcd_spi, SLPOUT, ARRAY_SIZE(SLPOUT));
	msleep(120);
}

static void disp_opmode_ifctl_cpu(void)
{
	const u8 IFCTL[] = {0xF7,
						0x00, 0x00, 0x00, 0x80};

 	spi_write_9bit_if2(lcd_spi, IFCTL, ARRAY_SIZE(IFCTL));
}

static void disp_opmode_ifctl_rgb(void)
{
	const u8 IFCTL[] = {0xF7,
						0x00, 0x01, 0x00, 0x80};

 	spi_write_9bit_if2(lcd_spi, IFCTL, ARRAY_SIZE(IFCTL));
}

static int lcd_suspend(struct spi_device *spi, pm_message_t message)
{
#if 0
	display_off_seq();
	sleep_in_seq();
#endif

	return 0;
}

static int lcd_resume(struct spi_device *spi)
{
#if 0
	initialize_seq_1();
	gamma_setting_seq();
	initialize_seq_2();
	display_on_seq();
#endif

	return 0;
}

static void lcd_poweron(void)
{
	if(lcdpoweron < 0)
	{
//		cabc_on_seq();
//		disp_opmode_ifctl_rgb();
		lcdpoweron = 1;
	}
	else if(!lcdpoweron)
	{
		DBG("LCD_DEBUG : lcd_poweron() execute\n");

		//2 power on : vci, vdd3
		msleep(50);
		lcd_reset();
		//2 wait 10ms
		initialize_seq_1();
		gamma_setting_seq();
		initialize_seq_2();
		display_on_seq();

//		afe_backlight_on();
		lcdpoweron = 1;
	}
}

static void lcd_poweroff(void)
{
	if(lcdpoweron)
	{
		DBG("LCD_DEBUG : lcd_poweroff() execute\n");

//		afe_backlight_off();

		display_off_seq();
		disp_opmode_ifctl_cpu();
		msleep(40);
		sleep_in_seq();

		lcdpoweron = 0;
	}
}

static int __init smd_lcd_init(void)
{
	lcd_cabc_on = cabc_on_seq;
	lcd_cabc_off = cabc_off_seq;

#ifdef CONFIG_PM
	lcd_spi_dev_driver.suspend = lcd_suspend;
	lcd_spi_dev_driver.resume = lcd_resume;
#endif

	return spi_register_driver(&lcd_spi_dev_driver);
}

static void __exit smd_lcd_exit(void)
{
	spi_unregister_driver(&lcd_spi_dev_driver);
}

module_init(smd_lcd_init);
module_exit(smd_lcd_exit);


MODULE_AUTHOR("Eung Chan Kim <eungchan.kim@samsung.com>");
MODULE_DESCRIPTION("Samsung mobile display QVGA LCD driver");
MODULE_LICENSE("GPL");
