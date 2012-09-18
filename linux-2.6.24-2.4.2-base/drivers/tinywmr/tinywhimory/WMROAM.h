/*****************************************************************************/
/*                                                                           */
/* COMPONENT   : Rainbow				                                     */
/* MODULE      : OS Adaptation Module                                        */
/* FILE        : WMROAM.h                                                    */
/* PURPOSE     : This file contains the definition and protypes of exported  */
/*              functions for OS Adaptation Module.                          */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*          COPYRIGHT 2003-2005 SAMSUNG ELECTRONICS CO., LTD.                */
/*                          ALL RIGHTS RESERVED                              */
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
/*   14-MAR-2007 [JunHo Jang] : first writing                                */
/*                                                                           */
/*****************************************************************************/

#ifndef _WMR_OAM_H_
#define _WMR_OAM_H_

//#include <linux/config.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/bitops.h>
#include <asm/io.h>

//#define OAM_DBGMSG_ENABLE

/*****************************************************************************/
/* OS dependent memory management functions definition                       */
/* if OS is changed, edit this MACROs									     */
/*****************************************************************************/
#define		WMR_MEMSET(x, y, z)				memset(x, y, z)
#define		WMR_MEMCPY(x, y, z)				memcpy(x, y, z)
#define		WMR_MALLOC(x)					OAM_Malloc(x)
#define		WMR_FREE(x)						OAM_Free(x)

/* Linux Specific feature */
#define		WMR_PA2VA(x)					OAM_Pa2Va(x) 

/*****************************************************************************/
/* OS dependent semaphore management functions definition                    */
/* if OS is changed, edit this MACROs									     */
/*****************************************************************************/
#define		WMR_CreateSM(x)					OAM_CreateSM(x)
#define		WMR_DestroySM(x)				OAM_DestroySM(x)
#define		WMR_AcquireSM(x)				OAM_AcquireSM(x)
#define		WMR_ReleaseSM(x)				OAM_ReleaseSM(x)

#define		WMR_SM_STL						(3)
#define		WMR_SM_BML						(1)
#define		WMR_SM_BUF						(2)

/*****************************************************************************/
/* Debug/Retail message print MACRO definition			                     */
/*****************************************************************************/
#if defined(OAM_RTLMSG_DISABLE)
#define	WMR_RTL_PRINT(x)
#else
//#define	WMR_RTL_PRINT(x)	OAM_Debug x  
#define	WMR_RTL_PRINT(x)	printk  x 
#endif /* OAM_RTLMSG_DISABLE */

#define OAM_DBGMSG_ENABLE	// [Hyunkwon]
#if defined(OAM_DBGMSG_ENABLE)
//#define	WMR_DBG_PRINT(x)	OAM_Debug x  
#define	WMR_DBG_PRINT(x)	printk   x 
#else
#define	WMR_DBG_PRINT(x)
#endif /* OAM_DBGMSG_ENABLE */

#undef	TEXT
#define	TEXT(x)			KERN_EMERG x 

#undef	INLINE
#define	INLINE					__inline


/*****************************************************************************/
/* ASSERT MACRO #define                                                      */
/*****************************************************************************/
#define	WMR_ASSERT(x)			OAM_Assert(x, (const UINT8*)__FILE__, (UINT32)__LINE__)

/*****************************************************************************/
/* NULL #defines                                                             */
/*****************************************************************************/
#ifndef		NULL
#ifdef		__cplusplus
#define		NULL				0
#else
#define		NULL				((void *)0)
#endif
#endif

/*****************************************************************************/
/* PRIVATE #defines                                                          */
/*****************************************************************************/
#define		PRIVATE				static

/*****************************************************************************/
/* exported function prototype of OAM                                        */
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

UINT16	 WMR_GetChkSum			(UINT8 *pBuf,	UINT32	nSize);

#if (WMR_MLC_SUPPORT)
UINT32	 WMR_GetRelatedChkSum	(UINT8 *pBuf1, 	UINT8	*pBuf2,		UINT32	nSize);
#endif


void    OAM_Assert(BOOL32 bVal, const UINT8 *szFile, UINT32 nLine);
void   *OAM_Malloc(UINT32 nSize);
void    OAM_Free(void *pMem);
UINT32  OAM_Pa2Va(UINT32 nPAddr);
BOOL32  OAM_CreateSM(UINT32 nHandle);
BOOL32  OAM_DestroySM(UINT32 nHandle);
BOOL32  OAM_AcquireSM(UINT32 nHandle);
BOOL32  OAM_ReleaseSM(UINT32 nHandle);
VOID    OAM_Debug(VOID *pStr, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _WMR_OAM_H_ */
