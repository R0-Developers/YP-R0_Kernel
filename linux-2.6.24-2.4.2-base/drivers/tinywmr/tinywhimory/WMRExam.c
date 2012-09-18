/*****************************************************************************/
/*                                                                           */
/* COMPONENT   : Rainbow				                                     */
/* MODULE      : Example			                                         */
/* NAME    	   : Example					                                 */
/* FILE        : WMRExam.c		                                             */
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
/*   12-SEP-2005 [Jaesung Jung] : first writing                              */
/*   15-MAR-2006 [Yangsup Lee ] : bad block detection bug fix                */
/*                                                                           */
/*****************************************************************************/
#include "WMRConfig.h"
#include "WMRTypes.h"
#include "WMROAM.h"
#include "VFLBuffer.h"
#include "VFL.h"
#include "VFLTypes.h"
#include "FIL.h"

#include "WMRExam.h"

/*****************************************************************************/
/* Constants																 */
/*****************************************************************************/
#define		WMR_SIGNATURE_POFFSET			(PAGES_PER_BLOCK-1)
//#define		WMR_SIGNATURE_POFFSET			(PAGES_PER_BLOCK*22-1)	// [Hyunkwon]

BOOL32		IS_PSEUDO_MLC = 0;

/*****************************************************************************/
/* Static function prototypes                                                */
/*****************************************************************************/

PRIVATE INLINE Buffer *	_GetBuffer			(BUFType eTypeT32);

/*****************************************************************************/
/* Debug Print #defines                                                      */
/*****************************************************************************/
//#define 	EXAM_INF_MSG_ON
//#define 	EXAM_LOG_MSG_ON

#define     EXAM_ERR_PRINT(x)            WMR_RTL_PRINT(x)
#define     EXAM_RTL_PRINT(x)            WMR_RTL_PRINT(x)

#if defined(EXAM_LOG_MSG_ON)
#define     EXAM_LOG_PRINT(x)            WMR_RTL_PRINT(x)
#else
#define     EXAM_LOG_PRINT(x)
#endif

#if defined(EXAM_INF_MSG_ON)
#define     EXAM_INF_PRINT(x)            WMR_RTL_PRINT(x)
#else
#define     EXAM_INF_PRINT(x)
#endif

/*****************************************************************************/
/* Code Implementation                                                       */
/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      _GetBuffer                                                           */
/* DESCRIPTION                                                               */
/*      This function get buffer                                             */
/* PARAMETERS                                                                */
/*		eType			[IN]	buffer type									 */
/* RETURN VALUES                                                             */
/* 		Buffer	        _GetBuffer is completed.                             */
/*      NULL			_GetBuffer is failed.                                */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
PRIVATE INLINE Buffer *
_GetBuffer(BUFType eTypeT32)
{
	Buffer *pBuf = NULL;

	do
	{
		pBuf = BUF_Get( eTypeT32, NULL);

		if ( pBuf == NULL )
		{
			/*  - tinyWhimory don't have write or erase operation */
			/*
			VFL_Sync();
			*/
			WMR_ASSERT(1);		
		}
		else
		{
			break;
		}
	}while(1);
	
	return pBuf;
}
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      WMR_LOW_Init			                                             */
/* DESCRIPTION                                                               */
/*      This function only inits VFL_Layer & open.		  					 */
/*				Open is faster than WMR_Init                                 */
/*              but, only APIs of VFL are avaiable to user.                  */
/* PARAMETERS                                                                */
/*                                                                           */
/* RETURN VALUES                                                             */
/*		WMR_SUCCESS															 */
/*				FTL_Open is completed										 */
/*		WMR_CRITICAL_ERROR													 */
/*				FTL_Open is failed											 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32
WMR_LOW_Init(VOID)
{
	Buffer *pBuf;
	INT32 	nRet;
	LowFuncTbl *pLowFuncTbl;
	UINT32	nSig;

	nRet = FIL_Init();

	if (nRet != FIL_SUCCESS)
	{
		return WMR_CRITICAL_ERROR;
	}

	EXAM_RTL_PRINT((TEXT("[FTL:MSG] FIL_Init			[OK]\n")));
	
	/* duplicate call ( VFL_Init call BUF_Init )
	nRet = BUF_Init();

	if (nRet != BUF_SUCCESS)
	{
		return WMR_CRITICAL_ERROR;
	}

	EXAM_RTL_PRINT((TEXT("[FTL:MSG] BUF_Init			[OK]\n")));
	*/

	nRet = VFL_Init();

	if (nRet != VFL_SUCCESS)
	{
		return WMR_CRITICAL_ERROR;
	}

	EXAM_RTL_PRINT((TEXT("[FTL:MSG] VFL_Init			[OK]\n")));


	pBuf = _GetBuffer(BUF_MAIN_AND_SPARE);

	if (pBuf == NULL)
	{
		return WMR_CRITICAL_ERROR;
	}

	pLowFuncTbl = FIL_GetFuncTbl();

	nRet = pLowFuncTbl->Read(0, WMR_SIGNATURE_POFFSET, FULL_SECTOR_BITMAP_PAGE, enuBOTH_PLANE_BITMAP, 
							 pBuf->pData, pBuf->pSpare, FALSE32, TRUE32);

	nSig = *(UINT32 *)(pBuf->pData);

	BUF_Release(pBuf);

	if (nRet == FIL_SUCCESS_CLEAN)
	{
		EXAM_RTL_PRINT((TEXT("[FTL:MSG] Not Formated !\n")));
		EXAM_RTL_PRINT((TEXT("[FTL:MSG]   --> use WMR_Init() to format!\n")));
		EXAM_RTL_PRINT((TEXT("[FTL:MSG] Not Found WMR Signature	[ERROR]\n")));

		return WMR_CRITICAL_ERROR;
	}
	else if (nRet != FIL_SUCCESS)
	{
		EXAM_RTL_PRINT((TEXT("[FTL:MSG] read WMR Signature		[ERROR]\n")));
		return WMR_CRITICAL_ERROR;
	}

	if (nSig != WMR_SIGNATURE)
	{
		EXAM_ERR_PRINT((TEXT("[FTL:ERR] Version Fail			[ERROR]\n")));
		return WMR_CRITICAL_ERROR;
	}

	nRet = VFL_Open();

	if (nRet != VFL_SUCCESS)
	{
		return WMR_CRITICAL_ERROR;
	}

	EXAM_RTL_PRINT((TEXT("[FTL:MSG] VFL_Open			[OK]\n")));

	return WMR_SUCCESS;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      WMR_LOW_Close								                         */
/* DESCRIPTION                                                               */
/*      This function releases whimory.										 */
/* PARAMETERS                                                                */
/*      none														         */
/* RETURN VALUES                                                             */
/*      none														         */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
VOID
WMR_LOW_Close(VOID)
{
	VFL_Close();

	return;
}
