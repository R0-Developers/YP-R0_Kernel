/*
 * wm1808.h  --  Audio Driver for Wolfson WM1808
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

#ifndef __LINUX_WM1808_H
#define __LINUX_WM1808_H

/*
 * Register values.
 */

#define WM1808_MAX_REGISTER                     0x3D

/*
 * Field Definitions.
 */

#define WM1808_REGISTER_DEFAULTS \
{ \
    0x000,     /* R0   - Software reset */ \
    0x000,     /* R1   - Power mgmt (1) */ \
    0x000,     /* R2   - Power mgmt (2) */ \
    0x000,     /* R3   - Power mgmt (3) */ \
    0x050,     /* R4   - Audio interface */ \
    0x000,     /* R5   - Companding ctrl */ \
    0x140,     /* R6   - Clock gen ctrl */ \
    0x080,     /* R7   - Additional ctrl */ \
    0x000,     /* R8   - Gpio stuff */ \
    0x000,     /* R9   - Jack detect ctrl 1 */ \
    0x000,     /* R10  - DAC ctrl */ \
    0x0FF,     /* R11  - Left DAC digital vol */ \
    0x0FF,     /* R12  - Right DAC digital vol */ \
    0x000,     /* R13  - Jack detect ctrl 2 */ \
    0x100,     /* R14  - ADC ctrl */ \
    0x0FF,     /* R15  - Left ADC digital vol */ \
    0x0FF,     /* R16  - Right ADC digital vol */ \
    0x000,     /* R17 */ \
    0x12C,     /* R18  - EQ1 */ \
    0x02C,     /* R19  - EQ2 */ \
    0x02C,     /* R20  - EQ3 */ \
    0x02C,     /* R21  - EQ4 */ \
    0x02C,     /* R22  - EQ5 */ \
    0x000,     /* R23 */ \
    0x032,     /* R24  - DAC Limiter 1 */ \
    0x000,     /* R25  - DAC Limiter 2 */ \
    0x000,     /* R26 */ \
    0x000,     /* R27  - Notch Filter 1 */ \
    0x000,     /* R28  - Notch Filter 2 */ \
    0x000,     /* R29  - Notch Filter 3 */ \
    0x000,     /* R30  - Notch Filter 4 */ \
    0x000,     /* R31 */ \
    0x038,     /* R32  - ALC ctrl 1 */ \
    0x00B,     /* R33  - ALC ctrl 2 */ \
    0x032,     /* R34  - ALC ctrl 3 */ \
    0x000,     /* R35  - Noise gate */ \
    0x008,     /* R36  - PLL N */ \
    0x00C,     /* R37  - PLL K 1 */ \
    0x093,     /* R38  - PLL K 2 */ \
    0x0E9,     /* R39  - PLL K 3 */ \
    0x000,     /* R40 */ \
    0x000,     /* R41  - 3D ctrl */ \
    0x000,     /* R42  - VMID current */ \
    0x000,     /* R43  - Beep ctrl */ \
    0x003,     /* R44  - INP ctrl */ \
    0x010,     /* R45  - Left INP PGA gain ctrl */ \
    0x000,     /* R46 */ \
    0x100,     /* R47  - Left ADC boost ctrl */ \
    0x000,     /* R48  - Right ADC Boost ctrl */ \
    0x002,     /* R49  - Output ctrl */ \
    0x001,     /* R50  - Left mixer ctrl */ \
    0x001,     /* R51  - Right mixer ctrl */ \
    0x039,     /* R52  - LOUT1 (HP) vol ctrl */ \
    0x039,     /* R53  - ROUT1 (HP) vol ctrl */ \
    0x039,     /* R54  - LOUT2 (SPK) vol ctrl */ \
    0x039,     /* R55  - ROUT2 (SPK) vol ctrl */ \
    0x001,     /* R56  - OUT3 mixer ctrl */ \
    0x000,     /* R57 */ \
    0x000,     /* R58 */ \
    0x000,     /* R59 */ \
    0x000,     /* R60 */ \
    0x000,     /* R61  - Bias ctrl */ \
}

/*
 * Write access masks.
 * Note: All the WM1808 register can not be read. 
 */

#define WM1808_ACCESS \
{  /* write */ \
    0xFFF,     /* R0   - Software reset */ \
    0x06F,     /* R1   - Power mgmt (1) */ \
    0x1F7,     /* R2   - Power mgmt (2) */ \
    0x0EF,     /* R3   - Power mgmt (3) */ \
    0x1FF,     /* R4   - Audio interface */ \
    0x03F,     /* R5   - Companding ctrl */ \
    0x1FD,     /* R6   - Clock gen ctrl */ \
    0x18F,     /* R7   - Additional ctrl */ \
    0x1FF,     /* R8   - Gpio stuff */ \
    0x070,     /* R9   - Jack detect ctrl 1 */ \
    0x04F,     /* R10  - DAC ctrl */ \
    0x1FF,     /* R11  - Left DAC digital vol */ \
    0x1FF,     /* R12  - Right DAC digital vol */ \
    0x0FF,     /* R13  - Jack detect ctrl 2 */ \
    0x1FB,     /* R14  - ADC ctrl */ \
    0x1FF,     /* R15  - Left ADC digital vol */ \
    0x1FF,     /* R16  - Right ADC digital vol */ \
    0x000,     /* R17 */ \
    0x17F,     /* R18  - EQ1 */ \
    0x17F,     /* R19  - EQ2 */ \
    0x17F,     /* R20  - EQ3 */ \
    0x17F,     /* R21  - EQ4 */ \
    0x07F,     /* R22  - EQ5 */ \
    0x000,     /* R23 */ \
    0x1FF,     /* R24  - DAC Limiter 1 */ \
    0x07F,     /* R25  - DAC Limiter 2 */ \
    0x000,     /* R26 */ \
    0x1FF,     /* R27  - Notch Filter 1 */ \
    0x17F,     /* R28  - Notch Filter 2 */ \
    0x17F,     /* R29  - Notch Filter 3 */ \
    0x17F,     /* R30  - Notch Filter 4 */ \
    0x000,     /* R31 */ \
    0x13F,     /* R32  - ALC ctrl 1 */ \
    0x0FF,     /* R33  - ALC ctrl 2 */ \
    0x1FF,     /* R34  - ALC ctrl 3 */ \
    0x00F,     /* R35  - Noise gate */ \
    0x01F,     /* R36  - PLL N */ \
    0x03F,     /* R37  - PLL K 1 */ \
    0x1FF,     /* R38  - PLL K 2 */ \
    0x1FF,     /* R39  - PLL K 3 */ \
    0x000,     /* R40 */ \
    0x00F,     /* R41  - 3D ctrl */ \
    0x004,     /* R42  - VMID current */ \
    0x180,     /* R43  - Beep ctrl */ \
    0x007,     /* R44  - INP ctrl */ \
    0x1FF,     /* R45  - Left INP PGA gain ctrl */ \
    0x000,     /* R46 */ \
    0x177,     /* R47  - Left ADC boost ctrl */ \
    0x077,     /* R48  - Right ADC Boost ctrl */ \
    0x067,     /* R49  - Output ctrl */ \
    0x1FF,     /* R50  - Left mixer ctrl */ \
    0x1E1,     /* R51  - Right mixer ctrl */ \
    0x1FF,     /* R52  - LOUT1 (HP) vol ctrl */ \
    0x1FF,     /* R53  - ROUT1 (HP) vol ctrl */ \
    0x1FF,     /* R54  - LOUT2 (SPK) vol ctrl */ \
    0x1FF,     /* R55  - ROUT2 (SPK) vol ctrl */ \
    0x047,     /* R56  - OUT3 mixer ctrl */ \
    0x000,     /* R57 */ \
    0x000,     /* R58 */ \
    0x000,     /* R59 */ \
    0x000,     /* R60 */ \
    0x100,     /* R61  - Bias ctrl */ \
}

#define WM1808_SOFTWARE_RESET					0x0
#define WM1808_POWER_MGMT_1						0x1
#define WM1808_POWER_MGMT_2						0x2
#define WM1808_POWER_MGMT_3						0x3
#define WM1808_AUDIO_INTERFACE					0x4
#define WM1808_COMPANDING_CTRL					0x5
#define WM1808_CLK_GEN_CTRL						0x6
#define WM1808_ADDITIONAL_CTRL					0x7
#define WM1808_GPIO_STUFF						0x8
#define WM1808_JACK_DET_CTRL_1					0x9
#define WM1808_DAC_CTRL							0xA
#define WM1808_LEFT_DAC_DIG_VOL					0xB
#define WM1808_RIGHT_DAC_DIG_VOL				0xC
#define WM1808_JACK_DET_CTRL_2					0xD
#define WM1808_ADC_CTRL							0xE
#define WM1808_LEFT_ADC_DIG_VOL					0xF
#define WM1808_RIGHT_ADC_DIG_VOL				0x10
#define WM1808_EQ1								0x12
#define WM1808_EQ2								0x13
#define WM1808_EQ3								0x14
#define WM1808_EQ4								0x15
#define WM1808_EQ5								0x16
#define WM1808_DAC_LIMITER_1					0x18
#define WM1808_DAC_LIMITER_2					0x19
#define WM1808_NOTCH_FILTER_1					0x1B
#define WM1808_NOTCH_FILTER_2					0x1C
#define WM1808_NOTCH_FILTER_3					0x1D
#define WM1808_NOTCH_FILTER_4					0x1E
#define WM1808_ALC_CTRL_1						0x20
#define WM1808_ALC_CTRL_2						0x21
#define WM1808_ALC_CTRL_3						0x22
#define WM1808_NOISE_GATE						0x23
#define WM1808_PLL_N							0x24
#define WM1808_PLL_K_1							0x25
#define WM1808_PLL_K_2							0x26
#define WM1808_PLL_K_3							0x27
#define WM1808_3D_CTRL							0x29
#define WM1808_VMID_CURRENT						0x2A
#define WM1808_BEEP_CTRL						0x2B
#define WM1808_INP_CTRL							0x2C
#define WM1808_LEFT_INP_PGA_GAIN_CTRL			0x2D
#define WM1808_LEFT_ADC_BOOST_CTRL				0x2F
#define WM1808_RIGHT_ADC_BOOST_CTRL				0x30
#define WM1808_OUTPUT_CTRL						0x31
#define WM1808_LEFT_MIXER_CTRL					0x32
#define WM1808_RIGHT_MIXER_CTRL					0x33
#define WM1808_LOUT1_HP_VOL_CTRL				0x34
#define WM1808_ROUT1_HP_VOL_CTRL				0x35
#define WM1808_LOUT2_SPK_VOL_CTRL				0x36
#define WM1808_ROUT2_SPK_VOL_CTRL				0x37
#define WM1808_OUT3_MIXER_CTRL					0x38
#define WM1808_BIAS_CTRL						0x3D

/*
 * R1 (0x01) - Power mgmt (1)
 */
#define WM1808_OUT3_MIXEN						0x040
#define WM1808_PLLEN							0x020
#define WM1808_BIASEN							0x008
#define WM1808_BUFIOEN							0x004
#define WM1808_VMIDSEL_MASK						0x003

/*
 * R2 (0x02) - Power mgmt (2)
 */
#define WM1808_ROUT1EN							0x100
#define WM1808_LOUT1EN							0x080	
#define WM1808_SLEEP							0x040
#define WM1808_BOOSTENR							0x020	
#define WM1808_BOOSTENL							0x010
#define WM1808_INPPGAENL						0x004
#define WM1808_ADCENR							0x002
#define WM1808_ADCENL							0x001

/*
 * R3 (0x03) - Power mgmt (3)
 */
#define WM1808_OUT3EN							0x080
#define WM1808_ROUT2EN							0x040
#define WM1808_LOUT2EN							0x020
#define WM1808_RMIXEN							0x008
#define WM1808_LMIXEN							0x004
#define WM1808_DACENR							0x002
#define WM1808_DACENL							0x001

/*
 * R4 (0x04) - Audio interface
 */
#define WM1808_BCP								0x100
#define WM1808_LRP								0x080
#define WM1808_WL_MASK							0x060
#define WM1808_FMT_MASK							0x018
#define WM1808_DLRSWAP							0x004
#define WM1808_ALRSWAP							0x002
#define WM1808_MONO								0x001

/*
 * R5 (0x05) - Companding ctrl
 */
#define WM1808_WL8								0x020
#define WM1808_DAC_COMP_MASK					0x018
#define WM1808_ADC_COMP_MASK					0x006
#define WM1808_LOOPBACK							0x001

/*
 * R6 (0x06) - Clock gen ctrl
 */
#define WM1808_CLKSEL							0x100
#define WM1808_MCLKDIV_MASK						0x0E0
#define WM1808_BCLKDIV_MASK						0x01C
#define WM1808_MS								0x001

/*
 * R7 (0x07) - Additional ctrl
 */
#define WM1808_M128ENB							0x100
#define WM1808_SR_MASK							0x00E
#define WM1808_SLOWCLKEN						0x001

/*
 * R8 (0x08) - Gpio stuff
 */
#define WM1808_GPIOGP							0x100
#define WM1808_GPIOGPU							0x080
#define WM1808_GPIOGPD							0x040
#define WM1808_OPCLKDIV_MASK					0x030
#define WM1808_GPIO1POL							0x008
#define WM1808_GPIO1SEL_MASK					0x007

/*
 * R9 (0x09) - Jack detect ctrl 1
 */
#define WM1808_JD_EN							0x040
#define WM1808_JD_SEL_MASK						0x030

/*
 * R10 (0x0A) - DAC ctrl
 */
#define WM1808_SOFTMUTE							0x040
#define WM1808_DACOSR128						0x008
#define WM1808_AMUTE							0x004
#define WM1808_DACPOLR							0x002
#define WM1808_DACPOLL							0x001

/*
 * R11 (0x0B) - Left DAC digital vol
 */
#define WM1808_DACVU_L							0x100
#define WM1808_DACVOLL_MASK						0x0FF

/*
 * R12 (0x0C) - Right DAC digital vol
 */
#define WM1808_DACVU_R							0x100
#define WM1808_DACVOLR_MASK						0x0FF

/*
 * R13 (0x0D) - Jack detect ctrl 2
 */
#define WM1808_JD_EN1_MASK						0x0F0
#define WM1808_JD_EN0_MASK						0x00F

/*
 * R14 (0x0E) - ADC ctrl
 */
#define WM1808_HPFEN							0x100
#define WM1808_HPFAPP							0x080
#define WM1808_HPFCUT_MASK						0x070
#define WM1808_ADCOSR128						0x008
#define WM1808_ADCRPOL							0x002
#define WM1808_ADCLPOL							0x001

/*
 * R15 (0x0F) - Left ADC digital vol
 */
#define WM1808_ADCVU_L							0x100
#define WM1808_ADCVOLL_MASK						0x0FF

/*
 * R16 (0x10) - Right ADC digital vol 
 */
#define WM1808_ADCVU_R							0x100
#define WM1808_ADCVOLR_MASK						0x0FF

/*
 * R18 (0x12) - EQ1
 */
#define WM1808_EQ3DMODE							0x100
#define WM1808_EQ1C_MASK						0x060
#define WM1808_EQ1G_MASK						0x01F

/*
 * R19 (0x13) -EQ2 
 */
#define WM1808_EQ2BW							0x100
#define WM1808_EQ2C_MASK						0x060
#define WM1808_EQ2G_MASK						0x01F

/*
 * R20 (0x14) - EQ3
 */
#define WM1808_EQ3BW							0x100
#define WM1808_EQ3C_MASK						0x060
#define WM1808_EQ3G_MASK						0x01F

/*
 * R21 (0x15) - EQ4
 */
#define WM1808_EQ4BW							0x100
#define WM1808_EQ4C_MASK						0x060
#define WM1808_EQ4G_MASK						0x01F

/*
 * R22 (0x16) - EQ5
 */
#define WM1808_EQ5C_MASK						0x060
#define WM1808_EQ5G_MASK						0x01F

/*
 * R24 (0x18) - DAC Limiter 1
 */
#define WM1808_LIMEN							0x100
#define WM1808_LIMDCY_MASK						0x0F0
#define WM1808_LIMATK_MASK						0x00F

/*
 * R25 (0x19) - DAC Limiter 2
 */
#define WM1808_LIMLVL_MASK						0x070
#define WM1808_LIMBOOST_MASK					0x00F

/*
 * R27 (0x1B) -Notch Filter 1 
 */
#define WM1808_NFU_1							0x100
#define WM1808_NFEN_1							0x080
#define WM1808_NFA0_H_MASK						0x07F

/*
 * R28 (0x1C) - Notch Filter 2
 */
#define WM1808_NFU_2							0x100
#define WM1808_NFA0_L_MASK						0x07F

/*
 * R29 (0x1D) - Notch Filter 3
 */
#define WM1808_NFU_3							0x100
#define WM1808_NFA1_H_MASK						0x07F

/*
 * R30 (0x1E) - Notch Filter 4
 */
#define WM1808_NFU_4							0x100
#define WM1808_NFA1_L_MASK						0x07F

/*
 * R32 (0x20) - ALC ctrl 1
 */
#define WM1808_ALCSEL							0x100
#define WM1808_ALCMAXGAIN_MASK					0x038
#define WM1808_ALCMINGAIN_MASK					0x007

/*
 * R33 (0x21) - ALC ctrl 2 
 */
#define WM1808_ALCHLD							0x0F0
#define WM1808_ALCLVL_MASK						0x00F

/*
 * R34 (0x22) - ALC ctrl 3
 */
#define WM1808_ALCMODE							0x100
#define WM1808_ALCDCY_MASK						0x0F0
#define WM1808_ALCATK_MASK						0x00F

/*
 * R35 (0x23) - Noise gate
 */
#define WM1808_NGEN								0x008
#define WM1808_NGTH_MASK						0x007

/*
 * R36 (0x24) - PLL N
 */
#define WM1808_PLLPRESCALE						0x010
#define WM1808_PLLN								0x00F

/*
 * R37 (0x25) - PLL K 1
 */
#define WM1808_PLLK_1_MASK						0x03F

/*
 * R38 (0x26) - PLL K 2
 */
#define WM1808_PLLK_2_MASK						0x1FF

/*
 * R39 (0x27) - PLL K 3
 */
#define WM1808_PLLK_3_MASK						0x1FF

/*
 * R41 (0x29) - 3D ctrl 
 */
#define WM1808_DEPTH3D_MASK						0x00F

/*
 * R42 (0x2A) - VMID current
 */
#define WM1808_POBCTRL							0x004

/*
 * R43 (0x2B) - Beep ctrl 
 */
#define WM1808_BYPL2RMIX						0x100
#define WM1808_BEEPVOL_MASK						0x00E
#define WM1808_BEEPEN							0x001

/*
 * R44 (0x2C) - INP ctrl
 */
#define WM1808_L2_2INPPGA						0x004
#define WM1808_LIN2INPPGA						0x002
#define WM1808_LIP2INPPGA						0x001

/*
 * R45 (0x2D) - Left INP PGA gain ctrl
 */
#define WM1808_INPPGAU							0x100
#define WM1808_INPPGAZCL						0x080
#define WM1808_INPPGAMUTEL						0x040
#define WM1808_INPPGAVOLL						0x03F

/*
 * R47 (0x2F) - Left ADC boost ctrl
 */
#define WM1808_PGABOOSTL						0x100
#define WM1808_L2_2BOOSTVOL_MASK				0x070
#define WM1808_AUXL2BOOSTVOL_MASK				0x007

/*
 * R48 (0x30) - Right ADC Boost ctrl
 */
#define WM1808_R2_2BOOSTVOL_MASK				0x070
#define WM1808_AUXR2BOOSTVOL_MASK				0x007

/*
 * R49 (0x31) - Output ctrl
 */
#define WM1808_DACL2RMIN						0x040
#define WM1808_DACR2LMIN						0x020
#define WM1808_TSOPCTRL							0x004
#define WM1808_TSDEN							0x002
#define WM1808_VROI								0x001

/*
 * R50 (0x32) - Left mixer ctrl
 */
#define WM1808_AUXLMIXVOL_MASK					0x1C0
#define WM1808_AUXL2LMIX						0x020
#define WM1808_BYPLMIXVOL_MASK					0x01C
#define WM1808_BYPL2LMIX						0x002
#define WM1808_DACL2LMIX						0x001

/*
 * R51 (0x33) - Right mixer ctrl
 */
#define WM1808_AUXRMIXVOL_MASK					0x1C0
#define WM1808_AUXR2RMIX						0x020
#define WM1808_DACR2RMIX						0x001

/*
 * R52 (0x34) - LOUT1 (HP) vol ctrl
 */
#define WM1808_OUT1VU							0x100
#define WM1808_LOUT1ZC							0x080
#define WM1808_LOUT1MUTE						0x040
#define WM1808_LOUT1VOL_MASK					0x03F

/*
 * R53 (0x35) - ROUT1 (HP) vol ctrl
 */
#define WM1808_ROUT1ZC							0x080
#define WM1808_ROUT1MUTE						0x040
#define WM1808_ROUT1VOL_MASK					0x03F

/*
 * R54 (0x36) - LOUT2 (SPK) vol ctrl
 */
#define WM1808_OUT2VU							0x100
#define WM1808_LOUT2ZC							0x080
#define WM1808_LOUT2MUTE						0x040
#define WM1808_LOUT2VOL_MASK					0x03F

/*
 * R55 (0x37) - ROUT2 (SPK) vol ctrl
 */
#define WM1808_ROUT2ZC							0x080
#define WM1808_ROUT2MUTE						0x040
#define WM1808_ROUT2VOL_MASK					0x03F

/*
 * R56 (0x38) - OUT3 mixer ctrl
 */
#define WM1808_OUT3MUTE							0x040
#define WM1808_BYPL2OUT3						0x004
#define WM1808_LMIX2OUT3						0x002
#define WM1808_LDAC3OUT3						0x001

/*
 * R61 (0x3D) - Bias ctrl
 */
#define WM1808_BIASCUT						0x100

/*
 * Clock sources & directions
 */
#define WM1808_MCLK_SEL_MCLK	0
#define WM1808_MCLK_SEL_PLL		1


#define WM1808_MCLK_DIR_OUT 	0
#define WM1808_MCLK_DIR_IN 	1

/* clock divider id's */
#define WM1808_ADC_CLKDIV	0
#define WM1808_DAC_CLKDIV	1
#define WM1808_BCLK_CLKDIV	2
#define WM1808_OPCLK_CLKDIV	3
#define WM1808_MCLK_CLKDIV	4
#define WM1808_SR			5

/* R14, ADC clock dividers */
#define WM1808_ADCDIV_2	(0x0 << 3)
#define WM1808_ADCDIV_4	(0x1 << 3)

/* R10, DAC clock dividers */
#define WM1808_DACDIV_2	(0x0 << 3)
#define WM1808_DACDIV_4	(0x1 << 3)

/*R6, BCLK clock dividers */
#define WM1808_BCLK_DIV_1	(0x0 << 2)
#define WM1808_BCLK_DIV_2	(0x1 << 2)
#define WM1808_BCLK_DIV_4	(0x2 << 2)
#define WM1808_BCLK_DIV_8	(0x3 << 2)
#define WM1808_BCLK_DIV_16	(0x4 << 2)
#define WM1808_BCLK_DIV_32	(0x5 << 2)

/* R6, sys (M) clock dividers */
#define WM1808_MCLK_DIV_1	(0x0 << 5)
#define WM1808_MCLK_DIV_1_5	(0x1 << 5)
#define WM1808_MCLK_DIV_2	(0x2 << 5)
#define WM1808_MCLK_DIV_3	(0x3 << 5)
#define WM1808_MCLK_DIV_4	(0x4 << 5)
#define WM1808_MCLK_DIV_6	(0x5 << 5)
#define WM1808_MCLK_DIV_8	(0x6 << 5)
#define WM1808_MCLK_DIV_12	(0x7 << 5)

/* R7, filter sample rate */
#define WM1808_SR_8K		(0x5 << 1)
#define WM1808_SR_12K		(0x4 << 1)
#define WM1808_SR_16K		(0x3 << 1)
#define WM1808_SR_24K		(0x2 << 1)
#define WM1808_SR_32K		(0x1 << 1)
#define WM1808_SR_48K		(0x0 << 1)

#define to_wm1808_from_audio(d) container_of(d, struct wm1808, audio)

struct wm1808 {
	/* device IO */
	unsigned short i2c_address;
	struct i2c_client *i2c_client;
	u16 *reg_cache;

	/* clients */
	struct snd_soc_machine *audio;
	
	/* irq handlers and workq */
	struct work_struct work;
	struct mutex work_mutex;
};

/*
 * WM1808 device IO
 */
int wm1808_clear_bits(struct wm1808 *wm1808, u8 reg, u16 mask);
int wm1808_set_bits(struct wm1808 *wm1808, u8 reg, u16 mask);
u16 wm1808_reg_read(struct wm1808 *wm1808, u8 reg);
int wm1808_reg_write(struct wm1808 *wm1808, u8 reg, u16 val);
int wm1808_create_cache(struct wm1808 *wm1808);

#endif	/* __LINUX_WM1808_H */
