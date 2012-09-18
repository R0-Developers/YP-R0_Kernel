/*****************************************************************************/
/*                                                                           */
/* PROJECT : Rainbow	                                                     */
/* MODULE  : Whimory configuration definition heade file                     */
/* NAME    : Whimory configuration definition                                */
/* FILE    : WMRConfig.h                                                     */
/* PURPOSE : Configuation definition for Whimory                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        COPYRIGHT 2003-2005, SAMSUNG ELECTRONICS CO., LTD.                 */
/*                      ALL RIGHTS RESERVED                                  */
/*                                                                           */
/*   Permission is hereby granted to licensees of Samsung Electronics        */
/*   Co., Ltd. products to use or abstract this computer program for the     */
/*   sole purpose of implementing a product based on Samsung                 */
/*   Electronics Co., Ltd. products. No other rights to reproduce, use,      */
/*   or disseminate this computer program, whether in part or in whole,      */
/*   are granted.                                                            */
/*                                                                           */
/*   Samsung Electronics Co., Ltd. makes no representation or warranties     */
/*   with respect to the performance of this computer program, and           */
/*   specifically disclaims any responsibility for any damages,              */
/*   special or consequential, connected with the use of this program.       */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/* REVISION HISTORY                                                          */
/*                                                                           */
/*   12-JUL-2005 [Jaesung Jung] : first writing                              */
/*   03-NOV-2005 [Yangsup Lee ] : Add wear-leveling algorithm				 */
/*                                                                           */
/*****************************************************************************/

#ifndef _WMR_CONFIG_H_
#define _WMR_CONFIG_H_

/*****************************************************************************/
/* Build Config Definition which should be shared by FTL, VFL, FIL           */
/* Must edit this configurations for your target							 */
/*****************************************************************************/

/* [Supported Device Selection]								*/
/* can reduce code size with editing this configurations	*/
#define		WMR_SLC_SUPPORT		(0)					/* support SLC large block nand		 */
#define		WMR_MLC_SUPPORT		(1)					/* support MLC large block nand		 */


#if ((!WMR_SLC_SUPPORT) && (!WMR_MLC_SUPPORT))
#error Configuration Error! Must Select Supported Device
#endif

// [Hyunkwon]
#define		WMR_4ECC_SUPPORT			(0)				/* support 4bit ECC nand	*/
#define		WMR_8ECC_SUPPORT			(1)				/* support 4bit ECC nand	*/

#if ((!WMR_4ECC_SUPPORT) && (!WMR_8ECC_SUPPORT))
#error Configuration Error! Must Select Supported ECC Type
#endif

#define		WMR_STDLIB_SUPPORT	(1)					/* support divide operation			 */

#define		WMR_ALIGN_CHECK		(1)					/* support 4 bytes align check		 */

#define		WMR_BIG_ENDIAN		(0)					/* support big endian 				 */

#define		WMR_READ_RECLAIM	(0)					/* support read 1bit ecc error reclaim */

#define		WMR_SUPPORT_META_WEAR_LEVEL  (1) 		/* support FTL meta block wear level algorithm      */

#define		WMR_MLC_LSB_RECOVERY	(0)				/* support reovery LSB corruption of MLC 	*/

#define		WMR_MLC_LSB_SQA			(0)		/* support SQA */

#define     WMR_PROGRAM_PER_PLANE       (1)                 /* support adoptation NOP per 1 plane when sequential parial plane write */

#define     WMR_SIMULATION      (0)     			/* support simulation                */


#if ((WMR_SLC_SUPPORT) && (WMR_MLC_LSB_RECOVERY || WMR_MLC_LSB_SQA))
#error Configuration Error! turn off MLC configurations when WMR_SLC_SUPPORT is 1.
#endif


#define		WMR_RO_AREA_SUPPORT		(1)		/* support Read Only Area for boot loader, OS and etc */

#define		WMR_RO_AREA_PB			(296)	/* Size of Basic Uint = PB ( Physical Block ) */
#define		WMR_RO_PAT0_PB			(8)		/* size of 1st RO Partition */
#define		WMR_RO_PAT1_PB			(40)	/* size of 2st RO Partition */
#define		WMR_RO_PAT2_PB			(96)	/* size of 3rd RO Partition */
#define		WMR_RO_PAT3_PB			(152)	/* size of 4th RO Partition */

#define		WMR_RO_PAT4_PB			(0)		/* size of 5th RO Partition */
#define		WMR_RO_PAT5_PB			(0)		/* size of 6th RO Partition */
#define		WMR_RO_PAT6_PB			(0)		/* size of 7th RO Partition */

/*****************************************************************************/
/* Global Config Definition which should be shared by FTL, VFL, FIL          */
/*****************************************************************************/
#define		WMR_MAX_DEVICE		(4)					/* the maximum number of bank		 */

#define		WMR_SECTOR_SIZE		(512)				/* the size of sector				 */

#if (WMR_4ECC_SUPPORT)
#define 	WMR_SPARE_SIZE		(16)					/* the size of spare				 	*/
#endif
#if (WMR_8ECC_SUPPORT)
#define 	WMR_SPARE_SIZE		(25)					/* the size of spare				 	*/
#endif

/* tinyWhimory needs only 1 buffer for read operation */
#define		WMR_NUM_BUFFERS		(1)					/* the number of buffers			 */

#define		WMR_SECTORS_PER_PAGE_MAX (16)			/* the maximum size of sectors per page */

#define		WMR_MAX_VB			(16384)				/* the maximum number of virtual block*/

#define		WMR_MAX_RESERVED_SIZE	(400) 			/* the maximum count of reserved blocks */

#define		WMR_MAPS_PER_BLOCK	(WMR_SECTOR_SIZE / 2)	/* the count of map index per block  */

#define		WMR_NUM_MAPS_MAX	(WMR_MAX_VB / WMR_MAPS_PER_BLOCK)	/* the maximum number of map blocks  */

#if (WMR_READ_RECLAIM)
#define		WMR_MAX_RECLAIM		(5)					/* the maximum number of reclaim count*/
#endif

/* reserved block ratio for whimory context & reserved section    */
/* you can change this value to change whimory total sector count */
#define		WMR_USER_SUBLKS_RATIO (244)

#define		WMR_WEAR_LEVEL_FREQUENCY	    (20)
#define		WMR_WEAR_LEVEL_MAX_DIFF		    (10)	/* maximum different between min and max erase count */
#define		WMR_META_WEAR_LEVEL_FREQUENCY	(200)

#endif /* _WMR_CONFIG_H_ */
