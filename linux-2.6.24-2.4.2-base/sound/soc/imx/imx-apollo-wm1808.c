/*
 * imx-apollo-wm1808.c  --  Audio Driver for Wolfson WM1808
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
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/clk.h>

#include <linux/err.h>
#include <sound/driver.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>
#include <sound/initval.h>

#include <asm/arch/dma.h>
#include <asm/arch/spba.h>
#include <asm/arch/clock.h>
#include <asm/arch/mxc.h>
#include <asm/arch/gpio.h>
#include <asm/mach/irq.h>

#include "imx-ssi.h"
#include "imx-pcm.h"
#include "../../i2c/wm1808/wm1808.h"
#include "../../../arch/arm/mach-mx37/crm_regs.h"
#include "../../../arch/arm/mach-mx37/mx37_pins.h"
#include "../codecs/DrvWM1808.h"

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
	
static struct platform_device *imx_wm1808_snd_device;
const char imx_wm1808_audio[32] = {
	"wm1808-imx-audio"
};
struct mxc_audio_platform_data imx_wm1808_platform_data = {
	.ssi_num = 2,
	.src_port = 2,
	.ext_port = 5,
};

extern const char wm1808_codec[SND_SOC_CODEC_NAME_SIZE];
extern const char wm1808_hifi_dai[SND_SOC_DAI_NAME_SIZE];

int wm1808_init(struct wm1808 *wm1808)
{
	int ret = 0;

	dbg("Registering imx37_snd_device");

	wm1808->audio = kzalloc(sizeof(struct snd_soc_machine), GFP_KERNEL);
	if (wm1808->audio == NULL){
		ret = -ENOMEM;
		goto snd_err;
	}

	imx_wm1808_snd_device = platform_device_alloc(imx_wm1808_audio, -1);
	if (!imx_wm1808_snd_device) {
		ret = -ENOMEM;
		dbg("wm1808_init() FAILED");
		return ret;
	}
	imx_wm1808_snd_device->dev.platform_data = &imx_wm1808_platform_data;
	platform_set_drvdata(imx_wm1808_snd_device, &wm1808->audio);
	ret = platform_device_add(imx_wm1808_snd_device);
	if (ret)
		goto snd_err;

	dbg("Exiting normally from wm1808_init()");
	return ret;

      snd_err:
	platform_device_put(imx_wm1808_snd_device);
	dbg("wm1808_init() FAILED");
	return ret;
}
EXPORT_SYMBOL_GPL(wm1808_init);

void gpio_activate_audio_ports(void);

/* SSI BCLK and LRC master */
#define WM1808_SSI_MASTER 1

struct imx_3stack_pcm_state {
	int lr_clk_active;
	int playback_active;
	int capture_active;
	struct clk *mclk;
};

struct _wm1808_audio {
	unsigned int channels;
	snd_pcm_format_t format;
	unsigned int rate;
	unsigned int sysclk;
	unsigned int mclkdiv;
	unsigned int bclkdiv;
	unsigned int filter_sr;
};

static const struct _wm1808_audio wm1808_audio[] = {
	/* 16bit mono modes */

	{1, SNDRV_PCM_FORMAT_S16_LE, 8000, 12288000,
	WM1808_MCLK_DIV_12, WM1808_BCLK_DIV_8, WM1808_SR_8K},
	{1, SNDRV_PCM_FORMAT_S16_LE, 16000, 12288000,
	WM1808_MCLK_DIV_6, WM1808_BCLK_DIV_8, WM1808_SR_16K},
	{1, SNDRV_PCM_FORMAT_S16_LE, 32000, 12288000,
	WM1808_MCLK_DIV_3, WM1808_BCLK_DIV_8, WM1808_SR_32K},
	{1, SNDRV_PCM_FORMAT_S16_LE, 48000, 12288000,
	WM1808_MCLK_DIV_2, WM1808_BCLK_DIV_8, WM1808_SR_48K},
	{1, SNDRV_PCM_FORMAT_S16_LE, 96000, 12288000,
	WM1808_MCLK_DIV_1, WM1808_BCLK_DIV_8, WM1808_SR_48K},
	{1, SNDRV_PCM_FORMAT_S16_LE, 11025, 11289600,
	WM1808_MCLK_DIV_8, WM1808_BCLK_DIV_8, WM1808_SR_12K},
	{1, SNDRV_PCM_FORMAT_S16_LE, 22050, 11289600,
	WM1808_MCLK_DIV_4, WM1808_BCLK_DIV_8, WM1808_SR_24K},
	{1, SNDRV_PCM_FORMAT_S16_LE, 44100, 11289600,
	WM1808_MCLK_DIV_2, WM1808_BCLK_DIV_8, WM1808_SR_48K},
	{1, SNDRV_PCM_FORMAT_S16_LE, 88200, 11289600,
	WM1808_MCLK_DIV_1, WM1808_BCLK_DIV_8, WM1808_SR_48K},

	/* 16 bit stereo modes */
	{2, SNDRV_PCM_FORMAT_S16_LE, 8000, 12288000,
	 WM1808_MCLK_DIV_12, WM1808_BCLK_DIV_4, WM1808_SR_8K},
	{2, SNDRV_PCM_FORMAT_S16_LE, 16000, 12288000,
	 WM1808_MCLK_DIV_6, WM1808_BCLK_DIV_4, WM1808_SR_16K},
	{2, SNDRV_PCM_FORMAT_S16_LE, 32000, 12288000,
	 WM1808_MCLK_DIV_3, WM1808_BCLK_DIV_4, WM1808_SR_32K},
	{2, SNDRV_PCM_FORMAT_S16_LE, 48000, 12288000,
	 WM1808_MCLK_DIV_2, WM1808_BCLK_DIV_4, WM1808_SR_48K},
	{2, SNDRV_PCM_FORMAT_S16_LE, 96000, 12288000,
	 WM1808_MCLK_DIV_1, WM1808_BCLK_DIV_4, WM1808_SR_48K},
	{2, SNDRV_PCM_FORMAT_S16_LE, 11025, 11289600,
	 WM1808_MCLK_DIV_8, WM1808_BCLK_DIV_4, WM1808_SR_12K},
	{2, SNDRV_PCM_FORMAT_S16_LE, 22050, 11289600,
	 WM1808_MCLK_DIV_4, WM1808_BCLK_DIV_4, WM1808_SR_24K},
	{2, SNDRV_PCM_FORMAT_S16_LE, 44100, 11289600,
	 WM1808_MCLK_DIV_2, WM1808_BCLK_DIV_4, WM1808_SR_48K},
	{2, SNDRV_PCM_FORMAT_S16_LE, 88200, 11289600,
	 WM1808_MCLK_DIV_1, WM1808_BCLK_DIV_4, WM1808_SR_48K},

	/* 24bit stereo modes */
	{2, SNDRV_PCM_FORMAT_S24_LE, 48000, 12288000,
	 WM1808_MCLK_DIV_2, WM1808_BCLK_DIV_4, WM1808_SR_48K},
	{2, SNDRV_PCM_FORMAT_S24_LE, 96000, 12288000,
	 WM1808_MCLK_DIV_1, WM1808_BCLK_DIV_4, WM1808_SR_48K},
	{2, SNDRV_PCM_FORMAT_S24_LE, 44100, 11289600,
	 WM1808_MCLK_DIV_2, WM1808_BCLK_DIV_4, WM1808_SR_48K},
	{2, SNDRV_PCM_FORMAT_S24_LE, 88200, 11289600,
	 WM1808_MCLK_DIV_1, WM1808_BCLK_DIV_4, WM1808_SR_48K},
};

#if WM1808_SSI_MASTER
/* Set ipg_perclk to be the source of CLKO, default rate as 8M.
 */
static int imx_3stack_startup(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_link *pcm_link = substream->private_data;
	struct imx_3stack_pcm_state *state = pcm_link->private_data;

	if (!state->lr_clk_active) {
		state->mclk = clk_get(NULL, "cko1_clk");
		clk_enable(state->mclk);

		dbg("%s: set clko, generated by ipg_perclk(8MHz)", __func__);
	}
	state->lr_clk_active++;

	if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
		state->capture_active = 1;
	else
		state->playback_active = 1;

	
	return 0;
}
#else
#define imx_3stack_startup NULL
#endif

static int imx_3stack_hifi_hw_params(struct snd_pcm_substream *substream,
				     struct snd_pcm_hw_params *params)
{
 	struct snd_soc_pcm_link *pcm_link = substream->private_data;
	struct snd_soc_dai *cpu_dai = pcm_link->cpu_dai;
	struct snd_soc_dai *codec_dai = pcm_link->codec_dai;
	struct imx_3stack_pcm_state *state = pcm_link->private_data;
	int i, found = 0;
	snd_pcm_format_t format = params_format(params);
	unsigned int rate = params_rate(params);
	unsigned int channels = params_channels(params);
	u32 dai_format;

	/* only need to do this once as capture and playback are sync */
	if (state->lr_clk_active > 1)
		return 0;

	/* find the correct audio parameters */
	for (i = 0; i < ARRAY_SIZE(wm1808_audio); i++) {
		if (rate == wm1808_audio[i].rate &&
		    format == wm1808_audio[i].format &&
		    channels == wm1808_audio[i].channels) {
			found = 1;
			break;
		}
	}
	if (!found) {
		err("%s: invalid params\n", __func__);
		return -EINVAL;
	}

	codec_dai->ops->set_pll(codec_dai, 0, 0, wm1808_audio[i].sysclk);
	
#if WM1808_SSI_MASTER
    
	dai_format = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
	    SND_SOC_DAIFMT_CBM_CFM | SND_SOC_DAIFMT_SYNC;
	if (channels == 2)
		dai_format |= SND_SOC_DAIFMT_TDM;

	/* set codec DAI configuration */
	codec_dai->ops->set_fmt(codec_dai, dai_format);

	/* set cpu DAI configuration */
	cpu_dai->ops->set_fmt(cpu_dai, dai_format);

	/* Enable pll */
	codec_dai->ops->set_pll(codec_dai, 0, 0, 1);

	/* set sysclk to 12.288MHz */
	codec_dai->ops->set_sysclk(codec_dai, WM1808_MCLK_SEL_PLL, 
				wm1808_audio[i].sysclk, SND_SOC_CLOCK_IN);

	/* set codec BCLKDIV */
	codec_dai->ops->set_clkdiv(codec_dai, WM1808_BCLK_CLKDIV,
				   wm1808_audio[i].bclkdiv);

	/* set codec MCLKDIV */
	codec_dai->ops->set_clkdiv(codec_dai, WM1808_MCLK_CLKDIV,
				   wm1808_audio[i].mclkdiv);

#else

	dai_format = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
	    SND_SOC_DAIFMT_CBS_CFS | SND_SOC_DAIFMT_SYNC;
	if (channels == 2)
		format |= SND_SOC_DAIFMT_TDM;

	/* set codec DAI configuration */
	codec_dai->ops->set_fmt(codec_dai, dai_format);

	/* set cpu DAI configuration */
	cpu_dai->ops->set_fmt(cpu_dai, dai_format);

#endif

	/* set i.MX active slot mask */
	cpu_dai->ops->set_tdm_slot(cpu_dai,
				   channels == 1 ? 0xfffffffe : 0xfffffffc,
				   channels);

	/* set the SSI system clock as input (unused) */
	cpu_dai->ops->set_sysclk(cpu_dai, IMX_SSP_SYS_CLK, 0, SND_SOC_CLOCK_IN);

	/* Set filter sample rate */
	codec_dai->ops->set_clkdiv(codec_dai, WM1808_SR,
				   wm1808_audio[i].filter_sr);

	return 0;
 }

static void imx_3stack_shutdown(struct snd_pcm_substream *substream)
{
	struct snd_soc_pcm_link *pcm_link = substream->private_data;
	struct snd_soc_dai *codec_dai = pcm_link->codec_dai;
	struct imx_3stack_pcm_state *state = pcm_link->private_data;

	dbg("%s: codec_dai->active=%d", __func__, codec_dai->active);
	/* disable the PLL if there are no active Tx or Rx channels */
	if (!codec_dai->active){
		codec_dai->ops->set_pll(codec_dai, 0, 0, 0);
		clk_disable(state->mclk);
	}
	state->lr_clk_active--;

	/*
	 * We need to keep track of active streams in master mode and
	 * switch LRC source if necessary.
	 */

	if (substream->stream == SNDRV_PCM_STREAM_CAPTURE)
		state->capture_active = 0;
	else
		state->playback_active = 0;
}

/*
 * imx_3stack WM1808 HiFi DAI opserations.
 */
static struct snd_soc_ops imx_3stack_hifi_ops = {
	.startup = imx_3stack_startup,
	.shutdown = imx_3stack_shutdown,
	.hw_params = imx_3stack_hifi_hw_params,
};
 
static void imx_3stack_init_dam(int ssi_port, int dai_port)
{
	/* WM1808 uses SSI1 or SSI2 via AUDMUX port dai_port for audio */

	/* reset port ssi_port & dai_port */
	DAM_PTCR(ssi_port) = 0;
	DAM_PDCR(ssi_port) = 0;
	DAM_PTCR(dai_port) = 0;
	DAM_PDCR(dai_port) = 0;

	/* set to synchronous */
	DAM_PTCR(ssi_port) |= AUDMUX_PTCR_SYN;
	DAM_PTCR(dai_port) |= AUDMUX_PTCR_SYN;

#if WM1808_SSI_MASTER
	/* set Rx sources ssi_port <--> dai_port */
	DAM_PDCR(ssi_port) |= AUDMUX_PDCR_RXDSEL(dai_port);
	DAM_PDCR(dai_port) |= AUDMUX_PDCR_RXDSEL(ssi_port);

	/* set Tx frame direction and source  dai_port--> ssi_port output */
	DAM_PTCR(ssi_port) |= AUDMUX_PTCR_TFSDIR;
	DAM_PTCR(ssi_port) |= AUDMUX_PTCR_TFSSEL(AUDMUX_FROM_TXFS, dai_port);

	/* set Tx Clock direction and source dai_port--> ssi_port output */
	DAM_PTCR(ssi_port) |= AUDMUX_PTCR_TCLKDIR;
	DAM_PTCR(ssi_port) |= AUDMUX_PTCR_TCSEL(AUDMUX_FROM_TXFS, dai_port);
#else
	/* set Rx sources ssi_port <--> dai_port */
	DAM_PDCR(ssi_port) |= AUDMUX_PDCR_RXDSEL(dai_port);
	DAM_PDCR(dai_port) |= AUDMUX_PDCR_RXDSEL(ssi_port);

	/* set Tx frame direction and source  ssi_port --> dai_port output */
	DAM_PTCR(dai_port) |= AUDMUX_PTCR_TFSDIR;
	DAM_PTCR(dai_port) |= AUDMUX_PTCR_TFSSEL(AUDMUX_FROM_TXFS, ssi_port);

	/* set Tx Clock direction and source ssi_port--> dai_port output */
	DAM_PTCR(dai_port) |= AUDMUX_PTCR_TCLKDIR;
	DAM_PTCR(dai_port) |= AUDMUX_PTCR_TCSEL(AUDMUX_FROM_TXFS, ssi_port);
#endif

}

static int imx_3stack_pcm_new(struct snd_soc_pcm_link *pcm_link)
{
	struct imx_3stack_pcm_state *state;
	int ret;

	state = kzalloc(sizeof(struct imx_3stack_pcm_state), GFP_KERNEL);
	if (state == NULL)
		return -ENOMEM;

	pcm_link->audio_ops = &imx_3stack_hifi_ops;
	pcm_link->private_data = state;

	ret = snd_soc_pcm_new(pcm_link, 1, 1);
	if (ret < 0) {
		err("%s: failed to create hifi pcm\n", __func__);
		kfree(state);
		return ret;
	}

	info("i.MX 3STACK WM1808 Audio Driver\n");

	return 0;
}

static int imx_3stack_pcm_free(struct snd_soc_pcm_link *pcm_link)
{
	kfree(pcm_link->private_data);
	return 0;
}

static const struct snd_soc_pcm_link_ops imx_3stack_pcm_ops = {
	.new = imx_3stack_pcm_new,
	.free = imx_3stack_pcm_free,
};

/* imx_3stack machine audio map */
static const char *audio_map[][3] = {

	{NULL, NULL, NULL},
};

#ifdef CONFIG_PM
static int imx_wm1808_audio_suspend(struct platform_device *dev,
					   pm_message_t state)
{

	int ret = 0;

	return ret;
}

static int imx_wm1808_audio_resume(struct platform_device *dev)
{

	int ret = 0;

	return ret;
}

#else
#define imx_wm1808_audio_suspend	NULL
#define imx_wm1808_audio_resume		NULL
#endif

int mach_probe(struct snd_soc_machine *machine)
{
	struct snd_soc_codec *codec;
	struct snd_soc_pcm_link *pcm_link;
	void * pdata = platform_get_drvdata(machine->pdev);
	struct wm1808 *wm1808 = to_wm1808_from_audio(pdata);
	int i, ret;

	pcm_link = list_first_entry(&machine->active_list,
				    struct snd_soc_pcm_link, active_list);

	codec = pcm_link->codec;
	codec->control_data = wm1808;
	codec->ops->io_probe(codec, machine);

	/* set up imx_3stack specific audio path audio map */
	for (i = 0; audio_map[i][0] != NULL; i++) {
		snd_soc_dapm_connect_input(machine, audio_map[i][0],
					   audio_map[i][1], audio_map[i][2]);
	}

    snd_soc_dapm_set_policy(machine, SND_SOC_DAPM_POLICY_MANUAL);

	/* register card with ALSA upper layers */
	ret = snd_soc_register_card(machine);
	if (ret < 0) {
		err("%s: failed to register sound card\n",
		       __FUNCTION__);
		snd_soc_machine_free(machine);
		return ret;
	}
	return 0;
}

struct snd_soc_machine_ops machine_ops = {
	.mach_probe = mach_probe,
};

static int __devinit imx_wm1808_audio_probe(struct platform_device *pdev)
{
	struct snd_soc_machine **machine = platform_get_drvdata(pdev);
	struct snd_soc_pcm_link *hifi;
	struct mxc_audio_platform_data *tmp;
	int ret, hp_status;

	dbg("%s: pdev->name=%s, machine->pcm_links=%d", __func__, 
		pdev->name, (*machine)->pcm_links);

	(*machine)->owner = THIS_MODULE;
	(*machine)->pdev = pdev;
	(*machine)->name = "i.MX_SAMSUNG";
	(*machine)->longname = "WM1808";
	(*machine)->ops = &machine_ops;

	/* register card */
	ret =
	    snd_soc_new_card(*machine, 1, SNDRV_DEFAULT_IDX1,
			     SNDRV_DEFAULT_STR1);
	if (ret < 0) {
		err("%s: failed to create pcms\n", __func__);
		return ret;
	}

	/* WM1808 hifi interface */
	ret = -ENODEV;
	tmp = pdev->dev.platform_data;

	if (tmp->src_port == 2)
		hifi = snd_soc_pcm_link_new(*machine, "imx_3stack-hifi",
					    &imx_3stack_pcm_ops, imx_pcm,
					    wm1808_codec, wm1808_hifi_dai,
					    imx_ssi_3);
	else
		hifi = snd_soc_pcm_link_new(*machine, "imx_3stack-hifi",
					    &imx_3stack_pcm_ops, imx_pcm,
					    wm1808_codec, wm1808_hifi_dai,
					    imx_ssi_1);
	if (hifi == NULL) {
		dbg("failed to create HiFi PCM link");
		snd_soc_machine_free(*machine);
		return ret;
	}
	ret = snd_soc_pcm_link_attach(hifi);
	if (ret < 0) {
		dbg("%s: failed to attach hifi pcm", __func__);
		snd_soc_machine_free(*machine);
		return ret;
	}
	gpio_activate_audio_ports();

	imx_3stack_init_dam(tmp->src_port, tmp->ext_port);

	return ret;

}

static int __devexit
imx_wm1808_audio_remove(struct platform_device *pdev)
{
	struct snd_soc_machine **machine = platform_get_drvdata(pdev);
	struct snd_soc_codec *codec;
	struct snd_soc_pcm_link *pcm_link;

	pcm_link = list_first_entry(&(*machine)->active_list,
				    struct snd_soc_pcm_link, active_list);

	codec = pcm_link->codec;
	codec->ops->io_remove(codec, *machine);
	
	free_irq(IOMUX_TO_IRQ(MX37_PIN_CSPI1_SS1), 0);

	snd_soc_machine_free(*machine);
	return 0;
}

static struct platform_driver imx_wm1808_audio_driver = {
	.probe = imx_wm1808_audio_probe,
	.remove = __devexit_p(imx_wm1808_audio_remove),
	.suspend = imx_wm1808_audio_suspend,
	.resume = imx_wm1808_audio_resume,
	.driver = {
		   .name = imx_wm1808_audio,
		   },
};

static int __init imx_3stack_wm1808_audio_init(void)
{
	return platform_driver_register(&imx_wm1808_audio_driver);
}

static void __exit imx_3stack_wm1808_audio_exit(void)
{
	platform_driver_unregister(&imx_wm1808_audio_driver);
}

module_init(imx_3stack_wm1808_audio_init);
module_exit(imx_3stack_wm1808_audio_exit);

MODULE_AUTHOR("Wang Shaojun");
MODULE_DESCRIPTION("WM1808 Driver for i.MX");
MODULE_LICENSE("GPL");
