/*
 * wm1808-i2c.c  --  Audio Driver for Wolfson WM1808
 *
 * Copyright 2008 Freescale Semiconductor
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 *  Revision history
 *    24th Oct 2008   Initial version.
 *
 */
 
#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
 
#include "wm1808.h"
#include "../../../sound/soc/codecs/DrvWM1808.h"

#define WM1808_I2C_ADDR         0x1a

#ifdef DRV_CODEC_DEBUG
#define dbg(format, arg...) \
	printk(AUDIO_NAME ": " format "\n" , ## arg)
#else
#define dbg(format, arg...) do {} while (0)
#endif
#define err(format, arg...) \
	printk(KERN_ERR AUDIO_NAME ": " format "\n" , ## arg)
#define info(format, arg...) \
	printk(KERN_INFO AUDIO_NAME ": " format "\n" , ## arg)
#define warn(format, arg...) \
	printk(KERN_WARNING AUDIO_NAME ": " format "\n" , ## arg)

/*
 * WM1808 Register IO access map
 */
static const int wm1808_reg_io_map[] = WM1808_ACCESS;
static const u16 wm1808_reg_map[] = WM1808_REGISTER_DEFAULTS;

static DEFINE_MUTEX(io_mutex);

extern void wm1808_free(struct wm1808 *wm1808);
extern int wm1808_init(struct wm1808* wm1808);

static int wm1808_i2c_detach(struct i2c_client *client);









static int wm1808_write_i2c_device(struct wm1808 *wm1808, char reg,
		int bytes, char *src)
{
	return i2c_master_send(wm1808->i2c_client, src, bytes);
}

int wm1808_reg_write(struct wm1808 *wm1808, u8 reg, u16 val)
{
	int ret;
	u8 data[2];

	if (reg > WM1808_MAX_REGISTER) {
		printk(KERN_ERR "wm1808: invalid reg %x\n", reg);
		return -EINVAL;
	}

	/* mask in WM1808 write bits */
	val &= wm1808_reg_io_map[reg];
	/* update register cache */
	wm1808->reg_cache[reg] = val;
	
    data[0] = reg << 0x1;
    data[1] = (char)(val >> 8);
    data[1] &= 0x1; 
    data[0] = data[0] | data[1];
    data[1] = (char)(val);

    dbg("[%s]reg=%x data=0x%x%x", __func__,reg, data[0], data[1]);
	mutex_lock(&io_mutex);
	ret = wm1808_write_i2c_device(wm1808, reg, 2, data);
	if (ret < 0)
		printk(KERN_ERR "wm1808: write to reg R%d failed\n", reg);
	mutex_unlock(&io_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(wm1808_reg_write);

u16 wm1808_reg_read(struct wm1808 *wm1808, u8 reg)
{
 	if (reg > WM1808_MAX_REGISTER) {
		printk(KERN_ERR "wm1808: invalid reg %x\n", reg);
		return -EINVAL;
	}

	return wm1808->reg_cache[reg];
}
EXPORT_SYMBOL_GPL(wm1808_reg_read);

/*
 * Safe read, modify, write methods
 */
int wm1808_clear_bits(struct wm1808 *wm1808, u8 reg, u16 mask)
{
	u16 data;
	int ret;

	data = wm1808_reg_read(wm1808, reg);
	data &= ~mask;
	ret = wm1808_reg_write(wm1808, reg, data);

	return ret;
}
EXPORT_SYMBOL_GPL(wm1808_clear_bits);

int wm1808_set_bits(struct wm1808 *wm1808, u8 reg, u16 mask)
{
	u16 data;
	int ret;

	data = wm1808_reg_read(wm1808, reg);
	data |= mask;
	ret = wm1808_reg_write(wm1808, reg, data);
	
	return ret;
}
EXPORT_SYMBOL_GPL(wm1808_set_bits);

int wm1808_create_cache(struct wm1808 *wm1808)
{
	int ret = 0;

	wm1808->reg_cache =
		kzalloc(sizeof(u16) * (WM1808_MAX_REGISTER + 1), GFP_KERNEL);
	if (wm1808->reg_cache == NULL)
		ret = -ENOMEM;

	memcpy(wm1808->reg_cache, wm1808_reg_map, WM1808_MAX_REGISTER * sizeof(u16));

	return ret;
}

static int wm1808_i2c_detach(struct i2c_client *client);

/*
 * WM1808 2 wire address
 */
static unsigned short normal_i2c[] = { WM1808_I2C_ADDR, I2C_CLIENT_END };

/* Magic definition of all other variables and things */
I2C_CLIENT_INSMOD;

static int wm1808_i2c_detect(struct i2c_adapter *adap, int addr, int kind)
{
	struct wm1808* wm1808;
	struct i2c_client *wm1808_i2c;
	int ret = 0;

	if (addr != WM1808_I2C_ADDR)
		return -ENODEV;

	wm1808_i2c = kzalloc(sizeof(struct i2c_client), GFP_KERNEL);
	if (wm1808_i2c == NULL)
		return -ENOMEM;

	wm1808 = kzalloc(sizeof(struct wm1808), GFP_KERNEL);
	if (wm1808 == NULL) {
		kfree(wm1808_i2c);
		return -ENOMEM;
	}
	wm1808_i2c->addr = addr;
	wm1808_i2c->adapter = adap;
	
	mutex_init(&wm1808->work_mutex);
	i2c_set_clientdata(wm1808_i2c, wm1808);
	wm1808->i2c_client = wm1808_i2c;

	ret = i2c_attach_client(wm1808_i2c);
	if (ret < 0) {
		printk(KERN_ERR "wm1808: failed to attach device at addr 0x%x\n", addr);
		goto err;
	}

	ret = wm1808_create_cache(wm1808);
	if (ret < 0) {
		printk(KERN_ERR "wm1808: failed to create register cache\n");
		goto err;
	}

	ret = wm1808_init(wm1808);
	if (ret == 0)
		return ret;

err:
	wm1808_i2c_detach(wm1808_i2c);
	return ret;
}

static int wm1808_i2c_attach(struct i2c_adapter *adap)
{
	return i2c_probe(adap, &addr_data, wm1808_i2c_detect);
}

static int wm1808_i2c_detach(struct i2c_client *client)
{
	struct wm1808 *wm1808 = i2c_get_clientdata(client);

	i2c_detach_client(client);
	kfree(client);
	if (wm1808->reg_cache)
		kfree(wm1808->reg_cache);
	kfree(wm1808);
	
	return 0;
}

#ifdef CONFIG_PM
static int wm1808_i2c_suspend(struct i2c_client *client, pm_message_t state)
{
	int ret = 0;
	return ret;
}

static int wm1808_i2c_resume(struct i2c_client *client)
{
	int ret = 0;
	return ret;
}

#else
#define wm1808_i2c_suspend	NULL
#define wm1808_i2c_resume	NULL
#endif

static struct i2c_driver wm1808_i2c_driver = {
	.driver = {
		.name = "WM1808",
	},
	.attach_adapter = wm1808_i2c_attach,
	.detach_client =  wm1808_i2c_detach,
	.suspend	= wm1808_i2c_suspend,
	.resume		= wm1808_i2c_resume,
	.command =        NULL,
};


static int __init wm1808_i2c_init(void)
{
	return i2c_add_driver(&wm1808_i2c_driver);
}

static void __exit wm1808_i2c_exit(void)
{
	i2c_del_driver(&wm1808_i2c_driver);
}

subsys_initcall(wm1808_i2c_init);
module_exit(wm1808_i2c_exit);

MODULE_AUTHOR("Wang Shaojun");
MODULE_DESCRIPTION("WM1808 I2C Driver");
MODULE_LICENSE("GPL");


