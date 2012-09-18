/*
 * as3543.c  --  Audio Driver for AS3543
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

#include "as3543.h"
#include "mx37_pins.h"
#include "../../../drivers/i2c/chips/i2c-as3543.h"
#include "../../../arch/arm/mach-mx37/mx37_pins.h"
#include "../../../arch/arm/mach-mx37/iomux.h"

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


typedef struct
{
	unsigned int 	playback_switch;
	long		 	volume_value;
	unsigned int 	mic_capture_switch;
	unsigned int 	fm_capture_switch;
	int 			master_mute;
	int 			soft_mute;
	int 			fm_mute;
	int 			power_state;
} AS3543PrivInfo;

typedef struct
{
	unsigned int uiInputMode;
	unsigned int uiOutputMode;
	long		 lVolume;
} AS3543_STATUS;


static int AS3543SetMuteExternal(int iMute);
static int AS3543SetPLL(int iMode, int iVCO, int iOSR, int iDIV);
static int AS3543SetI2SMode(int iMode);


static AS3543_STATUS g_as3543_status;
const char as3543_codec[SND_SOC_CODEC_NAME_SIZE];
const char as3543_hifi_dai[SND_SOC_DAI_NAME_SIZE];


#define AS3543_LINE_HIQ_ON		(0x01<<7) 									// Line input set to high quality mode
#define AS3543_AVDD27_2_3		(0x60)										// AVDD27: 2.3V, 200mA
#define AS3543_AVDD27_2_8		(0x68)										// AVDD27: 2.8V, 200mA
#define AS3543_AVDD27_2_9		(0x69)										// AVDD27: 2.9V, 200mA
#define AS3543_AVDD27_3_0		(0x6A)										// AVDD27: 3.0V, 200mA
#define AS3543_AVDD17_OFF		(0xA0)										// AVDD17: Off
#define AS3543_AVDD17_2_6		(0x33)										// AVDD17: 2.6
#define AS3543_AVDD17_2_7		(0x35)										// AVDD17: 2.7
#define AS3543_AVDD17_2_8		(0x37)										// AVDD17: 2.8
#define AS3543_AVDD17_2_9		(0x3b)										// AVDD17: 2.9
#define AS3543_AVDD17_3_0		(0x3d)										// AVDD17: 3.0
#define AS3543_SUM_HP_HIQ_ON	(0x01<<4)									// Mixer and Headphone stage in high quality mode
#define AS3543_GAIIN_STEP_8MS	(0x02<<2)									// 8ms: Sets the transition time of the auto fading for the output stage

#define AS3543_VOLUME_OUT		(as3543_reg_read(AFE_OUT_R) & 0x1F)
#define AS3543_VOLUME_MIC		(as3543_reg_read(AFE_MIC_R) & 0x1F)
#define AS3543_VOLUME_LINEIN	(as3543_reg_read(AFE_LINE_IN_R) & 0x1F)
#define AS3543_VOLUME_DAC		(as3543_reg_read(AFE_DAC_R) & 0x1F)
#define AS3543_VOLUME_ADC		(as3543_reg_read(AFE_ADC_R) & 0x1F)

#define AS3543_MUTE_OUTPUT(x)	x ? as3543_set_bits(AFE_OUT_L, 0x80) : as3543_clear_bits(AFE_OUT_L, 0x80)
#define AS3543_MUTE_LINEIN_R(x)	x ? as3543_clear_bits(AFE_LINE_IN_R, 0x20) : as3543_set_bits(AFE_LINE_IN_R, 0x20)
#define AS3543_MUTE_LINEIN_L(x)	x ? as3543_clear_bits(AFE_LINE_IN_L, 0x20) : as3543_set_bits(AFE_LINE_IN_L, 0x20)
#define AS3543_MUTE_DAC(x)		x ? as3543_clear_bits(AFE_DAC_L, 0x20) : as3543_set_bits(AFE_DAC_L, 0x20)
#define AS3543_MUTE_ADC(x)		x ? as3543_clear_bits(AFE_ADC_L, 0x20) : as3543_set_bits(AFE_ADC_L, 0x20)

#ifdef AS3543_USE_INTERNAL_MUTE
#define AS3543SetMute(x)		AS3543_MUTE_OUTPUT(x)
#else
#define AS3543SetMute(x)		AS3543SetMuteExternal(x)
#endif

// Power state
enum
{
	AS3543_POWER_UNKNOWN,
	AS3543_POWER_OFF,
	AS3543_POWER_IDLE,
	AS3543_POWER_SLEEP,
};

int AS3543Init(void)
{
	u8 data;

	///////////////////////////////////////////////////////////////////////////////

	AS3543SetMuteExternal(TRUE);

	///////////////////////////////////////////////////////////////////////////////

	// AVDD27: 2.8V, 200mA
	as3543_subreg_write(AFE_MUX_PMU_REG2, 0x06, AS3543_AVDD27_2_8);

	// AVDD17: 2.6V
	as3543_subreg_write(AFE_MUX_PMU_REG2, 0x07, AS3543_AVDD17_2_6);

	// PLL On
	AS3543SetPLL(AS3543_PLL_ON, AS3543_PLL_VCO_24_48, AS3543_PLL_OSR_256, AS3543_PLL_FOUT_DIV_2);

#if 0
	// Mixer: Mic off, ADC off, Line-in off, HP_BIAS: 1.5
	data = (0x01<<6) | (0x01<<3) | (0x01<<1);
#else
	// Mixer: Mic off, ADC off, Line-in off, HP_BIAS: 1.0
	data = (0x01<<6) | (0x01<<3);
#endif
	as3543_reg_write(AFE_AUDIOSET3, data);

	// Line In: High Quality, Input Line: Line 1
	data = (0x01<<7);
	as3543_reg_write(AFE_LINE_IN_R, data);

	// Input: Wave
	AS3543InputMode(g_as3543_status.uiInputMode);

	// Output: Headphone
	AS3543OutputMode(g_as3543_status.uiOutputMode);

	// STAGE On
	as3543_set_bits(AFE_OUT_L, 0x40);

	AS3543SetVolume(g_as3543_status.lVolume, TRUE);

	return 0;
}

int AS3543DeInit(void)
{
	// AVDD27: 2.3V, 200mA
	as3543_subreg_write(AFE_MUX_PMU_REG2, 0x06, AS3543_AVDD27_2_3);

	// AVDD17: Off
	as3543_subreg_write(AFE_MUX_PMU_REG2, 0x07, AS3543_AVDD17_OFF);

	// PLL Off
	AS3543SetPLL(AS3543_PLL_OFF, AS3543_PLL_VCO_24_48, AS3543_PLL_OSR_256, AS3543_PLL_FOUT_DIV_2);

	// STAGE Off
	as3543_clear_bits(AFE_OUT_L, 0x40);

	return 0;
}

int AS3543InputMode(AS3543_INPUT_TYPE eType)
{
	u8 data;


	switch(eType)
	{
		case AS3543_WAVE:
			{
				data = (0x01<<6) | (0x01<<5);							// DAC On, GST On
				as3543_reg_write(AFE_AUDIOSET1, data);

				as3543_set_bits(AFE_AUDIOSET3, 0x08);					// Mixer: Line-in off

#ifdef AS3543_DAC_ONLY
				data = (0x01<<6) | (0x02<<2);							// SUM Off, Gain Step: 8mx
				as3543_reg_write(AFE_AUDIOSET2, data);

				data = (0x01<<5) | AS3543_VOLUME_OUT;					// Input MUX type (DAC)
				as3543_reg_write(AFE_OUT_R, data);
#else
				data = AS3543_SUM_HP_HIQ_ON | AS3543_GAIIN_STEP_8MS;	// High Quality On, Gain Step: 8ms
				as3543_reg_write(AFE_AUDIOSET2, data);
#endif

				AS3543SetI2SMode(0);									// I2S Mode

				g_as3543_status.uiInputMode = AS3543_WAVE;
				AS3543SetVolume(g_as3543_status.lVolume, TRUE);

				AS3543_MUTE_DAC(FALSE);
				AS3543_MUTE_OUTPUT(TRUE);

				AS3543_MUTE_LINEIN_R(TRUE);
				AS3543_MUTE_LINEIN_L(TRUE);
			}
			break;

		case AS3543_FM:
			{
				data = (0x01<<2);										// Line-in On
				as3543_reg_write(AFE_AUDIOSET1, data);

				as3543_clear_bits(AFE_AUDIOSET3, 0x08);					// Mixer: Line-in on

#ifdef AS3543_DAC_ONLY
				data = (0x01<<6) | (0x02<<2);							// SUM Off, Gain Step: 8mx
				as3543_reg_write(AFE_AUDIOSET2, data);

				data = (0x02<<5) | AS3543_VOLUME_OUT;					// Input MUX (Line-in)
				as3543_reg_write(AFE_OUT_R, data);
#else
				data = AS3543_SUM_HP_HIQ_ON | AS3543_GAIIN_STEP_8MS;	// High Quality On, Gain Step: 8ms
				as3543_reg_write(AFE_AUDIOSET2, data);
#endif

				data = AS3543_LINE_HIQ_ON | AS3543_VOLUME_LINEIN;		// Line-In High Quality, Select Line 1
				as3543_reg_write(AFE_LINE_IN_R, data);


				g_as3543_status.uiInputMode = AS3543_FM;
				AS3543SetVolume(g_as3543_status.lVolume, TRUE);

				AS3543_MUTE_DAC(TRUE);
				AS3543_MUTE_OUTPUT(FALSE);

				AS3543_MUTE_LINEIN_R(TRUE);
				AS3543_MUTE_LINEIN_L(TRUE);
			}
			break;

		default:
			break;
	}

	return 0;
}

int AS3543OutputMode(AS3543_OUTPUT_TYPE eType)
{
	switch(eType)
	{
		case AS3543_HP:
			{
				as3543_clear_bits(AFE_OUT_R, 0x80);
				g_as3543_status.uiOutputMode = AS3543_HP;
			}
			break;

		case AS3543_LINE:
			{
				as3543_set_bits(AFE_OUT_R, 0x80);
				g_as3543_status.uiOutputMode = AS3543_LINE;
			}
			break;

		default:
			break;
	}


	return 0;
}

u32 AS3543SetVolume(long volume, int iRefresh)
{
	u8 uVolR, uVolL;
	u8 uTempVol, uCurrVol;


	uTempVol = (volume>>8) & 0x1F;

	if(g_as3543_status.uiInputMode == AS3543_WAVE)
	{
		uCurrVol = as3543_reg_read(AFE_DAC_R) & 0x1F;

		if((uCurrVol != uTempVol) || iRefresh)
		{
			uVolR = (as3543_reg_read(AFE_DAC_R) & ~(0x1F)) | uTempVol;
			uVolL = (as3543_reg_read(AFE_DAC_L) & ~(0x1F)) | uTempVol;

			as3543_reg_write(AFE_DAC_R, uVolR);
			as3543_reg_write(AFE_DAC_L, uVolL);
		}
	}
	else if(g_as3543_status.uiInputMode == AS3543_FM)
	{
		uCurrVol = as3543_reg_read(AFE_LINE_IN_R) & 0x1F;

		if((uCurrVol != uTempVol) || iRefresh)
		{
			uVolR = (as3543_reg_read(AFE_LINE_IN_R) & ~(0x1F)) | uTempVol;
			uVolL = (as3543_reg_read(AFE_LINE_IN_L) & ~(0x1F)) | uTempVol;

			as3543_reg_write(AFE_LINE_IN_R, uVolR);
			as3543_reg_write(AFE_LINE_IN_L, uVolL);
		}
	}


	uTempVol = volume & 0x1F;

	uVolR = (as3543_reg_read(AFE_OUT_R) & ~(0x1F)) | uTempVol;
	uVolL = (as3543_reg_read(AFE_OUT_L) & ~(0x1F)) | uTempVol;

	uVolL = volume ? uVolL & ~(0x80) : uVolL | 0x80;


	as3543_reg_write(AFE_OUT_R, uVolR);
	as3543_reg_write(AFE_OUT_L, uVolL);


	g_as3543_status.lVolume = volume;

	return 0;
}

static int AS3543SetMuteExternal(int iMute)
{
	//GPIO 초기화 코드
    /* POWER    : input, AUD3_BB_FS */
    mxc_request_iomux(MX37_PIN_AUD3_BB_FS, IOMUX_CONFIG_ALT4);  /* AL4 => GPIO2_17 */
    mxc_set_gpio_direction(MX37_PIN_AUD3_BB_FS, 0); /* output */

    //GPIO 값 설정
	if(iMute)
	{
    	mxc_set_gpio_dataout(MX37_PIN_AUD3_BB_FS, 0); /* set low --> mute on */

		mdelay(50);
        dbg("External Mute on...\n");
  	}
    else
    {
        mxc_set_gpio_dataout(MX37_PIN_AUD3_BB_FS, 1); /* set high --> mute off */
        dbg("External Mute off...\n");
    }

	mdelay(100);

    //GPIO 종료시
    mxc_free_iomux(MX37_PIN_AUD3_BB_FS, IOMUX_CONFIG_ALT4);


	return 1;
}

static int AS3543SetFMMute(int iMute)
{
	AS3543_MUTE_LINEIN_L(iMute);
	AS3543_MUTE_LINEIN_R(iMute);

	return 1;
}

static int AS3543SetPM(int iMode)
{
	switch (iMode)
	{
		case AS3543_POWER_OFF:
			AS3543SetMute(TRUE);

			AS3543DeInit();
			break;

		case AS3543_POWER_IDLE:
			AS3543Init();

			break;
	}

	return iMode;
}

static int AS3543SetI2SMode(int iMode)
{
	as3543_reg_write(AFE_DAC_IF, 0x00);

	return 0;
}

static int AS3543SetPLL(int iMode, int iVCO, int iOSR, int iDIV)
{
	u8 data;

	if(iMode == AS3543_PLL_ON)
	{
		// 24MHz Power on
		as3543_subreg_write(AFE_PLL, 0x05, 0x00);

		// Q24 and Q32k is 1mA push-pull output
		as3543_subreg_write(AFE_PLL, 0x02, 0xCC);

		// Set PLL
		data  = ((iDIV & 0x03) << 6);
		data |= ((iOSR & 0x03) << 4);
		data |= ((iVCO & 0x03) << 2);
		data |= (iMode & 0x03);

		as3543_subreg_write(AFE_PLL, 0x07, data);
	}
	else
	{
		// 24MHz Power down
		as3543_subreg_write(AFE_PLL, 0x05, 0x0C);
	}


	return 0;
}

static u32 as3543_codec_read(struct snd_soc_codec *codec, unsigned int reg)
{
	return as3543_reg_read(reg);
}

static int as3543_codec_write(struct snd_soc_codec *codec, unsigned int reg, unsigned int value)
{
	return as3543_reg_write(reg, value);
}


//*****************************************************************************************************
// Samsung control define
static int as3543_pcm_playback_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
    return 0;
}

static int as3543_pcm_playback_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	ucontrol->value.integer.value[0] = ((AS3543PrivInfo*) codec->private_data)->playback_switch;

   	return 0;
}

static int as3543_pcm_playback_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

    ((AS3543PrivInfo*) codec->private_data)->playback_switch	= ucontrol->value.integer.value[0];
    ((AS3543PrivInfo*) codec->private_data)->mic_capture_switch = 0;
    ((AS3543PrivInfo*) codec->private_data)->fm_capture_switch 	= 0;

	AS3543InputMode(AS3543_WAVE);

	dbg("wm1808_pcm_playback_pud_value=%d",(int)((AS3543PrivInfo*) codec->private_data)->playback_switch);
    return 0;
}

static int as3543_master_volume_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 0xFFFF;
    return 0;
}
static int as3543_master_volume_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	ucontrol->value.integer.value[0] = ((AS3543PrivInfo*) codec->private_data)->volume_value;

    return 0;
}

static int as3543_master_volume_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

    ((AS3543PrivInfo*) codec->private_data)->volume_value = ucontrol->value.integer.value[0];

	AS3543SetVolume(((AS3543PrivInfo*) codec->private_data)->volume_value, FALSE);

	dbg("as3543_master_volume_put value=%04X", (int)((AS3543PrivInfo*) codec->private_data)->volume_value);
    return 0;
}

static int as3543_capture_fm_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
    return 0;
}

static int as3543_capture_fm_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	ucontrol->value.integer.value[0] = ((AS3543PrivInfo*) codec->private_data)->fm_capture_switch;

    return 0;
}

static int as3543_capture_fm_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

    ((AS3543PrivInfo*) codec->private_data)->playback_switch	= 0;
    ((AS3543PrivInfo*) codec->private_data)->mic_capture_switch = 0;
    ((AS3543PrivInfo*) codec->private_data)->fm_capture_switch 	= ucontrol->value.integer.value[0];

	AS3543InputMode(AS3543_FM);

	dbg("as3543_capture_fm_put value=%d",(int) ((AS3543PrivInfo*) codec->private_data)->fm_capture_switch );
    return 0;
}

static int as3543_master_mute_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
    return 0;
}

static int as3543_master_mute_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	ucontrol->value.integer.value[0] = ((AS3543PrivInfo*) codec->private_data)->master_mute;

    return 0;
}

static int as3543_master_mute_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	((AS3543PrivInfo*) codec->private_data)->master_mute = ucontrol->value.integer.value[0];

	AS3543SetMute(((AS3543PrivInfo*) codec->private_data)->master_mute);

	dbg("as3543_master_mute_put=%s\n", (int)((AS3543PrivInfo*) codec->private_data)->master_mute ? "On" : "Off");
    return 0;
}

static int as3543_soft_mute_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
    return 0;
}

static int as3543_soft_mute_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	ucontrol->value.integer.value[0] = ((AS3543PrivInfo*) codec->private_data)->soft_mute;

    return 0;
}

static int as3543_soft_mute_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	((AS3543PrivInfo*) codec->private_data)->soft_mute = ucontrol->value.integer.value[0];

	AS3543_MUTE_OUTPUT(((AS3543PrivInfo*) codec->private_data)->soft_mute);

	if(((AS3543PrivInfo*) codec->private_data)->master_mute)
	{
		if(((AS3543PrivInfo*) codec->private_data)->soft_mute == FALSE)
		{
			as3543_master_mute_put(kcontrol, ucontrol);
		}
	}

	dbg("as3543_soft_mute_put=%s\n", (int)((AS3543PrivInfo*) codec->private_data)->soft_mute ? "On" : "Off");
    return 0;
}

static int as3543_fm_mute_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
    return 0;
}
static int as3543_fm_mute_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	ucontrol->value.integer.value[0] = ((AS3543PrivInfo*) codec->private_data)->fm_mute;

    return 0;
}

static int as3543_fm_mute_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	((AS3543PrivInfo*) codec->private_data)->fm_mute = ucontrol->value.integer.value[0];

	AS3543SetFMMute(((AS3543PrivInfo*) codec->private_data)->fm_mute);

	dbg("as3543_fm_mute_put=%s\n", (int)((AS3543PrivInfo*) codec->private_data)->fm_mute ? "On" : "Off");
    return 0;
}

static int as3543_power_info(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_info *uinfo)
{
	uinfo->type = SNDRV_CTL_ELEM_TYPE_INTEGER;
    uinfo->count = 1;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = 1;
    return 0;
}
static int as3543_power_get(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
    struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	ucontrol->value.integer.value[0] = ((AS3543PrivInfo*) codec->private_data)->power_state;

    return 0;
}

static int as3543_power_put(struct snd_kcontrol *kcontrol, struct snd_ctl_elem_value *ucontrol)
{
	struct snd_soc_codec *codec = snd_kcontrol_chip(kcontrol);

	((AS3543PrivInfo*) codec->private_data)->power_state = ucontrol->value.integer.value[0];

	AS3543SetPM(((AS3543PrivInfo*) codec->private_data)->power_state);

	dbg("as3543_power_put=%d\n", (int)((AS3543PrivInfo*) codec->private_data)->as3543_power_put);
    return 0;
}

static const struct snd_kcontrol_new as3543_snd_controls[] =
{
	{
	    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	    .name = "PCM PlayBack Switch",
	    .info = as3543_pcm_playback_info,
	    .get  = as3543_pcm_playback_get,
	    .put  = as3543_pcm_playback_put,
	    .private_value = 0xffff,
	},
	{
	    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	    .name = "Master Volume",
	    .info = as3543_master_volume_info,
	    .get  = as3543_master_volume_get,
	    .put  = as3543_master_volume_put,
	    .private_value = 0xffff,
	},
	{
	    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	    .name = "Capture FM Switch",
	    .info = as3543_capture_fm_info,
	    .get  = as3543_capture_fm_get,
	    .put  = as3543_capture_fm_put,
	    .private_value = 0xffff,
	},
	{
	    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	    .name = "Master Mute",
	    .info = as3543_master_mute_info,
	    .get  = as3543_master_mute_get,
	    .put  = as3543_master_mute_put,
	    .private_value = 0xffff,
	},
	{
	    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	    .name = "Soft Mute",
	    .info = as3543_soft_mute_info,
	    .get  = as3543_soft_mute_get,
	    .put  = as3543_soft_mute_put,
	    .private_value = 0xffff,
	},
	{
	    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	    .name = "FM Mute",
	    .info = as3543_fm_mute_info,
	    .get  = as3543_fm_mute_get,
	    .put  = as3543_fm_mute_put,
	    .private_value = 0xffff,
	},
	{
	    .iface = SNDRV_CTL_ELEM_IFACE_MIXER,
	    .name = "Master Power Switch",
	    .info = as3543_power_info,
	    .get  = as3543_power_get,
	    .put  = as3543_power_put,
	    .private_value = 0xffff,
	}

};


//==========================================================================================
// add controls only

/* add non dapm controls */
static int as3543_add_controls(struct snd_soc_codec *codec,	struct snd_card *card)
{
	int err, i;

	for (i = 0; i < ARRAY_SIZE(as3543_snd_controls); i++)
	{
		err = snd_ctl_add(card,	snd_soc_cnew(&as3543_snd_controls[i], codec, NULL));

		if (err < 0)
			return err;
	}

	return 0;
}

static const char *audio_map[][3] =
{
	/* terminator */
	{NULL, NULL, NULL},
};

static int as3543_add_widgets(struct snd_soc_codec *codec, struct snd_soc_machine *machine)
{
	int i;

	/* set up audio path audio_mapnects */
	for(i = 0; audio_map[i][0] != NULL; i++)
	{
		snd_soc_dapm_connect_input(machine, audio_map[i][0], audio_map[i][1], audio_map[i][2]);
	}

	snd_soc_dapm_new_widgets(machine);

	return 0;
}

/* assume sysclk=12.288MHz, mclk=66.5MHz */
static int as3543_set_dai_sysclk(struct snd_soc_dai *codec_dai,	int clk_id, unsigned int freq, int dir)
{
	return 0;
}

static int as3543_set_pll(struct snd_soc_dai *codec_dai, int pll_id, unsigned int freq_in, unsigned int freq_out)
{
	return 0;
}

static int as3543_set_clkdiv(struct snd_soc_dai *codec_dai,	int div_id, int div)
{
	return 0;
}

static int as3543_set_dai_fmt(struct snd_soc_dai *codec_dai, unsigned int fmt)
{
	return 0;
}

static int as3543_pcm_startup(struct snd_pcm_substream *substream)
{
//	Move to muon platform
//	AS3543_MUTE_OUTPUT(FALSE);

	return 0;
}

void as3543_pcm_shutdown(struct snd_pcm_substream *substream)
{
	if(g_as3543_status.uiInputMode == AS3543_FM)
		return;

	AS3543_MUTE_OUTPUT(TRUE);

	return;
}

static int as3543_pcm_trigger(struct snd_pcm_substream *substream,int cmd)
{
	return 0;
}

static int as3543_pcm_hw_params(struct snd_pcm_substream *substream, struct snd_pcm_hw_params *params)
{
	return 0;
}

#if 0
#define AS3543_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 | SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 | \
				SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 | SNDRV_PCM_RATE_48000 | SNDRV_PCM_RATE_88200 | SNDRV_PCM_RATE_96000)


#define AS3543_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE)
#endif

#define AS3543_RATES 	(SNDRV_PCM_RATE_44100)
#define AS3543_FORMATS 	(SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S20_3LE | SNDRV_PCM_FMTBIT_S24_LE)

static const struct snd_soc_pcm_stream as3543_hifi_dai_playback = {
	.stream_name	= "Playback",
	.channels_min	= 1,
	.channels_max	= 2,
	.rates		= AS3543_RATES,
	.formats	= AS3543_FORMATS,
};

static const struct snd_soc_pcm_stream as3543_hifi_dai_capture = {
	.stream_name	= "Capture",
	.channels_min	= 1,
	.channels_max	= 2,
	.rates		= AS3543_RATES,
	.formats	= AS3543_FORMATS,
};

/* dai ops, called by machine drivers */
static const struct snd_soc_dai_ops as3543_hifi_dai_ops = {
	.set_fmt = as3543_set_dai_fmt,
	.set_sysclk = as3543_set_dai_sysclk,
	.set_pll = as3543_set_pll,
	.set_clkdiv = as3543_set_clkdiv,
};

/* audio ops, called by alsa */
static const struct snd_soc_ops as3543_hifi_dai_audio_ops = {
	.startup = as3543_pcm_startup,
	.shutdown = as3543_pcm_shutdown,
	.hw_params = as3543_pcm_hw_params,
	.trigger = as3543_pcm_trigger,
};

static int as3543_suspend(struct device *dev, pm_message_t state)
{
	dbg("%s: not implemented", __func__);
	return 0;
}

static int as3543_resume(struct device *dev)
{
	dbg("%s: not implemented", __func__);
	return 0;
}

static int as3543_codec_io_probe(struct snd_soc_codec *codec, struct snd_soc_machine *machine)
{
	dbg("%s", __func__);

    // add widgets
	as3543_add_controls(codec, machine->card);
	as3543_add_widgets(codec, machine);
	return 0;
}

static int as3543_codec_io_remove(struct snd_soc_codec *codec, struct snd_soc_machine *machine)
{
	dbg("%s", __func__);

	AS3543DeInit();

	return 0;
}

static const struct snd_soc_codec_ops as3543_codec_ops = {
	.read		= as3543_codec_read,
	.write		= as3543_codec_write,
	.io_probe	= as3543_codec_io_probe,
	.io_remove	= as3543_codec_io_remove,
};

static int as3543_codec_probe(struct device *dev)
{
	struct snd_soc_codec *codec = to_snd_soc_codec(dev);
    AS3543PrivInfo *pAS3543PrivInfo;

	info("AS3543 Audio Codec %s", AS3543_VERSION);
    dbg("%s: init codec",__func__);

	pAS3543PrivInfo = kzalloc(sizeof(AS3543PrivInfo), GFP_KERNEL);
	if (pAS3543PrivInfo == NULL)
		return -ENOMEM;

	pAS3543PrivInfo->playback_switch		= TRUE;
	pAS3543PrivInfo->volume_value			= 0;
	pAS3543PrivInfo->mic_capture_switch		= FALSE;
	pAS3543PrivInfo->fm_capture_switch		= FALSE;
	pAS3543PrivInfo->master_mute			= TRUE;
	pAS3543PrivInfo->soft_mute				= TRUE;
	pAS3543PrivInfo->fm_mute				= TRUE;
	pAS3543PrivInfo->power_state			= FALSE;

	codec->owner 			= THIS_MODULE;
	codec->ops 				= &as3543_codec_ops;
	codec->reg_cache_size 	= AS3543_REG_MAX;
	codec->reg_cache_step 	= 1;
	codec->private_data 	= pAS3543PrivInfo;

 	snd_soc_register_codec(codec);
	return 0;
}

static int as3543_codec_remove(struct device *dev)
{
	struct snd_soc_codec *codec = to_snd_soc_codec(dev);
   	kfree(codec->private_data);
	return 0;
}

static int as3543_hifi_dai_probe(struct device *dev)
{
	struct snd_soc_dai *dai = to_snd_soc_dai(dev);

	dai->ops 		= &as3543_hifi_dai_ops;
	dai->audio_ops 	= &as3543_hifi_dai_audio_ops;
	dai->capture 	= &as3543_hifi_dai_capture;
	dai->playback 	= &as3543_hifi_dai_playback;
	snd_soc_register_codec_dai(dai);
	return 0;
}

const char as3543_codec[SND_SOC_CODEC_NAME_SIZE] = "as3543-codec";
EXPORT_SYMBOL_GPL(as3543_codec);

static struct snd_soc_device_driver as3543_codec_driver =
{
	.type	= SND_SOC_BUS_TYPE_CODEC,
	.driver	= {
		.name 		= as3543_codec,
		.owner		= THIS_MODULE,
		.bus 		= &asoc_bus_type,
		.probe		= as3543_codec_probe,
		.remove		= __devexit_p(as3543_codec_remove),
		.suspend	= as3543_suspend,
		.resume		= as3543_resume,
	},
};

const char as3543_hifi_dai[SND_SOC_CODEC_NAME_SIZE] = "as3543-hifi-dai";
EXPORT_SYMBOL_GPL(as3543_hifi_dai);

static struct snd_soc_device_driver as3543_hifi_dai_driver =
{
	.type	= SND_SOC_BUS_TYPE_DAI,
	.driver	=
	{
		.name 		= as3543_hifi_dai,
		.owner		= THIS_MODULE,
		.bus 		= &asoc_bus_type,
		.probe		= as3543_hifi_dai_probe,
	},
};

static __init int as3543_codec_init(void)
{
	int ret = 0;

	dbg("%s: register codec driver", __func__);

	ret = driver_register(&as3543_codec_driver.driver);
	if (ret < 0)
		return ret;
	ret = driver_register(&as3543_hifi_dai_driver.driver);
	if (ret < 0) {
		driver_unregister(&as3543_codec_driver.driver);
		return ret;
	}
	return ret;
}

static __exit void as3543_codec_exit(void)
{
	driver_unregister(&as3543_hifi_dai_driver.driver);
	driver_unregister(&as3543_codec_driver.driver);
}

module_init(as3543_codec_init);
module_exit(as3543_codec_exit);

MODULE_DESCRIPTION("AS3543 driver");
MODULE_AUTHOR("aitdark.park");
MODULE_LICENSE("GPL");

