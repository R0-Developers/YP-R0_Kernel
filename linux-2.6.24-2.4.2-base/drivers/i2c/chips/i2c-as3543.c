/*
 * i2c-as3543.c  --  I2C Driver for AS3543
 *
 */

/*
<b>revision history : </b>
- 2009/02/25 first release, aitdark.park
- 2009/03/02 add modified api, trial
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/rtc.h>

#include "i2c-as3543.h"


#ifdef DRV_CODEC_DEBUG
#define dbg(format, arg...) \
	printk("I2C_AS3543: " format "\n" , ## arg)
#else
#define dbg(format, arg...) do {} while (0)
#endif
#define err(format, arg...) \
	printk(KERN_ERR "I2C_AS3543: " format "\n" , ## arg)
#define info(format, arg...) \
	printk(KERN_INFO "I2C_AS3543: " format "\n" , ## arg)
#define warn(format, arg...) \
	printk(KERN_WARNING "I2C_AS3543: " format "\n" , ## arg)


#define AS3543_SUPPORT_RTC
#define AS3543_SUPPORT_ADC

#define AS3543_I2C_NAME		"AS3543 I2C"
#define AS3543_RTC_NAME		"AS3543 RTC"


static DEFINE_MUTEX(io_mutex);

static struct i2c_client *as3543_i2c;

#ifdef AS3543_SUPPORT_RTC
static struct rtc_device *as3543_rtc;
#endif

u8 as3543_reg[AS3543_REG_MAX] =
{
	0x0,
	0x0,
	0x0,	// AFE_OUT_R        (0x02)
	0x0, 	// AFE_OUT_L        (0x03)
	0x0,
	0x0,
	0x0,	// AFE_MIC_R        (0x06)
	0x0,	// AFE_MIC_L        (0x07)
	0x0,
	0x0,
	0x0,	// AFE_LINE_IN_R    (0x0A)
	0x0,	// AFE_LINE_IN_L    (0x0B)
	0x0,
	0x0,
	0x0,	// AFE_DAC_R        (0x0E)
	0x0,	// AFE_DAC_L        (0x0F)
	0x0,	// AFE_ADC_R        (0x10)
	0x0,	// AFE_ADC_L        (0x11)
	0x0,	// AFE_DAC_IF       (0x12)
	0x0,
	0x0,	// AFE_AUDIOSET1    (0x14)
	0x0,	// AFE_AUDIOSET2    (0x15)
	0x0,	// AFE_AUDIOSET3    (0x16)
	0x0,	// AFE_MUX_PMU_REG1 (0x17)		// PMU Register
	0x0,	// AFE_MUX_PMU_REG2 (0x18)
	0x0,	// AFE_MUX_PMU_REG3 (0x19)
	0x0,	// AFE_MUX_PMU_REG4 (0x1A)
	0x0,	// AFE_MUX_PMU_REG5 (0x1B)
	0x0,	// AFE_PMU_ENABLE   (0x1C)
	0x0,
	0x0,
	0x0,
	0x0,	// AFE_SYSTEM       (0x20)		// System Register
	0x0,	// AFE_SUPERVISOR   (0x21)
	0x0,	// AFE_RAM_WAKE_UP  (0x22)
	0x0,	// AFE_IRQ_ENRD0    (0x23)
	0x0,	// AFE_IRQ_ENRD1    (0x24)
	0x0,	// AFE_IRQ_ENRD2    (0x25)
	0x0,	// AFE_IRQ_ENRD3    (0x26)
	0x0,	// AFE_IRQ_ENRD4    (0x27)
	0x0,	// AFE_RTC_CNTR     (0x28)
	0x0,	// AFE_RTC_TIME     (0x29)
	0x0,	// AFE_RTC_0        (0x2A)
	0x0,	// AFE_RTC_1        (0x2B)
	0x0,	// AFE_RTC_2        (0x2C)
	0x0,	// AFE_RTC_3        (0x2D)
	0x0,	// AFE_ADC_0        (0x2E)
	0x0,	// AFE_ADC_1        (0x2F)
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,
	0x0,	// AFE_UID_0       	(0x38)		// UID Register
	0x0,	// AFE_UID_1       	(0x39)
	0x0,	// AFE_UID_2       	(0x3A)
	0x0,	// AFE_UID_3       	(0x3B)
	0x0,	// AFE_UID_4       	(0x3C)
	0x0,	// AFE_UID_5       	(0x3D)
	0x0,	// AFE_UID_6       	(0x3E)
	0x0		// AFE_UID_7		(0x3F)
};

u8 as3543_subreg[][AS3543_SUBREG_MAX] =
{
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},		// 0x17
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},		// 0x18
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},		// 0x19
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},		// 0x1A
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}		// 0x1B
};

static int as3543_write_i2c_device(char *data, int bytes)
{
	return i2c_master_send(as3543_i2c, data, bytes);
}

int as3543_reg_write(u8 reg, u8 val)
{
	int ret;
	u8 data[2];

	if(reg > AS3543_REG_MAX || as3543_i2c == NULL)
		return -EINVAL;

	data[0] = reg;
	data[1] = val;

	mutex_lock(&io_mutex);
	ret = as3543_write_i2c_device(data, 2);
	mutex_unlock(&io_mutex);

	if(ret <= 0)
		err("i2c-as3543: Write to register[%2X] failed (%s:%d)\n", reg, __FUNCTION__, __LINE__);

	as3543_reg[reg] = val;

	return ret;
}
EXPORT_SYMBOL_GPL(as3543_reg_write);

int as3543_subreg_write(u8 reg, u8 subreg, u8 val)
{
	int ret;
	u8 data[2];

	if(reg > AS3543_REG_MAX || as3543_i2c == NULL || !IS_SUBREG(reg))
		return -EINVAL;

	mutex_lock(&io_mutex);

	data[0] = AFE_PMU_ENABLE;
	data[1] = (1<<3) | subreg;
	ret = as3543_write_i2c_device(data, 2);

	data[0] = reg;
    data[1] = (char)(val);
	ret = as3543_write_i2c_device(data, 2);

	data[0] = AFE_PMU_ENABLE;
	data[1] = subreg;
	ret = as3543_write_i2c_device(data, 2);

	data[0] = AFE_PMU_ENABLE;
	data[1] = (1<<3) | subreg;
	ret = as3543_write_i2c_device(data, 2);

	mutex_unlock(&io_mutex);

	if (ret < 0)
		err("i2c-as3543: Write to register[%2X] failed (%s:%d)\n", reg, __FUNCTION__, __LINE__);

	as3543_subreg[reg - AFE_MUX_PMU_REG1][subreg] = val;

	return ret;
}
EXPORT_SYMBOL_GPL(as3543_subreg_write);

u8 as3543_reg_read(u8 reg)
{
	if(reg > AS3543_REG_MAX)
		return -EINVAL;

	if(IS_SUBREG(reg))
		return as3543_subreg[reg - AFE_MUX_PMU_REG1][0];

	return as3543_reg[reg];
}
EXPORT_SYMBOL_GPL(as3543_reg_read);

u8 as3543_subreg_read(u8 reg, u8 subreg)
{
	if(reg > AS3543_REG_MAX || !IS_SUBREG(reg))
		return -EINVAL;

	return as3543_subreg[reg - AFE_MUX_PMU_REG1][subreg];
}
EXPORT_SYMBOL_GPL(as3543_subreg_read);

u8 as3543_reg_read_from_device(u8 reg)
{
	u8 data[2];

	if(reg > AS3543_REG_MAX || as3543_i2c == NULL)
		return -EINVAL;

	if(IS_SUBREG(reg))
	{
		return as3543_subreg_read_from_device(reg, 0x00);
	}

	data[0] = reg;

	mutex_lock(&io_mutex);

	if(1 == i2c_master_send(as3543_i2c, data, 1))
	{
		if(i2c_master_recv(as3543_i2c, &data[1], 1))
			as3543_reg[reg] = data[1];
		else
			as3543_reg[reg] = 0x00;
	}
	else
		err("i2c-as3543: Read register[%2X] failed (%s:%d)\n", reg, __FUNCTION__, __LINE__);

	mutex_unlock(&io_mutex);

	return as3543_reg[reg];
}
EXPORT_SYMBOL_GPL(as3543_reg_read_from_device);

u8 as3543_subreg_read_from_device(u8 reg, u8 subreg)
{
	int ret = 0;
	u8 data[2];

	if(reg > AS3543_REG_MAX || as3543_i2c == NULL || !IS_SUBREG(reg))
		return -EINVAL;

	mutex_lock(&io_mutex);

	data[0] = AFE_PMU_ENABLE;
	data[1] = (1<<3) | subreg;
	ret = as3543_write_i2c_device(data, 2);

	data[0] = reg;

	if(1 == i2c_master_send(as3543_i2c, data, 1))
	{
		if(i2c_master_recv(as3543_i2c, &data[1], 1))
			as3543_subreg[reg - AFE_MUX_PMU_REG1][subreg] = data[1];
		else
			as3543_subreg[reg - AFE_MUX_PMU_REG1][subreg] = 0x00;
	}
	else
		err("i2c-as3543: Read register[%2X] failed (%s:%d)\n", reg, __FUNCTION__, __LINE__);

	mutex_unlock(&io_mutex);

	return as3543_subreg[reg - AFE_MUX_PMU_REG1][subreg];
}
EXPORT_SYMBOL_GPL(as3543_subreg_read_from_device);

#ifdef ADDED_API_FOR_PMUDRIVER
u8 as3543_read_subreg(u8 reg, u8 subreg)
{
	int ret = -1;
	u8 data[2];

	if(reg > AS3543_REG_MAX || as3543_i2c == NULL || !IS_SUBREG(reg))
		return -EINVAL;

	if((subreg < 1) || (subreg > AFE_SUB_MASK))
	{
		return -EINVAL;
	}

	mutex_lock(&io_mutex);

	data[0] = AFE_PMU_ENABLE;
	if(i2c_master_send(as3543_i2c, data, 1) > 0)
	{
		if(i2c_master_recv(as3543_i2c, &data[1], 1) > 0)
		{
			data[1] = (data[1]&0x08) | subreg;
			if(i2c_master_send(as3543_i2c, data, 2))
			{
				data[0] = reg;
				if(i2c_master_send(as3543_i2c, data, 1) > 0)
				{
					ret = i2c_master_recv(as3543_i2c, &data[1], 1);
					if(ret > 0)
					{
						// success
						as3543_subreg[reg-AFE_MUX_PMU_REG1][subreg-1] = data[1];
					}
				}
			}
		}
	}

	mutex_unlock(&io_mutex);

	if (ret < 0)
		err("read sub-reg R%02X failed\n", reg);

	return as3543_subreg[reg-AFE_MUX_PMU_REG1][subreg-1];
}

u8 as3543_subreg_readbyte_from_device(u8 reg, u8 subreg)
{
	return as3543_read_subreg(reg, subreg);
}

u8 as3543_read_reg(u8 reg)
{
	int ret = -1;
	u8 data[2];

	if(reg > AS3543_REG_MAX || as3543_i2c == NULL)
		return -EINVAL;

	if(IS_SUBREG(reg))
	{
		return as3543_read_subreg(reg, 0x1);
	}

	mutex_lock(&io_mutex);

	data[0] = reg;
	if(1 == i2c_master_send(as3543_i2c, data, 1))
	{
		ret = i2c_master_recv(as3543_i2c, &data[1], 1);

		if(ret > 0)
			as3543_reg[reg] = data[1];
	}

	mutex_unlock(&io_mutex);

	if (ret < 0)
		err("read reg R%02X failed\n", reg);

	return as3543_reg[reg];
}

u8 as3543_reg_readbyte_from_device(u8 reg)
{
	return as3543_read_reg(reg);
}

int as3543_write_subreg(u8 reg, u8 subreg, u8 val)
{
	int ret = -1;
	u8 data[2];
	u8 pmugate = 0;

	if(reg > AS3543_REG_MAX || as3543_i2c == NULL || !IS_SUBREG(reg))
		return -EINVAL;

	if((subreg < 1) || (subreg > AFE_SUB_MASK))
		return -EINVAL;

	mutex_lock(&io_mutex);

	data[0] = AFE_PMU_ENABLE;
	if(i2c_master_send(as3543_i2c, data, 1) > 0)
	{
		if(i2c_master_recv(as3543_i2c, &data[1], 1) > 0)
		{
			pmugate = data[1] & 0x08;
			data[1] = pmugate | subreg;
			if(i2c_master_send(as3543_i2c, data, 2))
			{
				data[0] = reg;
				data[1] = val;
				ret = i2c_master_send(as3543_i2c, data, 2);
				if(ret > 0)
				{
					if(pmugate != 0x08)
					{
						data[0] = AFE_PMU_ENABLE;
						data[1] = subreg | 0x08;
						ret = i2c_master_send(as3543_i2c, data, 2);
					}
				}
			}
		}
	}

	mutex_unlock(&io_mutex);

	if (ret < 0)
		err("write to reg R%02X failed\n", reg);

	as3543_subreg[reg-AFE_MUX_PMU_REG1][subreg-1] = val;

	return ret;
}

int as3543_subreg_writebyte(u8 reg, u8 subreg, u8 val)
{
	return as3543_write_subreg(reg, subreg, val);
}

int as3543_write_reg(u8 reg, u8 val)
{
	int ret;
	u8 data[2];

	if(reg > AS3543_REG_MAX || as3543_i2c == NULL)
		return -EINVAL;

	data[0] = reg;
	data[1] = val;

    dbg("[%s]reg=%x data=0x%x%x", __func__,reg, data[0], data[1]);

	mutex_lock(&io_mutex);
	ret = i2c_master_send(as3543_i2c, data, 2);
	mutex_unlock(&io_mutex);

	if (ret < 0)
		err("i2c-as3543: write to reg R%d failed\n", reg);

	as3543_reg[reg] = val;

	return ret;
}

int as3543_modify_subreg(u8 reg, u8 subreg, u8 mask, u8 value)
{
	u8 mval, val;

	if(reg > AS3543_REG_MAX || as3543_i2c == NULL || !IS_SUBREG(reg))
		return -EINVAL;

	if((subreg < 1) || (subreg > AFE_SUB_MASK))
		return -EINVAL;

	mval = (u8)(value & mask);
	val = (u8)(as3543_subreg[reg-AFE_MUX_PMU_REG1][subreg-1] & ~mask);
	val = (u8)(val | mval);

	return as3543_write_subreg(reg, subreg, val);
}

int as3543_modify_reg(u8 reg, u8 mask, u8 value)
{
	u8 mval, val;

	if(reg > AS3543_REG_MAX || as3543_i2c == NULL)
		return -EINVAL;

	mval = (u8)(value & mask);
	val = (u8)(as3543_reg[reg] & ~mask);
	val = (u8)(val | mval);

	return as3543_write_reg(reg, val);
}

EXPORT_SYMBOL_GPL(as3543_reg);
EXPORT_SYMBOL_GPL(as3543_subreg);
EXPORT_SYMBOL_GPL(as3543_read_reg);
EXPORT_SYMBOL_GPL(as3543_write_reg);
EXPORT_SYMBOL_GPL(as3543_modify_reg);
EXPORT_SYMBOL_GPL(as3543_read_subreg);
EXPORT_SYMBOL_GPL(as3543_write_subreg);
EXPORT_SYMBOL_GPL(as3543_modify_subreg);
EXPORT_SYMBOL_GPL(as3543_subreg_readbyte_from_device);  // as3543_read_subreg
EXPORT_SYMBOL_GPL(as3543_reg_readbyte_from_device);     // as3543_read_reg
EXPORT_SYMBOL_GPL(as3543_subreg_writebyte);             // as3543_write_subreg
#endif

/*
 * Safe read, modify, write methods
 */
int as3543_clear_bits(u8 reg, u8 mask)
{
	u8 data;
	int ret;

	data = as3543_reg_read(reg);
	data &= ~mask;
	ret = as3543_reg_write(reg, data);

	return ret;
}
EXPORT_SYMBOL_GPL(as3543_clear_bits);

int as3543_set_bits(u8 reg, u8 mask)
{
	u8 data;
	int ret;

	data = as3543_reg_read(reg);
	data |= mask;
	ret = as3543_reg_write(reg, data);

	return ret;
}
EXPORT_SYMBOL_GPL(as3543_set_bits);


#ifdef AS3543_SUPPORT_ADC
int as3543_read_adc(u8 uType)
{
	u8 data[2];
	u32 uRet;

	mutex_lock(&io_mutex);

	data[0] = AFE_ADC_0;
	data[1] = uType << 4 ;

	as3543_write_i2c_device(data, 2);

	if(1 == i2c_master_send(as3543_i2c, data, 1))
	{
		if(i2c_master_recv(as3543_i2c, &data[1], 1))
			as3543_reg[AFE_ADC_0] = data[1];
	}

	data[0] = AFE_ADC_1;
	if(1 == i2c_master_send(as3543_i2c, data, 1))
	{
		if(i2c_master_recv(as3543_i2c, &data[1], 1))
			as3543_reg[AFE_ADC_1] = data[1];
	}

	mutex_unlock(&io_mutex);

	uRet = ((as3543_reg[AFE_ADC_0]&0x03) << 8) | as3543_reg[AFE_ADC_1];

	return uRet;
}
EXPORT_SYMBOL_GPL(as3543_read_adc);
#endif

#ifdef AS3543_SUPPORT_RTC
int as3543_set_time(struct device *pDev, struct rtc_time *pTime)
{
	u8 data;
	u32 iRTCTime;

	if(rtc_valid_tm(pTime) != 0)
	{
		return -1;
	}

	rtc_tm_to_time(pTime, (unsigned long *) &iRTCTime);
	dbg("%s: %d\n", "as3543_set_time", iRTCTime);

	as3543_reg_write(AFE_RTC_TIME, 0x3A);

	data = iRTCTime & 0xFF;
	as3543_reg_write(AFE_RTC_0, data);

	data = (iRTCTime>>8) & 0xFF;
	as3543_reg_write(AFE_RTC_1, data);

	data = (iRTCTime>>16) & 0xFF;
	as3543_reg_write(AFE_RTC_2, data);

	data = (iRTCTime>>24) & 0xFF;
	as3543_reg_write(AFE_RTC_3, data);

	return 0;
}

int as3543_read_time(struct device *pDev, struct rtc_time *pTime)
{
	u8 	uRTCTmp[4];
	u32 iRTCTime;

	uRTCTmp[0] = as3543_reg_read_from_device(AFE_RTC_0);
	mdelay(10);
	uRTCTmp[1] = as3543_reg_read_from_device(AFE_RTC_1);
	mdelay(10);
	uRTCTmp[2] = as3543_reg_read_from_device(AFE_RTC_2);
	mdelay(10);
	uRTCTmp[3] = as3543_reg_read_from_device(AFE_RTC_3);

	iRTCTime = (uRTCTmp[3] << 24) | (uRTCTmp[2] << 16) | (uRTCTmp[1] << 8) | uRTCTmp[0];

	dbg("%s: %X:%X:%X:%X", "as3543_read_time", uRTCTmp[3], uRTCTmp[2], uRTCTmp[1], uRTCTmp[0]);
	dbg("%s: %d", "as3543_read_time", iRTCTime);

	rtc_time_to_tm(iRTCTime, pTime);

	if(rtc_valid_tm(pTime) != 0)
	{
		err("i2c-as3543: Invalid time format. reset time (%s:%d)\n", __FUNCTION__, __LINE__);
	}


	return 0;
}

static struct rtc_class_ops as3543_rtc_ops = {
	.read_time 		= as3543_read_time,
	.set_time 		= as3543_set_time,
};
#endif


static int 	as3543_i2c_detach(struct i2c_client *client);
static unsigned short normal_i2c[] = { AFE_I2C_SLAVE_ADDR, I2C_CLIENT_END };

/* Magic definition of all other variables and things */
I2C_CLIENT_INSMOD;

static int as3543_i2c_detect(struct i2c_adapter *adap, int addr, int kind)
{
	int ret = 0;

	if (addr != AFE_I2C_SLAVE_ADDR)
		return -ENODEV;

	as3543_i2c = kzalloc(sizeof(struct i2c_client), GFP_KERNEL);
	if (as3543_i2c == NULL)
		return -ENOMEM;

	as3543_i2c->addr = addr;
	as3543_i2c->adapter = adap;

	ret = i2c_attach_client(as3543_i2c);

	if (ret < 0) {
		err("as3543: failed to attach device at addr 0x%x\n", addr);
		goto detecterr;
	}

#ifdef AS3543_SUPPORT_RTC
	as3543_rtc = rtc_device_register(AS3543_RTC_NAME, &as3543_i2c->dev, &as3543_rtc_ops, THIS_MODULE);

	if(IS_ERR(as3543_rtc))
		goto detecterr;
#endif

	return ret;

detecterr:
	as3543_i2c_detach(as3543_i2c);
	return ret;
}

static int as3543_i2c_attach(struct i2c_adapter *adap)
{
	return i2c_probe(adap, &addr_data, as3543_i2c_detect);
}

static int as3543_i2c_detach(struct i2c_client *client)
{
	i2c_detach_client(client);

	kfree(client);

	return 0;
}

#ifdef CONFIG_PM
static int as3543_i2c_suspend(struct i2c_client *client, pm_message_t state)
{
	int ret = 0;
	return ret;
}

static int as3543_i2c_resume(struct i2c_client *client)
{
	int ret = 0;
	return ret;
}
#else
#define as3543_i2c_suspend	NULL
#define as3543_i2c_resume	NULL
#endif

static struct i2c_driver as3543_i2c_driver = {
	.driver =
	{
		.name = AS3543_I2C_NAME,
	},
	.attach_adapter = as3543_i2c_attach,
	.detach_client 	= as3543_i2c_detach,
	.suspend		= as3543_i2c_suspend,
	.resume			= as3543_i2c_resume,
	.command 		= NULL,
};

static int __init as3543_i2c_init(void)
{
	return i2c_add_driver(&as3543_i2c_driver);
}

static void __exit as3543_i2c_exit(void)
{
#ifdef AS3543_SUPPORT_RTC
	rtc_device_unregister(as3543_rtc);
#endif

	i2c_del_driver(&as3543_i2c_driver);
}

subsys_initcall(as3543_i2c_init);
module_exit(as3543_i2c_exit);

MODULE_AUTHOR("aitdark.park");
MODULE_DESCRIPTION("AS3543 I2C Driver");
MODULE_LICENSE("GPL");
