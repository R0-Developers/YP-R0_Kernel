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
 *  Header for WM1808 CODEC register driver module
 *
 * @name        Audio register driver for WM1800 (DrvWM1800RegMgr.h)
 * @author      dong yoon park (dongyoon21.park@samsung.com)
 * @version     1.0
 */

#ifndef DRV_WM1808_REGMGR_H_
#define DRV_WM1808_REGMGR_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//#include "../../../include/sound/soc.h"
#include <sound/soc.h>

// type definition for Muon Code 
typedef signed char			MInt8;
typedef unsigned int		MUint;
typedef unsigned char		MUint8;
typedef short				MInt16;
typedef unsigned short		MUint16;
typedef long				MInt32;
typedef unsigned long		MUint32;
typedef long long			MInt64;
typedef unsigned long long	MUint64;
typedef unsigned int		MAddr;
typedef unsigned int		MDid;
typedef int					MBool;

#define TRUE 1
#define FALSE 0

/*======================================================================================================
 * Software reset R0(00h)
 */
#define	DRV_WM1808_SOFTWARE_RESET	0x00

#define DRV_WM1808_SOFTWARE_RESET_DEFAULT 0x0000
/*TODO*/

/*======================================================================================================
 * Power Management 1 R1(01h)
 */
#define DRV_WM1808_POWER_MANAGEMENT1 0x01

#define DRV_WM1808_POWER_MANAGEMENT1_DEFAULT 0x0000

#define	DRV_WM1808_OUT3MIXEN(X) ((X&0x01)<<6)		/* OUT3 mixer enable, default 0 */
	#define DRV_WM1808_OUT3MIXEN_DISABLED 0x00			/* 0 = diasabled*/
	#define DRV_WM1808_OUT3MIXEN_ENABLED  0x01			/* 1 = enabled*/

#define	DRV_WM1808_PLLEN(X) ((X&0x01)<<5)			/* PLL enable, default 0 */
	#define DRV_WM1808_PLLEN_OFF 0x00					/* 0 = PLL off */
	#define DRV_WM1808_PLLEN_ON  0x01					/* 1 = PLL on */

#define DRV_WM1808_BIASEN(X) ((X&0x01)<<3)			/* Analogue amplifier bias control, default 0 */
	#define DRV_WM1808_BIASEN_DISABLED 0x00				/* 0 = disabled */
	#define DRV_WM1808_BIASEN_ENABLED  0x01				/* 1 = enabled */

#define DRV_WM1808_BUFIOEN(X) ((X&0x01)<<2)			/* Unused input/output tie off buffer enable, default 0 */
	#define DRV_WM1808_BUFIOEN_DISABLED	0x00			/* 0 = disabled */
	#define DRV_WM1808_BUFIOEN_ENABLED  0x01			/* 1 = enabled */

#define DRV_WM1808_VMIDSEL(X) ((X&0x03)<<0)			/* Reference string impedence to VMID pin, default 0 */
	#define DRV_WM1808_VMIDSEL_OFF		0x00			/* 0 = off (open circuit) */
	#define DRV_WM1808_VMIDSEL_75K_OHM	0x01			/* 1 = 75K ohm */
	#define DRV_WM1808_VMIDSEL_300K_OHM 0x02			/* 2 = 300K ohm */
	#define DRV_WM1808_VMIDSEL_5K_OHM	0x03			/* 3 = 5K ohm */

/*======================================================================================================
 * Power Management 2 R2(02h)
 */
#define DRV_WM1808_POWER_MANAGEMENT2 0x02	

#define DRV_WM1808_POWER_MANAGEMENT2_DEFAULT 0x0000

#define DRV_WM1808_ROUT1_EN(X) ((X&0x01)<<8)								/* ROUT1 output enable, default 0 */
	#define DRV_WM1808_ROUT1_EN_DISABLED 0x00									/* 0 = disabled */
	#define DRV_WM1808_ROUT1_EN_ENABLED  0x01									/* 1 = enable */	

#define DRV_WM1808_LOUT1_EN(X) ((X&0x01)<<7)								/* LOUT1 output enable, default 0 */
	#define DRV_WM1808_LOUT1_EN_DISABLED 0x00									/* 0 = disabled */
	#define DRV_WM1808_LOUT1_EN_ENABLED  0x01									/* 1 = enabled */

#define DRV_WM1808_SLEEP(X) ((X&0x01)<<6)									/* default 0 */
	#define DRV_WM1808_NORMAL_DEVICE_OPERATION					  0x00			/* 0 = normal device operation */
	#define DRV_WM1808_RESIDUAL_CURRENT_REDUCED_IN_DEVICE_STANDBY 0x01			/* 1 = residual current reduced in device standby mode 
																			(Device must be standby modesetting this bit)*/
																
#define DRV_WM1808_BOOSTENR(X) ((X&0x01)<<5)								/* Right channel Input BOOST enable, default 0 */
	#define DRV_WM1808_BOOST_STAGE_OFF 0x00										/* 0 = Boost stage OFF */	
	#define DRV_WM1808_BOOST_STAGE_ON  0x01										/* 1 = Boost stage ON */

#define DRV_WM1808_BOOSTENL(X) ((X&0x01)<<4)								/* Left channel Input PGA enable, default 0 */
	#define DRV_WM1808_BOOST_STAGE_OFF 0x00										/* 0 = disabled */
	#define DRV_WM1808_BOOST_STAGE_ON  0x01										/* 1 = enabled */	

#define DRV_WM1808_INPPGAENL(X) ((X&0x01)<<2)								/*Left channel input PGA enable, default 0 */			
	#define DRV_WM1808_INPPGAENL_DISABLED 0x00									/* 0 = disabled */
	#define DRV_WM1808_INPPGAENL_ENABLED  0x01									/* 1 = enabled */	

#define DRV_WM1808_ADCENR(X) ((X&0x01)<<1)									/* Enable ADC right channel, default 0 */
	#define DRV_WM1808_ADCENR_DISABLED 0x00										/* 0 = ADC disabled */
	#define DRV_WM1808_ADCENR_ENABLED  0x01										/* 1 = ADC enabled */

#define DRV_WM1808_ADCENL(X) ((X&0x01)<<0)									/* Enable ADC left channel, default 0 */
	#define DRV_WM1808_ADCENL_DISABLED 0x00										/* 0 = ADC disabled */
	#define DRV_WM1808_ADCENL_ENABLED  0x01										/* 1 = ADC enabled */

/*======================================================================================================
 * Power Management 3 R3(03h)
 */
#define DRV_WM1808_POWER_MANAGEMENT3 0x03

#define DRV_WM1808_POWER_MANAGEMENT3_DEFAULT 0x0000

#define DRV_WM1808_OUT3EN(X) ((X&0x01)<<7)							/* OUT3 enable, default 0 */
	#define DRV_WM1808_OUT3EN_DISABLED 0x00								/* 0 = disabled */
	#define DRV_WM1808_OUT3EN_ENABLED  0x01								/* 1 = enabled */

#define DRV_WM1808_ROUT2EN(X) ((X&0x01)<<6)							/* ROUT2 enable, default 0 */
	#define DRV_WM1808_ROUT2EN_DISABLED 0x00							/* 0 = disabled */	
	#define DRV_WM1808_ROUT2EN_ENABLED  0x01							/* 1 = enabled */

#define DRV_WM1808_LOUT2EN(X) ((X&0x01)<<5)							/* LOUT2 enable, default 0 */	
	#define DRV_WM1808_LOUT2EN_DISABLED 0x00							/* 0 = disabled */
	#define DRV_WM1808_LOUT2EN_ENABLED  0x01							/* 1 = enabled */

#define DRV_WM1808_RMIXEN(X) ((X&0x01)<<3)							/* Right output channel mixer enable, default 0 */
	#define DRV_WM1808_RMIXEN_DISABLED 0x00								/* 0 = disabled */
	#define DRV_WM1808_RMIXEN_ENABLED  0x01								/* 1 = enabled */	

#define DRV_WM1808_LMIXEN(X) ((X&0x01)<<2)							/* Left output channel mixer enable, default 0 */
	#define DRV_WM1808_LMIXEN_DISABLED 0x00								/* 0 = disabled */	
	#define DRV_WM1808_LMIXEN_ENABLED  0x01								/* 1 = enabled */

#define DRV_WM1808_DACENR(X) ((X&0x01)<<1)							/* Right channel DAC enable, default 0 */
	#define DRV_WM1808_DACENR_DISABLED 0x00								/* 0 = DAC disabled */
	#define DRV_WM1808_DACENR_ENABLED  0x01								/* 1 = DAC enabled */

#define DRV_WM1808_DACENL(X) ((X&0x01)<<0)							/* Left channel DAC enable, default 0 */
	#define DRV_WM1808_DACENL_DISABLED 0x00								/* 0 = DAC disabled */
	#define DRV_WM1808_DACENL_ENABLED  0x01								/* 1 = DAC enabled */

/*======================================================================================================
 * Audio Interface R4(04h)
 */
#define DRV_WM1808_AUDIO_INTERFACE 0x04

#define DRV_WM1808_AUDIO_INTERFACE_DEFAULT 0x0050

#define DRV_WM1808_BCP(X) ((X&0x01)<<8)							/* BCLK polarity, default 0 */
	#define DRV_WM1808_BCP_NORMAL   0x00							/* 0 = normal */
	#define DRV_WM1808_BCP_INVERTED 0x01							/* 1 = inverted */

#define DRV_WM1808_LRP(X) ((X&0x01)<<7)							/* LRC clock polarity, default 0 */
	#define DRV_WM1808_LRP_NORMAL   0x00							/* 0 = normal */
	#define DRV_WM1808_LRP_INVERTED 0x01							/* 1 = inverted */

#define	DRV_WM1808_WL(X) ((X&0x03)<<5)							/* Word length, default 2 */
	#define DRV_WM1808_WL_16_BITS 0x00								/* 0 = 16 bits */
	#define DRV_WM1808_WL_20_BITS 0x01								/* 1 = 20 bits */
	#define DRV_WM1808_WL_24_BITS 0x02								/* 2 = 24 bits */
	#define DRV_WM1808_WL_32_BITS 0x03								/* 3 = 32 bits */

#define DRV_WM1808_FMT(X) ((X&0x03)<<3)							/* Audio Interface Data Format Select, default 2 */
	#define DRV_WM1808_FMT_RIGHT_JUSTIFIED  0x00					/* 0 = Right Justified */
	#define DRV_WM1808_FMT_LEFT_JUSTIFIED   0x01					/* 1 = Left Justified */	
	#define DRV_WM1808_FMT_IIS_FORMAT		0x02					/* 2 = I2S format */
	#define DRV_WM1808_FMT_DSP_PCM_MODE	    0x03					/* 3 = DSP/PCM mode */

#define DRV_WM1808_DLRSWAP(X) ((X&0x01)<<2)						/* Controls whether DAC data appears in 'right' or 'left' phases of LRC clock, default 0*/
	#define DRV_WM1808_DAC_LEFT_PHASE_OF_LRC	0x00				/* 0 = DAC data appear in 'left' phase of LRC */
	#define DRV_WM1808_DAC_RIGHT_PHASE_OF_LRC  0x01					/* 1 = DAC data appear in 'right' phase of LRC */

#define DRV_WM1808_ALRSWAP(X) ((X&0x01)<<1)						/* Controls whether ADC data appears in 'right' or 'left' phases of LRC clock, default 0*/
	#define DRV_WM1808_ADC_LEFT_PHASE_OF_LRC	0x00				/* 0 = ADC data appear in 'left' phase of LRC */
	#define DRV_WM1808_ADC_RIGHT_PHASE_OF_LRC   0x01				/* 1 = ADC data appear in 'right' phase of LRC */

#define DRV_WM1808_MONO(X) ((X&0x01)<<0)								/* Selects between stereo and mono device operation, default 0 */								
	#define DRV_WM1808_MONO_STEREO 0x00								/* 0 = Stereo device operation */
	#define DRV_WM1808_MONO_MONO   0x01								/* 1 = Mono device operation. Data appears in 'left' phase of LRC */

/*======================================================================================================
 * Companding ctrl R5(05h)
 */
#define DRV_WM1808_COMPANDING_CTRL 0x05

#define DRV_WM1808_COMPANDING_CTRL_DEFAULT 0x0000

#define DRV_WM1808_WL8(X) ((X&0x01)<<5)					/* Companding Control 8-bit mode, default 0 */
	#define DRV_WM1808_WL8_OFF				  0x00			/* 0 = off */
	#define DRV_WM1808_WL8_8_BIT_MODE_OPERATE 0x01		/* 1 = device operates in 8-bit mode */

#define DRV_WM1808_DAC_COMP(X) ((X&0x03)<<3)			/* DAC companding, default 0 */
	#define DRV_WM1808_DAC_COMP_OFF	   0x00					/* 0 = off(linear mode) */
	#define DRV_WM1808_DAC_COMP_MU_LAW 0x02				/* 2 = mu-law */	
	#define DRV_WM1808_DAC_COMP_A_LAW  0x03					/* 3 = A-law */

#define DRV_WM1808_ADC_COMP(X) ((X&0x03)<<1)			/* ADC companding, default 0 */
	#define DRV_WM1808_ADC_COMP_OFF	   0x00					/* 0 = off(linear mode) */
	#define DRV_WM1808_ADC_COMP_MU_LAW 0x02				/* 2 = mu-law */
	#define DRV_WM1808_ADC_COMP_A_LAW  0x03					/* 3 = A-law */	

#define DRV_WM1808_LOOPBACK(X) ((X&0x01)<<0)			/* Digital loopback funtion, default 0 */
	#define DRV_WM1808_LOOPBACK_NO		0x00				/* 0 = No loopback */
	#define DRV_WM1808_LOOPBACK_ENABLED 0x01				/* 1 = Loopback enabled, ADC data output is fed directly into DAC data input */

/*======================================================================================================
 * Clock Gen ctrl R6(06h)
 */
#define DRV_WM1808_CLOCK_GEN_CTRL 0x06

#define DRV_WM1808_CLOCK_GEN_CTRL_DEFAULT 0x0140

#define DRV_WM1808_CLKSEL(X) ((X&0x01)<<8)			/* Controls the source of the clock for all internal operation, default 1 */
	#define DRV_WM1808_CLKSEL_MCLK 0x00					/* 0 = MCLK */ 
	#define DRV_WM1808_CLKSEL_PLL_OUTPUT 0x01			/* 1 = PLL output */

#define DRV_WM1808_MCLKDIV(X) ((X&0x07)<<5)			/* Sets the scailing for either the MCLK or PLL clock output(under control of CLKSEL), default 2 */
	#define DRV_WM1808_MCLKDIV_DIV_BY1	  0x00			/* 0 = devide by 1 */
	#define DRV_WM1808_MCLKDIV_DIV_BY1_5 0x01			/* 1 = devide by 1.5 */
	#define DRV_WM1808_MCLKDIV_DIV_BY2   0x02			/* 2 = devide by 2 */
	#define DRV_WM1808_MCLKDIV_DIV_BY3   0x03			/* 3 = devide by 3 */
	#define DRV_WM1808_MCLKDIV_DIV_BY4   0x04			/* 4 = devide by 4 */
	#define DRV_WM1808_MCLKDIV_DIV_BY6   0x05			/* 5 = devide by 6 */
	#define DRV_WM1808_MCLKDIV_DIV_BY8   0x06			/* 6 = devide by 8 */
	#define DRV_WM1808_MCLKDIV_DIV_BY12  0x07			/* 7 = devide by 12 */	

#define DRV_WM1808_BCLKDIV(X) ((X&0x07)<<2)			/* Configures the BCLK output frequency, for use when the chip is master over BCLK, default 0 */
	#define DRV_WM1808_BCLKDIV_DIV_BY1  0x00			/* 0 = divede by 1 (BLCK=MCLK) */
	#define DRV_WM1808_BCLKDIV_DIV_BY2  0x01			/* 1 = divede by 2 (BLCK=MCLK2) */
	#define DRV_WM1808_BCLKDIV_DIV_BY4  0x02			/* 2 = divede by 4 */
	#define DRV_WM1808_BCLKDIV_DIV_BY8  0x03			/* 3 = divede by 8 */
	#define DRV_WM1808_BCLKDIV_DIV_BY16 0x04			/* 4 = divede by 16 */
	#define DRV_WM1808_BCLKDIV_DIV_BY32 0x05			/* 5 = divede by 32 */

#define DRV_WM1808_MS(X) ((X&0x01)<<0)						/* Sets the chip to be master over LRC and BLCK, default 0 */
	#define DRV_WM1808_MS_SLAVE  0x00					/* 0 = BCLK and LRC clock are inputs */
	#define DRV_WM1808_MS_MASTER 0x01					/* 1 = BCLK and LRC clock are outputs generated by the WM8978(MASTER) */

/*======================================================================================================
 * Additional ctrl R7(07h)
 */
#define DRV_WM1808_ADDITIONAL_CTRL 0x07

#define DRV_WM1808_ADDITIONAL_CTRL_DEFAULT 0x0080

#define DRV_WM1808_M128ENB(X) ((X&0x01)<<8)			/* default 0 */
	#define DRV_WM1808_M128ENB_LO_POW_MOD_ENABLED  0x00	/* low power mode enabled */
	#define DRV_WM1808_M128ENB_LO_POW_MOD_DISABLED 0x01	/* low power mode disabled */

#define DRV_WM1808_SR(X) ((X&0x07)<<1)				/* Approximate sample rate(configures the coefficients for the internal digital filters), default 0*/
	#define DRV_WM1808_SR48KHZ 0x00					/* 0 = 48 Khz */
	#define DRV_WM1808_SR32KHZ 0x01					/* 0 = 32 Khz */
	#define DRV_WM1808_SR24KHZ 0x02					/* 0 = 24 Khz */	
	#define DRV_WM1808_SR16KHZ 0x03					/* 0 = 16 Khz */
	#define DRV_WM1808_SR12KHZ 0x04					/* 0 = 12 Khz */
	#define DRV_WM1808_SR8KHZ  0x05					/* 0 = 8 Khz */

#define DRV_WM1808_SLOWCLKEN(X) ((X&0x01)<<0)			/* Slow clock enable. Used for both the jack insert detect debounce circut and the zero cross timeout, default 0 */
	#define DRV_WM1808_SLOWCLKEN_DISABLED 0x00			/* slow clock disabled */
	#define DRV_WM1808_SLOWCLKEN_ENABLED  0x01			/* slow clock enabled */

/*======================================================================================================
 * GPIO Stuff R8(08h)
 */
#define DRV_WM1808_GPIO_STUFF 0x08

#define DRV_WM1808_GPIO_STUFF_DEFAULT 0x0000

#define DRV_WM1808_GPIO1GP(X) ((X&0x01)<<8)			/* GPIO1 Open drain enable, default 0 */
	#define DRV_WM1808_GPIO1GP_OPEN_DISABLED 0x00		/* 0 = Open drain disabled */
	#define DRV_WM1808_GPIO1GP_OPEN_ENABLED  0x01		/* 1 = Open drain enabled */

#define DRV_WM1808_GPIO1GPU(X) ((X&0x01)<<7)		/* GPIO1 Internal pull-up enable, default 0 */
	#define DRV_WM1808_GPIO1GPU_DISABLED 0x00			/* 0 = Internal pull-up disabled */
	#define DRV_WM1808_GPIO1GPU_ENABLED  0x01			/* 1 = internal pull-up enabled */

#define DRV_WM1808_GPIO1GPD(X) ((X&0x01)<<6)		/* GPIO1 Internal pull-down enable, default 0 */
	#define DRV_WM1808_GPIO1GPD_DISABLED 0x00			/* 0 = Internal pull-down disabled */
	#define DRV_WM1808_GPIO1GPD_ENABLED  0x01			/* 1 = Internal pull-down enabled */

#define DRV_WM1808_GPIO1POL(X) ((X&0x01)<<3)		/* GPIO1 Polarity invert */
	#define DRV_WM1808_GPIO1POL_NON_INVERTED 0x00		/* 0 = Non inverted */	
	#define DRV_WM1808_GPIO1POL_INVERTED	 0x01		/* 1 = Inverted */

#define DRV_WM1808_GPIO1SEL(X) ((X&0x07)<<0)		/* CSB/GPIO1 pin function select, default 0 */
	#define DRV_WM1808_GPIO1SEL_INPUT		 0x00		/* 0 = input (CSB/jack detection : depending on MODE setting) */
	#define DRV_WM1808_GPIO1SEL_TEMP_OK		 0x02		/* 2 = Temp ok */	
	#define DRV_WM1808_GPIO1SEL_AMUTE_ACTIVE 0x03		/* 3 = Amute active */
	#define DRV_WM1808_GPIO1SEL_PLL_CLK_OP   0x04		/* 4 = PLL clk o/p */
	#define DRV_WM1808_GPIO1SEL_PLL_LOCK     0x05		/* 5 = PLL lock */
	#define DRV_WM1808_GPIO1SEL_LOGIC_1		 0x06		/* 6 = logic 1 */
	#define DRV_WM1808_GPIO1SEL_LOGIC_0		 0x07		/* 7 = logic 0 */	

/*======================================================================================================
 * Jack detect control 1 R9(09h)
 */
#define DRV_WM1808_JACK_DETECT_CONTROL1 0x09

#define DRV_WM1808_JACK_DETECT_CONTROL1_DEFAULT 0x0000 

#define DRV_WM1808_JD_EN(X) ((X&0x01)<<6)			/* Jack Detection Enable, default 0 */	
	#define DRV_WM1808_JD_EN_DISABLED 0x00				/* 0 = disabled */
	#define DRV_WM1808_JD_EN_ENABLED  0x01				/* 1 = enabled */

#define DRV_WM1808_JD_SEL(X) ((X&0x03)<<4)			/* Pin selected as jack detection input, default 0 */
	#define DRV_WM1808_JD_SEL_GPIO1 0x00				/* 0 = GPIO1 */
	#define DRV_WM1808_JD_SEL_GPIO2 0x01				/* 1 = GPIO2 */
	#define DRV_WM1808_JD_SEL_GPIO3 0x02				/* 2 = GPIO3 */

/*======================================================================================================
 * DAC Control R10(0Ah)
 */
#define DRV_WM1808_DAC_CONTROL 0x0A

#define DRV_WM1808_DAC_CONTROL_DEFAULT 0x0000

#define DRV_WM1808_SOFTMUTE(X) ((X&0x01)<<6)		/* Softmute enable, default 0 */
	#define DRV_WM1808_SOFTMUTE_DISABLED 0x00			/* 0 = disabled */
	#define DRV_WM1808_SOFTMUTE_ENABLED  0x01			/* 1 = enabled */

#define DRV_WM1808_DACOSR128(X) ((X&0x01)<<3)		/* DAC oversample rate select, default 0 */
	#define DRV_WM1808_DACOSR128_64X  0x00				/* 0 = 64x (lowest power) */
	#define DRV_WM1808_DACOSR128_128X 0x01				/* 1 = 128x (best SNR) */	

#define DRV_WM1808_AMUTE(X) ((X&0x01)<<2)			/* Automute enable, default 0 */
	#define DRV_WM1808_AMUTE_DISABLED 0x00				/* 0 = Amute disabled */
	#define DRV_WM1808_AMUTE_ENABLED  0x01				/* 1 = Amute enabled */

#define DRV_WM1808_DACPOLR(X) ((X&0x01)<<1)			/* Right DAC output polarity, default 0 */
	#define DRV_WM1808_DACPOLR_NON_INVERTED 0x00		/* 0 = non-inverted */
	#define DRV_WM1808_DACPOLR_INVERTED     0x01		/* 1 = inverted (180 degrees phase shift) */

#define DRV_WM1808_DACPOLL(X) ((X&0x01)<<0)			/* Left DAC output polarity, default 0 */
	#define DRV_WM1808_DACPOLL_NON_INVERTED 0x00		/* 0 = non-inverted */
	#define DRV_WM1808_DACPOLL_INVERTED     0x01		/* 1 = inverted (180 degrees phase shift) */

/*======================================================================================================
 * Left DAC digital Vol R11(0Bh), Right DAC digital Vol R12(0Ch)
 */
#define DRV_WM1808_LEFT_DAC_DIGITAL_VOL 0x0B
#define DRV_WM1808_RIGHT_DAC_DIGITAL_VOL 0x0C

#define DRV_WM1808_LEFT_DAC_DIGITAL_VOL_DEFAULT 0x00FF
#define DRV_WM1808_RIGHT_DAC_DIGITAL_VOL_DEFAULT 0x00FF

#define DRV_WM1808_DACVU(X) ((X&0x01)<<8)		/* DAC left and DAC right volume do not update until a 1 is written to DACVU, default N/A*/
	#define DRV_WM1808_DACVU_UPDATE 0x01			

#define DRV_WM1808_DACVOLL(X) ((X&0xFF)<<0)		
#define DRV_WM1808_DACVOLR(X) ((X&0xFF)<<0)		
/* DAC Digital Volume Control, default 127 */
/*  0000 0000 = Digital Mute   */
/*  0000 0001 = -127dB         */
/*  0000 0010 = -126.5dB       */
/*  ... 0.5dB steps up to      */
/*  1111 1111 = 0dB            */
   
/*======================================================================================================
 * Jack detect control 2 R13(0Dh)
 */
#define DRV_WM1808_JACK_DETECT_CONTROL2 0x0D

#define DRV_WM1808_JACK_DETECT_CONTROL2_DEFAULT 0x0000

#define DRV_WM1808_JD_EN1(X) ((X&0x0F)<<4)
/*Output enabled when selected jack detection input is logic 1, default 0 */
/* [4] = OUT1_EN_1 */
/* [5] = OUT2_EN_1 */
/* [6] = OUT3_EN_1 */

#define DRV_WM1808_JD_EN0(X) ((X&0x0F)<<0)
/*Output enabled when selected jack detection input is logic 0, default 0 */
/* [0] = OUT1_EN_0 */
/* [1] = OUT2_EN_0 */
/* [2] = OUT3_EN_0 */

/*======================================================================================================
 * ADC Control R14(0Eh)
 */
#define DRV_WM1808_ADC_CONTROL 0x0E

#define DRV_WM1808_ADC_CONTROL_DEFAULT 0x0100

#define DRV_WM1808_HPFEN(X) ((X&0x01)<<8)					/*High Pass Filter Enable, default 1 */
	#define DRV_WM1808_HPFEN_DISABLED 0x00						/* 0 = disabled */
	#define DRV_WM1808_HPFEN_ENABLED  0x01						/* 1 = enabled */

#define DRV_WM1808_HPFAPP(X) ((X&0x01)<<7)					/* Select audio mode or application mode, default 0 */
	#define DRV_WM1808_HPFAPP_AUDIO_MODE 0x00					/* 0 = audio mode (1st order, fc = ~3.7Hz) */
	#define DRV_WM1808_HPFAPP_APP_MODE   0x01					/* 1 = Application mode (2nd order, fc = HPFCUT) */

#define DRV_WM1808_HPFCUT_OR_OPCLKDIV(X) ((X&0x07)<<4)
/* TODO */

#define DRV_WM1808_ADCOSR128(X) ((X&0x01)<<3)				/* ADC oversample rate select, default 0 */
	#define DRV_WM1808_ADCOSR128_64X  0x00						/* 0 = 64x (lower power) */
	#define DRV_WM1808_ADCOSR128_128X 0x01						/* 1 = 128x (best SNR) */

#define DRV_WM1808_ADCRPOL(X) ((X&0x01)<<1)					/* ADC right channel polarity adjust */
	#define DRV_WM1808_ADCRPOL_NORMAL   0x00					/* 0 = normal */
	#define DRV_WM1808_ADCRPOL_INVERTED 0x01					/* 1 = inverted */

#define DRV_WM1808_ADCLPOL(X) ((X&0x01)<<0)					/* ADC left channel polarity adjust */
	#define DRV_WM1808_ADCRPOL_NORMAL   0x00					/* 0 = normal */
	#define DRV_WM1808_ADCRPOL_INVERTED 0x01					/* 1 = inverted */

/*======================================================================================================
 * Left ADC Digital Vol R15(0Fh), Right ADC Digital Vol R16(10h)
 */
#define DRV_WM1808_LEFT_ADC_DIGITAL_VOL 0x0F
#define DRV_WM1808_RIGHT_ADC_DIGITAL_VOL 0x10

#define DRV_WM1808_LEFT_ADC_DIGITAL_VOL_DEFAULT 0x00FF
#define DRV_WM1808_RIGHT_ADC_DIGITAL_VOL_DEFAULT 0x00FF

#define DRV_WM1808_ADCVU(X) ((X&0x01)<<8)	/*ADC left and ADC right volume do not update until a 1 is written to ADCVU */
	#define	DRV_WM1808_ADCVU_UPDATE 0x01

#define DRV_WM1808_ADCVOLL(X) ((0xFF)<<0)
#define DRV_WM1808_ADCVOLR(X) ((0xFF)<<0)
/* left ADC Digital Volume Control, default 127 */
/*  0000 0000 = Digital Mute   */
/*  0000 0001 = -127dB         */
/*  0000 0010 = -126.5dB       */
/*  ... 0.5dB steps up to      */
/*  1111 1111 = 0dB            */

/*======================================================================================================
 * EQ1 - low shelf R18(12h)
 */
#define DRV_WM1808_EQ1_LOW_SHELF 0x12

#define DRV_WM1808_EQ1_LOW_SHELF_DEFAULT 0X012C

#define DRV_WM1808_EQ3DMODE(X) ((X&0x01)<<8)	/* default 1 */
	#define DRV_WM1808_EQ3DMODE_TO_ADC_PATH 0x00	/* 0 = Equaliser and 3D Enhancement applied to ADC path */
	#define DRV_WM1808_EQ3DMODE_TO_DAC_PATH 0x01	/* 1 = Equaliser and 3D Enhancement applied to DAC path */

#define DRV_WM1808_EQ1C(X) ((X&0x03)<<5)		/* Eq BAnd 1 Cut-off Frequency, default 1 */
	#define DRV_WM1808_EQ1C_80Hz  0x00				/* 0 = 80 Hz */
	#define DRV_WM1808_EQ1C_105Hz 0x01				/* 1 = 105 Hz */
	#define DRV_WM1808_EQ1C_135Hz 0x02				/* 2 = 135 Hz */
	#define DRV_WM1808_EQ1C_175Hz 0x03				/* 3 = 175 Hz */

#define DRV_WM1808_EQ1G(X) ((X&0x1F)<<0)		/* EQ Band 1 Gain Control, default 12 */	
/*TODO*/

/*======================================================================================================
 * EQ2 - peak 1 R19(13h)
 */
#define DRV_WM1808_EQ2_PEAK1 0x13

#define DRV_WM1808_EQ2_PEAK1_DEFAULT 0x002C

#define DRV_WM1808_EQ2BW(X) ((X&0x01)<<8)	/*EQ 2 Bandwidth control, default 0 */
	#define DRV_WM1808_EQ2BW_NARROW 0x00		/* 0 = narrow bandwidth */
	#define DRV_WM1808_EQ2BW_WIDE   0x01		/* 1 = wide bandwidth */	

#define DRV_WM1808_EQ2C(X) ((X&0x03)<<5)	/* EQ Band 2 Centre Frequency, default 1 */
	#define DRV_WM1808_EQ2C_230HZ 0x00			/* 0 = 230 Hz */
	#define DRV_WM1808_EQ2C_300HZ 0x01			/* 1 = 300 Hz */
	#define DRV_WM1808_EQ2C_385HZ 0x02			/* 2 = 385 Hz */
	#define DRV_WM1808_EQ2C_500HZ 0x03			/* 3 = 500 Hz */	

#define DRV_WM1808_EQ2G(X) ((X&0x1F)<<0)	/* EQ Band 2 Gain Control, default 12 */
/*TODO*/

/*======================================================================================================
 * EQ3 - peak 2 R20(14h)
 */
#define DRV_WM1808_EQ3_PEAK2 0x14

#define DRV_WM1808_EQ3_PEAK2_DEFAULT 0x002C

#define DRV_WM1808_EQ3BW(X) ((X&0x01)<<8)	/*EQ 3 Bandwidth control, default 0 */
#define DRV_WM1808_EQ3BW_NARROW 0x00		/* 0 = narrow bandwidth */
#define DRV_WM1808_EQ3BW_WIDE   0x01		/* 1 = wide bandwidth */	

#define DRV_WM1808_EQ3C(X) ((X&0x03)<<5)	/* EQ Band 3 Centre Frequency, default 1 */
#define DRV_WM1808_EQ3C_650HZ   0x00			/* 0 = 650 Hz */
#define DRV_WM1808_EQ3C_850HZ   0x01			/* 1 = 850 Hz */
#define DRV_WM1808_EQ3C_1_1KHZ 0x02			/* 2 = 1.1 KHz */
#define DRV_WM1808_EQ3C_1_4KHZ 0x03			/* 3 = 1.4 KHz */	

#define DRV_WM1808_EQ3G(X) ((X&0x1F)<<0)	/* EQ Band 3 Gain Control, default 12 */
/*TODO*/

/*======================================================================================================
 * EQ4 - peak 3 R21(15h)
 */
#define DRV_WM1808_EQ4_PEAK3 0x15

#define DRV_WM1808_EQ4_PEAK3_DEFAULT 0x002C

#define DRV_WM1808_EQ4BW(X) ((X&0x01)<<8)	/*EQ 4 Bandwidth control, default 0 */
#define DRV_WM1808_EQ4BW_NARROW 0x00		/* 0 = narrow bandwidth */
#define DRV_WM1808_EQ4BW_WIDE   0x01		/* 1 = wide bandwidth */	

#define DRV_WM1808_EQ4C(X) ((X&0x03)<<5)	/* EQ Band 4 Centre Frequency, default 1 */
#define DRV_WM1808_EQ4C_1_8KHZ   0x00			/* 0 = 1.8 KHz */
#define DRV_WM1808_EQ4C_2_4KHZ   0x01			/* 1 = 2.4 KHz */
#define DRV_WM1808_EQ4C_3_2KHZ	  0x02			/* 2 = 3.2 KHz */
#define DRV_WM1808_EQ4C_4_1KHZ   0x03			/* 3 = 4.1 KHz */	

#define DRV_WM1808_EQ4G(X) ((X&0x1F)<<0)	/* EQ Band 4 Gain Control, default 12 */
/*TODO*/

/*======================================================================================================
 * EQ5 - high shelf R22(16h)
 */
#define DRV_WM1808_EQ5_HIGH_SHELF 0x16

#define DRV_WM1808_EQ5_HIGH_SHELF_DEFAULT 0x002C

#define DRV_WM1808_EQ5C(X) ((X&0x03)<<5)	/* EQ Band 5 Cut-off Frequency, default 1 */
#define DRV_WM1808_EQ5C_5_3KHZ   0x00			/* 0 = 5.3 KHz */
#define DRV_WM1808_EQ5C_6_9KHZ   0x01			/* 1 = 6.9 KHz */
#define DRV_WM1808_EQ5C_9KHZ	  0x02			/* 2 = 9 KHz */
#define DRV_WM1808_EQ5C_11_7KHZ   0x03			/* 3 = 11.7 KHz */	

#define DRV_WM1808_EQ5G(X) ((X&0x1F)<<0)	/* EQ Band 5 Gain Control, default 12 */
/*TODO*/

/*======================================================================================================
 * DAC Limiter 1 R24(18h)
 */
#define DRV_WM1808_DAC_LIMITER1 0x18

#define DRV_WM1808_DAC_LIMITER1_DEFAULT 0x0032

#define DRV_WM1808_LIMEN(X) ((X&0x01)<<8)		/* Enable the DAC digital limiter, default 0 */
	#define DRV_WM1808_LIMEN_DISABLED 0x00			/* 0 = disabled */
	#define DRV_WM1808_LIMEN_ENABLED  0x01			/* 1 = enabled */

#define DRV_WM1808_LIMDCY(X) ((X&0x0F)<<4)		/* DAC Limiter Decay time(per 6DB gain change) for 44.1 khz sampling. Note that these will scale with sample rate, default 2 */
	#define DRV_WM1808_LIMDCY_750US 0x00			/* 0 = 750 us */
	#define DRV_WM1808_LIMDCY_1_5MS 0x01			/* 1 = 1.5 ms */
	#define DRV_WM1808_LIMDCY_3MS   0x02			/* 2 = 3 ms */	
	#define DRV_WM1808_LIMDCY_6MS   0x03			/* 3 = 6 ms */
	#define DRV_WM1808_LIMDCY_12MS  0x04			/* 4 = 12 ms */
	#define DRV_WM1808_LIMDCY_24MS  0x05			/* 5 = 24 ms */
	#define DRV_WM1808_LIMDCY_48MS  0x06			/* 6 = 48 ms */
	#define DRV_WM1808_LIMDCY_96MS  0x07			/* 7 = 96 ms */	
	#define DRV_WM1808_LIMDCY_192MS 0x08			/* 8 = 192 ms */
	#define DRV_WM1808_LIMDCY_384MS 0x09			/* 9 = 384 ms */
	#define DRV_WM1808_LIMDCY_768MS 0x0A			/* 10 = 768 ms */

#define DRV_WM1808_LIMATK(X) ((X&0x0F)<<0)		/* DAC Limiter Attack time (per 6dB gain change) for 44.1 kHz sampling. Note that these will scale with sample rate */
/*TODO*/


/*======================================================================================================
 * DAC Limiter 2 R25(19h)
 */
#define DRV_WM1808_DAC_LIMITER2 0x19	
	
#define DRV_WM1808_DAC_LIMITER2_DEFAULT 0x0000

#define DRV_WM1808_LIMLVL(X) ((X&0x07)<<4)		/* Programmable signal threshold level(determines level at which the DAC limiter starts to operate, default 0 */
	#define DRV_WM1808_LIMLVL_MINUS_1DB 0x00		/* 0 = -1 dB */
	#define DRV_WM1808_LIMLVL_MINUS_2DB 0x01		/* 1 = -2 dB */
	#define DRV_WM1808_LIMLVL_MINUS_3DB 0x02		/* 2 = -3 dB */	
	#define DRV_WM1808_LIMLVL_MINUS_4DB 0x03		/* 3 = -4 dB */
	#define DRV_WM1808_LIMLVL_MINUS_5DB 0x04		/* 4 = -5 dB */	
	#define DRV_WM1808_LIMLVL_MINUS_6DB 0x05		/* 5 = -6 dB */
//	#define DRV_WM1808_LIMLVL_MINUS_6DB 0x06		/* 6 = -5 dB */	
//	#define DRV_WM1808_LIMLVL_MINUS_6DB 0x07		/* 7 = -5 dB */

#define DRV_WM1808_LIMBOOST(X) ((X&0x0F)<<0)	/* DAC Limiter volume boost (can be used as a stand alone volume boost when LIMEN = 0), default 0 */
	#define DRV_WM1808_LIMBOOST_0DB  0x00			/* 0 = 0 dB */
	#define DRV_WM1808_LIMBOOST_1DB  0x01			/* 1 = 1 dB */
	#define DRV_WM1808_LIMBOOST_2DB  0x02			/* 2 = 2 dB */
	#define DRV_WM1808_LIMBOOST_3DB  0x03			/* 3 = 3 dB */
	#define DRV_WM1808_LIMBOOST_4DB  0x04			/* 4 = 4 dB */
	#define DRV_WM1808_LIMBOOST_5DB  0x05			/* 5 = 5 dB */
	#define DRV_WM1808_LIMBOOST_6DB  0x06			/* 6 = 6 dB */
	#define DRV_WM1808_LIMBOOST_7DB  0x07			/* 7 = 7 dB */
	#define DRV_WM1808_LIMBOOST_8DB  0x08			/* 8 = 8 dB */
	#define DRV_WM1808_LIMBOOST_9DB  0x09			/* 9 = 9 dB */
	#define DRV_WM1808_LIMBOOST_10DB 0x0A			/* 10 = 10 dB */
	#define DRV_WM1808_LIMBOOST_11DB 0x0B			/* 11 = 11 dB */	
	#define DRV_WM1808_LIMBOOST_12DB 0x0C			/* 12 = 12 dB */

/*======================================================================================================
 * Notch Filter 1 R27(1Bh)
 */
#define DRV_WM1808_NOTCH_FILTER1 0x1B

#define DRV_WM1808_NOTCH_FILTER1_DEFAULT 0x0000

#define DRV_WM1808_NFU1(X) ((X&0x01)<<8)				/* Notch filter update. The notch filter values used internally only update when one of the NFU bits is set high, default 0 */
	#define DRV_WM1808_NFU1_UPDATE 0x01					

#define DRV_WM1808_NFEN(X) ((X&0x01)<<7)			/* Notch filter enable, default 0 */
	#define DRV_WM1808_NFEN_DISABLED 0x00				/* 0 = Disabled */
	#define DRV_WM1808_NFEN_ENABLED  0x01				/* 1 = Enabled */

#define DRV_WM1808_NFA0_13_TO_7(X) ((X&0x3F)<<0)	/* Notch Filter a0 coefficient, bits [13:7], default 0 */	

/*======================================================================================================
 * Notch Filter 2 R28(1Ch)
 */
#define DRV_WM1808_NOTCH_FILTER2 0x1C

#define DRV_WM1808_NOTCH_FILTER2_DEFAULT 0x0000

#define DRV_WM1808_NFU2(X) ((X&0x01)<<8)				/* Notch filter update. The notch filter values used internally only update when one of the NFU bits is set high, default 0 */
	#define DRV_WM1808_NFU2_UPDATE 0x01					

#define DRV_WM1808_NFA0_6_TO_0(X) ((X&0x3F)<<0)	/* Notch Filter a0 coefficient, bits [13:7], default 0 */	

/*======================================================================================================
 * Notch Filter 3 R29(1Dh)
 */
#define DRV_WM1808_NOTCH_FILTER3 0x1D

#define DRV_WM1808_NOTCH_FILTER3_DEFAULT 0x0000

#define DRV_WM1808_NFU3(X) ((X&0x01)<<8)				/* Notch filter update. The notch filter values used internally only update when one of the NFU bits is set high, default 0 */
	#define DRV_WM1808_NFU3_UPDATE 0x01					

#define DRV_WM1808_NFA1_13_TO_7(X) ((X&0x3F)<<0)	/* Notch Filter a1 coefficient, bits [13:7], default 0 */	

/*======================================================================================================
 * Notch Filter 4 R30(1Eh)
 */
#define DRV_WM1808_NOTCH_FILTER4 0x1E

#define DRV_WM1808_NOTCH_FILTER4_DEFAULT 0x0000

#define DRV_WM1808_NFU4(X) ((X&0x01)<<8)				/* Notch filter update. The notch filter values used internally only update when one of the NFU bits is set high, default 0 */
	#define DRV_WM1808_NFU4_UPDATE 0x01					

#define DRV_WM1808_NFA0_6_TO_0(X) ((X&0x3F)<<0)	/* Notch Filter a1 coefficient, bits [13:7], default 0 */	

/*======================================================================================================
 * ALC Control 1 R32(20h)
 */
#define DRV_WM1808_ALC_CONTROL1 0x20

#define DRV_WM1808_ALC_CONTROL1_DEFAULT 0x0038

#define DRV_WM1808_ALCSEL(X) ((X&0x01)<<8)			/* ALC function select, default 0 */
	#define DRV_WM1808_ALCSEL_ALC_OFF		0x00		/* 0 = ALC off */
	#define DRV_WM1808_ALCSEL_ALC_LEFT_ONLY 0x01		/* 1 = ALC left only */

#define DRV_WM1808_ALCMAXGAIN(X) ((X&0x07)<<3)		/* Set Maximum Gain of PGA, default 7 */
	#define DRV_WM1808_ALCMAXGAIN_MINUS_6_75DB 0x00		/* 0 = -6.75 dB */
	#define DRV_WM1808_ALCMAXGAIN_MINUS_0_75DB 0x01		/* 1 = -0.75 dB */
	#define DRV_WM1808_ALCMAXGAIN_5_25DB	   0x02		/* 2 = 5.25 dB */
	#define DRV_WM1808_ALCMAXGAIN_11_25DB	   0x03		/* 3 = 11.25 dB */	
	#define DRV_WM1808_ALCMAXGAIN_17_25DB	   0x04		/* 4 = 17.25 dB */
	#define DRV_WM1808_ALCMAXGAIN_23_25DB	   0x05		/* 5 = 23.25 dB */
	#define DRV_WM1808_ALCMAXGAIN_29_25DB	   0x06		/* 6 = 29.25 dB */
	#define DRV_WM1808_ALCMAXGAIN_35_25DB	   0x07		/* 7 = 35.25 dB */

#define DRV_WM1808_ALCMINGAIN(X) ((X&0x07)<<0)		/* Set minimum gain of PGA, default 0 */
	#define DRV_WM1808_ALCMINGAIN_MINUS_12DB 0x00		/* 0 = -12 dB */
	#define DRV_WM1808_ALCMINGAIN_MINUS_6DB	 0x01		/* 1 = -6 dB */
	#define DRV_WM1808_ALCMINGAIN_0DB		 0x02		/* 2 = 0 dB */
	#define DRV_WM1808_ALCMINGAIN_6DB		 0x03		/* 3 = 6 dB */
	#define DRV_WM1808_ALCMINGAIN_12DB		 0x04		/* 4 = 12 dB */
	#define DRV_WM1808_ALCMINGAIN_18DB		 0x05		/* 5 = 18 dB */
	#define DRV_WM1808_ALCMINGAIN_24DB		 0x06		/* 6 = 24 dB */
	#define DRV_WM1808_ALCMINGAIN_30DB		 0x07		/* 7 = 30 dB */	

/*======================================================================================================
 * ALC Control 2 R33(21h)
 */
#define DRV_WM1808_ALC_CONTROL2 0x21

#define DRV_WM1808_ALC_CONTROL2_DEFAULT 0x000B

#define DRV_WM1808_ALCHLD(X) ((X&0x0F)<<4)	/* ALC hold time before gain is increased, default 0 */
/* 0 = 0ms */
/* 1 = 2.67ms */
/* 2 = 5.33ms */
/* ...(time doubles with every step) */
/* 15 = 43.691s */

#define DRV_WM1808_ALCLVL(X) ((X&0x0F)<<0) /* ALC target - sets signal level at ADC input, default 11 */
/* 0 = -22.5 dBFS */
/* 1 = -21 dBFS */
/* ......(-1.5dB steps) */
/* 12 = -4.5 dBFS */
/* 13 = -3 dBFS */
/* 14 = -1.5 dBFS */
/* 15 = -1.5 dBFS */

/*======================================================================================================
 * ALC Control 3 R34(22h)
 */
#define DRV_WM1808_ALC_CONTROL3 0x22

#define DRV_WM1808_ALC_CONTROL3_DEFAULT 0x0032

#define DRV_WM1808_ALCMODE(X) ((X&0x01)<<8)	/* Determines theALC mode of operation */
	#define DRV_WM1808_ALCMODE_ALC     0x00		/* 0 = ALC mode */	
	#define DRV_WM1808_ALCMODE_LIMITER 0x01		/* 1 = Limiter mode */

#define DRV_WM1808_ALCDCY(X) ((X&0x0F)<<4)	/* Decay (gain ramp-up) time */
/*TODO*/

#define DRV_WM1808_ALCATK(X) ((X&0x0F)<<0)	/* ALC attack (gain ramp-up) time */	
/*TODO*/


/*======================================================================================================
 * Noise Gate R35(23h)
 */
#define DRV_WM1808_NOISE_GATE 0x23

#define DRV_WM1808_NOISE_GATE_DEFAULT 0x0000

#define DRV_WM1808_NGEN(X) ((X&0x01)<<3)	/* ALC Noise gate function enable, default 0 */
	#define DRV_WM1808_NGEN_DISABLE 0x01		/* 0 = disable */
	#define DRV_WM1808_NGEN_ENABLE  0x00		/* 1 = enable */

#define DRV_WM1808_NGTH(X) ((X&0x07)<<0)	/* ALC Noise gate threshold (6dB steps), default 0 */
	#define DRV_WM1808_NGTH_MINUS_39DB 0x00	/* 0 = -39 dB */
	#define DRV_WM1808_NGTH_MINUS_45DB 0x01 /* 1 = -45 dB */
	#define DRV_WM1808_NGTH_MINUS_51DB 0x02 /* 2 = -51 dB */
	#define DRV_WM1808_NGTH_MINUS_57DB 0x03 /* 3 = -57 dB */
	#define DRV_WM1808_NGTH_MINUS_63DB 0x04 /* 4 = -63 dB */
	#define DRV_WM1808_NGTH_MINUS_69DB 0x05 /* 5 = -69 dB */
	#define DRV_WM1808_NGTH_MINUS_75DB 0x06 /* 6 = -75 dB */
	#define DRV_WM1808_NGTH_MINUS_81DB 0x07 /* 7 = -81 dB */


/*======================================================================================================
 * PLL N R36(24h)
 */
#define DRV_WM1808_PLL_N 0x24

#define DRV_WM1808_PLL_N_DEFAULT 0x0008

#define DRV_WM1808_PLLPRESCALE(X) ((X&0x01)<<4)		/* default 0 */	
	#define DRV_WM1808_PLLPRESCALE_MCLK_NOT_DIV 0x00	/* 0 = MCLK input not divided (default) */
	#define DRV_WM1808_PLLPRESCALE_MCLK_DIV 0x01		/* 1 = Divide MCLK by2 before input to PLL */

#define DRV_WM1808_PLLN_3_TO_0(X) ((X&0x0F)<<0)		/* Integer(N) part of PLL input/output frequency ratio. Use values greater than 5 and less than 13, default 8 */

/*======================================================================================================
 * PLL K1 R37(25h)
 */
#define DRV_WM1808_PLL_K1 0x25

#define DRV_WM1808_PLL_K1_DEFAULT 0x0006

#define DRV_WM1808_PLLK_23_TO_18(X) ((X&0x3F)<<0)	/* Fractional(K) part of PLL1 input/output frequency ratio (treat as one 24-digit binary number), default 12 */


/*======================================================================================================
 * PLL K2 R38(26h)
 */
#define DRV_WM1808_PLL_K2 0x26

#define DRV_WM1808_PLL_K2_DEFAULT 0x0093

#define DRV_WM1808_PLLK_17_TO_9(X) ((X&0x1FF)<<0) /* Fractional(K) part of PLL1 input/output frequency ratio (treat as one 24-digit binary number), default 147  */
/*======================================================================================================
 * PLL K3 R39(27h)
 */
#define DRV_WM1808_PLL_K3 0x27

#define DRV_WM1808_PLL_K3_DEFAULT 0x00E9

#define DRV_WM1808_PLLK_8_TO_0(X) ((X&0x1FF)<<0) /* Fractional(K) part of PLL1 input/output frequency ratio (treat as one 24-digit binary number), default 233 */

/*======================================================================================================
 * 3D control R41(29h)
 */
#define DRV_WM1808_3D_CONTROL 0x29

#define DRV_WM1808_3D_CONTROL_DEFAULT 0x0000

#define DRV_WM1808_DEPTH3D(X) ((X&0x0F)<<0) /* stereo depth, default 0 */
/* 0 : 0% (minimum 3D effect) */
/* 1 : 6.67% */
/* .... */
/* 14 : 93.3% */
/* 15 : 100% (maximum 3D effect) */

/*======================================================================================================
* VMID R42(2Ah)
*/
#define DRV_WM1808_VMID 0x2A

#define DRV_WM1808_VMID_DEFAULT 0x0000

#define DRV_WM1808_VMIDEN(X) ((X&0x01)<<2)		/* VMID indpendent current bias control */
	#define DRV_WM1808_VMIDEN_DISABLED 0x00			/* 0 = disable VMID independent current bias */
	#define DRV_WM1808_VMIDEN_ENABLED  0x01			/* 1 = enable VMID independent current bias */

/*======================================================================================================
 * Beep control R43(2Bh)
 */
#define DRV_WM1808_BEEP_CONTROL 0x2B

#define DRV_WM1808_BEEP_CONTROL_DEFAULT 0x0000

#define DRV_WM1808_BYPL2RMIX(X) ((X&0x01)<<8)		/* Left bypass path(from the left channel input PGA stage) to right output mixer, default 0 */
	#define DRV_WM1808_BYPL2RMIX_NOT_SELECTED 0x00		/* 0 = not selected */
	#define DRV_WM1808_BYPL2RMIX_SELECTED	  0x01		/* 1 = selected */

/*======================================================================================================
 * INP control R44(2Ch)
 */
#define DRV_WM1808_INP_CONTROL 0x2C

#define DRV_WM1808_INP_CONTROL_DEFAULT 0x0003

#define DRV_WM1808_L2_2INPPGA(X) ((X&0x01)<<2)		/* Connect L2 pin to left channel input PGA positive terminal, default 0 */
	#define DRV_WM1808_L2_2INPPGA_NOT_CONNECTED 0x00	/* 0 = L2 not connected to input PGA */	
	#define DRV_WM1808_L2_2INPPGA_CONNECTED		0x01	/* 1 = L2 connected to input PGA amplifier positive terminal(constant input impedance) */

#define DRV_WM1808_LIN2INPPGA(X) ((X&0x01)<<1)		/* Connect LIN pin to left channel input PGA negative terminal, default 1 */
	#define DRV_WM1808_LIN2INPPGA_NOT_CONNECTED 0x00	/* 0 = LIN not connected to input PGA */
	#define DRV_WM1808_LIN2INPPGA_CONNECTED		0x01	/* 1 = LIN connected to input PGA amplifier negative terminal */

#define DRV_WM1808_LIP2INPPGA(X) ((X&0x01)<<0)		/* Connect LIP pin to left channel input PGA amplifier positive terminal, default 1 */
	#define DRV_WM1808_LIP2INPPGA_NOT_CONNECTED 0x00	/* 0 = LIP not connected to input PGA */
	#define DRV_WM1808_LIP2INPPGA_CONNECTED		0x01	/* 1 = input PGA amplifier positive terminal connected to LIP(constant input impedance) */

/*======================================================================================================
 * Left INP PGA gain ctrl R45(2Dh)
 */
#define DRV_WM1808_LEFT_INP_PGA_GAIN_CTRL 0x2D

#define DRV_WM1808_LEFT_INP_PGA_GAIN_CTRL_DEFAULT 0x0010

#define DRV_WM1808_INPPGAU(X) ((X&0x01)<<8)		/* INPPGAVOLL volume do not update until a 1 is written to INPPGAUPDATE, default N/A */
#define DRV_WM1808_INPPGAU_UPDATE 0x01			

#define DRV_WM1808_INPPGAZCL(X) ((X&0x01)<<7)	/* Left channel input PGA zero cross enable, default 0 */
#define DRV_WM1808_INPPGAZCL_DISABLE 0x00		/* 0 = update gain when gain register changes */
#define DRV_WM1808_INPPGAZCL_ENABLE  0x01		/* 1 = update gain on 1st zero cross after gain register write */

#define DRV_WM1808_INPPGAMUTEL(X) ((X&0x01)<<6) /* mute control for left channel input PGA, default 0 */
#define DRV_WM1808_INPPGAMUTEL_NOT_MUTED 0x00	/* 0 = input PGA not muted, normal operation */
#define DRV_WM1808_INPPGAMUTEL_MUTED     0x01	/* 1 = input PGA muted (and disconnected from the following input BOOST stage */

#define DRV_WM1808_INPPGAVOLL(X) ((X&0x3F)<<0)  /* Left channel input PGA volume, default 16 */
/* 0 = -12 dB */
/* 1 = -11.25 dB */
/* ...... */
/* 16 = 0 dB */
/* ...... */
/* 64 = 35.25 dB */

/*======================================================================================================
 * Left ADC Boost ctrl R47(2Fh)
 */
#define DRV_WM1808_LEFT_ADC_BOOST_CTRL 0x2F

#define DRV_WM1808_LEFT_ADC_BOOST_CTRL_DEFAULT 0x0100

#define DRV_WM1808_PGABOOSTL(X) ((X&0x01)<<8)		/* Boost enable for left channel input PGA, default 1 */
#define DRV_WM1808_PGABOOSTL_0DB_GAIN  0x00			/* 0 = PGA output has +0dB gain through input BOOST stage */
#define DRV_WM1808_PGABOOSTL_20DB_GAIN 0x01			/* 1 = PGA output has +20dB gain through input BOOST stage */

#define DRV_WM1808_L2_2BOOSTVOL(X) ((X&0x07)<<4)	/* Controls the L2 pin to the left channel input boost stage, default 0 */
/* 0 = Path disabled (disconnected) */
/* 1 = -12dB gain through boost stage */
/* 2 = -9dB gain through boost stage */
/* ...... */
/* 7 = 6dB gain through boost stage */

#define DRV_WM1808_AUXL2BOOSTVOL(X) ((X&0x07)<<0)	/* Controls the auxilliary to the left channel input boost stage */
/* 0 = Path disabled (disconnected) */
/* 1 = -12dB gain through boost stage */
/* 2 = -9dB gain through boost stage */
/* ...... */
/* 7 = 6dB gain through boost stage */

/*======================================================================================================
 * Right ADC Boost ctrl R48(30h)
 */
#define DRV_WM1808_RIGHT_ADC_BOOST_CTRL 0x30

#define DRV_WM1808_RIGHT_ADC_BOOST_CTRL_DEFAULT 0x0000

#define DRV_WM1808_R2_2BOOSTVOL(X) ((X&0x07)<<4)		/* Controls the R2 pin to the right channel input boost stage, default 0 */
/* 0 = Path disabled(disconnected) */
/* 1 = -12dB gain through boost stage */
/* 2 = -9dB gain through boost stage */
/* ...... */
/* 7 = 6dB gain through boost stage */

#define DRV_WM1808_AUXR2BOOSTVOL(X) ((X&0x07)<<0)		/* Controls the auxilliary amplifier to the right channel input boost stage, default 0 */
/* 0 = Path disabled (disconnected) */
/* 1 = -12dB gain through boost stage */
/* 2 = -9dB gain through boost stage */
/* ...... */
/* 7 = 6dB gain through boost stage */

/*======================================================================================================
 * Output ctrl R49(31h)
 */
#define DRV_WM1808_OUTPUT_CTRL 0x31

#define DRV_WM1808_OUTPUT_CTRL_DEFAULT 0x0002

#define DRV_WM1808_DACL2RMIX(X)	((X&0x01)<<6)	/* Left DAC output to right output mixer, default 0 */
	#define DRV_WM1808_DACL2RMIX_NOT_SELECTED 0x00	/* 0 = not selected */
	#define DRV_WM1808_DACR2LMIX_SELECTED     0x01	/* 1 = selected */

#define DRV_WM1808_DACR2LMIX(X) ((X&0x01)<<5)   /* Right DAC output to left output mixer, default 0 */
	#define DRV_WM1808_DACR2LMIX_NOT_SELECTED 0x00	/* 0 = not selected */
	#define DRV_WM1808_DACR2LMIX_SELECTED	  0x01	/* 1 = selected */

#define DRV_WM1808_TSOPCTRL(X) ((X&0x01)<<2)	/* Thermal Shutdown Output enable, default 0 */
	#define DRV_WM1808_TSOPCTRL_DISABLED 0x00		/* 0 = disabled */
	#define DRV_WM1808_TSOPCTRL_ENABLED	 0x01		/* 1 = enabled, i.e. all outputs will be disabled if TI set and the device junction temperature is more than 125 degree */

#define DRV_WM1808_TSDEN(X) ((X&0x01)<<1)		/* Thermal Shutdown Enable, default 1 */
	#define DRV_WM1808_TSDEN_DISABLED 0x00			/* 0 = thermal shutdown disabled */
	#define DRV_WM1808_TSDEN_ENABLED  0x01			/* 1 = thermal shutdown enabled */

#define DRV_WM1808_VROL(X) ((X&0x01)<<0)		/* VREF(AVDD/2 or 1.5*AVDD/2) to analogue output resistance, default 0 */
	#define DRV_WM1808_VROL_1K_OHM	0x00			/* 0 = approx 1Kohm */
	#define DRV_WM1808_VROL_30K_OHM 0x01			/* 1 = approx 30Kohm */	

/*======================================================================================================
 * Left mixer ctrl R50(32h)
 */
#define DRV_WM1808_LEFT_MIXER_CTRL 0x32

#define DRV_WM1808_LEFT_MIXER_CTRL_DEFAULT 0x0001

#define DRV_WM1808_AUXLMIXVOL(X) ((X&0x07)<<6)		/* Aux left channel input to left mixer volume, default 0 */
/* 0 = -15dB */
/* 1 = -12dB */
/* ....... */
/* 5 = 0dB */
/* 6 = +3dB */
/* 7 = +6dB */

#define DRV_WM1808_AUXL2LMIX(X) ((X&0x01)<<5)		/* Left Auxilliary input to left channel output mixer, default 0 */
	#define DRV_WM1808_AUXL2LMIX_NOT_SELECTED 0x00		/* 0 = not selected */
	#define DRV_WM1808_AUXL2LMIX_SELECTED     0x01		/* 1 = selected */

#define DRV_WM1808_BYPLMIXVOL(X) ((X&0x07)<<2)		/* Left bypass volume control to output channel, default 0 */		
/* 0 = -15dB */
/* 1 = -12dB */
/* ...... */
/* 5 = 0dB */
/* 6 = +3dB */
/* 7 = +6dB */

#define DRV_WM1808_BYPL2LMIX(X) ((X&0x01)<<1)		/* Left bypass path(from the left channel input boost output) to left output mixer, default 0 */	
	#define DRV_WM1808_BYPL2LMIX_NOT_SELECTED 0x00		/* 0 = not selected */
	#define DRV_WM1808_BYPL2LMIX_SELECTED	  0x01		/* 1 = selected */

#define DRV_WM1808_DACL2LMIX(X) ((X&0x01)<<0)		/* Left DAC output to left output mixer, default 0 */
	#define DRV_WM1808_DACL2LMIX_NOT_SELECTED 0x00		/* 0 = not selected */
	#define DRV_WM1808_DACL2LMIX_SELECTED	  0x01		/* 1 = selected */

/*======================================================================================================
 * Right mixer ctrl R51(33h)
 */
#define DRV_WM1808_RIGHT_MIXER_CTRL 0x33

#define DRV_WM1808_RIGHT_MIXER_CTRL_DEFAULT 0x0001

#define DRV_WM1808_AUXRMIXVOL(X) ((X&0x07)<<6)		/* Aux right channel input to right mixer volume control, default 0 */
/* 0 = -15dB */
/* 1 = -12dB */
/* ...... */
/* 5 = 0dB */
/* 6 = +3dB */
/* 7 = +6dB */

#define DRV_WM1808_AUXR2RMIX(X) ((X&0x01)<<5)		/* Right Auxilliary input to right channel output mixer, default 0 */
	#define DRV_WM1808_AUXR2RMIX_NOT_SELECTED 0x00		/* 0 = not selected */
	#define DRV_WM1808_AUXR2RMIX_SELECTED	  0x01		/* 1 = selected */

#define DRV_WM1808_DACR2RMIX(X) ((X&0x01)<<0)		/* Right DAC output to right output mixer, default 1 */
	#define DRV_WM1808_DACR2RMIX_NOT_SELECTED 0x00		/* 0 = not selected */
	#define DRV_WM1808_DACR2RMIX_SELECTED	  0x01		/* 1 = selected */


/*======================================================================================================
 * LOUT1(HP) volume ctrl R52(34h)
 */
#define DRV_WM1808_LOUT1_HP_VOLUME_CTRL 0x34		

#define DRV_WM1808_LOUT1_HP_VOLUME_CTRL_DEFAULT 0x0039		

#define DRV_WM1808_OUT1VU_L(X) ((X&0x01)<<8)		/* LOUT1 and ROUT1 volumes do not update until a 1 is written to OUT1VU, default N/A */
	#define DRV_WM1808_OUT1VU_L_UPDATE 0x01

#define DRV_WM1808_LOUT1ZC(X) ((X&0x01)<<7)			/* Headphone volume zero cross enable, default 0 */
	#define DRV_WM1808_LOUT1ZC_IMMEDIATELY 0x00			/* 0 = Change gain immediately */	
	#define DRV_WM1808_LOUT1ZC_ZC_ONLY	   0x01			/* 1 = Change gain on zero cross only */

#define DRV_WM1808_LOUT1MUTE(X) ((X&0x01)<<6)		/* Left headphone output mute, default 0 */
	#define DRV_WM1808_LOUT1MUTE_NORMAL 0x00			/* 0 = Normal operation */
	#define DRV_WM1808_LOUT1MUTE_MUTE	0x01			/* 1 = Mute */

#define DRV_WM1808_LOUT1VOL(X) ((X&0x3F)<<0)		/* Left headphone output volume, default 57 */
/* 0 = -57dB */
/* ...... */
/* 57 = 0dB */
/* ...... */
/* 63 = +6dB */

/*======================================================================================================
 * ROUT1(HP) volume ctrl R53(35h)
 */
#define DRV_WM1808_ROUT1_HP_VOLUME_CTRL 0x35

#define DRV_WM1808_ROUT1_HP_VOLUME_CTRL_DEFAULT 0x0039

#define DRV_WM1808_OUT1VU_R(X) ((X&0x01)<<8)		/* LOUT1 and ROUT1 volumes do not update until a 1 is written to OUT1VU, default N/A */
	#define DRV_WM1808_OUT1VU_R_UPDATE 0x01

#define DRV_WM1808_ROUT1ZC(X) ((X*0x01)<<7)			/* Headphone volume zero cross enable, default 0 */
	#define DRV_WM1808_ROUT1ZC_IMMEDIATELY 0x00			/* 0 = Change gain immediately */
	#define DRV_WM1808_ROUT1ZC_ZC_ONLY	   0x01			/* 1 = Change gain on zero cross only */

#define DRV_WM1808_ROUT1MUTE(X) ((X&0x01)<<6)		/* Right headphone output mute, default 0 */
	#define DRV_WM1808_ROUT1MUTE_NORMAL 0x00			/* 0 = Normal operation */
	#define DRV_WM1808_ROUT1MUTE_MUTE   0x01			/* 1 = Mute */

#define DRV_WM1808_ROUT1VOL(X) ((X&0x3F)<<0)		/* Right headphone output volume, default 57 */
/* 0 = -57dB */
/* ...... */
/* 57 = 0dB */
/* ...... */
/* 63 = +6dB */

/*======================================================================================================
 * LOUT2(SPK) volume ctrl R54(36h)
 */
#define DRV_WM1808_LOUT2_SPK_VOLUME_CTRL 0x36

#define DRV_WM1808_LOUT2_SPK_VOLUME_CTRL_DEFAULT 0x0039

#define DRV_WM1808_OUT2VU_L(X) ((X&0x01)<<8)		/* LOUT2 and ROUT2 volumes do not update until a 1 is written to OUT2VU, default N/A */
#define DRV_WM1808_OUT2VU_L_UPDATE 0x01				

#define DRV_WM1808_LOUT2ZC(X) ((X&0x01)<<7)			/* Speaker volume zero cross enable, default 0 */
#define DRV_WM1808_LOUT2ZC_IMMEDIATELY 0x00			/* 0 = Change gain immediately */
#define DRV_WM1808_LOUT2ZC_ZC_ONLY     0x01			/* 1 = Change gain on zero cross only */	

#define DRV_WM1808_LOUT2MUTE(X) ((X&0x01)<<6)		/* Left speaker output mute, default 0 */	
#define DRV_WM1808_LOUT2MUTE_NORMAL 0x00			/* 0 = Normal operation */
#define DRV_WM1808_LOUT2MUTE_MUTE   0x01			/* 1 = Mute */

#define DRV_WM1808_LOUT2VOL(X) ((X&0x3F)<<0)		/* Left speaker output volume, default 57 */
/* 0 = -57dB */
/* ...... */
/* 57 = 0dB */
/* ...... */
/* 63 = +6dB */

/*======================================================================================================
 * ROUT2(SPK) volume ctrl R55(37h)
 */
#define DRV_WM1808_ROUT2_SPK_VOLUME_CTRL 0x37

#define DRV_WM1808_ROUT2_SPK_VOLUME_CTRL_DEFAULT 0x0039

#define DRV_WM1808_OUT2VU_R(X) ((X&0x01)<<8)		/* LOUT2 and ROUT2 volumes do not update until a 1 is written to OUT2VU, default N/A */
#define DRV_WM1808_OUT2VU_R_UPDATE 0x01				

#define DRV_WM1808_ROUT2ZC(X) ((X&0x01)<<7)			/* Speaker volume zero cross enable, default 0 */
#define DRV_WM1808_ROUT2ZC_IMMEDIATELY 0x00			/* 0 = Change gain immediately */
#define DRV_WM1808_ROUT2ZC_ZC_ONLY	   0x01			/* 1 = Change gain on zero cross only */

#define DRV_WM1808_ROUT2MUTE(X) ((X&0x01)<<6)		/* Right speaker output mute, default 0 */
#define DRV_WM1808_ROUT2MUTE_NORMAL 0x00			/* 0 = Normal operation */
#define DRV_WM1808_ROUT2MUTE_MUTE   0x01			/* 1 = Mute */	

#define DRV_WM1808_ROUT2VOL(X) ((X&0x3F)<<0)		/* Right speaker output volume, default 57 */
/* 0 = -57dB */
/* ...... */
/* 57 = 0dB */
/* ...... */
/* 63 = +6dB */
		

/*======================================================================================================
 * OUT3 mixer ctrl R56(38h)
 */
#define DRV_WM1808_OUT3_MIXER_CTRL 0x38

#define DRV_WM1808_OUT3_MIXER_CTRL_DEFAULT 0x0001

#define DRV_WM1808_OUT3MUTE(X) ((X&0x01)<<6)		/*default 0 */
#define DRV_WM1808_OUT3MUTE_MIXER 0x00				/* 0 = Output stage outputs OUT2 mixer */
#define DRV_WM1808_OUT3MUTE_MUTE  0x01				/* 1 = Output stage muted - drives out VMID. Can be used as VMID buffer in this mode.(Not to be used for Capless HP pseudo GND) */

#define DRV_WM1808_BYPL2OUT3(X) ((X&0x01)<<2)		/* Left ADC input to OUT3, default 0 */
#define DRV_WM1808_BYPL2OUT3_DISABLED 0x00			/* 0 = disabled */
#define DRV_WM1808_BYPL2OUT3_ENABLED  0x01			/* 1 = enabled */

#define DRV_WM1808_LMIX2OUT3(X) ((X&0x01)<<1)		/* Left DAC mixer to OUT3, default 0 */
#define DRV_WM1808_LMIX2OUT3_DISABLED 0x00			/* 0 = disabled */
#define DRV_WM1808_LMIX2OUT3_ENABLED  0x01			/* 1 = enabled */	

#define DRV_WM1808_LDAC2OUT3(X) ((X&0x01)<<0)		/* Left DAC output to OUT3, default 1 */
#define DRV_WM1808_LDAC2OUT3_DISABLED 0x00			/* 0 = disabled */
#define DRV_WM1808_LDAC2OUT3_ENABLED  0x01			/* 1 = enabled */

/*======================================================================================================
 * Bias Control R61(39h)
 */
#define DRV_WM1808_BIAS_CONTROL 0x39

#define DRV_WM1808_BIAS_CONTROL_DEFAULT 0x0000

#define DRV_WM1808_GLOBAL_BIAS_CTRL(X) ((X&0x01)<<8)		/* Global bias control, default 0 */
#define DRV_WM1808_GLOBAL_BIAS_CTRL_NORMAL 0x00				/* 0 = normal */
#define DRV_WM1808_GLOBAL_BIAS_CTRL_0_5X   0x01				/* 1 = 0.5x */

/*****************************************************************************************************
*                                                                                                   *
* DRV_WM1808 Register Define                                                                           *
*                                                                                                   *
*****************************************************************************************************/
#define DRV_WM1808_REG_CNT             61


typedef enum 
{
	DRV_WM1808_SUBADDR= 	 0xff000000,
	DRV_WM1808_UPDATE_FLAG=  0x00f00000,
	DRV_WM1808_PROPERTY=	 0x000f0000,
	DRV_WM1808_VALUE=		 0x0000ffff
}DrvWM1808RegDataCategory;



typedef enum 
{
	/* validation properties */
	DRV_WM1808_INVALID_DATA=(0x00<<20),
	DRV_WM1808_VALID_DATA  =(0x01<<20),

	/* registser properties */
	DRV_WM1808_READONLY		=(0x01<<16),
	DRV_WM1808_WRITABLE		=(0x02<<16),
	DRV_WM1808_READWRITE		=(0x03<<16),
	DRV_WM1808_DO_NOT_USE		=(0x04<<16)	
}DrvWM1808RegProperty;


#define DrvWM1808RegGetSubAddr(X)		(((X)&DRV_WM1808_SUBADDR)>>24)
#define DrvWM1808RegGetUpdateFlag(X)  	((X)&DRV_WM1808_UPDATE_FLAG)
#define DrvWM1808RegGetProperty(X)		((X)&DRV_WM1808_PROPERTY)
#define DrvWM1808RegGetValue(X)			((X)&DRV_WM1808_VALUE)

#define DrvWM1808RegSetSubAddr(X,Y)	 	{ X&=~(DRV_WM1808_SUBADDR); X|=((Y)<<24);}

#define DrvWM1808RegSetUpdateFlag(X,Y)	{\
	X&=~(DRV_WM1808_UPDATE_FLAG);\
	X|= ((Y)&DRV_WM1808_UPDATE_FLAG);\
	}	

#define DrvWM1808RegSetProperty(X,Y) 	{\
	X&=~(DRV_WM1808_PROPERTY);\
	X|= (Y&DRV_WM1808_PROPERTY);\
	}									

#define DrvWM1808RegSetValue(X,Y) 		{\
	X&=~(DRV_WM1808_VALUE);\
	X|=((Y)&DRV_WM1808_VALUE);\
	}				

#define DrvWM1808RegAddValue(X,Y) 	{\
	X|=((Y)&DRV_WM1808_VALUE);\
	}	

typedef MUint32 DrvWM1808Register;

/** 
* This function initialize alsa codec objec to samsung driver manager.
*
* @author      Park Dong yoon 	(dongyoon21.park@samsung.com)
*/
MBool _DrvWM1808SetCodecObj(struct snd_soc_codec* pCodec);

/** 
* This function makes all registser data to defalt settings.
* it is only data reset. it is not effect hardware audio register.
*
* @author      Park Dong yoon 	(dongyoon21.park@samsung.com)
*/
MBool _DrvWM1808RegMgrResetData(void);

/** 
* This function sets value to the audio register manager module,that is controller of CODEC registsers. 
* changed value is invalid until @fn _DrvWM1800RegMgrApplyData is called.
*
* @return		This function returns TRUE on success or failure returns FALSE.
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies register value data
* @author      Park Dong yoon 	(dongyoon21.park@samsung.com)
* @see			_DrvWM1808RegMgrSetDataEx,_DrvWM1808RegMgrGetData,_DrvWM1808RegMgrApplyData,_DrvWM1808RegMgrAddData
*/
MBool  _DrvWM1808RegMgrSetData (MUint8 regSubaddr, MUint32 regValue);

/** 
* This function adds value to the audio register manager module,that is controller of CODEC registsers. 
* changed value is invalid until @fn _DrvWM1808RegMgrApplyData is called.
*
* @return		This function returns TRUE on success or failure returns FALSE.
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies register value data
* @author      Park Dong yoon 	(dongyoon21.park@samsung.com)
* @see			_DrvWM1808RegMgrSetDataEx,_DrvWM1808RegMgrGetData,_DrvWM1808RegMgrApplyData,_DrvWM1808RegMgrSetData
*/
MBool _DrvWM1808RegMgrAddData (MUint8 regSubaddr, MUint32 regValue);

/** 
* This function adds value to the audio register manager module,that is controller of CODEC registsers. 
*
* @return		This function returns TRUE on success or failure returns FALSE.
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies register value data
* @author      Park Dong yoon 	(dongyoon21.park@samsung.com)
* @see			_DrvWM1808RegMgrSetDataEx,_DrvWM1808RegMgrGetData,_DrvWM1808RegMgrApplyData,_DrvWM1808RegMgrSetData
*/
MBool _DrvWM1808RegMgrAddDataEx (MUint8 regSubaddr, MUint32 regValue);


/** 
* This function sets value to the audio register manager module and apply data to hardware
* CODEC register immediately.
*
* @return		This function returns TRUE on success or failure returns FALSE.
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies register value data
* @author      Park Dong yoon 	(dongyoon21.park@samsung.com)
* @see			_DrvWM1808RegMgrSetDataEx,_DrvWM1808RegMgrGetData,_DrvWM1808RegMgrApplyData
*/
MBool _DrvWM1808RegMgrSetDataEx(MUint8 regSubaddr, MUint32 regValue);


/** 
* This function gets audio register data.
*
* @return		This function returns register value.
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies register value category 
* @author      Park Dong yoon 	(dongyoon21.park@samsung.com)
* @see			_DrvWM1808RegMgrSetDataEx,_DrvWM1808RegMgrGetData,_DrvWM1808RegMgrApplyData,
WM1808RegDataCategory 
*/
MUint16 _DrvWM1808RegMgrGetData (MUint8 regSubaddr, DrvWM1808RegDataCategory category );

/** 
* This function searchs all invalied audio register data
* and sends invalid value to hardware audio register through the IIc module.
* CODEC configurations are changed by sending IIc value.
*
* @return		This function returns register value.
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies register value category 
* @author      Park Dong yoon 	(dongyoon21.park@samsung.com)
* @see			_DrvWM1808RegMgrSetDataEx,_DrvWM1808RegMgrApplyData,
WM1808RegDataCategory,_DrvWM1808RegMgrApplyDataEx
*/
MBool _DrvWM1808RegMgrApplyData(void);

/** 
* This function sends specificate register address value to hardware audio register with IIC communication.
* CODEC configurations are changed by setted value.
*
* @return		This function returns register value.
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies register value category 
* @author      Park Dong yoon 	(dongyoon21.park@samsung.com)
* @see			_DrvWM1808RegMgrApplyData
*/
MBool _DrvWM1808RegMgrApplyDataEx(MUint8 regSubaddr);


/** 
* This function clears register data.
*
* @return		This function returns 1 on success or failure returns 0 
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies value to set the Wm1808 devices 
* @param	   bitmask,	   [in] 
* @author      Park Dong yoon 	(032-277-7293, dongyoon21.park@samsung.com)
*/
MBool _DrvWM1808RegMgrClearData (MUint8 regSubaddr, MUint16 bitMask);

/** 
* This function clears register data.
*
* @return		This function returns 1 on success or failure returns 0 
* @param       regSubaddr, [in] Specifies register sub address
* @param       regValue,   [in] Specifies value to set the Wm1808 devices 
* @param	   bitmask,	   [in] 
* @author      Park Dong yoon 	(032-277-7293, dongyoon21.park@samsung.com)
*/
MBool _DrvWM1808RegMgrClearDataEx (MUint8 regSubaddr, MUint16 bitMask);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DRV_WM1808_REGMGR_H_ */

