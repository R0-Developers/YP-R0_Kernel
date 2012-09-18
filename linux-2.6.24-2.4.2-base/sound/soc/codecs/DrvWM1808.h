/*
* Muon Platform
* Copyright (c) 2008 Samsung Electronics, Inc.
* All rights reserved.
*
* This software is the confidential and proprietary information
* of Samsung Electronics, Inc. ("Confidential Information").  You
* shall not disclose such Confidential Information and shall use
* it only in accordance with the terms of the license agreement
* you entered into with Samsung Electronics. 
*/

/** 
*	Header for WM1808 driver module.
*
* @name		Device driver for WM1808 (DevWM1808.h)
* @author		dong yoon Park (dongyoon21.park@samsung.com) 	
* @version		1.0
*/

#ifndef DRV_WM1808_H_
#define DRV_WM1808_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "DrvWM1808RegMgr.h"

#undef DRV_CODEC_DEBUG 1
#define AUDIO_NAME "WM1808 ALSA"
#define WM1808_VERSION "4.0"

MBool _DrvWM1808PlayBack(void);
MBool _DrvWM1808MasterVolume(long vol);
MBool _DrvWM1808MicRecord(void);
MBool _DrvWM1808FMRecord(void);
MBool _DrvWM1808MasterMute(int muteFlag);

 /*
  * Codec Device Status
  */ 
typedef enum
{
 	AV_ADEVMGR_CODEC_STATUS_MIN=0,
	AV_ADEVMGR_CODEC_STATUS_NONE,
 	AV_ADEVMGR_CODEC_STATUS_PLAY,
 	AV_ADEVMGR_CODEC_STATUS_RECORD,
 	AV_ADEVMGR_CODEC_STATUS_FULLDUPLEX,
 	AV_ADEVMGR_CODEC_STATUS_HANDSFREE,
 	AV_ADEVMGR_CODEC_STATUS_FM,
 	AV_ADEVMGR_CODEC_STATUS_LINE_IN,
 	AV_ADEVMGR_CODEC_STATUS_LINE_OUT,
 	AV_ADEVMGR_CODEC_STATUS_MAX 
}AvAudDevMgrCodecStatus;
/*
typedef struct 
{
	
	AvAudDevMgrCodecStatus asdf;
	

}AvAudDevMgr;
*/

typedef struct
{
	//Power Management2
	MBool regROUT1;		//WM1808_ROUT1_EN		
	MBool regLOUT1;		//WM1808_LOUT1_EN
	MBool regRBOOST;	//WM1808_BOOSTENR
	MBool regLBOOST;	//WM1808_BOOSTENL
	MBool regPGA;		//WM1808_INPPGAENL
	MBool regRADC;		//WM1808_ADCENR
	MBool regLADC;		//WM1808_ADCENL

	//Power Management3
	MBool regRMIX;		//WM1808_RMIXEN	
	MBool regLMIX;		//WM1808_LMIXEN
	MBool regRDAC;		//WM1808_DACENL
	MBool regLDAC;		//WM1808_DACENR

	//Input PGA Path
	MBool regLIN2PGA;	//WM1808_LIN2INPPGA	
	MBool regLIP2PGA;	//WM1808_LIP2INPPGA	

	//Left Mixer Path
	MBool regAUXL2LMIX;	//WM1808_AUXL2LMIX
	MBool regBYPL2LMIX;	//WM1808_BYPL2LMIX
	MBool regDACL2LMIX;	//WM1808_DACL2LMIX

	//Right Mixer Path
	MBool regAUXR2RMIX;	//WM1808_AUXR2RMIX
	MBool regDACR2RMIX;	//WM1808_DACR2RMIX
	MBool regBYPL2RMIX;	//WM1808_BYPL2RMIX

	//Left Mixer Volume	
	MInt8 regAUXLMIXvol;	//WM1808_AUXLMIXVOL
	MInt8 regBYPLMIXvol;	//WM1808_BYPLMIXVOL

	//Right Mixer Volume
	MInt8 regAUXRMIXvol;	//WM1808_AUXRMIXVOL
}AvAudDrvSettingMgr;

MBool _DrvWM1808Init(void);
MBool _DrvWM1808Deinit(void);
MBool _DrvWM1808InitState(AvAudDevMgrCodecStatus state);
MBool _DrvWM1808Switch(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DRV_WM1800_H_ */

