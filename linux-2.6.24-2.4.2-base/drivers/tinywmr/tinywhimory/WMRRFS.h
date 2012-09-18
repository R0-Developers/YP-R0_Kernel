/*****************************************************************************/
/*                                                                           */
/* COMPONENT   : Rainbow				                                     */
/* MODULE      : Example			                                         */
/* NAME    	   : Example					                                 */
/* FILE        : WMRRFS.h		                                             */
/* PURPOSE 	   : the example for whimory initialization						 */
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
/*   17-MAR-2007 [Junho   Jang] : first writing                              */
/*                                                                           */
/*****************************************************************************/
#ifndef _WMR_RFS_H_
#define _WMR_RFS_H_

#include "WMRConfig.h"
#include "WMRTypes.h"
#include "WMROAM.h"
//#include "FTL.h"
#include "VFLBuffer.h"
#include "VFL.h"
#include "VFLTypes.h"
#include "FIL.h"

#include "WMRExam.h"

/*****************************************************************************/
/* Return value of WMR_XXX()                                                 */
/*****************************************************************************/
#ifndef		WMR_SUCCESS
#define     WMR_SUCCESS                     WMR_RETURN_VALUE(0, 0x0000, 0x0000)
#endif
#ifndef		WMR_CRITICAL_ERROR
#define     WMR_CRITICAL_ERROR              WMR_RETURN_VALUE(1, 0x0001, 0x0000)
#endif

/*****************************************************************************/
/* Parameter constant of IOCTL                                               */
/*****************************************************************************/

/*****************************************************************************/
/* structure                                                                 */
/*****************************************************************************/
typedef struct {
    UINT32		nSctsPerPgLLD;
    UINT32		nSctsPerPgNAND;
    UINT32		nNumOfPlane;
    UINT32		nPgsPerBlock;
    UINT32		nBlksPerNand;	
}WMRLLDSpec;

/*****************************************************************************/
/* exported function prototype of RFS                                        */
/*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/************************************************/
/* API for BML layer							*/
/************************************************/
/* common APIs */
INT32	BML_Open					(VOID);
VOID	BML_Close					(VOID);

INT32	BML_GetDevInfo				(WMRLLDSpec *stLLDSpec);

/* WMR_AREA APIs */
INT32	BML_WMRArea_Read			(UINT32  nPpn, UINT8 *pBuf);

/* RO_AREA APIs */
INT32	BML_Mread					(UINT32  nROPartNo, UINT32  nLPN, UINT32  nPages, UINT8 *pBuf);
INT32	BML_GetInfo					(UINT32  nROPartNo, UINT32 *nMaxPages);

INT32	BML_MreadPL				(UINT32  nROPartNo, UINT32  nPPO, UINT32  nPages, UINT8 *pBuf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RFS_H_ */
