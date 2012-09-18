/*****************************************************************************/
/*                                                                           */
/* COMPONENT   : Rainbow				                                     */
/* MODULE      : Virtual Flash Layer                                         */
/* NAME    	   : VFL header file			                                 */
/* FILE        : VFL.h		                                                 */
/* PURPOSE 	   : This file contains the definition and protypes of exported  */
/*           	 functions for VFL. 				                         */
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
/*   18-JUL-2005 [Jaesung Jung] : first writing                              */
/*   31-MAR-2006 [Yangsup Lee ] : support ftl meta block wear leveling       */
/*                                                                           */
/*****************************************************************************/

#ifndef _VFL_H_
#define	_VFL_H_

/*****************************************************************************/
/* Return value of VFL_XXX()                                                 */
/*****************************************************************************/
#define     VFL_SUCCESS                     WMR_RETURN_VALUE(0, 0x0000, 0x0000)
#define     VFL_SUCCESS_CLEAN               WMR_RETURN_VALUE(0, 0x0000, 0x0001)
#define     VFL_CRITICAL_ERROR              WMR_RETURN_VALUE(1, 0x0001, 0x0000)
#define     VFL_U_ECC_ERROR		            WMR_RETURN_VALUE(1, 0x0002, 0x0000)

/*****************************************************************************/
/* Type definition of checksum (confirm) state                               */
/*****************************************************************************/

/*****************************************************************************/
/* exported function prototype of VFL                                        */
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*****************************************************************************/
/* API for User Space ( out of eWhimory )                                    */
/*****************************************************************************/

/* API for RO area                                                           */
#if (WMR_RO_AREA_SUPPORT)
INT32	VFL_RO_GetInfo			(UINT32  nPartNo, UINT32 *nMaxPages);
INT32	VFL_RO_Read				(UINT32  nPartNo, UINT32  nLpn,	UINT8 *pBuf);
INT32	VFL_RO_ReadPL			(UINT32  nPartNo, UINT32  nPPO,	UINT8 *pBuf);
#endif

/* API for Bootable WMR_Area area                                            */
INT32	VFL_WMRArea_Read		(UINT32  nPpn,	UINT8 *pBuf);

/*****************************************************************************/
/* common API ( for WMR API )                                                */
/*****************************************************************************/
INT32	 VFL_Init				(VOID);
INT32	 VFL_Open				(VOID);
void	 VFL_Close				(VOID);

/*****************************************************************************/
/* API for RW area ( for FTL Area )                                          */
/*****************************************************************************/
INT32	 VFL_Read				(UINT32	 nVpn,	Buffer *pBuf,	BOOL32 	bCleanCheck);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _VFL_H_ */
