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
*	body for WM1808 driver module.  
*
* @name		    Device driver for WM1808 (DevWM1808.c)
* @author		Kim Hyun Wook 	(hw123.kim@samsung.com)
* @version		1.0
*/

/*
<b>revision history : </b>
- 2008/08/29 first creation 
*/

#include "DrvWM1808.h"
#include <linux/delay.h>
#include "iomux.h"
#include "mx37_pins.h"

#define DRV_WM1808_VALUE_CLEARED 0x0000
/*=====================================================================================================
*Global variable
*/


static AvAudDrvSettingMgr AudDrvSettingMgr=
{
	//Power Management2
	DRV_WM1808_ROUT1_EN_DISABLED,
	DRV_WM1808_LOUT1_EN_DISABLED,
	DRV_WM1808_BOOST_STAGE_OFF,
	DRV_WM1808_BOOST_STAGE_OFF,
	DRV_WM1808_INPPGAENL_DISABLED,
	DRV_WM1808_ADCENR_DISABLED,
	DRV_WM1808_ADCENL_DISABLED,

	//Power Management3
	DRV_WM1808_RMIXEN_DISABLED,
	DRV_WM1808_LMIXEN_DISABLED,
	DRV_WM1808_DACENR_DISABLED,
	DRV_WM1808_DACENL_DISABLED,

	//Input PGA Path
	DRV_WM1808_LIN2INPPGA_NOT_CONNECTED,
	DRV_WM1808_LIP2INPPGA_NOT_CONNECTED,

	//Left Mixer Path
	DRV_WM1808_AUXL2LMIX_NOT_SELECTED,
	DRV_WM1808_BYPL2LMIX_NOT_SELECTED,
	DRV_WM1808_DACL2LMIX_NOT_SELECTED,

	//Right Mixer Path
	DRV_WM1808_AUXR2RMIX_NOT_SELECTED,
	DRV_WM1808_DACR2RMIX_NOT_SELECTED,
	DRV_WM1808_BYPL2RMIX_NOT_SELECTED,

	//Left Mixer Volume
	0x00,
	0x00,

	//Right Mixer Volume
	0x00
};

static AvAudDrvSettingMgr *pAudDrvSettingMgr = &AudDrvSettingMgr;

/** 
* MBool _DrvWM1808InitState(AvAudDrvCodecStatus state)
*
* This function initializes WM1808 device controls of status. 
*
* @param		status[in]enum type
* @return		This function returns TRUE on success, or FALSE on failure
* @author       Kim Hyun Wook 	(hw123.kim@samsung.com)
**/
MBool _DrvWM1808InitState(AvAudDevMgrCodecStatus state)
{
	switch(state)
	{
		case AV_ADEVMGR_CODEC_STATUS_PLAY:
			pAudDrvSettingMgr->regROUT1 = DRV_WM1808_ROUT1_EN_ENABLED;
			pAudDrvSettingMgr->regLOUT1 = DRV_WM1808_LOUT1_EN_ENABLED;
			pAudDrvSettingMgr->regRBOOST = DRV_WM1808_BOOST_STAGE_OFF;
			pAudDrvSettingMgr->regLBOOST = DRV_WM1808_BOOST_STAGE_OFF;
			pAudDrvSettingMgr->regPGA = DRV_WM1808_INPPGAENL_DISABLED;
			pAudDrvSettingMgr->regRADC = DRV_WM1808_ADCENR_DISABLED;
			pAudDrvSettingMgr->regLADC = DRV_WM1808_ADCENL_DISABLED;
			
			pAudDrvSettingMgr->regRMIX = DRV_WM1808_RMIXEN_ENABLED;
			pAudDrvSettingMgr->regLMIX = DRV_WM1808_LMIXEN_ENABLED;
			pAudDrvSettingMgr->regRDAC = DRV_WM1808_DACENR_ENABLED;
			pAudDrvSettingMgr->regLDAC = DRV_WM1808_DACENL_ENABLED;

			pAudDrvSettingMgr->regLIN2PGA = DRV_WM1808_LIN2INPPGA_NOT_CONNECTED;
			pAudDrvSettingMgr->regLIP2PGA = DRV_WM1808_LIP2INPPGA_NOT_CONNECTED;
			
			pAudDrvSettingMgr->regAUXL2LMIX = DRV_WM1808_AUXL2LMIX_NOT_SELECTED;
			pAudDrvSettingMgr->regBYPL2LMIX = DRV_WM1808_BYPL2LMIX_NOT_SELECTED;
			pAudDrvSettingMgr->regDACL2LMIX = DRV_WM1808_DACL2LMIX_SELECTED;

			pAudDrvSettingMgr->regAUXR2RMIX = DRV_WM1808_AUXR2RMIX_NOT_SELECTED;
			pAudDrvSettingMgr->regDACR2RMIX = DRV_WM1808_DACR2RMIX_SELECTED;
			pAudDrvSettingMgr->regBYPL2RMIX = DRV_WM1808_BYPL2RMIX_NOT_SELECTED;

			pAudDrvSettingMgr->regAUXLMIXvol = 0x00;
			pAudDrvSettingMgr->regBYPLMIXvol = 0x00;

			pAudDrvSettingMgr->regAUXRMIXvol = 0x00;
			break;

		case AV_ADEVMGR_CODEC_STATUS_RECORD:
			pAudDrvSettingMgr->regROUT1 = DRV_WM1808_ROUT1_EN_ENABLED;
			pAudDrvSettingMgr->regLOUT1 = DRV_WM1808_LOUT1_EN_ENABLED;
			pAudDrvSettingMgr->regRBOOST = DRV_WM1808_BOOST_STAGE_OFF;
			pAudDrvSettingMgr->regLBOOST = DRV_WM1808_BOOST_STAGE_ON;
			pAudDrvSettingMgr->regPGA = DRV_WM1808_INPPGAENL_ENABLED;
			pAudDrvSettingMgr->regRADC = DRV_WM1808_ADCENR_DISABLED;
			pAudDrvSettingMgr->regLADC = DRV_WM1808_ADCENL_ENABLED;
			
			pAudDrvSettingMgr->regRMIX = DRV_WM1808_RMIXEN_ENABLED;
			pAudDrvSettingMgr->regLMIX = DRV_WM1808_LMIXEN_ENABLED;
			pAudDrvSettingMgr->regRDAC = DRV_WM1808_DACENR_DISABLED;
			pAudDrvSettingMgr->regLDAC = DRV_WM1808_DACENL_DISABLED;

			pAudDrvSettingMgr->regLIN2PGA = DRV_WM1808_LIN2INPPGA_CONNECTED;
			pAudDrvSettingMgr->regLIP2PGA = DRV_WM1808_LIP2INPPGA_CONNECTED;
			
			pAudDrvSettingMgr->regAUXL2LMIX = DRV_WM1808_AUXL2LMIX_NOT_SELECTED;
			pAudDrvSettingMgr->regBYPL2LMIX = DRV_WM1808_BYPL2LMIX_SELECTED;
			pAudDrvSettingMgr->regDACL2LMIX = DRV_WM1808_DACL2LMIX_NOT_SELECTED;

			pAudDrvSettingMgr->regAUXR2RMIX = DRV_WM1808_AUXR2RMIX_NOT_SELECTED;
			pAudDrvSettingMgr->regDACR2RMIX = DRV_WM1808_DACR2RMIX_NOT_SELECTED;
			pAudDrvSettingMgr->regBYPL2RMIX = DRV_WM1808_BYPL2RMIX_SELECTED;

			pAudDrvSettingMgr->regAUXLMIXvol = 0x00;
			pAudDrvSettingMgr->regBYPLMIXvol = 0x07;

			pAudDrvSettingMgr->regAUXRMIXvol = 0x00;			
			break;

		case AV_ADEVMGR_CODEC_STATUS_FM:
			pAudDrvSettingMgr->regROUT1 = DRV_WM1808_ROUT1_EN_ENABLED;
			pAudDrvSettingMgr->regLOUT1 = DRV_WM1808_LOUT1_EN_ENABLED;
			pAudDrvSettingMgr->regRBOOST = DRV_WM1808_BOOST_STAGE_ON;
			pAudDrvSettingMgr->regLBOOST = DRV_WM1808_BOOST_STAGE_ON;
			pAudDrvSettingMgr->regPGA = DRV_WM1808_INPPGAENL_DISABLED;
			pAudDrvSettingMgr->regRADC = DRV_WM1808_ADCENR_ENABLED;
			pAudDrvSettingMgr->regLADC = DRV_WM1808_ADCENL_ENABLED;
			
			pAudDrvSettingMgr->regRMIX = DRV_WM1808_RMIXEN_ENABLED;
			pAudDrvSettingMgr->regLMIX = DRV_WM1808_LMIXEN_ENABLED;
			pAudDrvSettingMgr->regRDAC = DRV_WM1808_DACENR_DISABLED;
			pAudDrvSettingMgr->regLDAC = DRV_WM1808_DACENL_DISABLED;

			pAudDrvSettingMgr->regLIN2PGA = DRV_WM1808_LIN2INPPGA_NOT_CONNECTED;
			pAudDrvSettingMgr->regLIP2PGA = DRV_WM1808_LIP2INPPGA_NOT_CONNECTED;
			
			pAudDrvSettingMgr->regAUXL2LMIX = DRV_WM1808_AUXL2LMIX_SELECTED;
			pAudDrvSettingMgr->regBYPL2LMIX = DRV_WM1808_BYPL2LMIX_NOT_SELECTED;
			pAudDrvSettingMgr->regDACL2LMIX = DRV_WM1808_DACL2LMIX_NOT_SELECTED;

			pAudDrvSettingMgr->regAUXR2RMIX = DRV_WM1808_AUXR2RMIX_SELECTED;
			pAudDrvSettingMgr->regDACR2RMIX = DRV_WM1808_DACR2RMIX_NOT_SELECTED;
			pAudDrvSettingMgr->regBYPL2RMIX = DRV_WM1808_BYPL2RMIX_NOT_SELECTED;

			pAudDrvSettingMgr->regAUXLMIXvol = 0x07;
			pAudDrvSettingMgr->regBYPLMIXvol = 0x00;

			pAudDrvSettingMgr->regAUXRMIXvol = 0x07;
			break;

		default:
			break;
	}
    
    // switch codec status 
    _DrvWM1808Switch();
 
	return 1;
}



/** 
* MBool _DrvWM1808Init(void)
*
* This function initializes WM1808 device. 
*
* @return		This function returns TRUE on success, or FALSE on failure
* @author       Kim Hyun Wook 	(hw123.kim@samsung.com)
**/
MBool 
_DrvWM1808Init(void)
{
	//Set default register values
	_DrvWM1808RegMgrResetData();

	//software reset		
	//This is processed in _DrvWM1808RegMgrResetData()
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_SOFTWARE_RESET, DRV_WM1808_SOFTWARE_RESET_DEFAULT);
    _DrvWM1808MasterMute(TRUE);

	//BIASCUT=1
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_BIAS_CONTROL,
		DRV_WM1808_GLOBAL_BIAS_CTRL(DRV_WM1808_GLOBAL_BIAS_CTRL_0_5X));

	//TSDEN=TSOPCTRL=1
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_OUTPUT_CTRL, DRV_WM1808_TSOPCTRL(DRV_WM1808_TSOPCTRL_ENABLED)|
		DRV_WM1808_TSDEN(DRV_WM1808_TSDEN_ENABLED));

	//POBCTRL=1
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_VMID, DRV_WM1808_VMIDEN(DRV_WM1808_VMIDEN_ENABLED));	

	//BIASEN=VMIDSEL=1
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_POWER_MANAGEMENT1, DRV_WM1808_BIASEN(DRV_WM1808_BIASEN_ENABLED)|
		DRV_WM1808_VMIDSEL(DRV_WM1808_VMIDSEL_75K_OHM));

    //Mute all outputs and set PGAs to minimum gain, R52 to R57 = 0x140
    _DrvWM1808RegMgrSetDataEx(DRV_WM1808_LOUT1_HP_VOLUME_CTRL, DRV_WM1808_OUT1VU_L(DRV_WM1808_OUT1VU_L_UPDATE)|
        DRV_WM1808_LOUT1MUTE(DRV_WM1808_LOUT1MUTE_MUTE));

    _DrvWM1808RegMgrSetDataEx(DRV_WM1808_ROUT1_HP_VOLUME_CTRL, DRV_WM1808_OUT1VU_R(DRV_WM1808_OUT1VU_R_UPDATE)|
        DRV_WM1808_ROUT1MUTE(DRV_WM1808_ROUT1MUTE_MUTE));
    
    _DrvWM1808RegMgrSetDataEx(DRV_WM1808_LOUT2_SPK_VOLUME_CTRL, DRV_WM1808_OUT2VU_L(DRV_WM1808_OUT2VU_L_UPDATE)|
        DRV_WM1808_LOUT2MUTE(DRV_WM1808_LOUT2MUTE_MUTE));

    _DrvWM1808RegMgrSetDataEx(DRV_WM1808_ROUT2_SPK_VOLUME_CTRL, DRV_WM1808_OUT2VU_R(DRV_WM1808_OUT2VU_R_UPDATE)|
        DRV_WM1808_ROUT2MUTE(DRV_WM1808_ROUT2MUTE_MUTE));

    _DrvWM1808RegMgrSetDataEx(DRV_WM1808_OUT3_MIXER_CTRL, DRV_WM1808_OUT3MUTE(DRV_WM1808_OUT3MUTE_MUTE));

	//delay 10ms
   	/* Wait discharge */
	schedule_timeout_interruptible(msecs_to_jiffies(100));

    // Set Pll on
    _DrvWM1808RegMgrAddDataEx(DRV_WM1808_POWER_MANAGEMENT1,DRV_WM1808_PLLEN(DRV_WM1808_PLLEN_ON));

	//Set up digital interface
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_AUDIO_INTERFACE, DRV_WM1808_WL(DRV_WM1808_WL_24_BITS)|
		DRV_WM1808_FMT(DRV_WM1808_FMT_IIS_FORMAT)|DRV_WM1808_DLRSWAP(DRV_WM1808_DAC_RIGHT_PHASE_OF_LRC)|
		DRV_WM1808_ALRSWAP(DRV_WM1808_ADC_RIGHT_PHASE_OF_LRC)); 
    
	//This is processed in _DrvWM1808RegMgrResetData()
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_COMPANDING_CTRL, DRV_WM1808_VALUE_CLEARED);	
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_CLOCK_GEN_CTRL,DRV_WM1808_CLKSEL(DRV_WM1808_CLKSEL_PLL_OUTPUT));	

	//DAC control, DAC volume
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_DAC_CONTROL, DRV_WM1808_DACOSR128(DRV_WM1808_DACOSR128_128X));


	//Set PGA gain, BOOST gain, ADC control, ADC volume
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_ADC_CONTROL,DRV_WM1808_ADCOSR128(DRV_WM1808_ADCOSR128_128X));

	//Set Output volume
	_DrvWM1808MasterVolume(0x0);
 	//PORBCTRL=0
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_VMID, DRV_WM1808_VALUE_CLEARED);

   _DrvWM1808MasterMute(FALSE);

	return 1;
}

/** 
* MBool _DrvWM1808MasterVolume(long vol)
*
* This function controls output volume 
*
* @param		vol[in]long
* @return		This function returns TRUE on success, or FALSE on failure
* @author       Kim Hyun Wook 	(hw123.kim@samsung.com)
**/
MBool _DrvWM1808MasterVolume(long vol)
{
		if((vol<0)||(vol>63))
		{
			return 0;
		}
        else if(vol==0)
        {
        	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_ADC_DIGITAL_VOL,
		        DRV_WM1808_ADCVU(DRV_WM1808_ADCVU_UPDATE)|DRV_WM1808_ADCVOLL(0x0));
        	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_RIGHT_ADC_DIGITAL_VOL,
		        DRV_WM1808_ADCVU(DRV_WM1808_ADCVU_UPDATE)|DRV_WM1808_ADCVOLR(0x0));
        	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_DAC_DIGITAL_VOL,
		        DRV_WM1808_DACVU(DRV_WM1808_DACVU_UPDATE)|DRV_WM1808_DACVOLL(0x0));
        	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_RIGHT_DAC_DIGITAL_VOL,
	        	DRV_WM1808_DACVU(DRV_WM1808_DACVU_UPDATE)|DRV_WM1808_DACVOLR(0x0));
		    _DrvWM1808RegMgrSetDataEx(DRV_WM1808_LOUT1_HP_VOLUME_CTRL,
			    DRV_WM1808_OUT1VU_L(DRV_WM1808_OUT1VU_L_UPDATE)|
			    DRV_WM1808_LOUT1ZC(DRV_WM1808_LOUT1ZC_ZC_ONLY)|
			    DRV_WM1808_LOUT1MUTE(DRV_WM1808_LOUT1MUTE_MUTE)|
			    DRV_WM1808_LOUT1VOL(vol));
		    _DrvWM1808RegMgrSetDataEx(DRV_WM1808_ROUT1_HP_VOLUME_CTRL,
			    DRV_WM1808_OUT1VU_R(DRV_WM1808_OUT1VU_R_UPDATE)|
			    DRV_WM1808_ROUT1ZC(DRV_WM1808_ROUT1ZC_ZC_ONLY)|
			    DRV_WM1808_ROUT1MUTE(DRV_WM1808_ROUT1MUTE_MUTE)|
			    DRV_WM1808_ROUT1VOL(vol));
	        return 1;
        }
        else
        {
	        _DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_ADC_DIGITAL_VOL,
		        DRV_WM1808_ADCVU(DRV_WM1808_ADCVU_UPDATE)|DRV_WM1808_ADCVOLL(0xFF));
	        _DrvWM1808RegMgrSetDataEx(DRV_WM1808_RIGHT_ADC_DIGITAL_VOL,
		        DRV_WM1808_ADCVU(DRV_WM1808_ADCVU_UPDATE)|DRV_WM1808_ADCVOLR(0xFF)); 
            _DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_DAC_DIGITAL_VOL,
		        DRV_WM1808_DACVU(DRV_WM1808_DACVU_UPDATE)|DRV_WM1808_DACVOLL(0xFF));
        	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_RIGHT_DAC_DIGITAL_VOL,
		        DRV_WM1808_DACVU(DRV_WM1808_DACVU_UPDATE)|DRV_WM1808_DACVOLR(0xFF));
		    _DrvWM1808RegMgrSetDataEx(DRV_WM1808_LOUT1_HP_VOLUME_CTRL,
			    DRV_WM1808_OUT1VU_L(DRV_WM1808_OUT1VU_L_UPDATE)|
			    DRV_WM1808_LOUT1ZC(DRV_WM1808_LOUT1ZC_ZC_ONLY)|
			    DRV_WM1808_LOUT1MUTE(DRV_WM1808_LOUT1MUTE_NORMAL)|
			    DRV_WM1808_LOUT1VOL(vol));
		    _DrvWM1808RegMgrSetDataEx(DRV_WM1808_ROUT1_HP_VOLUME_CTRL,
			    DRV_WM1808_OUT1VU_R(DRV_WM1808_OUT1VU_R_UPDATE)|
			    DRV_WM1808_ROUT1ZC(DRV_WM1808_ROUT1ZC_ZC_ONLY)|
			    DRV_WM1808_ROUT1MUTE(DRV_WM1808_ROUT1MUTE_NORMAL)|
			    DRV_WM1808_ROUT1VOL(vol));
	        return 1;
        }
}

/** 
* MBool _DrvWM1808MasterMute(MBool flag)
*
* This function controls mute. 
*
* @param		flag[in]MBool
* @return		This function returns TRUE on success, or FALSE on failure
* @author       Kim Hyun Wook 	(hw123.kim@samsung.com)
**/
MBool _DrvWM1808MasterMute(MBool flag)
{
#if 0
	BITCSET(HwPORTCFG13, Hw4-Hw0, HwPORTCFG13_HPCSN(0x01));
	BITSET(HwGPFEN, Hw20);
	if(flag!=0)
	{
		BITCLR(HwGPFDAT, Hw20);	//mute on
	}
	else
	{
		BITSET(HwGPFDAT, Hw20);	//mute off
	}
//	mdelay(60);
#endif 
    //GPIO 초기화 코드 
    /* POWER    : input, AUD3_BB_FS */
    mxc_request_iomux(MX37_PIN_AUD3_BB_FS, IOMUX_CONFIG_ALT4);  /* AL4 => GPIO2_17 */
    mxc_set_gpio_direction(MX37_PIN_AUD3_BB_FS, 0); /* output */
    
    //GPIO 값 설정
	if(flag==TRUE)
	{
     	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_INP_PGA_GAIN_CTRL,
		DRV_WM1808_INPPGAU(DRV_WM1808_INPPGAU_UPDATE)|DRV_WM1808_INPPGAMUTEL(DRV_WM1808_INPPGAMUTEL_MUTED)|DRV_WM1808_INPPGAVOLL(0x00));
     	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_ADC_BOOST_CTRL, DRV_WM1808_PGABOOSTL(DRV_WM1808_PGABOOSTL_20DB_GAIN)|
	    	DRV_WM1808_L2_2BOOSTVOL(0x0)|DRV_WM1808_AUXL2BOOSTVOL(0x0));
       	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_RIGHT_ADC_BOOST_CTRL, DRV_WM1808_AUXR2BOOSTVOL(0x0));

       	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_ADC_DIGITAL_VOL,
	        DRV_WM1808_ADCVU(DRV_WM1808_ADCVU_UPDATE)|DRV_WM1808_ADCVOLL(0x0));
       	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_RIGHT_ADC_DIGITAL_VOL,
	        DRV_WM1808_ADCVU(DRV_WM1808_ADCVU_UPDATE)|DRV_WM1808_ADCVOLR(0x0));	
       	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_DAC_DIGITAL_VOL,
	        DRV_WM1808_DACVU(DRV_WM1808_DACVU_UPDATE)|DRV_WM1808_DACVOLL(0x0));
       	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_RIGHT_DAC_DIGITAL_VOL,
        	DRV_WM1808_DACVU(DRV_WM1808_DACVU_UPDATE)|DRV_WM1808_DACVOLR(0x0));    	
        mxc_set_gpio_dataout(MX37_PIN_AUD3_BB_FS, 0); /* set low --> mute on */
        mdelay(60);
        printk("\nMute on & Mute Delay 60ms...\n");        
  	}
    else
    {
    	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_INP_PGA_GAIN_CTRL,
		DRV_WM1808_INPPGAU(DRV_WM1808_INPPGAU_UPDATE)|DRV_WM1808_INPPGAVOLL(0x10));
    	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_ADC_BOOST_CTRL, DRV_WM1808_PGABOOSTL(DRV_WM1808_PGABOOSTL_20DB_GAIN)|
	    	DRV_WM1808_L2_2BOOSTVOL(0x07)|DRV_WM1808_AUXL2BOOSTVOL(0x07));
       	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_RIGHT_ADC_BOOST_CTRL, DRV_WM1808_AUXR2BOOSTVOL(0x07));

        _DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_ADC_DIGITAL_VOL,
	        DRV_WM1808_ADCVU(DRV_WM1808_ADCVU_UPDATE)|DRV_WM1808_ADCVOLL(0xFF));
        _DrvWM1808RegMgrSetDataEx(DRV_WM1808_RIGHT_ADC_DIGITAL_VOL,
	        DRV_WM1808_ADCVU(DRV_WM1808_ADCVU_UPDATE)|DRV_WM1808_ADCVOLR(0xFF));     
       	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_DAC_DIGITAL_VOL,
	        DRV_WM1808_DACVU(DRV_WM1808_DACVU_UPDATE)|DRV_WM1808_DACVOLL(0xFF));
       	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_RIGHT_DAC_DIGITAL_VOL,
        	DRV_WM1808_DACVU(DRV_WM1808_DACVU_UPDATE)|DRV_WM1808_DACVOLR(0xFF));
        mxc_set_gpio_dataout(MX37_PIN_AUD3_BB_FS, 1); /* set high --> mute off */
        printk("\nMute off...\n");        
    }
    
    //GPIO 종료시
    mxc_free_iomux(MX37_PIN_AUD3_BB_FS, IOMUX_CONFIG_ALT4);
	return 1;
}

/** 
* MBool _DrvWM1808MasterMute(MBool flag)
*
* This function switches controls(playback, mic record, FM record, BT) using a set of value in the structure.
*
* @return		This function returns TRUE on success, or FALSE on failure
* @author       Kim Hyun Wook 	(hw123.kim@samsung.com)
**/
MBool _DrvWM1808Switch(void)
{
	//MUTE ON, delay 60ms
    //_DrvWM1808MasterMute(TRUE);
   
	//Enable output, BOOST, ADC, Disable PGA 
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_POWER_MANAGEMENT2,
		DRV_WM1808_ROUT1_EN(pAudDrvSettingMgr->regROUT1)|
		DRV_WM1808_LOUT1_EN(pAudDrvSettingMgr->regLOUT1)|
		DRV_WM1808_BOOSTENL(pAudDrvSettingMgr->regLBOOST)|
		DRV_WM1808_BOOSTENR(pAudDrvSettingMgr->regRBOOST)|
		DRV_WM1808_INPPGAENL(pAudDrvSettingMgr->regPGA)|
		DRV_WM1808_ADCENL(pAudDrvSettingMgr->regLADC)|
		DRV_WM1808_ADCENR(pAudDrvSettingMgr->regRADC));
        
	//MUTE OFF

	//Enable mixer, Disable DAC
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_POWER_MANAGEMENT3,
		DRV_WM1808_RMIXEN(pAudDrvSettingMgr->regRMIX)|DRV_WM1808_LMIXEN(pAudDrvSettingMgr->regLMIX)|
		DRV_WM1808_DACENL(pAudDrvSettingMgr->regLDAC)|DRV_WM1808_DACENR(pAudDrvSettingMgr->regRDAC));

	//Set path PGA
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_INP_CONTROL,
		DRV_WM1808_LIN2INPPGA(pAudDrvSettingMgr->regLIN2PGA)|
		DRV_WM1808_LIP2INPPGA(pAudDrvSettingMgr->regLIP2PGA));
	
	//Set path mixer
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_LEFT_MIXER_CTRL,
		DRV_WM1808_AUXLMIXVOL(pAudDrvSettingMgr->regAUXLMIXvol)|
		DRV_WM1808_AUXL2LMIX(pAudDrvSettingMgr->regAUXL2LMIX)|
		DRV_WM1808_BYPLMIXVOL(pAudDrvSettingMgr->regBYPLMIXvol)|
		DRV_WM1808_BYPL2LMIX(pAudDrvSettingMgr->regBYPL2LMIX)|
		DRV_WM1808_DACL2LMIX(pAudDrvSettingMgr->regDACL2LMIX));


	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_RIGHT_MIXER_CTRL,
		DRV_WM1808_AUXRMIXVOL(pAudDrvSettingMgr->regAUXRMIXvol)|
		DRV_WM1808_AUXR2RMIX(pAudDrvSettingMgr->regAUXR2RMIX)|
		DRV_WM1808_DACR2RMIX(pAudDrvSettingMgr->regDACR2RMIX));
    
	_DrvWM1808RegMgrSetDataEx(DRV_WM1808_BEEP_CONTROL, DRV_WM1808_BYPL2RMIX(pAudDrvSettingMgr->regBYPL2RMIX));

    //_DrvWM1808MasterMute(FALSE);
    return 1;
}


MBool 
_DrvWM1808Deinit()
{

    _DrvWM1808RegMgrSetDataEx(DRV_WM1808_OUTPUT_CTRL, DRV_WM1808_TSDEN(DRV_WM1808_TSDEN_DISABLED)|
                                                           DRV_WM1808_TSOPCTRL(DRV_WM1808_TSOPCTRL_DISABLED));
    
     _DrvWM1808RegMgrSetDataEx (DRV_WM1808_POWER_MANAGEMENT1,DRV_WM1808_VMIDSEL(DRV_WM1808_VMIDSEL_OFF));
    
   	/* Wait discharge */
	schedule_timeout_interruptible(msecs_to_jiffies(1000));

    // release code here
    _DrvWM1808RegMgrSetDataEx (DRV_WM1808_POWER_MANAGEMENT1,DRV_WM1808_POWER_MANAGEMENT1_DEFAULT);
    _DrvWM1808RegMgrSetDataEx (DRV_WM1808_POWER_MANAGEMENT2,DRV_WM1808_POWER_MANAGEMENT2_DEFAULT);
    _DrvWM1808RegMgrSetDataEx (DRV_WM1808_POWER_MANAGEMENT3,DRV_WM1808_POWER_MANAGEMENT3_DEFAULT);
    
    return TRUE;
}



