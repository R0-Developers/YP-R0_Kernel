/*
 * Copyright 2008 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*!
 * @file ak4647.c
 * @brief Driver for AK4647
 *
 * @ingroup Sound
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pm.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>

#include <sound/driver.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>
#include <sound/tlv.h>

#include "ak4647.h"

#define AUDIO_NAME "AK4647"

#define SET_BIT_IN_BYTE(byte, pos) (byte |= (0x01 << pos))
#define CLEAR_BIT_IN_BYTE(byte, pos) (byte &= ~(0x01 << pos))

static struct i2c_client *ak4647_i2c_client;

int ak4647_read_reg(unsigned int reg, u8 *value)
{
	s32 retval;
	retval = i2c_smbus_read_byte_data(ak4647_i2c_client, reg);
	if (-1 == retval) {
		pr_err("%s:read reg errorr:reg=%x,val=%x\n",
		       __func__, reg, *value);
		return -1;
	} else {
		*value = (u8) retval;
		return 0;
	}
}

int ak4647_write_reg(unsigned int reg, u8 value)
{
	if (i2c_smbus_write_byte_data(ak4647_i2c_client, reg, value) < 0) {
		pr_err("%s:write reg errorr:reg=%x,val=%x\n",
		       __func__, reg, value);
		return -1;
	}
	return 0;
}

static unsigned int ak4647_codec_read(struct snd_soc_codec *codec,
				      unsigned int reg)
{
	u8 value;
	ak4647_read_reg(reg, &value);
	return value;
}

static int ak4647_codec_write(struct snd_soc_codec *codec, unsigned int reg,
			      unsigned int value)
{
	return ak4647_write_reg(reg, value);
}

#define DEBUG_AK4647 0

#if DEBUG_AK4647

static char *ak4647_reg_names[] = {
	"AK4647_PM1",
	"AK4647_PM2",
	"AK4647_SIG1",
	"AK4647_SIG2",
	"AK4647_MODE1",
	"AK4647_MODE2",
	"AK4647_TIMER",
	"AK4647_ALC1",
	"AK4647_ALC2",
	"AK4647_LEFT_INPUT_VOLUME",
	"AK4647_LEFT_DGT_VOLUME",
	"AK4647_ALC3",
	"AK4647_RIGHT_INPUT_VOLUME",
	"AK4647_RIGHT_DGT_VOLUME",
	"AK4647_MODE3",
	"AK4647_MODE4",
	"AK4647_PM3",
	"AK4647_DGT_FIL_SEL",
	"AK4647_FIL3_COEF0",
	"AK4647_FIL3_COEF1",
	"AK4647_FIL3_COEF2",
	"AK4647_FIL3_COEF3",
	"AK4647_EQ_COEF0",
	"AK4647_EQ_COEF1",
	"AK4647_EQ_COEF2",
	"AK4647_EQ_COEF3",
	"AK4647_EQ_COEF4",
	"AK4647_EQ_COEF5",
	"AK4647_FIL1_COEF0",
	"AK4647_FIL1_COEF1",
	"AK4647_FIL1_COEF2",
	"AK4647_FIL1_COEF3",
};

static void dump_all_regs(void)
{
	int i;
	u8 value;

	for (i = AK4647_REG_START; i < AK4647_REG_NUMBER; i++) {
		ak4647_read_reg(i, &value);
		pr_info("%s = 0x%x\n", ak4647_reg_names[i], value);
	}
}
#endif

static int loopback_xhandle_get(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol);
static int loopback_xhandle_put(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol);

static const char *ak4647_hp_out[] = { "Stereo", "Mono" };

static const char *ak4647_left_in[] = { "LIN1", "LIN2" };

static const char *ak4647_right_in[] = { "RIN1", "RIN2" };

static const char *ak4647_deemp[] = { "44.1kHz", "Off", "48kHz", "32kHz" };

static const struct soc_enum ak4647_enum[] = {
	SOC_ENUM_SINGLE(AK4647_MODE4, 2, 2, ak4647_hp_out),
	SOC_ENUM_SINGLE(AK4647_MODE3, 0, 4, ak4647_deemp),
	SOC_ENUM_SINGLE(AK4647_PM3, 1, 2, ak4647_left_in),
	SOC_ENUM_SINGLE(AK4647_PM3, 2, 2, ak4647_right_in),
};

#undef snd_soc_info_bool_ext
#define snd_soc_info_bool_ext		snd_ctl_boolean_mono_info
static const struct snd_kcontrol_new ak4647_snd_controls[] = {
	SOC_ENUM("Headphone Output", ak4647_enum[0]),
	SOC_ENUM("Playback Deemphasis", ak4647_enum[1]),
	SOC_ENUM("Left Capture Select", ak4647_enum[2]),
	SOC_ENUM("Right Capture Select", ak4647_enum[3]),
	SOC_SINGLE("Bass Volume", AK4647_MODE3, 2, 3, 0),
	SOC_SINGLE("Mic Boost (+20dB) Switch", AK4647_SIG1, 0, 1, 0),
	SOC_SINGLE("Mic Bias", AK4647_SIG1, 2, 1, 0),
	SOC_SINGLE("ALC Switch", AK4647_ALC1, 5, 1, 0),
	SOC_SINGLE("ALC Recovery Time", AK4647_TIMER, 2, 3, 0),
	SOC_SINGLE("ALC ZC Time", AK4647_TIMER, 4, 3, 0),
	SOC_SINGLE("ALC Volume", AK4647_ALC2, 0, 127, 0),
	SOC_SINGLE("Left Capture Volume", AK4647_LEFT_INPUT_VOLUME, 0, 242, 0),
	SOC_SINGLE("Right Capture Volume",
		   AK4647_RIGHT_INPUT_VOLUME, 0, 242, 0),
	SOC_SINGLE("Left Playback Volume", AK4647_LEFT_DGT_VOLUME, 0, 255, 1),
	SOC_SINGLE("Right Playback Volume", AK4647_RIGHT_DGT_VOLUME, 0, 255, 1),
	SOC_SINGLE_BOOL_EXT("Loopback Line-in", 0,
			    loopback_xhandle_get, loopback_xhandle_put),
};

/* add non dapm controls */
static int ak4647_add_controls(struct snd_soc_codec *codec,
			       struct snd_card *card)
{
	int err, i;

	for (i = 0; i < ARRAY_SIZE(ak4647_snd_controls); i++) {
		err = snd_ctl_add(card,
				  snd_soc_cnew(&ak4647_snd_controls[i],
					       codec, NULL));
		if (err < 0)
			return err;
	}
	return 0;
}

/* Stereo Mixer for HP*/
static const struct snd_kcontrol_new ak4647_hp_stereo_mixer_controls[] = {
	SOC_DAPM_SINGLE("Playback Switch", AK4647_MODE4, 0, 1, 0),
	SOC_DAPM_SINGLE("Mic Sidetone Switch", AK4647_MODE4, 1, 1, 0),
};

/* Stereo Mixer for Line out*/
static const struct snd_kcontrol_new ak4647_line_stereo_mixer_controls[] = {
	SOC_DAPM_SINGLE("Playback Switch", AK4647_SIG1, 4, 1, 0),
	SOC_DAPM_SINGLE("Mic Sidetone Switch", AK4647_SIG2, 2, 1, 0),
};

/* ak4647 dapm widgets */
static const struct snd_soc_dapm_widget ak4647_dapm_widgets[] = {
	SND_SOC_DAPM_MIXER("Headphone Mixer", SND_SOC_NOPM, 0, 0,
			   &ak4647_hp_stereo_mixer_controls[0],
			   ARRAY_SIZE(ak4647_hp_stereo_mixer_controls)),
	SND_SOC_DAPM_MIXER("Lineout Mixer", SND_SOC_NOPM, 0, 0,
			   &ak4647_line_stereo_mixer_controls[0],
			   ARRAY_SIZE(ak4647_line_stereo_mixer_controls)),
	SND_SOC_DAPM_DAC("DAC", "Playback", AK4647_PM1, 2, 0),
	SND_SOC_DAPM_OUTPUT("HPL"),
	SND_SOC_DAPM_OUTPUT("HPR"),
	SND_SOC_DAPM_OUTPUT("LOUT"),

	SND_SOC_DAPM_ADC("Left ADC", "Capture", AK4647_PM1, 0, 0),
	SND_SOC_DAPM_ADC("Right ADC", "Capture", AK4647_PM3, 0, 0),
	SND_SOC_DAPM_PGA("HP R Amp", AK4647_PM2, 4, 0, NULL, 0),
	SND_SOC_DAPM_PGA("HP L Amp", AK4647_PM2, 5, 0, NULL, 0),
	SND_SOC_DAPM_PGA("Mic", AK4647_PM1, 5, 0, NULL, 0),
	SND_SOC_DAPM_PGA("Line Out Amp", AK4647_PM1, 3, 0, NULL, 0),

	SND_SOC_DAPM_MICBIAS("Mic Bias", AK4647_SIG1, 2, 0),
	SND_SOC_DAPM_INPUT("Left Input"),
	SND_SOC_DAPM_INPUT("Right Input"),
};

static const char *audio_map[][3] = {
	/*headphone stereo mixer */
	{"Headphone Mixer", "Playback Switch", "DAC"},
	{"Headphone Mixer", "Mic Sidetone Switch", "Mic"},

	/*lineout stereo mixer */
	{"Lineout Mixer", "Playback Switch", "DAC"},
	{"Lineout Mixer", "Mic Sidetone Switch", "Mic"},

	/* headphone amp */
	{"HP R Amp", NULL, "Headphone Mixer"},
	{"HP L Amp", NULL, "Headphone Mixer"},

	/* headphone */
	{"HPR", NULL, "HP R Amp"},
	{"HPL", NULL, "HP L Amp"},

	/* line out */
	{"Line Out Amp", NULL, "Lineout Mixer"},
	{"LOUT", NULL, "Line Out Amp"},

	/* ADC */
	{"Left ADC", NULL, "Left Input"},
	{"Right ADC", NULL, "Right Input"},

	/* terminator */
	{NULL, NULL, NULL},

};

static int ak4647_add_widgets(struct snd_soc_codec *codec,
			      struct snd_soc_machine *machine)
{
	int i;
	int ret;

	for (i = 0; i < ARRAY_SIZE(ak4647_dapm_widgets); i++) {
		snd_soc_dapm_new_control(machine, codec,
					 &ak4647_dapm_widgets[i]);
	}

	/* set up audio path audio_mapnects */
	for (i = 0; audio_map[i][0] != NULL; i++) {
		ret = snd_soc_dapm_connect_input(machine, audio_map[i][0],
						 audio_map[i][1],
						 audio_map[i][2]);
	}

	snd_soc_dapm_new_widgets(machine);
	return 0;
}

#define AK4647_RATES (SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 | \
		SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 | \
		SNDRV_PCM_RATE_44100 | SNDRV_PCM_RATE_32000 | \
		SNDRV_PCM_RATE_48000)

#define AK4647_FORMATS SNDRV_PCM_FMTBIT_S16_LE

static const struct snd_soc_pcm_stream ak4647_hifi_dai_playback = {
	.stream_name = "Playback",
	.channels_min = 2,
	.channels_max = 2,
	.rates = AK4647_RATES,
	.formats = AK4647_FORMATS,
};

static const struct snd_soc_pcm_stream ak4647_hifi_dai_capture = {
	.stream_name = "Capture",
	.channels_min = 2,
	.channels_max = 2,
	.rates = AK4647_RATES,
	.formats = AK4647_FORMATS,
};

static int ak4647_set_dai_sysclk(struct snd_soc_dai *codec_dai,
				 int clk_id, unsigned int freq, int dir)
{
	u8 value;
	u8 fs = 0;

	/* FS3 is on D5 */
	switch (freq) {
	case 8000:
		fs = 0x0;
		break;
	case 11025:
		fs = 0x5;
		break;
	case 16000:
		fs = 0x2;
		break;
	case 22050:
		fs = 0x7;
		break;
	case 32000:
		fs = 0x22;
		break;
	case 44100:
		fs = 0x27;
		break;
	case 48000:
		fs = 0x23;
		break;
	default:
		pr_err("unsupported sample rate");
		return -1;
	}

	ak4647_read_reg(AK4647_MODE2, &value);
	value &= 0xC0;
	value |= fs;
	ak4647_write_reg(AK4647_MODE2, value);
	return 0;
}

static int ak4647_set_dai_fmt(struct snd_soc_dai *codec_dai, unsigned int fmt)
{
	u8 reg_mode1, reg_pm2;

	/* get current values */
	ak4647_read_reg(AK4647_MODE1, &reg_mode1);
	ak4647_read_reg(AK4647_PM2, &reg_pm2);

	/* set master/slave audio interface */
	switch (fmt & SND_SOC_DAIFMT_MASTER_MASK) {
	case SND_SOC_DAIFMT_CBM_CFM:
		SET_BIT_IN_BYTE(reg_pm2, 3);
		break;
	case SND_SOC_DAIFMT_CBS_CFS:
		CLEAR_BIT_IN_BYTE(reg_pm2, 3);
		break;
	default:
		return -EINVAL;
	}

	/* interface format */
	switch (fmt & SND_SOC_DAIFMT_FORMAT_MASK) {
	case SND_SOC_DAIFMT_I2S:
		SET_BIT_IN_BYTE(reg_mode1, 0);
		SET_BIT_IN_BYTE(reg_mode1, 1);
		break;
	case SND_SOC_DAIFMT_RIGHT_J:
		SET_BIT_IN_BYTE(reg_mode1, 0);
		CLEAR_BIT_IN_BYTE(reg_mode1, 1);
		break;
	case SND_SOC_DAIFMT_LEFT_J:
		CLEAR_BIT_IN_BYTE(reg_mode1, 0);
		SET_BIT_IN_BYTE(reg_mode1, 1);
		break;
	case SND_SOC_DAIFMT_DSP_A:
	case SND_SOC_DAIFMT_DSP_B:
	default:
		pr_err("dai format %d not supported", fmt);
		return -EINVAL;
	}

	ak4647_write_reg(AK4647_MODE1, reg_mode1);
	ak4647_write_reg(AK4647_PM2, reg_pm2);
	return 0;
}

static int ak4647_set_clkdiv(struct snd_soc_dai *codec_dai, int div_id, int div)
{
	int retval = 0;
	u8 value;
	if (AK4647_BCLK_CLKDIV == div_id) {
		ak4647_read_reg(AK4647_MODE1, &value);
		switch (div) {
		case AK4647_BCLK_DIV_32:
			CLEAR_BIT_IN_BYTE(value, 3);
			ak4647_write_reg(AK4647_MODE1, value);
			break;
		case AK4647_BCLK_DIV_64:
			SET_BIT_IN_BYTE(value, 3);
			ak4647_write_reg(AK4647_MODE1, value);
			break;
		default:
			retval = -1;
			pr_err("wrong div value for divid %d", div_id);
			break;
		}
	} else if (AK4647_MCLK_CLKDIV == div_id) {
		ak4647_read_reg(AK4647_MODE2, &value);
		switch (div) {
		case AK4647_MCLK_DIV_32:
			SET_BIT_IN_BYTE(value, 7);
			SET_BIT_IN_BYTE(value, 6);
			ak4647_write_reg(AK4647_MODE2, value);
			break;
		case AK4647_MCLK_DIV_64:
			SET_BIT_IN_BYTE(value, 7);
			CLEAR_BIT_IN_BYTE(value, 6);
			ak4647_write_reg(AK4647_MODE2, value);
			break;
		case AK4647_MCLK_DIV_128:
			CLEAR_BIT_IN_BYTE(value, 7);
			SET_BIT_IN_BYTE(value, 6);
			ak4647_write_reg(AK4647_MODE2, value);
			break;
		case AK4647_MCLK_DIV_256:
			CLEAR_BIT_IN_BYTE(value, 7);
			CLEAR_BIT_IN_BYTE(value, 6);
			ak4647_write_reg(AK4647_MODE2, value);
			break;
		default:
			retval = -1;
			pr_err("wrong div value for div id %d", div_id);
			break;
		}
	} else {
		retval = -1;
		pr_err("wrong div id");
	}

	return retval;
}
static int ak4647_mute(struct snd_soc_dai *dai, int mute)
{
	u8 value;

	ak4647_read_reg(AK4647_MODE3, &value);

	if (mute)
		SET_BIT_IN_BYTE(value, 5);
	else
		CLEAR_BIT_IN_BYTE(value, 5);
	ak4647_write_reg(AK4647_MODE3, value);
	return 0;
}

struct ak4647_state_data {
	u8 left_capture_vol;
	u8 right_capture_vol;
};
static struct ak4647_state_data ak4647_state;

/* dai ops, called by machine drivers */
static const struct snd_soc_dai_ops ak4647_hifi_dai_ops = {
	.digital_mute = ak4647_mute,
	.set_fmt = ak4647_set_dai_fmt,
	.set_sysclk = ak4647_set_dai_sysclk,
	.set_clkdiv = ak4647_set_clkdiv,
};

static int ak4647_pcm_startup(struct snd_pcm_substream *substream)
{
	u8 value;
	/* for playback, save down capture volume */
	if (SNDRV_PCM_STREAM_PLAYBACK == substream->stream) {
		ak4647_read_reg(AK4647_LEFT_INPUT_VOLUME, &value);
		ak4647_state.left_capture_vol = value;
		ak4647_read_reg(AK4647_RIGHT_INPUT_VOLUME, &value);
		ak4647_state.right_capture_vol = value;

		/* when PMADL=PMADR=0 set IVL &IVR to be 0x91 (0db) */
		ak4647_write_reg(AK4647_LEFT_INPUT_VOLUME, 0x91);
		ak4647_write_reg(AK4647_RIGHT_INPUT_VOLUME, 0x91);
	}

	/* output digital volume independent */
	ak4647_read_reg(AK4647_MODE3, &value);
	CLEAR_BIT_IN_BYTE(value, 4);
	ak4647_write_reg(AK4647_MODE3, value);
	return 0;
}

static void ak4647_pcm_shutdown(struct snd_pcm_substream *substream)
{
	u8 value;
	ak4647_read_reg(AK4647_PM2, &value);
	/* mute */
	CLEAR_BIT_IN_BYTE(value, 6);
	ak4647_write_reg(AK4647_PM2, value);

	/* after playback, restore capture volume */
	if (SNDRV_PCM_STREAM_PLAYBACK == substream->stream) {
		ak4647_write_reg(AK4647_LEFT_INPUT_VOLUME,
				 ak4647_state.left_capture_vol);
		ak4647_write_reg(AK4647_RIGHT_INPUT_VOLUME,
				 ak4647_state.right_capture_vol);
	}
}

static int ak4647_pcm_prepare(struct snd_pcm_substream *substream)
{
	u8 value;

	/* VCOM power on */
	ak4647_write_reg(AK4647_PM1, 0x44);
	msleep(30);

	ak4647_read_reg(AK4647_PM2, &value);
	/* PLL enabled */
	SET_BIT_IN_BYTE(value, 0);

	ak4647_write_reg(AK4647_PM2, value);
	/* wait for PLL locked */
	msleep(40);

	/* don't mute */
	SET_BIT_IN_BYTE(value, 6);
	ak4647_write_reg(AK4647_PM2, value);
	return 0;
}

/* audio ops, called by alsa */
static const struct snd_soc_ops ak4647_hifi_dai_audio_ops = {
	.startup = ak4647_pcm_startup,
	.shutdown = ak4647_pcm_shutdown,
	.prepare = ak4647_pcm_prepare,
};

static int ak4647_codec_io_probe(struct snd_soc_codec *codec,
				 struct snd_soc_machine *machine)
{
	/* setup init value for audio path controls here */
	/* enable DAC to headphone */
	ak4647_write_reg(AK4647_MODE4, 0x9);
	/* capture source to be LIN2 and RIN2 */
	ak4647_write_reg(AK4647_PM3, 0x6);
	/* MPWR pin up */
	ak4647_write_reg(AK4647_SIG1, 0x5);

	ak4647_add_controls(codec, machine->card);
	ak4647_add_widgets(codec, machine);

	/* MCKI = 12M, default audio interface format as "left-justified" */
	ak4647_write_reg(AK4647_MODE1, 0x62);

	/* ALC disabled */
	ak4647_write_reg(AK4647_ALC1, 0x0);

	ak4647_state.left_capture_vol = 0x91;
	ak4647_state.right_capture_vol = 0x91;

	return 0;
}

static int ak4647_codec_io_remove(struct snd_soc_codec *codec,
				  struct snd_soc_machine *machine)
{
	return 0;
}

static const struct snd_soc_codec_ops ak4647_codec_ops = {
	.read = ak4647_codec_read,
	.write = ak4647_codec_write,
	.io_probe = ak4647_codec_io_probe,
	.io_remove = ak4647_codec_io_remove,
};

static int ak4647_codec_probe(struct device *dev)
{
	struct snd_soc_codec *codec = to_snd_soc_codec(dev);
	codec->owner = THIS_MODULE;
	codec->ops = &ak4647_codec_ops;
	snd_soc_register_codec(codec);

	return 0;
}

static int ak4647_codec_remove(struct device *dev)
{
	return 0;
}

static int ak4647_hifi_dai_probe(struct device *dev)
{
	struct snd_soc_dai *dai = to_snd_soc_dai(dev);

	dai->ops = &ak4647_hifi_dai_ops;
	dai->audio_ops = &ak4647_hifi_dai_audio_ops;
	dai->capture = &ak4647_hifi_dai_capture;
	dai->playback = &ak4647_hifi_dai_playback;
	snd_soc_register_codec_dai(dai);
	return 0;
}

const char ak4647_codec[SND_SOC_CODEC_NAME_SIZE] = "ak4647-codec";
EXPORT_SYMBOL_GPL(ak4647_codec);

static struct snd_soc_device_driver ak4647_codec_driver = {
	.type = SND_SOC_BUS_TYPE_CODEC,
	.driver = {
		   .name = ak4647_codec,
		   .owner = THIS_MODULE,
		   .bus = &asoc_bus_type,
		   .probe = ak4647_codec_probe,
		   .remove = __devexit_p(ak4647_codec_remove),
		   },
};

const char ak4647_hifi_dai[SND_SOC_CODEC_NAME_SIZE] = "ak4647-hifi-dai";
EXPORT_SYMBOL_GPL(ak4647_hifi_dai);

static struct snd_soc_device_driver ak4647_hifi_dai_driver = {
	.type = SND_SOC_BUS_TYPE_DAI,
	.driver = {
		   .name = ak4647_hifi_dai,
		   .owner = THIS_MODULE,
		   .bus = &asoc_bus_type,
		   .probe = ak4647_hifi_dai_probe,
		   },
};

static int ak4647_i2c_probe(struct i2c_client *client)
{
	ak4647_i2c_client = client;
	return 0;
}

static int ak4647_i2c_remove(struct i2c_client *client)
{
	return 0;
}

static struct i2c_driver ak4647_i2c_driver = {
	.driver = {.owner = THIS_MODULE,
		   .name = "ak4647-i2c",
		   },
	.probe = ak4647_i2c_probe,
	.remove = ak4647_i2c_remove,
};

static __init int ak4647_init(void)
{
	int ret = 0;
	ret = i2c_add_driver(&ak4647_i2c_driver);
	if (ret) {
		pr_err("ak4647 i2c driver register failed");
		return ret;
	}

	ret = driver_register(&ak4647_codec_driver.driver);
	if (ret < 0)
		return ret;
	ret = driver_register(&ak4647_hifi_dai_driver.driver);
	if (ret < 0) {
		driver_unregister(&ak4647_codec_driver.driver);
		return ret;
	}

	return ret;
}

static __exit void ak4647_exit(void)
{
	driver_unregister(&ak4647_hifi_dai_driver.driver);
	driver_unregister(&ak4647_codec_driver.driver);
	i2c_del_driver(&ak4647_i2c_driver);
}

int pmic_audio_fm_output_enable(bool enable)
{
	u8 val;
	if (enable) {
		ak4647_set_dai_fmt(NULL,
				   SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				   SND_SOC_DAIFMT_CBM_CFM |
				   SND_SOC_DAIFMT_SYNC);
		ak4647_set_dai_sysclk(NULL, 0, 44100, 0);
		ak4647_set_clkdiv(NULL, 0, 0);
		/* VCOM power on */
		ak4647_write_reg(AK4647_PM1, 0x44);
		msleep(30);

		ak4647_read_reg(AK4647_PM2, &val);
		/* PLL enabled */
		SET_BIT_IN_BYTE(val, 0);

		ak4647_write_reg(AK4647_PM2, val);
		/* wait for PLL locked */
		msleep(40);

		/* don't mute */
		SET_BIT_IN_BYTE(val, 6);
		ak4647_write_reg(AK4647_PM2, val);

		/* loopback STDO to DAC */
		ak4647_read_reg(AK4647_MODE3, &val);
		SET_BIT_IN_BYTE(val, 6);
		ak4647_write_reg(AK4647_MODE3, val);

		/* switch to R/L 1 */
		ak4647_read_reg(AK4647_PM3, &val);
		CLEAR_BIT_IN_BYTE(val, 1);
		CLEAR_BIT_IN_BYTE(val, 2);
		ak4647_write_reg(AK4647_PM3, val);

		/* power up ADC */
		ak4647_read_reg(AK4647_PM1, &val);
		SET_BIT_IN_BYTE(val, 0);
		ak4647_write_reg(AK4647_PM1, val);
		ak4647_read_reg(AK4647_PM3, &val);
		SET_BIT_IN_BYTE(val, 0);
		ak4647_write_reg(AK4647_PM3, val);

		/* power up DAC */
		ak4647_read_reg(AK4647_PM1, &val);
		SET_BIT_IN_BYTE(val, 2);
		ak4647_write_reg(AK4647_PM1, val);

		msleep(30);

		/* headphone output switch on */
		ak4647_read_reg(AK4647_MODE4, &val);
		SET_BIT_IN_BYTE(val, 0);
		ak4647_write_reg(AK4647_MODE4, val);

		/* power on headphone amp */
		ak4647_read_reg(AK4647_PM2, &val);
		SET_BIT_IN_BYTE(val, 4);
		SET_BIT_IN_BYTE(val, 5);
		ak4647_write_reg(AK4647_PM2, val);

		ak4647_mute(NULL, 0);
	} else {
		ak4647_mute(NULL, 1);

		/* disbale loopback */
		ak4647_read_reg(AK4647_MODE3, &val);
		CLEAR_BIT_IN_BYTE(val, 6);
		SET_BIT_IN_BYTE(val, 5);
		ak4647_write_reg(AK4647_MODE3, val);

		/* switch to R/L 2 */
		ak4647_read_reg(AK4647_PM3, &val);
		SET_BIT_IN_BYTE(val, 1);
		SET_BIT_IN_BYTE(val, 2);
		ak4647_write_reg(AK4647_PM3, val);
	}
	return 0;
}

static int loopback_xhandle_get(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	ucontrol->value.integer.value[0] = kcontrol->private_value;
	return 0;
}

static int loopback_xhandle_put(struct snd_kcontrol *kcontrol,
				struct snd_ctl_elem_value *ucontrol)
{
	int changed;
	long flag = ucontrol->value.integer.value[0];
	changed =
	    (ucontrol->value.integer.value[0] ==
	     kcontrol->private_value) ? 0 : 1;
	kcontrol->private_value = flag;
	if (flag)
		pmic_audio_fm_output_enable(true);
	else
		pmic_audio_fm_output_enable(false);

	return changed;
}

subsys_initcall(ak4647_init);
module_exit(ak4647_exit);

MODULE_DESCRIPTION("ASoC ak4647 driver");
MODULE_LICENSE("GPL");
