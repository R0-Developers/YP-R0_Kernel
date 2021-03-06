/*
 * Copyright 2005-2009 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/fsl_devices.h>
#include <linux/usb/fsl_xcvr.h>

#include <asm/hardware.h>
#include <asm/arch/arc_otg.h>
#include <asm/mach-types.h>

static void usb_utmi_init(struct fsl_xcvr_ops *this)
{
}

static void usb_utmi_uninit(struct fsl_xcvr_ops *this)
{
}

/*!
 * set vbus power
 *
 * @param       view  viewport register
 * @param       on    power on or off
 */
static void set_power(struct fsl_xcvr_ops *this,
		      struct fsl_usb2_platform_data *pdata, int on)
{
	struct device *dev = &pdata->pdev->dev;
	struct regulator *usbotg_regux;

	pr_debug("real %s(on=%d) pdata=0x%p\n", __func__, on, pdata);

	if (UOG_HCSPARAMS & HCSPARAMS_PPC)
		UOG_PORTSC1 |= PORTSC_PORT_POWER;

	if (machine_is_mx37_3ds()) {
		usbotg_regux = regulator_get(dev, "DCDC2");
		if (on) {
			regulator_enable(usbotg_regux);
		} else {
			regulator_disable(usbotg_regux);
		}
		regulator_put(usbotg_regux, dev);
	}
}

static struct fsl_xcvr_ops utmi_ops = {
	.name = "utmi",
	.xcvr_type = PORTSC_PTS_UTMI,
	.init = usb_utmi_init,
	.uninit = usb_utmi_uninit,
	.set_vbus_power = set_power,
};

extern void fsl_usb_xcvr_register(struct fsl_xcvr_ops *xcvr_ops);

static int __init utmixc_init(void)
{
	fsl_usb_xcvr_register(&utmi_ops);
	return 0;
}

extern void fsl_usb_xcvr_unregister(struct fsl_xcvr_ops *xcvr_ops);

static void __exit utmixc_exit(void)
{
	fsl_usb_xcvr_unregister(&utmi_ops);
}

module_init(utmixc_init);
module_exit(utmixc_exit);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("utmi xcvr driver");
MODULE_LICENSE("GPL");
