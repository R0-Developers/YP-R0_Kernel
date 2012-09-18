#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/rtc.h>
#include <linux/bcd.h>
#include <asm/uaccess.h>

//#include "s35392aReg.h"

#define S35392A_ADDR		(0x6<<3)

/* I2C address of s35392a is 0x6, but it shift-right the adress by 3bit 
   and use the low 3bit as a command register. 111b(0x7) is free register
   command. So we use 0x37. Read data sheet 7p if you don't understand. */

#define S35392A_STATUS_REG1		(S35392A_ADDR|0)
#define S35392A_STATUS_REG2		(S35392A_ADDR|1)
#define S35392A_REALTIME_DATA1	(S35392A_ADDR|2)
#define S35392A_REALTIME_DATA2	(S35392A_ADDR|3)
#define S35392A_INT_REG1		(S35392A_ADDR|4)
#define S35392A_INT_REG2		(S35392A_ADDR|5)
#define S35392A_CLK_ADJ_REG		(S35392A_ADDR|6)
#define S35392A_FREE_REG		(S35392A_ADDR|7)

#define S35392A_PROBE_ADDR	(S35392A_FREE_REG)	/* used to probe rtc device */

#define S35392A_YEAR_MASK	(0xFF)
#define S35392A_MON_MASK	(0x1F)
#define S35392A_DAY_MASK	(0x3F)
#define S35392A_WDAY_MASK	(0x07)
#define S35392A_HOUR_MASK	(0x3F)
#define S35392A_MIN_MASK	(0x7F)
#define S35392A_SEC_MASK	(0x7F)

#define S35392A_24HOUR_MASK	(0x40)


#define S35392A_DEV_NAME	"s35392a_rtc"
#define S35392A_DEV_NUM		233


#define DEBUG
#ifdef DEBUG
#define MYTRACE(x)	printk x
#else
#define MYTRACE(x)
#endif


static int s35392a_detach(struct i2c_client* client);
static int s35392a_probe(struct i2c_adapter* adapter);
static int s35392a_attach(struct i2c_adapter* adapter, int address, int kind);
static int s35392a_read_reg(unsigned char reg_addr, unsigned char* buf, int buf_len);
static int s35392a_write_reg(unsigned char reg_addr, unsigned char* buf, int buf_len);


/* this is used by i2c_add_driver */
static struct i2c_driver s35392a_driver =
{
	.driver = {.name = S35392A_DEV_NAME, },
	.attach_adapter = s35392a_probe,
	.detach_client = s35392a_detach,
};

static unsigned short normal_i2c[] = {S35392A_PROBE_ADDR, I2C_CLIENT_END};	/* var. name MUST be 'normal_i2c' */
static struct i2c_client gClient;

I2C_CLIENT_INSMOD;		/* DO NOT MISS THIS after normal_i2c!!! from i2c.h */



static unsigned char fourbit_reverse[16] = {0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};

static int
s35392a_is_valid_time(struct rtc_time* pTm)
{
	return 1;
}

static int
s35392a_read_time(struct device* pDev, struct rtc_time* pTm)
{
	unsigned char rtc_reg[7];
	int ret;

	ret = s35392a_read_reg(S35392A_REALTIME_DATA1, rtc_reg, sizeof(rtc_reg));

	if (ret < 0)
	{
		return ret;
	}

	pTm->tm_sec = BCD2BIN(rtc_reg[6] & S35392A_SEC_MASK);
	pTm->tm_min = BCD2BIN(rtc_reg[5] & S35392A_MIN_MASK);
	pTm->tm_hour = BCD2BIN(rtc_reg[4] & S35392A_HOUR_MASK);
	pTm->tm_wday = BCD2BIN(rtc_reg[3] & S35392A_WDAY_MASK);
	pTm->tm_mday = BCD2BIN(rtc_reg[2] & S35392A_DAY_MASK);
	pTm->tm_mon = BCD2BIN(rtc_reg[1]  & S35392A_MON_MASK) - 1;		/* 0~11 for linux appl, 1~12 for rtc */
	pTm->tm_year = BCD2BIN(rtc_reg[0] & S35392A_YEAR_MASK) + 100;	/* 1900 for linux appl, 2000 for rtc */
	/* don't use tm_yday and tm_dst */

	MYTRACE(("%s : \n", __func__));
	MYTRACE(("YY:MM:DD:HH:MM:SS(%d:%2d:%2d:%2d:%2d:%2d)\n", pTm->tm_year, pTm->tm_mon, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec));

	return 0;
}

static int
s35392a_set_time(struct device* pDev, struct rtc_time* pTm)
{
	unsigned char rtc_reg[7];
	int ret;

	MYTRACE(("%s : \n", __func__));
	MYTRACE(("YY:MM:DD:HH:MM:SS(%d:%2d:%2d:%2d:%2d:%2d)\n", pTm->tm_year, pTm->tm_mon, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec));

	rtc_reg[6] = BIN2BCD(pTm->tm_sec);
	rtc_reg[5] = BIN2BCD(pTm->tm_min);
	rtc_reg[4] = BIN2BCD(pTm->tm_hour);
	rtc_reg[3] = BIN2BCD(pTm->tm_wday);
	rtc_reg[2] = BIN2BCD(pTm->tm_mday);
	rtc_reg[1] = BIN2BCD(pTm->tm_mon + 1);		/*0~11 for linux appl, 1~12 for rtc */
	rtc_reg[0] = BIN2BCD(pTm->tm_year - 100);	/* 1900 for linux appl, 2000 for rtc */
	/* don't use tm_yday and tm_dst */

	ret = s35392a_write_reg(S35392A_REALTIME_DATA1, rtc_reg, sizeof(rtc_reg));

	if (ret < 0)
	{
		return ret;
	}

	return 0;
}

static void
write_time(void)
{
	int ret;
	struct rtc_time tm;

	tm.tm_year = 19;
	tm.tm_mon = 2;
	tm.tm_mday = 1;
	tm.tm_wday = 0;
	tm.tm_hour = 10;
	tm.tm_min = 35;
	tm.tm_sec = 22;

	ret = s35392a_set_time(NULL, &tm);
}

static void
read_time(void)
{
	struct rtc_time tm;
	int ret;

	ret = s35392a_read_time(NULL, &tm);
}


static const struct rtc_class_ops s35392a_rtc_ops = {
	.read_time = s35392a_read_time,
	.set_time = s35392a_set_time,
};

static int
s35392a_bootup_proc(void)
{
	unsigned char reg1_buf = 0x0;
	unsigned char reg2_buf = 0x0;
	int ret = 0;
	int fail_cnt = 0;

	do
	{		
		ret = s35392a_read_reg(S35392A_STATUS_REG1, &reg1_buf, sizeof(reg1_buf));
		if (ret < 0)
		{
			MYTRACE(("%s i2c read fail(STATUS_REG1).\n", __func__));
			return ret;	/* boot up fail */
		}
		if (fail_cnt++ > 5)
		{
			MYTRACE(("%s i2c read fail(endless loop exit).\n", __func__));
			break;
		}
	}while(!(reg1_buf&0x1<<7 || reg1_buf&0x1<<6));	/* while !(POC=1 or BLD=1) */

	reg1_buf |= 0x1<<1;	/* if 1 => 24-hour expression, 12-hour otherwise. */
	ret = s35392a_write_reg(S35392A_STATUS_REG1, &reg1_buf, sizeof(reg1_buf));

#if 0
	reg1_buf |= 0x1; 	/* RESET */

	fail_cnt = 0;

	do
	{
		ret = s35392a_write_reg(S35392A_STATUS_REG1, &reg1_buf, sizeof(reg1_buf));
		if (ret < 0)
		{
			MYTRACE(("%s i2c write fail(STATUS_REG1).\n", __func__));
			return ret;	/* boot up fail */
		}

		ret = s35392a_read_reg(S35392A_STATUS_REG2, &reg2_buf, sizeof(reg2_buf));
		if (ret < 0)
		{
			MYTRACE(("%s i2c read fail(STATUS_REG2).\n", __func__));
			return ret;	/* boot up fail */
		}

		if (fail_cnt++ > 10)
		{
			MYTRACE(("S35392A RTC Init Failed!\n"));
			return -1;
		}
	}while(reg2_buf&0x1<<7);	/* while TEST==1 */
#endif

	MYTRACE(("S35392A RTC Enabled.\n"));

	return 0;	/* rtc bootup succeed! */
}

static void
s35392a_fourbit_reverse(unsigned char* pData, int len)
{
	int i = 0;
	unsigned char tmp = 0;

	/* convert endian(big to little) in each bytes */
	for (i = 0; i < len; i++)
	{
		tmp = pData[i];
		pData[i] = 0;
		pData[i] |= fourbit_reverse[tmp>>4];			/* 4bit high -> low */
		pData[i] |= fourbit_reverse[tmp&0x0F]<<4;	/* 4bit low -> high */
	}
}

static int
s35392a_write_reg(unsigned char reg_addr, unsigned char* buf, int buf_len)
{
	struct i2c_msg msg[1];
	int ret = 0;

	msg[0].addr = reg_addr;
	msg[0].flags = 0;
	msg[0].len = buf_len;
	msg[0].buf = buf;

	s35392a_fourbit_reverse(buf, buf_len);	/* convert endian */

	ret = i2c_transfer(gClient.adapter, msg, 1);

	if (ret < 0)
	{
		return -EIO;
	}

	return ret;
}

static int
s35392a_read_reg(unsigned char reg_addr, unsigned char* buf, int buf_len)
{
	struct i2c_msg msg[1];
	int ret = 0;
	//int i;

	msg[0].addr = reg_addr;
	msg[0].flags = I2C_M_RD;
	msg[0].len = buf_len;
	msg[0].buf = buf;

	ret = i2c_transfer(gClient.adapter, msg, 1);

//	MYTRACE(("%s : %d \n", __func__, ret));

	if (ret < 0)
	{
		return -EIO;
	}

	s35392a_fourbit_reverse(buf, buf_len);	/* convert endian */

#if 0	
	MYTRACE(("addr : %x\n", reg_addr));
	for(i = 0; i < buf_len; i++)
	{
		MYTRACE(("buf[%02d] : %x\n", i, buf[i]);
	}
#endif

	return ret;
}

static int
s35392a_detach(struct i2c_client* client)
{
	int ret;

	ret = i2c_detach_client(client);
	MYTRACE(("detach : %d\n", ret));

	return 0;
}

static int
s35392a_probe(struct i2c_adapter* adapter)
{
	int ret;

	MYTRACE(("ADAPTER id : %d\n", i2c_adapter_id(adapter)));

	ret = i2c_probe(adapter, &addr_data, &s35392a_attach);

	MYTRACE(("%s : %d\n", __func__, ret));

	return ret;
}

static int
s35392a_attach(struct i2c_adapter* adapter, int address, int kind)
{
	int ret;
	struct rtc_device *rtc = NULL;

	if (i2c_adapter_id(adapter) != 1)	/* rtc is on i2c-1 */
	{
		return -ENODEV;
	}

	gClient.addr = address;
	gClient.adapter = adapter;
	gClient.driver = &s35392a_driver;
	gClient.flags = 0;
	strlcpy(gClient.name, S35392A_DEV_NAME, I2C_NAME_SIZE);

	MYTRACE(("%s\n", __func__));
	MYTRACE(("Adapter : %d\n", i2c_adapter_id(adapter)));
	MYTRACE(("ADDR : %x\n", gClient.addr));

	ret = i2c_attach_client(&gClient);
	if (ret < 0)
	{
		MYTRACE(("i2c attach failed : %s\n", __func__));
	}

	rtc = rtc_device_register(gClient.name, &gClient.dev, &s35392a_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc))
	{
		MYTRACE(("RTC driver regist failed!\n"));
	}

	return ret;
}

static int
__init s35392a_init(void)
{
	int ret;

	MYTRACE(("%s\n", __func__));

	ret = i2c_add_driver(&s35392a_driver);
	if (ret)	/* >0 means error for i2c_add_driver */
	{
		MYTRACE(("Can't add i2c driver : %s\n", __func__));
		return ret;
	}

	if (s35392a_bootup_proc() < 0)
	{
		return -ENODEV;
	}

	MYTRACE(("S35392A RTC Driver Successfully Loaded!\n"));

	return 0;
}

static void
__exit s35392a_exit(void)
{
	MYTRACE(("%s\n", __func__));

	i2c_del_driver(&s35392a_driver);
}

module_init(s35392a_init);
module_exit(s35392a_exit);

