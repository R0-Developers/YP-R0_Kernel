/*
 * wm1808.c  --  Audio Driver for Wolfson WM1808
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
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/platform_device.h>
#include <sound/driver.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>
#include <sound/tlv.h>

#include "../../i2c/wm1808/wm1808.h"
#include "DrvWM1808RegMgr.h"
#include "DrvWM1808.h"

#define AUDIO_NAME "WM1808 ALSA"
#define WM1808_VERSION "4.0"

/*
 * Debug
 */
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

// dong yoon park modified private data
typedef struct 
{
	unsigned int playback_switch;
	long		 volume_value;
	unsigned int mic_capture_switch;
	unsigned int fm_capture_switch;
	int master_mute;	
	unsigned int handsfree_switch;
}Wm1808PrivInfo;

static u32 wm1808_codec_read(struct snd_soc_codec *codec, 
	unsigned int reg)
{
    return _DrvWM1808RegMgrGetData(reg,DRV_WM1808_VALUE);
}

static int wm1808_codec_write(struct snd_soc_codec *codec, unsigned int reg,
	unsigned int value)
{
    return _DrvWM1808RegMgrSetDataEx(reg,value);
}


//*****************************************************************************************************
// Samsung control define
static int wm1808_pcm_playback_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
    return 0;
}

static int wm1808_pcm_playback_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	ucontrol->value.integer.value[0] = ((Wm1808PrivInfo*) codec->private_data)->playback_switch;
   return 0;
}

static int wm1808_pcm_playback_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
    ((Wm1808PrivInfo*) codec->private_data)->playback_switch=ucontrol->value.integer.value[0];
    ((Wm1808PrivInfo*) codec->private_data)->mic_capture_switch = 0;
    ((Wm1808PrivInfo*) codec->private_data)->fm_capture_switch = 0;
    ((Wm1808PrivInfo*) codec->private_data)->handsfree_switch = 0;
	
	_DrvWM1808SetCodecObj(codec);
	_DrvWM1808InitState(AV_ADEVMGR_CODEC_STATUS_PLAY);
	
	dbg("wm1808_pcm_playback_pud_value=%d",(int)((Wm1808PrivInfo*) codec->private_data)->playback_switch);
    return 0;
}

static int wm1808_master_volume_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1000;
    return 0;
}
static int wm1808_master_volume_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	ucontrol->value.integer.value[0] = ((Wm1808PrivInfo*) codec->private_data)->volume_value;
	
    return 0;
}

static int wm1808_master_volume_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
    ((Wm1808PrivInfo*) codec->private_data)->volume_value=ucontrol->value.integer.value[0];
  
	_DrvWM1808SetCodecObj(codec);
	_DrvWM1808MasterVolume(((Wm1808PrivInfo*) codec->private_data)->volume_value);
	
	dbg("wm1808_master_volume_put value=%d", (int)((Wm1808PrivInfo*) codec->private_data)->volume_value);	
    return 0;
}

static int wm1808_capture_fm_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
    return 0;
}
static int wm1808_capture_fm_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	ucontrol->value.integer.value[0] = ((Wm1808PrivInfo*) codec->private_data)->fm_capture_switch;
	
    return 0;
}

static int wm1808_capture_fm_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
    ((Wm1808PrivInfo*) codec->private_data)->playback_switch=0;
    ((Wm1808PrivInfo*) codec->private_data)->mic_capture_switch = 0;
    ((Wm1808PrivInfo*) codec->private_data)->fm_capture_switch = ucontrol->value.integer.value[0];
    ((Wm1808PrivInfo*) codec->private_data)->handsfree_switch = 0;
    
	_DrvWM1808SetCodecObj(codec);
	_DrvWM1808InitState(AV_ADEVMGR_CODEC_STATUS_FM);
    
	dbg("wm1808_capture_fm_put value=%d",(int) ((Wm1808PrivInfo*) codec->private_data)->fm_capture_switch );	
    return 0;
}

static int wm1808_capture_mic_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
    return 0;
}
static int wm1808_capture_mic_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	ucontrol->value.integer.value[0] = ((Wm1808PrivInfo*) codec->private_data)->mic_capture_switch;

    return 0;
}

static int wm1808_capture_mic_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
    ((Wm1808PrivInfo*) codec->private_data)->playback_switch=0;
    ((Wm1808PrivInfo*) codec->private_data)->mic_capture_switch = ucontrol->value.integer.value[0];
    ((Wm1808PrivInfo*) codec->private_data)->fm_capture_switch = 0;
    ((Wm1808PrivInfo*) codec->private_data)->handsfree_switch = 0;
    
	_DrvWM1808SetCodecObj(codec);
	_DrvWM1808InitState(AV_ADEVMGR_CODEC_STATUS_RECORD);

	dbg("wm1808_capture_mic_put value=%d",(int) ((Wm1808PrivInfo*) codec->private_data)->mic_capture_switch );	
    return 0;
}

static int wm1808_master_mute_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
    return 0;
}
static int wm1808_master_mute_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	ucontrol->value.integer.value[0] = ((Wm1808PrivInfo*) codec->private_data)->master_mute;
    return 0;
}

static int wm1808_master_mute_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	((Wm1808PrivInfo*) codec->private_data)->master_mute = ucontrol->value.integer.value[0];
	
	 _DrvWM1808MasterMute(((Wm1808PrivInfo*) codec->private_data)->master_mute);
	
	dbg("wm1808_master_mute_put value=%d",(int)((Wm1808PrivInfo*) codec->private_data)->master_mute);	
    return 0;
}

static int wm1808_handsfree_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
    return 0;
}
static int wm1808_handsfree_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
	ucontrol->value.integer.value[0] = ((Wm1808PrivInfo*) codec->private_data)->handsfree_switch;
    return 0;
}

static int wm1808_handsfree_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);
    ((Wm1808PrivInfo*) codec->private_data)->playback_switch=0;
    ((Wm1808PrivInfo*) codec->private_data)->mic_capture_switch =0;
    ((Wm1808PrivInfo*) codec->private_data)->fm_capture_switch = 0;
    ((Wm1808PrivInfo*) codec->private_data)->handsfree_switch = ucontrol->value.integer.value[0];

    // TODO: implement later

	dbg("wm1808_handsfree_put value=%d",(int)((Wm1808PrivInfo*) codec->private_data)->handsfree_switch);	
    return 0;
}

static const struct snd_kcontrol_new wm1808_snd_controls[] = {
{
    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
    .name = "PCM PlayBack Switch",
    .info = wm1808_pcm_playback_info,
    .get  = wm1808_pcm_playback_get,
    .put  = wm1808_pcm_playback_put,
    .private_value = 0xffff,
},
{
    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
    .name = "Master Volume",
    .info = wm1808_master_volume_info,
    .get  = wm1808_master_volume_get,
    .put  = wm1808_master_volume_put,
    .private_value = 0xffff,
},
{
    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
    .name = "Capture Mic Switch",
    .info = wm1808_capture_mic_info,
    .get  = wm1808_capture_mic_get,
    .put  = wm1808_capture_mic_put,
    .private_value = 0xffff,
},
{
    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
    .name = "Capture FM Switch",
    .info = wm1808_capture_fm_info,
    .get  = wm1808_capture_fm_get,
    .put  = wm1808_capture_fm_put,
    .private_value = 0xffff,
},
{
    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
    .name = "Master Mute",
    .info = wm1808_master_mute_info,
    .get  = wm1808_master_mute_get,
    .put  = wm1808_master_mute_put,
    .private_value = 0xffff,
},
{
    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
    .name = "Master Handfree Switch",
    .info = wm1808_handsfree_info,
    .get  = wm1808_handsfree_get,
    .put  = wm1808_handsfree_put,
    .private_value = 0xffff,
},


};


//==========================================================================================
// add controls only

/* add non dapm controls */
static int wm1808_add_controls(struct snd_soc_codec *codec, 
	struct snd_card *card)
{
	int err, i;

	for (i = 0; i < ARRAY_SIZE(wm1808_snd_controls); i++) {
		err = snd_ctl_add(card,
				snd_soc_cnew(&wm1808_snd_controls[i], 
					codec, NULL));
		if (err < 0)
			return err;
	}
	return 0;
}
static const char *audio_map[][3] = {

	/* terminator */
	{NULL, NULL, NULL},
};

static int wm1808_add_widgets(struct snd_soc_codec *codec, 
	struct snd_soc_machine *machine)
{
	int i;

	/* set up audio path audio_mapnects */
	for(i = 0; audio_map[i][0] != NULL; i++) {
		snd_soc_dapm_connect_input(machine, audio_map[i][0],
			audio_map[i][1], audio_map[i][2]);
	}

	snd_soc_dapm_new_widgets(machine);
	return 0;
}

/* assume sysclk=12.288MHz, mclk=66.5MHz */
static int wm1808_set_dai_sysclk(struct snd_soc_dai *codec_dai,
		int clk_id, unsigned int freq, int dir)
{
    dbg("%s: set clk source to %s", __func__,clk_id==WM1808_MCLK_SEL_PLL ? "PLL" : "MCLK");

    if (clk_id == WM1808_MCLK_SEL_PLL)
    {
        _DrvWM1808RegMgrAddDataEx(DRV_WM1808_CLOCK_GEN_CTRL,DRV_WM1808_CLKSEL(DRV_WM1808_CLKSEL_PLL_OUTPUT));
    }
	else
    {   
        _DrvWM1808RegMgrClearDataEx(DRV_WM1808_CLOCK_GEN_CTRL,DRV_WM1808_CLKSEL(DRV_WM1808_CLKSEL_PLL_OUTPUT));        
    }
    
	return 0;
}

static int wm1808_set_pll(struct snd_soc_dai *codec_dai,
		int pll_id, unsigned int freq_in, unsigned int freq_out)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	struct wm1808* wm1808 = codec->control_data;
	u16 data;
	printk("%s: set sysclk to %d\n", __func__, freq_out);
#if 0
	if (freq_out == 12288000){
		/* PLL prescale=0, PLLN=12 */
		data = 0x0C;
		wm1808_reg_write(wm1808, WM1808_PLL_N, data);
		/* k = 0x49BA5E */
		data = 0x12;
		wm1808_reg_write(wm1808, WM1808_PLL_K_1, data);
		data = 0xDD;
		wm1808_reg_write(wm1808, WM1808_PLL_K_2, data);
		data = 0x5E;
		wm1808_reg_write(wm1808, WM1808_PLL_K_3, data);
	} else if(freq_out == 11289600){
		/* PLL prescale=0, PLLN=11 */
		data = 0x0B;
		wm1808_reg_write(wm1808, WM1808_PLL_N, data);
		/* k = 0x4A2339 */
		data = 0x12;
		wm1808_reg_write(wm1808, WM1808_PLL_K_1, data);
		data = 0x111;
		wm1808_reg_write(wm1808, WM1808_PLL_K_2, data);
		data = 0x139;
		wm1808_reg_write(wm1808, WM1808_PLL_K_3, data);
	} else if(freq_out == 1){
		/* Enable PLL */
		wm1808_set_bits(wm1808, WM1808_POWER_MGMT_1, WM1808_PLLEN);
	} else if(freq_out == 0) {
		wm1808_clear_bits(wm1808, WM1808_POWER_MGMT_1, WM1808_PLLEN);
 	} else
		printk("%s: can not support sysclk=%d\n", __func__, freq_out);
		
	return 0;
#endif
	dbg("%s: set sysclk to %d", __func__, freq_out);

	if (freq_out == 12288000){
		/* PLL prescale=0, PLLN=0x7 */

        _DrvWM1808RegMgrSetDataEx(DRV_WM1808_PLL_N,DRV_WM1808_PLLPRESCALE(DRV_WM1808_PLLPRESCALE_MCLK_NOT_DIV)|
                                                        DRV_WM1808_PLLN_3_TO_0(0x0C));
  
        _DrvWM1808RegMgrSetDataEx(DRV_WM1808_PLL_K1,DRV_WM1808_PLLK_23_TO_18(0x12));
        _DrvWM1808RegMgrSetDataEx(DRV_WM1808_PLL_K2,DRV_WM1808_PLLK_17_TO_9(0xDD));
        _DrvWM1808RegMgrSetDataEx(DRV_WM1808_PLL_K3,DRV_WM1808_PLLK_8_TO_0(0x5E));
    
	} else if(freq_out == 11289600){

         _DrvWM1808RegMgrSetDataEx(DRV_WM1808_PLL_N,DRV_WM1808_PLLPRESCALE(DRV_WM1808_PLLPRESCALE_MCLK_NOT_DIV)|
                                                        DRV_WM1808_PLLN_3_TO_0(0x0B));
  
        _DrvWM1808RegMgrSetDataEx(DRV_WM1808_PLL_K1,DRV_WM1808_PLLK_23_TO_18(0x12));
        _DrvWM1808RegMgrSetDataEx(DRV_WM1808_PLL_K2,DRV_WM1808_PLLK_17_TO_9(0x111));
        _DrvWM1808RegMgrSetDataEx(DRV_WM1808_PLL_K3,DRV_WM1808_PLLK_8_TO_0(0x139));

	} else if(freq_out == 1){
		/* Enable PLL */
		
	} else if(freq_out == 0) {
	
 	} else
		dbg("%s: can not support sysclk=%d", __func__, freq_out);
		
	return 0;
}

static int wm1808_set_clkdiv(struct snd_soc_dai *codec_dai, 
	int div_id, int div)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	u16 val;

	dbg("%s", __func__);
	switch (div_id) {
	case  WM1808_ADC_CLKDIV:
		val = wm1808_codec_read(codec, WM1808_ADC_CTRL) &
			~WM1808_ADCOSR128;    
		wm1808_codec_write(codec, WM1808_ADC_CTRL, val | div);
	break;
	case WM1808_DAC_CLKDIV:
		val = wm1808_codec_read(codec, WM1808_DAC_CTRL) &
			~WM1808_DACOSR128;    
		wm1808_codec_write(codec, WM1808_DAC_CTRL, val | div);
	break;
	case WM1808_BCLK_CLKDIV:
		val = wm1808_codec_read(codec, WM1808_CLK_GEN_CTRL) &
			~WM1808_BCLKDIV_MASK;    
		wm1808_codec_write(codec, WM1808_CLK_GEN_CTRL, val | div);
	break;
	case WM1808_OPCLK_CLKDIV:
		val = wm1808_codec_read(codec, WM1808_GPIO_STUFF) &
			~WM1808_OPCLKDIV_MASK;    
		wm1808_codec_write(codec, WM1808_GPIO_STUFF, val | div);
	break;
	case WM1808_MCLK_CLKDIV:
		val = wm1808_codec_read(codec, WM1808_CLK_GEN_CTRL) &
			~WM1808_MCLKDIV_MASK;    
		wm1808_codec_write(codec, WM1808_CLK_GEN_CTRL, val | div);
	case WM1808_SR:
		val = wm1808_codec_read(codec, WM1808_ADDITIONAL_CTRL) &
			~WM1808_SR_MASK;
		wm1808_codec_write(codec, WM1808_ADDITIONAL_CTRL, val | div);
		
	break;
	default:
		return -EINVAL;
	}
	
	return 0;
}

static int wm1808_set_dai_fmt(struct snd_soc_dai *codec_dai,
		unsigned int fmt)
{
	struct snd_soc_codec *codec = codec_dai->codec;
	u16 iface = wm1808_codec_read(codec, WM1808_AUDIO_INTERFACE) &
		~(WM1808_BCP | WM1808_LRP | WM1808_FMT_MASK);
	u16 master = wm1808_codec_read(codec, WM1808_CLK_GEN_CTRL) &
		~WM1808_MS;
	dbg("%s", __func__);
	/* set master/slave audio interface */
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
		dbg("%s: wm1808 is clk master", __func__);
		master |= WM1808_MS;
		break;
	case SND_SOC_DAIFMT_CBS_CFS:
        dbg("%s: wm1808 is clk slave", __func__);		
		break;
	default:
		return -EINVAL;
	}

	/* interface format */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		iface |= 0x2 << 3;
		break;
	case SND_SOC_DAIFMT_RIGHT_J:
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		iface |= 0x1 << 3;
		break;
	case SND_SOC_DAIFMT_DSP_A:
		iface |= 0x3 << 3;
		break;
	case SND_SOC_DAIFMT_DSP_B:
		iface |= 0x3 << 3; // lg not sure which mode
		break;
	default:
		return -EINVAL;
	}

	/* clock inversion */
	switch (fmt & SND_SOC_DAIFMT_INV_MASK) {
	case SND_SOC_DAIFMT_NB_NF:
		break;
	case SND_SOC_DAIFMT_IB_IF:
		iface |= WM1808_BCP | WM1808_LRP;
		break;
	case SND_SOC_DAIFMT_IB_NF:
		iface |= WM1808_BCP;
		break;
	case SND_SOC_DAIFMT_NB_IF:
		iface |= WM1808_LRP;
		break;
	default:
		return -EINVAL;
	}
    // change DrvManager Code 
	wm1808_codec_write(codec, WM1808_AUDIO_INTERFACE, iface);
	wm1808_codec_write(codec, WM1808_CLK_GEN_CTRL, master);
	return 0;
}

static int wm1808_pcm_startup(struct snd_pcm_substream *substream)
{
	return 0;
}

void wm1808_pcm_shutdown(struct snd_pcm_substream *substream)
{
	return ;
}

static int wm1808_pcm_trigger(struct snd_pcm_substream *substream,int cmd)
{
	return 0;
}

static int wm1808_pcm_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params)
{
	struct snd_soc_pcm_link *pcm_link = substream->private_data;
	struct snd_soc_codec *codec = pcm_link->codec;
	u16 iface = wm1808_codec_read(codec, WM1808_AUDIO_INTERFACE) &
		~WM1808_WL_MASK; 
	dbg("%s", __func__);
	/* bit size */
	switch (params_format(params)) {
	case SNDRV_PCM_FORMAT_S16_LE:
		break;
	case SNDRV_PCM_FORMAT_S20_3LE:
		iface |= 0x1 << 5;
		break;
	case SNDRV_PCM_FORMAT_S24_LE:
		iface |= 0x2 << 5;
		break;
	case SNDRV_PCM_FORMAT_S32_LE:
		iface |= 0x3 << 5;
		break;
	}

	wm1808_codec_write(codec, WM1808_AUDIO_INTERFACE, iface);
	return 0;
}

#define WM1808_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 | \
		SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 | \
        SNDRV_PCM_RATE_44100 | SNDRV_PCM_RATE_32000 | \
        SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_88200 | \
        SNDRV_PCM_RATE_96000)


#define WM1808_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE |\
	SNDRV_PCM_FMTBIT_S24_LE)

static const struct snd_soc_pcm_stream wm1808_hifi_dai_playback = {
	.stream_name	= "Playback",
	.channels_min	= 2,
	.channels_max	= 2,
	.rates		= WM1808_RATES,
	.formats	= WM1808_FORMATS,
};

static const struct snd_soc_pcm_stream wm1808_hifi_dai_capture = {
	.stream_name	= "Capture",
	.channels_min	= 1,
	.channels_max	= 2,
	.rates		= WM1808_RATES,
	.formats	= WM1808_FORMATS,
};

/* dai ops, called by machine drivers */
static const struct snd_soc_dai_ops wm1808_hifi_dai_ops = {
	.set_fmt = wm1808_set_dai_fmt,
	.set_sysclk = wm1808_set_dai_sysclk,
	.set_pll = wm1808_set_pll,
	.set_clkdiv = wm1808_set_clkdiv,
};

/* audio ops, called by alsa */
static const struct snd_soc_ops wm1808_hifi_dai_audio_ops = {
	.startup = wm1808_pcm_startup,
	.shutdown = wm1808_pcm_shutdown,
	.hw_params = wm1808_pcm_hw_params,
	.trigger = wm1808_pcm_trigger,
};

static int wm1808_suspend(struct device *dev, pm_message_t state)
{
	dbg("%s: not implemented", __func__);
	return 0;
}

static int wm1808_resume(struct device *dev)
{
	dbg("%s: not implemented", __func__);
	return 0;
}

static int wm1808_codec_io_probe(struct snd_soc_codec *codec,
	struct snd_soc_machine *machine)
{
	dbg("%s", __func__);

    // samsung codec init driver
   	_DrvWM1808SetCodecObj(codec);
	_DrvWM1808Init();

    // add widgets 
	wm1808_add_controls(codec, machine->card);
	wm1808_add_widgets(codec, machine);    
	return 0;
}

static int wm1808_codec_io_remove(struct snd_soc_codec *codec,
	struct snd_soc_machine *machine)
{
	dbg("%s", __func__);

    _DrvWM1808Deinit();
	
	return 0;	
}

static const struct snd_soc_codec_ops wm1808_codec_ops = {
	.read		= wm1808_codec_read,
	.write		= wm1808_codec_write,
	.io_probe	= wm1808_codec_io_probe,
	.io_remove	= wm1808_codec_io_remove,
};

static int wm1808_codec_probe(struct device *dev)
{
	struct snd_soc_codec *codec = to_snd_soc_codec(dev);
    Wm1808PrivInfo *pWm1808PrivInfo;

	info("WM1808 Audio Codec %s", WM1808_VERSION);
    dbg("%s: init codec",__func__);

	pWm1808PrivInfo = kzalloc(sizeof(Wm1808PrivInfo), GFP_KERNEL);
	if (pWm1808PrivInfo == NULL)
		return -ENOMEM;

	codec->owner = THIS_MODULE;
	codec->ops = &wm1808_codec_ops;
	codec->reg_cache_size = WM1808_MAX_REGISTER;
	codec->reg_cache_step = 1;
	codec->private_data = pWm1808PrivInfo;

 	snd_soc_register_codec(codec);
	return 0;
}

static int wm1808_codec_remove(struct device *dev)
{
	struct snd_soc_codec *codec = to_snd_soc_codec(dev);
   	kfree(codec->private_data);
	return 0;
}

static int wm1808_hifi_dai_probe(struct device *dev)
{
	struct snd_soc_dai *dai = to_snd_soc_dai(dev);
		
	dai->ops = &wm1808_hifi_dai_ops;
	dai->audio_ops = &wm1808_hifi_dai_audio_ops;
	dai->capture = &wm1808_hifi_dai_capture;
	dai->playback = &wm1808_hifi_dai_playback;
	snd_soc_register_codec_dai(dai);
	return 0;
}

const char wm1808_codec[SND_SOC_CODEC_NAME_SIZE] = "wm1808-codec";
EXPORT_SYMBOL_GPL(wm1808_codec);

static struct snd_soc_device_driver wm1808_codec_driver = {
	.type	= SND_SOC_BUS_TYPE_CODEC,
	.driver	= {
		.name 		= wm1808_codec,
		.owner		= THIS_MODULE,
		.bus 		= &asoc_bus_type,
		.probe		= wm1808_codec_probe,
		.remove		= __devexit_p(wm1808_codec_remove),
		.suspend	= wm1808_suspend,
		.resume		= wm1808_resume,
	},
};

const char wm1808_hifi_dai[SND_SOC_CODEC_NAME_SIZE] = "wm1808-hifi-dai";
EXPORT_SYMBOL_GPL(wm1808_hifi_dai);

static struct snd_soc_device_driver wm1808_hifi_dai_driver = {
	.type	= SND_SOC_BUS_TYPE_DAI,
	.driver	= {
		.name 		= wm1808_hifi_dai,
		.owner		= THIS_MODULE,
		.bus 		= &asoc_bus_type,
		.probe		= wm1808_hifi_dai_probe,
	},
};

static __init int wm1808_codec_init(void)
{
	int ret = 0;
	dbg("%s: register codec driver", __func__);
	
	ret = driver_register(&wm1808_codec_driver.driver);
	if (ret < 0)
		return ret;
	ret = driver_register(&wm1808_hifi_dai_driver.driver);
	if (ret < 0) {
		driver_unregister(&wm1808_codec_driver.driver);
		return ret;
	}
	return ret;
}

static __exit void wm1808_codec_exit(void)
{
	driver_unregister(&wm1808_hifi_dai_driver.driver);
	driver_unregister(&wm1808_codec_driver.driver);
}

module_init(wm1808_codec_init);
module_exit(wm1808_codec_exit);

MODULE_DESCRIPTION("ASoC WM1808 driver");
MODULE_AUTHOR("Wang Shaojun");
MODULE_LICENSE("GPL");

