/*
* Bigbang project
* Copyright (c) 2009 VPS R&D Group, Samsung Electronics, Inc.
* All rights reserved.
*/

/**
*
* @file		sc900776.c
* @brief	MiniVet interface driver
* @author  	Eung Chan Kim (eungchan.kim@samsung.com)
* @version 	0.1
* @see
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/sysfs.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <asm/mach/irq.h>

#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>
#include <linux/kernel_stat.h>

#include <../arch/arm/mach-mx37/iomux.h>
#include "sc900776.h"

#define SC900776_DEV_NAME	"minivet"
#define SC900776_DEV_NUM	242

#define DBG(f, x...) \
	pr_debug(SC900776_DEV_NAME " [%s()]: " f, __func__, ## x)
#define NOTICE(f, x...) \
	pr_notice(SC900776_DEV_NAME " [%s()]: " f, __func__, ## x)
#define ERR(f, x...) \
	pr_err(SC900776_DEV_NAME " [%s()]: " f, __func__, ## x)
#define INFO(f, x...) \
	pr_info(SC900776_DEV_NAME " [%s()]: " f, __func__, ## x)

#define DET_EXTPWR_GPIO
#if defined(DET_EXTPWR_GPIO)
#define PMU_IS_EXT_PWR() 	mxc_get_gpio_datain(MX37_PIN_SD2_DATA2) /* HIGH : if ext_pwr is connected */
#endif

#if defined(DET_EXTPWR_GPIO)
#define PMU_EXT_PWR_IRQ		IOMUX_TO_IRQ(MX37_PIN_SD2_DATA2)
#else
#define PMU_USW_INT_IRQ		IOMUX_TO_IRQ(MX37_PIN_CSPI2_MISO)
#endif

static DEFINE_MUTEX(minivet_lock);

static u8 minivet_reg[SC900776_REG_MAX]={0, };
static bool is_minivet_init = 0;

static int sc900776_open(struct inode* inode, struct file* filp);
static int sc900776_release(struct inode* inode, struct file* filp);
static int sc900776_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
static int sc900776_detach(struct i2c_client* client);
static int sc900776_probe(struct i2c_adapter* adapter);
static int sc900776_attach(struct i2c_adapter* adapter, int address, int kind);
static int sc900776_read_reg(u8 reg, u8* buf);
static int sc900776_write_reg(u8 reg, u8 val);
static void minivet_refresh(void);
static void minivet_manual_usb(void);
static void minivet_manual_uart(void);


static struct file_operations sc900776_fops =
{
	.owner		= THIS_MODULE,
	.open		= sc900776_open,
	.release	= sc900776_release,
	.ioctl		= sc900776_ioctl,
};

/* this is used by i2c_add_driver */
static struct i2c_driver sc900776_driver =
{
	.driver = {
		.name = SC900776_DEV_NAME, },
	.attach_adapter = sc900776_probe,
	.detach_client = sc900776_detach,
};

static unsigned short normal_i2c[] = {SC900776_I2C_SLAVE_ADDR, I2C_CLIENT_END};	/* var. name MUST be normal_i2c */
I2C_CLIENT_INSMOD;		/* DO NOT MISS THIS after normal_i2c!!! from i2c.h */

static struct i2c_client minivet_i2c;


#if defined(DET_EXTPWR_GPIO)
irqreturn_t
extpwr_irq_handler(int irq, void* dev_id)
{
	if (irq != PMU_EXT_PWR_IRQ)	/* This shouldn't happen */
	{
		INFO("Ext. Pwr IRQ doesn't match!\n");
		return IRQ_NONE;
	}

	return IRQ_HANDLED;
}
#else
static void update_vbus_status(void);
static void uswint_wq_handler(struct work_struct* work);
DECLARE_WORK(uswint_work, uswint_wq_handler);
static int vbus_status = EXT_PWR_UNPLUGGED;

irqreturn_t
uswint_irq_handler(int irq, void* dev_id)
{
	if (irq != PMU_USW_INT_IRQ)	/* This shouldn't happen */
	{
		INFO("Ext. Pwr IRQ doesn't match!\n");
		return IRQ_NONE;
	}

	schedule_work(&uswint_work);
//	schedule_delayed_work(&uswint_work, msecs_to_jiffies(10));

	return IRQ_HANDLED;
}

static void uswint_wq_handler(struct work_struct* work)
{
	update_vbus_status();
//	schedule_delayed_work(&uswint_work, msecs_to_jiffies(3000));
}

static void update_vbus_status(void)
{
	unsigned char val;

	if(!sc900776_read_reg(SC900776_FSL_STATUS, &val))
	{
		if(MINIVET_FSLSTATUS_VBUSDET & val)
			vbus_status = EXT_PWR_PLUGGED;
		else
			vbus_status = EXT_PWR_UNPLUGGED;
	}
	INFO("usw int %d\n", vbus_status);
}
#endif

static int
sc900776_open(struct inode* inode, struct file* filp)
{
	/* do nothing */
	DBG("%s\n", __func__);
	return 0;
}

static int
sc900776_release(struct inode* inode, struct file* filp)
{
	/* do nothing */
	DBG("%s\n", __func__);
	return 0;
}

static int
sc900776_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = -1;
	sMinivet_t sData;

	switch(cmd)
	{
		case IOCTL_MINIVET_INIT:
			minivet_refresh();
			break;

		case IOCTL_MINIVET_WRITE_BYTE:
			if(!copy_from_user((void*)&sData, (const void*)arg, _IOC_SIZE(cmd)))
			{
				if(!sc900776_write_reg(sData.addr, sData.value))
				{
					// NOTICE("Write [%02Xh]:0x%02X\n", sData.addr, sData.value);
					ret = 0;
				}
			}
			break;

		case IOCTL_MINIVET_READ_BYTE:
			if(!copy_from_user((void*)&sData, (const void*)arg, _IOC_SIZE(cmd)))
			{
				if(!sc900776_read_reg(sData.addr, &sData.value))
				{
					// NOTICE("Read [%02Xh]:0x%02X\n", sData.addr, sData.value);
					if(!copy_to_user((void*)arg, &sData, sizeof(sMinivet_t)))
					{
						ret = 0;
					}
				}
			}
			break;

		case IOCTL_MINIVET_DET_VBUS:
		#if defined(DET_EXTPWR_GPIO)
			if (PMU_IS_EXT_PWR() == 0)
			{
				return EXT_PWR_UNPLUGGED;
			}
			else
			{
				return EXT_PWR_PLUGGED;
			}
		#else
			/* 1: yes, 0: no, -1: fail */
			return vbus_status;
		#endif
			break;

		case IOCTL_MINIVET_MANUAL_USB:
			minivet_manual_usb();
			ret = 0;
			break;

		case IOCTL_MINIVET_MANUAL_UART:
			minivet_manual_uart();
			ret = 0;
			break;

	}

	if (ret == -1)
	{
		ERR("failed!\n");
	}

	return ret;
}


static int
sc900776_write_reg(u8 reg, u8 val)
{
	int ret = -1;
	u8 buf[2];

	if(reg >= SC900776_REG_MAX || &minivet_i2c == NULL)
		return -EINVAL;

	buf[0] = reg;
	buf[1] = val;

	mutex_lock(&minivet_lock);
	if(i2c_master_send(&minivet_i2c, buf, 2))
	{
		minivet_reg[reg] = val;
		ret = 0;
	}
	else
	{
		ERR("write to reg R%d failed\n", reg);
	}
	mutex_unlock(&minivet_lock);

	return ret;
}

static int
sc900776_read_reg(u8 reg, u8* buf)
{
	int ret = -1;
	u8 value;
	u8 reg_addr = reg;

	if(reg >= SC900776_REG_MAX || &minivet_i2c == NULL)
		return -EINVAL;

	mutex_lock(&minivet_lock);
	if(i2c_master_send(&minivet_i2c, &reg_addr, 1) > 0)
	{
		if(i2c_master_recv(&minivet_i2c, &value, 1) > 0)
		{
			minivet_reg[reg] = value;
			*buf = value;
			ret = 0;
		}
		else
		{
			ERR("receive failed\n");
		}
	}
	else
	{
		ERR("send failed\n");
	}
	mutex_unlock(&minivet_lock);

	return ret;
}

static int
sc900776_detach(struct i2c_client* client)
{
	int ret;
	DBG("%s\n", __func__);

	ret = i2c_detach_client(client);
	if(ret < 0)
		ERR("detach failed %d\n", ret);

	return ret;
}

static int
sc900776_probe(struct i2c_adapter* adapter)
{
	int ret;
	DBG("%s\n", __func__);

	ret = i2c_probe(adapter, &addr_data, &sc900776_attach);
	if(ret < 0)
		ERR("probe failed -> adapter id[%d] : %d\n", i2c_adapter_id(adapter), ret);

	return ret;
}

static int
sc900776_attach(struct i2c_adapter* adapter, int address, int kind)
{
	int ret;
	DBG("%s\n", __func__);

	if(i2c_adapter_id(adapter) != 0)
	{
		DBG("Different i2c adapter\n");
		return -ENODEV;
	}

	if(address != SC900776_I2C_SLAVE_ADDR)
	{
		DBG("Different i2c address\n");
		return -ENODEV;
	}

	minivet_i2c.addr = address;
	minivet_i2c.adapter = adapter;
	minivet_i2c.driver = &sc900776_driver;
	minivet_i2c.flags = 0;
	strlcpy(minivet_i2c.name, SC900776_DEV_NAME, I2C_NAME_SIZE);

	ret = i2c_attach_client(&minivet_i2c);
	if (ret < 0)
		ERR("i2c attach failed [%d]\n", i2c_adapter_id(adapter));
	else
		NOTICE("i2c attached : adapter[%d] addr[0x%X]\n", i2c_adapter_id(adapter), minivet_i2c.addr);

	return ret;
}

u32 minivet_get_devicetype(void)
{
	u8 dev1, dev2;
	eMinivet_device_t deviceType = DEVICETYPE1_UNDEFINED;

	if(!is_minivet_init)
		return DEVICETYPE1_UNDEFINED;

	sc900776_read_reg(SC900776_DEVICE_TYPE1, &dev1);
	sc900776_read_reg(SC900776_DEVICE_TYPE2, &dev2);

	switch(dev1)
	{
		case MINIVET_DEVICETYPE1_USB:
			deviceType = DEVICETYPE1_USB;
			break;
		case MINIVET_DEVICETYPE1_DEDICATED:
			deviceType = DEVICETYPE1_DEDICATED;
			break;
	}

	switch(dev2)
	{
		case MINIVET_DEVICETYPE2_JIGUARTON:
			deviceType = DEVICETYPE2_JIGUARTON;
			break;
		case MINIVET_DEVICETYPE2_JIGUSBOFF:
			deviceType = DEVICETYPE2_JIGUSBOFF;
			break;
		case MINIVET_DEVICETYPE2_JIGUSBON:
			deviceType = DEVICETYPE2_JIGUSBON;
			break;
	}

	return (u32)deviceType;
}
EXPORT_SYMBOL_GPL(minivet_get_devicetype);

int minivet_is_pbajig(void)
{
	int mode = 0;

	if(!is_minivet_init)
		return -1;

	switch((eMinivet_device_t)minivet_get_devicetype())
	{
		case DEVICETYPE2_JIGUARTON:
		case DEVICETYPE2_JIGUSBON:
			mode = 1;
			break;

		default:
			mode = 0;
			break;
	}

	return mode;
}
EXPORT_SYMBOL_GPL(minivet_is_pbajig);

static void minivet_refresh(void)
{
	//	sc900776_write_reg(SC900776_FSL_CONTROL, 0x24);
	//	msleep(100);
	sc900776_write_reg(SC900776_CONTROL, 0x1E);
	msleep(30);

	is_minivet_init = 1;
}

static void minivet_manual_usb(void)
{
	sc900776_write_reg(SC900776_MANUAL_SWITCH1, 0x25);
	sc900776_write_reg(SC900776_MANUAL_SWITCH2, 0x14);
	sc900776_write_reg(SC900776_CONTROL, 0x1A);
}

static void minivet_manual_uart(void)
{
	sc900776_write_reg(SC900776_MANUAL_SWITCH1, 0x6C);
	sc900776_write_reg(SC900776_MANUAL_SWITCH2, 0x14);
	sc900776_write_reg(SC900776_CONTROL, 0x1A);
}

void minivet_initialize(void)
{
	eSc900776_register_t reg;
	u8 dummy;

	for(reg=SC900776_DEVICE_ID; reg<SC900776_REG_MAX; reg++)
		sc900776_read_reg(reg, &dummy);

#if defined(DET_EXTPWR_GPIO)
	set_irq_type(PMU_EXT_PWR_IRQ, IRQT_RISING);
	if(request_irq(PMU_EXT_PWR_IRQ, extpwr_irq_handler, IRQF_DISABLED, SC900776_DEV_NAME, NULL))
	{
		ERR("minivet: Ext. power connect IRQ Register Failed!\n");
		return;
	}
	set_irq_wake(PMU_EXT_PWR_IRQ, 1);
#else
	update_vbus_status();

//	sc900776_write_reg(SC900776_INTERRUPT1, 0x03);
	mxc_request_iomux(MX37_PIN_CSPI2_MISO, IOMUX_CONFIG_ALT4);
//	mxc_iomux_set_pad(MX37_PIN_CSPI2_MISO, PAD_CTL_SRE_FAST);
//	mxc_iomux_set_pad(MX37_PIN_GRP_H9, PAD_CTL_HYS_ENABLE);
	mxc_set_gpio_direction(MX37_PIN_CSPI2_MISO, 1);

//	set_irq_type(PMU_USW_INT_IRQ, IRQT_RISING);
	set_irq_type(PMU_USW_INT_IRQ, IRQT_FALLING);
	if(request_irq(PMU_USW_INT_IRQ, uswint_irq_handler, IRQF_DISABLED, SC900776_DEV_NAME, NULL))
	{
		ERR("minivet: Ext. power connect IRQ Register Failed!\n");
		return;
	}
	set_irq_wake(PMU_USW_INT_IRQ, 1);

//	schedule_delayed_work(&uswint_work, msecs_to_jiffies(1000));
#endif

	is_minivet_init = 1;
}

static ssize_t minivet_show_registers(struct device *dev, struct device_attribute *attr, char *buf)
{
	int i;
	ssize_t len = 0;

	len += sprintf(buf+len, "\n");
	len += sprintf(buf+len, "       0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");
	len += sprintf(buf+len, "      -----------------------------------------------\n");
	len += sprintf(buf+len, "[%02Xh]:", 0);
	for(i=0; i<SC900776_REG_MAX; i++)
	{
		if(i && !(i%0x10))
			len += sprintf(buf+len, "\n[%02Xh]:", i);
		len += sprintf(buf+len, "%02X ", minivet_reg[i]);
	}
	len += sprintf(buf+len, "\n");

	return len;
}

static DEVICE_ATTR(registers, S_IRUSR | S_IRGRP, minivet_show_registers, NULL);

static ssize_t minivet_show_detvbus(struct device *dev, struct device_attribute *attr, char *buf)
{
	/* 'Y': yes, 'N': no, 'F': fail */
	ssize_t len = 0;
	char ch = 'F';

#if defined(DET_EXTPWR_GPIO)
	if (PMU_IS_EXT_PWR() == 0)
		ch = 'N';
	else
		ch = 'Y';
#else
	if(vbus_status == EXT_PWR_PLUGGED)
		ch = 'Y';
	else
		ch = 'N';
#endif

	len += sprintf(buf+len, "%c\n", ch);

	return len;
}

static DEVICE_ATTR(detvbus, S_IRUSR | S_IRGRP, minivet_show_detvbus, NULL);

static struct attribute *minivet_attributes[] = {
	&dev_attr_registers.attr,
	&dev_attr_detvbus.attr,
	NULL
};

static struct attribute_group minivet_attr_group = {
	.attrs = minivet_attributes
};

static int minivetattr_probe(struct platform_device *pdev)
{
	int ret = 0;

	INFO("minivet platform driver attached\n");
	ret = sysfs_create_group(&pdev->dev.kobj, &minivet_attr_group);
	if( ret < 0 )
	{
		ERR("minivet sysfs create group error\n");
		return -EINVAL;
	}

	return 0;
}

static int minivetattr_remove(struct platform_device *pdev)
{
	INFO("minivet platform driver release\n");
	sysfs_remove_group(&pdev->dev.kobj, &minivet_attr_group);
	return 0;
}

#ifdef CONFIG_PM
static int minivetattr_suspend(struct platform_device *pdev, pm_message_t state)
{
	if (PMU_IS_EXT_PWR() || mxc_get_gpio_datain(MX37_PIN_AUD3_BB_CK)) {
		INFO("ext. power or power key is on, should not go to cpu_stop!\n");
		return  -1;
	}

	return 0;
}
static int minivetattr_resume(struct platform_device *pdev)
{
	return 0;
}
#else
#define	minivetattr_suspend	NULL
#define	minivetattr_resume	NULL
#endif

static void minivetattr_release(struct device *dev)
{
}

static struct platform_driver minivetattr_driver = {
    .probe  = minivetattr_probe,
    .remove = minivetattr_remove,
	.suspend_late = minivetattr_suspend,
	.resume_early = minivetattr_resume,
    .driver		= {
		.name	= SC900776_DEV_NAME,
		.owner  = THIS_MODULE,
	},
};

static struct platform_device minivetattr_device = {
	.name	= SC900776_DEV_NAME,
	.id		= 0,
	.dev	= {
		.release = minivetattr_release,
	},
};


static int
__init sc900776_init(void)
{
	int ret;
	DBG("%s\n", __func__);

	ret = i2c_add_driver(&sc900776_driver);
	if (ret)	/* >0 means error for i2c_add_driver */
	{
		DBG("Can't add i2c driver\n");
		return ret;
	}

	ret = register_chrdev(SC900776_DEV_NUM, SC900776_DEV_NAME, &sc900776_fops);

	if (ret < 0)
	{
		ERR("Can't get major number for minivet driver\n");
		return ret;
	}

	minivet_initialize();

	/* register platform device */
	ret = platform_device_register(&minivetattr_device);
	if(ret)
	{
		ERR("failed to add platform device %s (%d) \n", minivetattr_device.name, ret);
		return ret;
	}

	ret = platform_driver_register(&minivetattr_driver);
	if(ret)
	{
		ERR("failed to add platrom driver %s (%d) \n", minivetattr_driver.driver.name, ret);
		return ret;
	}

	return 0;
}

static void
__exit sc900776_exit(void)
{
	DBG("%s\n", __func__);

    platform_driver_unregister(&minivetattr_driver);
	platform_device_unregister(&minivetattr_device);

#if defined(DET_EXTPWR_GPIO)
	free_irq(IOMUX_TO_IRQ(PMU_EXT_PWR_IRQ), extpwr_irq_handler);
#else
	cancel_work_sync(&uswint_work);
	free_irq(IOMUX_TO_IRQ(PMU_USW_INT_IRQ), uswint_irq_handler);
#endif

#if 0	// removed
	if(!sc900776_write_reg(SC900776_CONTROL, 0x1F))
	{
		INFO("minivet: set INT Mask\n");
	}
#endif

	unregister_chrdev(SC900776_DEV_NUM, SC900776_DEV_NAME);
	i2c_del_driver(&sc900776_driver);
}

module_init(sc900776_init);
module_exit(sc900776_exit);

MODULE_AUTHOR("Eung Chan Kim <eungchan.kim@samsung.com>");
MODULE_DESCRIPTION("minivet interface driver for SC900776");
MODULE_SUPPORTED_DEVICE("minivet");
MODULE_LICENSE("GPL");
