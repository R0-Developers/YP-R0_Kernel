/*
* Muon Platform
* Copyright (c) 2004 Software Center, Samsung Electronics, Inc.
* All rights reserved.
*
* This software is the confidential and proprietary information
* of Samsung Electronics, Inc. ("Confidential Information").  You
* shall not disclose such Confidential Information and shall use
* it only in accordance with the terms of the license agreement
* you entered into with Samsung Electronics. 
*/

/** 
*	body for WM1808 Audio register drive module.  
*
* @name		Audio Register management driver for WM1808 (DevWM1808RegMgr.c)
* @author		Park dong yoon (dongyoon21.park@samsung.com)
* @version		1.0
*/

/*
<b>revision history : </b>
- 2007/04/05 first creation 
*/

#include "DrvWM1808RegMgr.h"
#include "../../i2c/wm1808/wm1808.h"

#define DRV_AUDIO_NAME "SAMSUNG_DRV_WM1808"
#define WM1808_VERSION "1.0"



#ifdef DRV_CODEC_DEBUG
#define dbg(format, arg...) \
	printk(DRV_AUDIO_NAME ": " format "\n" , ## arg)
#else
#define dbg(format, arg...) do {} while (0)
#endif

#define err(format, arg...) \
	printk(KERN_ERR DRV_AUDIO_NAME ": " format "\n" , ## arg)
#define info(format, arg...) \
	printk(KERN_INFO DRV_AUDIO_NAME ": " format "\n" , ## arg)
#define warn(format, arg...) \
	printk(KERN_WARNING DRV_AUDIO_NAME ": " format "\n" , ## arg)
	

MBool __DrvWM1808RegMgrSendIICCmd(MUint8 subAddr, MUint16 data);

static DrvWM1808Register __WM1808Register[DRV_WM1808_REG_CNT]={0};

/*=====================================================================================================
*global varible 
*===================================================================================================*/
struct snd_soc_codec *__pCodec=NULL; //for IIC


/*=====================================================================================================
*internal functions 
*===================================================================================================*/
/** 
* This function sets WM1808 register using IIC.
* it retrys  same job until succsss to register
*
* @return		This function returns 1 on success or failure returns 0 
* @param       subAddr, [in] Specifies register sub address
* @param       data,    [in] Specifies data to set the Wm1808 devices 
* @author      Park Dong yoon 	(032-277-7293, dongyoon21.park@samsung.com)
*/
MBool
__DrvWM1808RegMgrSendIICCmd(MUint8 subAddr, MUint16 data)
{
    MUint ret;
    struct wm1808* wm1808 = __pCodec->control_data;
	ret= wm1808_reg_write(wm1808, (u8)subAddr, (u16)data);

    return (ret>0)?1:0;     
}

/** 
* This function refresh global varible. 
*
* @param       pCodec [in] IIC control variable pointer.
* @author      Park Dong yoon 	(dongyoon21.park@samsung.com)
*/
MBool _DrvWM1808SetCodecObj(struct snd_soc_codec* pCodec)
{
	__pCodec=pCodec;
	dbg("\nCODEC2=%p\n",__pCodec);
	return 1;
}

/** 
* This function reset register data to default value.
*
* @return	This function returns TRUE on success or failure returns FASE 
* @author   Park Dong yoon 	(dongyoon21.park@samsung.com)
*/
MBool 
_DrvWM1808RegMgrResetData()
{
	/* Software Reset(00h) */
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_SOFTWARE_RESET],DRV_WM1808_SOFTWARE_RESET);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_SOFTWARE_RESET],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_SOFTWARE_RESET],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_SOFTWARE_RESET], DRV_WM1808_SOFTWARE_RESET_DEFAULT);					//default N/A, 일단 clear로 하였음.

	/* Power manage't 1(01h) */
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT1],DRV_WM1808_POWER_MANAGEMENT1);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT1],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT1],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT1], DRV_WM1808_POWER_MANAGEMENT1_DEFAULT);					

	/* Power manage't 2(02h) */
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT2],DRV_WM1808_POWER_MANAGEMENT2);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT2],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT2],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT2], DRV_WM1808_POWER_MANAGEMENT2_DEFAULT);	

	/* Power manage't 3(03h) */
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT3],DRV_WM1808_POWER_MANAGEMENT3);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT3],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT3],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_POWER_MANAGEMENT3], DRV_WM1808_POWER_MANAGEMENT3_DEFAULT);	

	/* Audio Interface(04h) */
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_AUDIO_INTERFACE],DRV_WM1808_AUDIO_INTERFACE);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_AUDIO_INTERFACE],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_AUDIO_INTERFACE],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_AUDIO_INTERFACE], DRV_WM1808_AUDIO_INTERFACE_DEFAULT);	

	/* Companding ctrl(05h) */
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_COMPANDING_CTRL],DRV_WM1808_COMPANDING_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_COMPANDING_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_COMPANDING_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_COMPANDING_CTRL], DRV_WM1808_COMPANDING_CTRL_DEFAULT);	

	/* Clock Gen ctrl(06h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_CLOCK_GEN_CTRL],DRV_WM1808_CLOCK_GEN_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_CLOCK_GEN_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_CLOCK_GEN_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_CLOCK_GEN_CTRL], DRV_WM1808_CLOCK_GEN_CTRL_DEFAULT);	

	/* Additional ctrl(07h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_ADDITIONAL_CTRL],DRV_WM1808_ADDITIONAL_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_ADDITIONAL_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_ADDITIONAL_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_ADDITIONAL_CTRL], DRV_WM1808_ADDITIONAL_CTRL_DEFAULT);	

	/* GPIO Stuff(08h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_GPIO_STUFF],DRV_WM1808_GPIO_STUFF);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_GPIO_STUFF],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_GPIO_STUFF],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_GPIO_STUFF], DRV_WM1808_GPIO_STUFF_DEFAULT);	

	/* Jack detect control(09h) */
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_JACK_DETECT_CONTROL1],DRV_WM1808_JACK_DETECT_CONTROL1);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_JACK_DETECT_CONTROL1],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_JACK_DETECT_CONTROL1],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_JACK_DETECT_CONTROL1], DRV_WM1808_JACK_DETECT_CONTROL1_DEFAULT);	

	/* DAC Control(0Ah) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_DAC_CONTROL],DRV_WM1808_DAC_CONTROL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_DAC_CONTROL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_DAC_CONTROL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_DAC_CONTROL], DRV_WM1808_DAC_CONTROL_DEFAULT);	

	/* Left DAC digital Vol(0Bh) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_LEFT_DAC_DIGITAL_VOL],DRV_WM1808_LEFT_DAC_DIGITAL_VOL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_LEFT_DAC_DIGITAL_VOL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_LEFT_DAC_DIGITAL_VOL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_LEFT_DAC_DIGITAL_VOL], DRV_WM1808_LEFT_DAC_DIGITAL_VOL_DEFAULT);		//8번째 비트 N/A, 일단 0으로 해 놓았음	

	/* Right DAC digital Vol(0Ch) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_RIGHT_DAC_DIGITAL_VOL],DRV_WM1808_RIGHT_DAC_DIGITAL_VOL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_RIGHT_DAC_DIGITAL_VOL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_RIGHT_DAC_DIGITAL_VOL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_RIGHT_DAC_DIGITAL_VOL], DRV_WM1808_RIGHT_DAC_DIGITAL_VOL_DEFAULT);		//8번째 비트 N/A, 일단 0으로 해 놓았음	

	/* Jack Detect Control(0Dh) */
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_JACK_DETECT_CONTROL2],DRV_WM1808_JACK_DETECT_CONTROL2);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_JACK_DETECT_CONTROL2],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_JACK_DETECT_CONTROL2],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_JACK_DETECT_CONTROL2], DRV_WM1808_JACK_DETECT_CONTROL2_DEFAULT);	

	/* ADC Control(0Eh) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_ADC_CONTROL],DRV_WM1808_ADC_CONTROL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_ADC_CONTROL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_ADC_CONTROL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_ADC_CONTROL], DRV_WM1808_ADC_CONTROL_DEFAULT);	

	/* Left ADC digital Vol(0Fh) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_LEFT_ADC_DIGITAL_VOL],DRV_WM1808_LEFT_ADC_DIGITAL_VOL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_LEFT_ADC_DIGITAL_VOL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_LEFT_ADC_DIGITAL_VOL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_LEFT_ADC_DIGITAL_VOL], DRV_WM1808_LEFT_ADC_DIGITAL_VOL_DEFAULT);		//8번째 비트 N/A, 일단 0으로 해 놓았음	

	/* Right ADC digital Vol(10h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_RIGHT_ADC_DIGITAL_VOL],DRV_WM1808_RIGHT_ADC_DIGITAL_VOL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_RIGHT_ADC_DIGITAL_VOL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_RIGHT_ADC_DIGITAL_VOL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_RIGHT_ADC_DIGITAL_VOL], DRV_WM1808_RIGHT_ADC_DIGITAL_VOL_DEFAULT);		//8번째 비트 N/A, 일단 0으로 해 놓았음	

	/* EQ1 - low shelf(12h) */
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_EQ1_LOW_SHELF],DRV_WM1808_EQ1_LOW_SHELF);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_EQ1_LOW_SHELF],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_EQ1_LOW_SHELF],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_EQ1_LOW_SHELF], DRV_WM1808_EQ1_LOW_SHELF_DEFAULT);	

	/* EQ2 - peak 1(13h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_EQ2_PEAK1],DRV_WM1808_EQ2_PEAK1);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_EQ2_PEAK1],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_EQ2_PEAK1],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_EQ2_PEAK1], DRV_WM1808_EQ2_PEAK1_DEFAULT);	

	/* EQ3 - peak 2(14h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_EQ3_PEAK2],DRV_WM1808_EQ3_PEAK2);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_EQ3_PEAK2],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_EQ3_PEAK2],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_EQ3_PEAK2], DRV_WM1808_EQ3_PEAK2_DEFAULT);	

	/* EQ4 - peak 3(15h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_EQ4_PEAK3],DRV_WM1808_EQ4_PEAK3);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_EQ4_PEAK3],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_EQ4_PEAK3],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_EQ4_PEAK3], DRV_WM1808_EQ4_PEAK3_DEFAULT);	

	/* EQ5 - high shelf(16h) */
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_EQ5_HIGH_SHELF],DRV_WM1808_EQ5_HIGH_SHELF);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_EQ5_HIGH_SHELF],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_EQ5_HIGH_SHELF],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_EQ5_HIGH_SHELF], DRV_WM1808_EQ5_HIGH_SHELF_DEFAULT);	

	/* DAC LIMITER 1(18h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_DAC_LIMITER1],DRV_WM1808_DAC_LIMITER1);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_DAC_LIMITER1],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_DAC_LIMITER1],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_DAC_LIMITER1], DRV_WM1808_DAC_LIMITER1_DEFAULT);	

	/* DAC LIMITER 2(19h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_DAC_LIMITER2],DRV_WM1808_DAC_LIMITER2);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_DAC_LIMITER2],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_DAC_LIMITER2],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_DAC_LIMITER2], DRV_WM1808_DAC_LIMITER2_DEFAULT);	

	/* Notch Filter 1(1Bh) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_NOTCH_FILTER1],DRV_WM1808_NOTCH_FILTER1);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_NOTCH_FILTER1],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_NOTCH_FILTER1],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_NOTCH_FILTER1], DRV_WM1808_NOTCH_FILTER1_DEFAULT);	

	/* Notch Filter 2(1Ch) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_NOTCH_FILTER2],DRV_WM1808_NOTCH_FILTER2);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_NOTCH_FILTER2],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_NOTCH_FILTER2],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_NOTCH_FILTER2], DRV_WM1808_NOTCH_FILTER2_DEFAULT);	

	/* Notch Filter 3(1Dh) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_NOTCH_FILTER3],DRV_WM1808_NOTCH_FILTER3);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_NOTCH_FILTER3],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_NOTCH_FILTER3],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_NOTCH_FILTER3], DRV_WM1808_NOTCH_FILTER3_DEFAULT);	

	/* Notch Filter 4(1Eh) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_NOTCH_FILTER4],DRV_WM1808_NOTCH_FILTER4);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_NOTCH_FILTER4],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_NOTCH_FILTER4],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_NOTCH_FILTER4], DRV_WM1808_NOTCH_FILTER4_DEFAULT);	

	/* ALC control 1(20h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_ALC_CONTROL1],DRV_WM1808_ALC_CONTROL1);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_ALC_CONTROL1],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_ALC_CONTROL1],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_ALC_CONTROL1], DRV_WM1808_ALC_CONTROL1_DEFAULT);	

	/* ALC control 2(21h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_ALC_CONTROL2],DRV_WM1808_ALC_CONTROL2);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_ALC_CONTROL2],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_ALC_CONTROL2],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_ALC_CONTROL2], DRV_WM1808_ALC_CONTROL2_DEFAULT);	

	/* ALC control 3(22h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_ALC_CONTROL3],DRV_WM1808_ALC_CONTROL3);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_ALC_CONTROL3],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_ALC_CONTROL3],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_ALC_CONTROL3], DRV_WM1808_ALC_CONTROL3_DEFAULT);	

	/* Noise Gate(23h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_NOISE_GATE],DRV_WM1808_NOISE_GATE);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_NOISE_GATE],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_NOISE_GATE],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_NOISE_GATE], DRV_WM1808_NOISE_GATE_DEFAULT);	

	/* PLL N(24h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_PLL_N],DRV_WM1808_PLL_N);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_PLL_N],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_PLL_N],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_PLL_N], DRV_WM1808_PLL_N_DEFAULT);	

	/* PLL K 1(25h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_PLL_K1],DRV_WM1808_PLL_K1);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_PLL_K1],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_PLL_K1],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_PLL_K1], DRV_WM1808_PLL_K1_DEFAULT);	

	/* PLL K 2(26h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_PLL_K2],DRV_WM1808_PLL_K2);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_PLL_K2],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_PLL_K2],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_PLL_K2], DRV_WM1808_PLL_K2_DEFAULT);	

	/* PLL K 3(27h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_PLL_K3],DRV_WM1808_PLL_K3);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_PLL_K3],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_PLL_K3],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_PLL_K3], DRV_WM1808_PLL_K3_DEFAULT);	

	/* 3D control(29h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_3D_CONTROL],DRV_WM1808_3D_CONTROL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_3D_CONTROL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_3D_CONTROL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_3D_CONTROL], DRV_WM1808_3D_CONTROL_DEFAULT);	

	/* VMID(2Ah) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_VMID],DRV_WM1808_VMID);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_VMID],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_VMID],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_VMID], DRV_WM1808_VMID_DEFAULT);	

	/* Beep control(2Bh) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_BEEP_CONTROL],DRV_WM1808_BEEP_CONTROL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_BEEP_CONTROL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_BEEP_CONTROL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_BEEP_CONTROL], DRV_WM1808_BEEP_CONTROL_DEFAULT);	

	/* INP control(2Ch) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_INP_CONTROL],DRV_WM1808_INP_CONTROL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_INP_CONTROL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_INP_CONTROL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_INP_CONTROL], DRV_WM1808_INP_CONTROL_DEFAULT);	

	/* Left INP PGA gain ctrl(2Dh) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_LEFT_INP_PGA_GAIN_CTRL],DRV_WM1808_LEFT_INP_PGA_GAIN_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_LEFT_INP_PGA_GAIN_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_LEFT_INP_PGA_GAIN_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_LEFT_INP_PGA_GAIN_CTRL], DRV_WM1808_LEFT_INP_PGA_GAIN_CTRL_DEFAULT);	

	/* Left ADC Boost ctrl(2Fh) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_LEFT_ADC_BOOST_CTRL],DRV_WM1808_LEFT_ADC_BOOST_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_LEFT_ADC_BOOST_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_LEFT_ADC_BOOST_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_LEFT_ADC_BOOST_CTRL], DRV_WM1808_LEFT_ADC_BOOST_CTRL_DEFAULT);	

	/* Right INP PGA gain ctrl(30h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_RIGHT_ADC_BOOST_CTRL],DRV_WM1808_RIGHT_ADC_BOOST_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_RIGHT_ADC_BOOST_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_RIGHT_ADC_BOOST_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_RIGHT_ADC_BOOST_CTRL], DRV_WM1808_RIGHT_ADC_BOOST_CTRL_DEFAULT);

	/* Output ctrl(31h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_OUTPUT_CTRL],DRV_WM1808_OUTPUT_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_OUTPUT_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_OUTPUT_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_OUTPUT_CTRL], DRV_WM1808_OUTPUT_CTRL_DEFAULT);	

	/* Left mixer ctrl(32h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_LEFT_MIXER_CTRL],DRV_WM1808_LEFT_MIXER_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_LEFT_MIXER_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_LEFT_MIXER_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_LEFT_MIXER_CTRL], DRV_WM1808_LEFT_MIXER_CTRL_DEFAULT);	

	/* Right mixer ctrl(33h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_RIGHT_MIXER_CTRL],DRV_WM1808_RIGHT_MIXER_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_RIGHT_MIXER_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_RIGHT_MIXER_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_RIGHT_MIXER_CTRL], DRV_WM1808_RIGHT_MIXER_CTRL_DEFAULT);	

	/* LOUT1(HP) volume ctrl(34h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_LOUT1_HP_VOLUME_CTRL],DRV_WM1808_LOUT1_HP_VOLUME_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_LOUT1_HP_VOLUME_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_LOUT1_HP_VOLUME_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_LOUT1_HP_VOLUME_CTRL], DRV_WM1808_LOUT1_HP_VOLUME_CTRL_DEFAULT);	//8번째 비트 N/A, 일단 0으로 해 놓았음	

	/* ROUT1(HP) volume ctrl(35h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_ROUT1_HP_VOLUME_CTRL],DRV_WM1808_ROUT1_HP_VOLUME_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_ROUT1_HP_VOLUME_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_ROUT1_HP_VOLUME_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_ROUT1_HP_VOLUME_CTRL], DRV_WM1808_ROUT1_HP_VOLUME_CTRL_DEFAULT);		//8번째 비트 N/A, 일단 0으로 해 놓았음	

	/* LOUT2(SPK) volume ctrl(36h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_LOUT2_SPK_VOLUME_CTRL],DRV_WM1808_LOUT2_SPK_VOLUME_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_LOUT2_SPK_VOLUME_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_LOUT2_SPK_VOLUME_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_LOUT2_SPK_VOLUME_CTRL], DRV_WM1808_LOUT2_SPK_VOLUME_CTRL_DEFAULT);			//8번째 비트 N/A, 일단 0으로 해 놓았음	

	/* ROUT2(SPK) volume ctrl(37h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_ROUT2_SPK_VOLUME_CTRL],DRV_WM1808_ROUT2_SPK_VOLUME_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_ROUT2_SPK_VOLUME_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_ROUT2_SPK_VOLUME_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_ROUT2_SPK_VOLUME_CTRL], DRV_WM1808_ROUT2_SPK_VOLUME_CTRL_DEFAULT);			//8번째 비트 N/A, 일단 0으로 해 놓았음	

	/* OUT3 mixer ctrl(38h) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_OUT3_MIXER_CTRL],DRV_WM1808_OUT3_MIXER_CTRL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_OUT3_MIXER_CTRL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_OUT3_MIXER_CTRL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_OUT3_MIXER_CTRL], DRV_WM1808_OUT3_MIXER_CTRL_DEFAULT);	

	/* Bias Control(3Dh) */ 
	DrvWM1808RegSetSubAddr(__WM1808Register[DRV_WM1808_BIAS_CONTROL],DRV_WM1808_BIAS_CONTROL);
	DrvWM1808RegSetProperty(__WM1808Register[DRV_WM1808_BIAS_CONTROL],DRV_WM1808_WRITABLE);
	DrvWM1808RegSetUpdateFlag(__WM1808Register[DRV_WM1808_BIAS_CONTROL],DRV_WM1808_VALID_DATA);
	DrvWM1808RegSetValue(__WM1808Register[DRV_WM1808_BIAS_CONTROL], DRV_WM1808_BIAS_CONTROL_DEFAULT);	

	return 1;
}

/** 
* This function sets register data.
*
* @return		This function returns 1 on success or failure returns 0 
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies value to set the Wm1808 devices 
* @author      Park Dong yoon 	(032-277-7293, dongyoon21.park@samsung.com)
*/
MBool 
_DrvWM1808RegMgrSetData (MUint8 regSubaddr, MUint32 regValue)
{
	/* check register value and property */
	/* check register value and property */
	if (DRV_WM1808_WRITABLE!=DrvWM1808RegGetProperty(__WM1808Register[regSubaddr])) return 0;	

	/* clear data and set data */
	DrvWM1808RegSetValue(__WM1808Register[regSubaddr],regValue);
	/* update invalid flag */ 
	DrvWM1808RegSetUpdateFlag(__WM1808Register[regSubaddr],DRV_WM1808_INVALID_DATA);
	return 1;
}

/** 
* This function clears register data.
*
* @return		This function returns 1 on success or failure returns 0 
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies value to set the Wm1808 devices 
* @param	   bitmask,	   [in] 
* @author      Park Dong yoon 	(032-277-7293, dongyoon21.park@samsung.com)
*/
MBool 
_DrvWM1808RegMgrClearData (MUint8 regSubaddr, MUint16 bitMask)
{
	MUint16 setData=0x00;
	/* check register value and property */
	if (DRV_WM1808_WRITABLE!=DrvWM1808RegGetProperty(__WM1808Register[regSubaddr])) return 0;	

	setData=DrvWM1808RegGetValue(__WM1808Register[regSubaddr]);
	setData&=(~bitMask);

	/* clear data and set data */
	DrvWM1808RegSetValue(__WM1808Register[regSubaddr],setData);
	/* update invalid flag */ 
	DrvWM1808RegSetUpdateFlag(__WM1808Register[regSubaddr],DRV_WM1808_INVALID_DATA);
	return 1;
}

/** 
* This function clears register data.
*
* @return		This function returns 1 on success or failure returns 0 
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies value to set the Wm1808 devices 
* @param	   bitmask,	   [in] 
* @author      Park Dong yoon 	(032-277-7293, dongyoon21.park@samsung.com)
*/
MBool 
_DrvWM1808RegMgrClearDataEx (MUint8 regSubaddr, MUint16 bitMask)
{
	MUint16 setData=0x00;
	/* check register value and property */
	if (DRV_WM1808_WRITABLE!=DrvWM1808RegGetProperty(__WM1808Register[regSubaddr])) return 0;	

	setData=DrvWM1808RegGetValue(__WM1808Register[regSubaddr]);
	setData&=(~bitMask);

	/* clear data and set data */
	DrvWM1808RegSetValue(__WM1808Register[regSubaddr],setData);

	/* send iic data */ 
	__DrvWM1808RegMgrSendIICCmd(DrvWM1808RegGetSubAddr(__WM1808Register[regSubaddr]),
		DrvWM1808RegGetValue(__WM1808Register[regSubaddr]));	
	/* update invalid flag */ 
	DrvWM1808RegSetUpdateFlag(__WM1808Register[regSubaddr],DRV_WM1808_VALID_DATA);
	return 1;
}

MBool 
_DrvWM1808RegMgrAddData (MUint8 regSubaddr, MUint32 regValue)
{
	/* check register value and property */
	if (DRV_WM1808_WRITABLE!=DrvWM1808RegGetProperty(__WM1808Register[regSubaddr])) return 0;	

	/* clear data and set data */
	DrvWM1808RegAddValue(__WM1808Register[regSubaddr],regValue);
	/* update invalid flag */ 
	DrvWM1808RegSetUpdateFlag(__WM1808Register[regSubaddr],DRV_WM1808_INVALID_DATA);
	return 1;
}

MBool 
_DrvWM1808RegMgrAddDataEx (MUint8 regSubaddr, MUint32 regValue)
{
	/* check register value and property */
	if (DRV_WM1808_WRITABLE!=DrvWM1808RegGetProperty(__WM1808Register[regSubaddr])) return 0;	

	/* clear data and set data */
	DrvWM1808RegAddValue(__WM1808Register[regSubaddr],regValue);
	/* update invalid flag */ 
	DrvWM1808RegSetUpdateFlag(__WM1808Register[regSubaddr],DRV_WM1808_INVALID_DATA);

	/* send iic data */ 
	__DrvWM1808RegMgrSendIICCmd(DrvWM1808RegGetSubAddr(__WM1808Register[regSubaddr]),
		DrvWM1808RegGetValue(__WM1808Register[regSubaddr]));	
	/* update valid flag */ 
	DrvWM1808RegSetUpdateFlag(__WM1808Register[regSubaddr],DRV_WM1808_VALID_DATA);

	return 1;
}

MBool 
_DrvWM1808RegMgrSetDataEx(MUint8 regSubaddr, MUint32 regValue)
{
	/* check register value and property */
	if (DRV_WM1808_WRITABLE!=DrvWM1808RegGetProperty(__WM1808Register[regSubaddr])) return 0;	

	/* clear data and set data */
	DrvWM1808RegSetValue(__WM1808Register[regSubaddr],regValue);

	/* send iic data */ 
	__DrvWM1808RegMgrSendIICCmd(DrvWM1808RegGetSubAddr(__WM1808Register[regSubaddr]),
		DrvWM1808RegGetValue(__WM1808Register[regSubaddr]));	
	/* update valid flag */ 
	DrvWM1808RegSetUpdateFlag(__WM1808Register[regSubaddr],DRV_WM1808_VALID_DATA);	

	return 1;
}

MUint16
_DrvWM1808RegMgrGetData (MUint8 regSubaddr, DrvWM1808RegDataCategory category )
{
	return ((__WM1808Register[regSubaddr])&category);
}


MBool
_DrvWM1808RegMgrApplyData()
{
	int i=0;
	for (i=0;i<DRV_WM1808_REG_CNT;i++)
	{
		if (DRV_WM1808_VALID_DATA==DrvWM1808RegGetUpdateFlag(__WM1808Register[i])) continue;

		/* send iic data */ 
		__DrvWM1808RegMgrSendIICCmd(DrvWM1808RegGetSubAddr(__WM1808Register[i]),
			DrvWM1808RegGetValue(__WM1808Register[i]));			
		/* update valid flag */ 
		DrvWM1808RegSetUpdateFlag(__WM1808Register[i],DRV_WM1808_VALID_DATA);			
	}
	return 1;
}

MBool
_DrvWM1808RegMgrApplyDataEx(MUint8 regSubaddr)
{
	if (DRV_WM1808_VALID_DATA==DrvWM1808RegGetUpdateFlag(__WM1808Register[regSubaddr])) return 0;

	/* send iic data */ 
	__DrvWM1808RegMgrSendIICCmd ( DrvWM1808RegGetSubAddr(__WM1808Register[regSubaddr]),
		DrvWM1808RegGetValue(__WM1808Register[regSubaddr]));	
	/* update valid flag */ 
	DrvWM1808RegSetUpdateFlag(__WM1808Register[regSubaddr],DRV_WM1808_VALID_DATA);	
	return 1;
}

