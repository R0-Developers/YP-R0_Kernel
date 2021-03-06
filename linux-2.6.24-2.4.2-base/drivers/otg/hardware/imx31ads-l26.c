/*
 * Copyright 2005-2007 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
/*
 * otg/hardware/mx31-l26.c - iMX31ADS EVB OTG Peripheral and OTG Controller Drivers Module Initialization
 * @(#) balden@belcarra.com/seth2.rillanon.org|otg/platform/imx31ads/imx31ads-l26.c|20070909224442|14233
 *
 *      Copyright (c) 2004-2005 Belcarra Technologies Corp
 *      Copyright (c) 2005-2007 Belcarra Technologies 2005 Corp
 *
 * By:
 *      Stuart Lynne <sl@belcarra.com>,
 *      Bruce Balden <balden@belcarra.com>
 */
/*!
 * @defgroup MX31 High Speed
 * @ingroup Hardware
 */
/*!
 * @file otg/hardware/imx31ads-l26.c
 * @brief MX31 USB Host Controller Driver
 *
 * Linux MX31 OTG PCD/OCD/TCD Driver Initialization
 *
 * This file selects and initializes all of the low level hardware drivers
 * for the MX31 High Speed.
 *
 * Notes.
 *
 *
 * @ingroup MX31
 * @ingroup MXC
 * @ingroup LINUXOS
 */

#include <otg/pcd-include.h>
#include <otg/otg-dev.h>
#include <linux/module.h>

#include <linux/pci.h>
#include <asm/arch/gpio.h>

//#include "mxc-lnx.h"
//#include "mxc-hardware.h"

MOD_AUTHOR ("sl@belcarra.com");
MOD_DESCRIPTION ("Belcarra MX31");
EMBED_LICENSE();


MOD_PARM_STR(serial_number_str, "Serial Number String", NULL);


/* ************************************************************************************* */

extern int arc_dev_module_init(struct otg_device_driver *otg_device_driver,
                int (*device_probe)(struct device *),
                int (*device_remove)(struct device *)
                );

extern void arc_dev_module_exit(struct otg_device_driver *);

extern int arc_pcd_module_init (struct otg_device_driver *);
extern void arc_pcd_module_exit (struct otg_device_driver *);

//extern int arc_tcd_module_init (struct otg_device_driver *);
//extern void arc_tcd_module_exit (struct otg_device_driver *);

//extern int arc_ocd_module_init (struct otg_device_driver *otg_device_driver);
//extern void arc_ocd_module_exit (struct otg_device_driver *otg_device_driver);

static struct otg_device_driver mx31_otg_device_driver = {
        .name =         "arc_udc",
};



/*! arc_dev_probe - called to initialize platform
 * @param device - device
 *
 * This is used to call the dev level probe functions with
 * the additional otg_device_driver structure.
 */
static int
arc_dev_probe(struct device *device)
{
        printk(KERN_INFO"%s: l26\n", __FUNCTION__);
        RETURN_ZERO_UNLESS (mx31_otg_device_driver.probe);
        return mx31_otg_device_driver.probe(device, &mx31_otg_device_driver);
}

/*! arc_dev_remove- called to remote device
 * @param device - device
 *
 * This is used to call the dev level probe functions with
 * the additional otg_device_driver structure.
 */
static int
arc_dev_remove(struct device *device)
{
        int rc;
        printk(KERN_INFO"%s: l26\n", __FUNCTION__);
        RETURN_ZERO_UNLESS (mx31_otg_device_driver.remove);
        rc =  mx31_otg_device_driver.remove(device, &mx31_otg_device_driver);
        return rc;
}


/*!
 * mx31_modexit() - This is used as module exit, and as cleanup if modinit fails.
 */
static void mx31_modexit (void)
{
        /* unload the dev driver, this will stop otg and destroy
         * the otg_dev and otg instances etc.
         */
        printk(KERN_INFO"%s:\n", __FUNCTION__);
        arc_dev_module_exit(&mx31_otg_device_driver);

        /* cleanup the rest of the sub-drivers
         */
        arc_pcd_module_exit(&mx31_otg_device_driver);
        //arc_tcd_module_exit(&mx31_otg_device_driver);
        //arc_ocd_module_exit(&mx31_otg_device_driver);
}

/*!
 * mx31_modinit() - linux module initialization
 *
 * This needs to initialize the hcd, pcd and tcd drivers. This includes tcd and possibly hcd
 * for some architectures.
 *
 */
static int mx31_modinit (void)
{
        int ocd = -1, tcd = -1, pcd = -1, dev = -1;

        /* initialize all of the sub-drivers except for dev
         */
        //ocd = arc_ocd_module_init(&mx31_otg_device_driver);
        //tcd = arc_tcd_module_init(&mx31_otg_device_driver);
        pcd = arc_pcd_module_init(&mx31_otg_device_driver);

        /* ensure everything is ok until now */
        //THROW_IF(ocd || tcd || pcd, error);
        THROW_IF(pcd, error);

        /* serial number needs to be set prior to initializing the dev
         * driver
         */
        mx31_otg_device_driver.serial_number = MODPARM(serial_number_str);

        /* initialize the dev driver, this will get all sub-drivers
         * started via their probe functions, create otg and otg_dev
         * instances and finally start otg state machine.
         */
        THROW_IF((dev = arc_dev_module_init(&mx31_otg_device_driver, arc_dev_probe, arc_dev_remove)), error);

        return 0;

        CATCH(error) {

                printk(KERN_INFO"%s: FAILED\n", __FUNCTION__);

                UNLESS (dev) arc_dev_module_exit(&mx31_otg_device_driver);
                //UNLESS (tcd) arc_tcd_module_exit(&mx31_otg_device_driver);
                UNLESS (pcd) arc_pcd_module_exit(&mx31_otg_device_driver);
                //UNLESS (ocd) arc_ocd_module_exit(&mx31_otg_device_driver);

                return -EINVAL;
        }
}

module_init (mx31_modinit);
module_exit (mx31_modexit);
