/*
 * as3543.h -- Audio Driver for AS3543
 *
 */

#ifndef _SOUND_AS3543_H_
#define _SOUND_AS3543_H_

#define AUDIO_NAME "AS3543 ALSA"
#define AS3543_VERSION "1.0"

// Clock sources & directions
#define AS3543_MCLK_SEL_MCLK		0
#define AS3543_MCLK_SEL_PLL			1

#ifndef TRUE
#define TRUE						1
#endif

#ifndef FALSE
#define FALSE						0
#endif


#undef AS3543_USE_INTERNAL_MUTE


typedef enum
{
	AS3543_WAVE,
	AS3543_FM
} AS3543_INPUT_TYPE;

typedef enum
{
	AS3543_HP,			// Earphone
	AS3543_LINE			// Line Out
} AS3543_OUTPUT_TYPE;

typedef enum
{
	AS3543_PLL_AUTO,
	AS3543_PLL_ON,
	AS3543_PLL_OFF,
	AS3543_PLL_AUTO_INV,
	AS3543_PLL_MAX,
} AS3543_PLL_MODE;

typedef enum
{
	AS3543_PLL_VCO_24_48,
	AS3543_PLL_VCO_8_23,
	AS3543_PLL_VCO_HIGH_24M,
	AS3543_PLL_VCO_MAX,
} AS3543_PLL_VCO_MODE;

typedef enum
{
	AS3543_PLL_OSR_256,
	AS3543_PLL_OSR_128,
	AS3543_PLL_OSR_1024,
	AS3543_PLL_OSR_512,
	AS3543_PLL_OSR_MAX,
} AS3543_PLL_OSR_MODE;

typedef enum
{
	AS3543_PLL_FOUT_DIV_2,
	AS3543_PLL_FOUT_DIV_1,
	AS3543_PLL_FOUT_DIV_8,
	AS3543_PLL_FOUT_DIV_4,
	AS3543_PLL_FOUT_DIV_MAX,
} AS3543_PLL_FOUT_DIV;



//////////////////////////

extern int AS3543Init(void);
extern int AS3543DeInit(void);

int AS3543InputMode(AS3543_INPUT_TYPE eType);
int AS3543OutputMode(AS3543_OUTPUT_TYPE eType);
u32 AS3543SetVolume(long volume, int iRefresh);


#endif	// _SOUND_AS3543_H_

