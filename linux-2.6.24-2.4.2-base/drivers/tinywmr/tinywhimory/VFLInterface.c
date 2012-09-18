/*****************************************************************************/
/*                                                                           */
/* COMPONENT   : Rainbow				                                     */
/* MODULE      : Virtual Flash Layer                                         */
/* NAME    	   : VFL interface				                                 */
/* FILE        : VFLinterface.c                                              */
/* PURPOSE 	   : This file contains the exported routine for interfacing with*/
/*           	 the upper layer of VFL.                                     */
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
/* 	 16-SEP-2003 [SJ Myoung]	: multi-block read speed up 	             */
/*						   flash_read(,,,UINT8 sector_offset) add argument	 */
/* 	 22-SEP-2003 [JH Kim]		: buf_get() bug-fix (if no free buffer)		 */
/* 	 29-SEP-2003 [HM Jung] 		: code size reduction 						 */
/*								  check init bad from PAGE_CLEAN			 */
/* 	 03-NOV-2003 [JH Kim]		: replace_bad_block() - add loop when erase	 */
/*								  if erase fail								 */
/* 	 12-JAN-2004 [JH Kim]		: fix by sequential order					 */
/* 	 26-JAN-2004 [JH Kim] 		: add buffer usage trace code				 */
/* 	 13-MAR-2005 [Jaesung Jung]	: duet port									 */
/* 	 21-MAR-2005 [Jaesung Jung] : move ASSERT DEFINE to copy_data.h			 */
/* 	 21-MAR-2005 [Jaesung Jung] : remove pattern_test code					 */
/*	 28-MAR-2005 [Jaesung Jung] : add logical to physical API				 */
/*	 07-APR-2005 [Jaesung Jung] : MLC support (2way)						 */
/*	 08-APR-2005 [Jaesung Jung] : add context checksum check				 */
/*	 11-APR-2005 [Jaesung Jung] : optimize PAGE_CLEAN check					 */
/*   18-JUL-2005 [Jaesung Jung] : reorganize code							 */
/*	 06-SEP-2005 [Jaesung Jung] : bug-fix from code inspection				 */
/*   22-NOV-2005 [Yangsup Lee ] : remove checksum code						 */
/*   24-JAN-2006 [Yangsup Lee ] : change sector offset to sector bitmap      */
/*   24-JAN-2006 [Yangsup Lee ] : support un-pair bad block management       */
/*   31-MAR-2006 [Yangsup Lee ] : support ftl meta block wear leveling       */
/*   11-JUL-2006 [Yangsup Lee ] : allow different plane mapping				 */
/*   01-JAN-2007 [Inhwan Choi ] : 4k page nand support                       */
/*   13-FEB-2007 [Inhwan Choi ] : runtime unpair BBM	                     */
/*   13-FEB-2007 [Junho  Jang ] : support RO_AREA		                     */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Header file inclusions                                                    */
/*****************************************************************************/
#include "WMRConfig.h"
#include "WMRTypes.h"
#include "WMROAM.h"
#include "VFLBuffer.h"
#include "VFLTypes.h"
#include "VFL.h"
#include "FIL.h"

/*****************************************************************************/
/* Debug Print #defines                                                      */
/*****************************************************************************/

#define     VFL_ERR_PRINT(x)            WMR_RTL_PRINT(x)
#define     VFL_RTL_PRINT(x)            WMR_RTL_PRINT(x)

//#define VFL_LOG_MSG_ON
#if defined(VFL_LOG_MSG_ON)
#define     VFL_LOG_PRINT(x)            WMR_RTL_PRINT(x)
#else
#define     VFL_LOG_PRINT(x)
#endif

//#define VFL_INF_MSG_ON
#if defined(VFL_INF_MSG_ON)
#define     VFL_INF_PRINT(x)            WMR_RTL_PRINT(x)
#else
#define     VFL_INF_PRINT(x)
#endif

/*****************************************************************************/
/* Constants					                                             */
/*****************************************************************************/
#define		WMR_BADTBL_VFLCXT_POFFSET		(PAGES_PER_BLOCK-2)
#define		WMR_SIGNATURE_POFFSET			(PAGES_PER_BLOCK-1)
//#define		WMR_BADTBL_VFLCXT_POFFSET		(PAGES_PER_BLOCK*22-2)	// [Hyunkwon]
//#define		WMR_SIGNATURE_POFFSET			(PAGES_PER_BLOCK*22-1)	// [Hyunkwon]

#define		VFL_CXT_POFFSET				(0)

extern		BOOL32			IS_PSEUDO_MLC;

/*****************************************************************************/
/* Static variables definitions                                              */
/*****************************************************************************/
PRIVATE	VFLMeta	 	*pstVFLMeta = NULL;
PRIVATE	AsyncOp		*pstAsyncOp = NULL;
PRIVATE LowFuncTbl	*pFuncTbl = NULL;
PRIVATE UINT8       *pVFLSpare = NULL;

#if (WMR_SUPPORT_META_WEAR_LEVEL)
/* KJH - for meta block wear-leveling */
PRIVATE UINT32      gMaxGlobalAge;
#endif

#if (WMR_RO_AREA_SUPPORT)
PRIVATE ROAreaMeta	*pstROAreaMeta = NULL;
#endif

/*****************************************************************************/
/* Definitions                                                               */
/*****************************************************************************/
#define 	GET_VFLCxt(v)       		(&(pstVFLMeta + v)->stVFLCxt)
#define 	GET_AsyncOp(v)				(pstAsyncOp + v)
#define 	GET_Ppn(Pbn, POffset) 		((Pbn) * PAGES_PER_BLOCK + (POffset))
#define		GET_PlaneBitmap(v)			((v % 2) + 1) // even number->0x01, odd number->0x10

/*****************************************************************************/
/* Static function prototypes                                                */
/*****************************************************************************/

#if (WMR_RO_AREA_SUPPORT )
PRIVATE INLINE BOOL32	_ROInit			(VOID);
PRIVATE INLINE BOOL32	_ROOpen			(VOID);
PRIVATE INLINE VOID		_ROClose		(VOID);

PRIVATE BOOL32			_ConfigureROAreaMeta(VOID);
PRIVATE BOOL32 			_LoadROAreaMeta		(BOOL32  bCmpPatTbl);

PRIVATE INLINE VOID		_GetROPhysicalAddr	(UINT32  nPartNo,	UINT32 nLpn,
											 UINT32 *pBank,		UINT32 *pPbn,	
											 UINT32 *pPOffset);
#endif
PRIVATE INLINE Buffer *	_GetBuffer		(BUFType eTypeT32);

PRIVATE INLINE VOID	_GetVFLSpare		(UINT32 *pAge,		UINT8  *pStatus,
										 UINT8  *pSBuf);
PRIVATE BOOL32 		_LoadVFLCxt			(UINT32  nBank);

PRIVATE BOOL32 		_RemapBadBlk		(UINT32  nBank,		UINT32  nSrcPbn,
                                         UINT32 *pDesLPbn,  UINT32 *pDesRPbn);

PRIVATE VOID		_GetPhysicalAddr	(UINT32  nVpn, 		UINT32 *pBank, 
										 UINT32 *pPbn, 		UINT32 *pPOffset);

const UINT8 aChecksumTable[256]  =
   { 8, 7, 7, 6, 7, 6, 6, 5, 7, 6, 6, 5, 6, 5, 5, 4, 
      7, 6, 6, 5, 6, 5, 5, 4, 6, 5, 5, 4, 5, 4, 4, 3, 
      7, 6, 6, 5, 6, 5, 5, 4, 6, 5, 5, 4, 5, 4, 4, 3, 
      6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, 
      7, 6, 6, 5, 6, 5, 5, 4, 6, 5, 5, 4, 5, 4, 4, 3, 
      6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, 
      6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, 
      5, 4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3, 2, 2, 1, 
      7, 6, 6, 5, 6, 5, 5, 4, 6, 5, 5, 4, 5, 4, 4, 3, 
      6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, 
      6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, 
      5, 4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3, 2, 2, 1, 
      6, 5, 5, 4, 5, 4, 4, 3, 5, 4, 4, 3, 4, 3, 3, 2, 
      5, 4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3, 2, 2, 1, 
      5, 4, 4, 3, 4, 3, 3, 2, 4, 3, 3, 2, 3, 2, 2, 1, 
      4, 3, 3, 2, 3, 2, 2, 1, 3, 2, 2, 1, 2, 1, 1, 0
    }; 
/*****************************************************************************/
/* Code Implementation                                                       */
/*****************************************************************************/


/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      _ROInit                                                              */
/* DESCRIPTION                                                               */
/*      This function initializes RO layer. 							     */
/* PARAMETERS                                                                */
/*      none			                                                     */
/* RETURN VALUES                                                             */
/* 		VFL_SUCCESS                                                          */
/*            _ROInit is completed.                                          */
/*      VFL_CRITICAL_ERROR                                                   */
/*            _ROInit is failed    		                                     */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
#if (WMR_RO_AREA_SUPPORT )
PRIVATE INLINE BOOL32
_ROInit(VOID)
{	
	VFL_LOG_PRINT((TEXT("[VFL: IN] ++_ROInit()\r\n")));
	
	pstROAreaMeta = (ROAreaMeta *)WMR_MALLOC(sizeof(ROAreaMeta));
	
	if (pstROAreaMeta == NULL)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  there is no memory to initialize pstROAreaMeta!\r\n")));	
		return FALSE32;
	}

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --_ROInit()\r\n")));

	return TRUE32;
}
#endif

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      _ROOpen                                                              */
/* DESCRIPTION                                                               */
/*      This function opens RO layer.									     */
/* PARAMETERS                                                                */
/*      none			                                                     */
/* RETURN VALUES                                                             */
/* 		VFL_SUCCESS                                                          */
/*            _ROOpen is completed.                                          */
/*      VFL_CRITICAL_ERROR                                                   */
/*            _ROOpen is failed    		                                     */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
#if (WMR_RO_AREA_SUPPORT )
PRIVATE INLINE BOOL32
_ROOpen(VOID)
{
	BOOL32 bRet;

	VFL_LOG_PRINT((TEXT("[VFL: IN] ++_ROOpen()\r\n")));

	bRet = _ConfigureROAreaMeta();

	if (bRet == FALSE32)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  _ConfigureROAreaMeta() fail!\r\n")));	
		return FALSE32;
	}

	bRet = _LoadROAreaMeta(TRUE32);

	if (bRet == FALSE32)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  _LoadROAreaMeta() fail!\r\n")));	
		return FALSE32;
	}

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --_ROOpen()\r\n")));

	return TRUE32;
}
#endif
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      _ROClose                                                             */
/* DESCRIPTION                                                               */
/*      This function releases RO layer.								     */
/* PARAMETERS                                                                */
/*      none			                                                     */
/* RETURN VALUES                                                             */
/*      none			                                                     */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
#if (WMR_RO_AREA_SUPPORT )
PRIVATE INLINE VOID
_ROClose(VOID)
{	
	VFL_LOG_PRINT((TEXT("[VFL: IN] ++_ROClose()\r\n")));

    if (pstROAreaMeta != NULL)
    {
        WMR_FREE(pstROAreaMeta);

        pstROAreaMeta = NULL;
    }

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --_ROClose()\r\n")));
}
#endif

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      _GetROPhysicalAddr	                                                 */
/* DESCRIPTION                                                               */
/*      This function returns the physical bank, block & page address of	 */
/*		the logical page address under RO_AREA								 */
/* PARAMETERS                                                                */
/*      nPartNo  [IN] 	partition number			                         */
/*      nLpn     [IN] 	logical page number			                         */
/*      pBank    [OUT] 	physical bank number		                         */
/*      pPbn     [OUT] 	physical block number		                         */
/*      pPOffset [OUT] 	physical page offset		                         */
/* RETURN VALUES                                                             */
/*		none																 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
#if (WMR_RO_AREA_SUPPORT )
PRIVATE INLINE VOID		
_GetROPhysicalAddr	(UINT32  nPartNo, UINT32 nLpn,
					 UINT32 *pBank,	UINT32 *pPbn, UINT32 *pPOffset)
{
	UINT32	nAreaStartPB;
	UINT32	nPatOffsetPB;

	VFL_LOG_PRINT((TEXT("[VFL: IN] ++_GetROPhysicalAddr(nPartNo:%d,nLpn:%d)\r\n"), nPartNo, nLpn));
	
	WMR_ASSERT( nPartNo < MAX_RO_PARTS );
	WMR_ASSERT( nLpn < pstROAreaMeta->aPatTbl[nPartNo].nSizePB * PAGES_PER_BLOCK );

	/* Pbn of RO_AREA_START for each bank = Vbn of RO_AREA_START */
	nAreaStartPB = RO_AREA_START; 	/* don't use pstROAreaMeta->nRoAreaStart */
	nPatOffsetPB = pstROAreaMeta->aPatTbl[nPartNo].nOffset;

	*pBank = (nPatOffsetPB + nLpn / PAGES_PER_BLOCK) % BANKS_TOTAL;
	*pPbn  = (nPatOffsetPB + nLpn / PAGES_PER_BLOCK) / BANKS_TOTAL + nAreaStartPB;
	*pPOffset = nLpn % PAGES_PER_BLOCK;

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --_GetROPhysicalAddr(nPartNo:%d,nLpn:%d)\r\n"), nPartNo, nLpn));

	return;
}
#endif

/*****************************************************************************/
/*                                            								 */
/* NAME                                        								 */
/*      _ConfigureROAreaMeta               							         */
/* DESCRIPTION                                                               */
/*      This function make RO Area Meta (PartitionTbl) using User Constants. */
/*                 - make ROAreaMeta struture                                */
/* PARAMETERS                                                                */
/* RETURN VALUES                                                             */
/*		TRUE32																 */
/*				_ConfigureROAreaMeta is completed							 */
/*		FALSE32																 */
/*				_ConfigureROAreaMeta is failed								 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
#if (WMR_RO_AREA_SUPPORT)
PRIVATE BOOL32 
_ConfigureROAreaMeta(VOID)
{	
	INT32	nIdx;
	INT32	nLastOffset;
	
	VFL_LOG_PRINT((TEXT("[VFL: IN] ++_ConfigureROAreaMeta()\r\n")));

	/* 1. Get RO Area Meta from WMR config Parameters */
	pstROAreaMeta->nRoAreaSizePB = WMR_RO_AREA_PB;
	pstROAreaMeta->nRoAreaSizeVB = RO_AREA_SIZE;
	pstROAreaMeta->nRoAreaStart  = RO_AREA_START;

	/* 2. Get RO Partition Table from WMR config Parameters */
	pstROAreaMeta->aPatTbl[0].nSizePB = WMR_RO_PAT0_PB;
	pstROAreaMeta->aPatTbl[1].nSizePB = WMR_RO_PAT1_PB;
	pstROAreaMeta->aPatTbl[2].nSizePB = WMR_RO_PAT2_PB;
	pstROAreaMeta->aPatTbl[3].nSizePB = WMR_RO_PAT3_PB;
	pstROAreaMeta->aPatTbl[4].nSizePB = WMR_RO_PAT4_PB;
	pstROAreaMeta->aPatTbl[5].nSizePB = WMR_RO_PAT5_PB;
	pstROAreaMeta->aPatTbl[6].nSizePB = WMR_RO_PAT6_PB;
	pstROAreaMeta->nRoPartNum = 0;

	/* 3. Calc the start PB number (Offset) of each Partition */
	/* 		start is 2st PB because 1st PB is used to store ROAreaMeta */
	
	nLastOffset = 1;	
	
	for ( nIdx=0; nIdx < MAX_RO_PARTS; nIdx++ )
	{
		pstROAreaMeta->aPatTbl[nIdx].nOffset = nLastOffset;
		pstROAreaMeta->aPatTbl[nIdx].bRWFlag = FALSE32;	/* Read-Only Partition */
			
		nLastOffset += pstROAreaMeta->aPatTbl[nIdx].nSizePB;

		if ( pstROAreaMeta->aPatTbl[nIdx].nSizePB > 0 )
		{
			pstROAreaMeta->nRoPartNum++;
		}		
	}	

	/* 4. check validation for RO Area Meta */
	if ( nLastOffset < 0 || nLastOffset - 1 > (INT32)pstROAreaMeta->nRoAreaSizePB )
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  nRoAreaSizePB(%d) < SUM(nRoPartPB:%d).\r\n"), 
							pstROAreaMeta->nRoAreaSizePB, nLastOffset-1 ));
		return FALSE32;
	}		

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --_ConfigureROAreaMeta()\r\n")));

	return TRUE32;
}
#endif

/*****************************************************************************/
/*                                            								 */
/* NAME                                        								 */
/*      _LoadROAreaMeta                    							         */
/* DESCRIPTION                                                               */
/*      This function loads RO Area Meta (PartitionTbl) from RO area.		 */
/* PARAMETERS                                                                */
/*      bCmpPatTbl  [IN]	whether Configured PatTbl (WMRConfig.h) is       */
/*                                    compared with saved PatTbl under NAND  */
/* RETURN VALUES                                                             */
/*		TRUE32																 */
/*				_LoadROAreaMeta is completed								 */
/*		FALSE32																 */
/*				_LoadROAreaMeta is failed									 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
#if (WMR_RO_AREA_SUPPORT)
PRIVATE BOOL32 
_LoadROAreaMeta(BOOL32 bCmpPatTbl)
{	
	ROAreaMeta *pROAreaMetaSaved = NULL;
	INT32	nIdx;
	
	Buffer	*pBuf = NULL;	
	INT32	 nFILRet;
//	BOOL32	 bRet;

	UINT32	nBank, nPbn, nPOffset;
	UINT32	nPpnRemapped, nLPbnRemapped, nRPbnRemapped;

	VFL_LOG_PRINT((TEXT("[VFL: IN] ++_LoadROAreaMeta()\r\n")));

	/*************************************************************************/
	/* read ROAreaMeta struture from NAND                                    */
	/*************************************************************************/

	/* 1. calculate physical address		*/
	nBank = 0;
	nPbn  = RO_AREA_START;
	nPOffset = 0;
	
	/* 2. check previous write operation	*/
	/*  - tinyWhimory don't have write or erase operation */
	/*
	bRet = _DoSync(nBank);

	if (bRet == FALSE32)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  _DoSync(nBank:%d) fail!\r\n"), nBank));
		return FALSE32;
	}
	*/
	
	/* 3. get remapped block address from VFL context and remapped page address*/	
	_RemapBadBlk(nBank, nPbn, &nLPbnRemapped, &nRPbnRemapped);

	if(TWO_PLANE_PROGRAM==TRUE32)
	{
		nPpnRemapped = GET_Ppn((nLPbnRemapped>>1), nPOffset);
	}
	else
	{
		nPpnRemapped = GET_Ppn(nLPbnRemapped, nPOffset);
	}

	/* 4. get buffer */
	pBuf = _GetBuffer(BUF_MAIN_AND_SPARE);

	if (pBuf == NULL)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  _GetBuffer fail.\r\n")));

		return FALSE32;
	}	

	/* 5. read saved ROAreaMeta from NAND */
	nFILRet = pFuncTbl->Read(nBank, nPpnRemapped, 0x1, enuLEFT_PLANE_BITMAP, 
								pBuf->pData, NULL, FALSE32, FALSE32);

	if (nFILRet == FIL_SUCCESS_CLEAN)
	{
		BUF_Release(pBuf);
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  Not Formatted!\r\n")));
		return FALSE32;
	}		
	else if (nFILRet != FIL_SUCCESS)
	{
		BUF_Release(pBuf);
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  pFuncTbl->Read fail!\r\n")));
		return FALSE32;
	}		

	/*************************************************************************/
	/* compared saved ROAreaMeta with configured ROAreaMeta                  */
	/*************************************************************************/
	pROAreaMetaSaved = (ROAreaMeta *)(pBuf->pData);

	/* Start and Size of RO_AREA should be matched							*/
	/*			between configuration of user and saved one under NAND		*/
	if ( pstROAreaMeta->nRoAreaStart  != pROAreaMetaSaved->nRoAreaStart  ||
	     pstROAreaMeta->nRoAreaSizePB != pROAreaMetaSaved->nRoAreaSizePB ||
		 pstROAreaMeta->nRoAreaSizeVB != pROAreaMetaSaved->nRoAreaSizeVB )
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  unmatch pstROAreaMeta fail!\r\n")));

		BUF_Release(pBuf);
		return FALSE32;
	}

	if (bCmpPatTbl) /* compare */
	{
		if ( pstROAreaMeta->nRoPartNum != pROAreaMetaSaved->nRoPartNum )
		{
			VFL_ERR_PRINT((TEXT("[VFL:ERR]  unmatch pstROAreaMeta fail!\r\n")));

			BUF_Release(pBuf);
			return FALSE32;
		}

		for ( nIdx=0; nIdx < MAX_RO_PARTS; nIdx++ )
		{
			if ( pstROAreaMeta->aPatTbl[nIdx].nOffset != pROAreaMetaSaved->aPatTbl[nIdx].nOffset ||
				 pstROAreaMeta->aPatTbl[nIdx].nSizePB != pROAreaMetaSaved->aPatTbl[nIdx].nSizePB )
			{
				VFL_ERR_PRINT((TEXT("[VFL:ERR]  unmatch pstROAreaMeta fail!\r\n")));

				BUF_Release(pBuf);
				return FALSE32;
			}
		}
	}
	else /* overwrite */
	{
		pstROAreaMeta->nRoPartNum =pROAreaMetaSaved->nRoPartNum;

		for ( nIdx=0; nIdx < MAX_RO_PARTS; nIdx++ )
		{
			pstROAreaMeta->aPatTbl[nIdx].nOffset = pROAreaMetaSaved->aPatTbl[nIdx].nOffset;
			pstROAreaMeta->aPatTbl[nIdx].nSizePB = pROAreaMetaSaved->aPatTbl[nIdx].nSizePB;
		}
	}

	/* release buffer */
	BUF_Release(pBuf);
	
	VFL_LOG_PRINT((TEXT("[VFL:OUT] --_LoadROAreaMeta()\r\n")));

	return TRUE32;
}
#endif

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
	UINT32 nBusyBank;

	do{
		pBuf = BUF_Get( eTypeT32, &nBusyBank);

		if ( pBuf == NULL )
		{
			/*  - tinyWhimory don't have write or erase operation */
			/*
			_DoSync(nBusyBank);
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
/*      _GetVFLSpare                                                         */
/* DESCRIPTION                                                               */
/*      This function returns VFL context info from spare area.			     */
/* PARAMETERS                                                                */
/*      *pAge    [OUT] 	the pointer of context age                           */
/*      *pStatus [OUT] 	the pointer of status mark                           */
/*      *pSBu    [IN] 	the pointer of spare buffer                          */
/* RETURN VALUES                                                             */
/*		none																 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
PRIVATE INLINE VOID
_GetVFLSpare(UINT32 *pAge, UINT8 *pStatus, UINT8 *pSBuf)
{
	VFLSpare *pSpare = (VFLSpare *)pSBuf;
	if (pAge != NULL)		
		*pAge = pSpare->nCxtAge;
	if (pStatus != NULL)
		*pStatus = pSpare->cStatusMark;
}


/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      _LoadVFLCxt                                                          */
/* DESCRIPTION                                                               */
/*      This function loads VFL context from VFL area.					     */
/* PARAMETERS                                                                */
/*      nBank   bank number                                                  */
/* RETURN VALUES                                                             */
/*		TRUE32																 */
/*				_LoadVFLCxt is completed									 */
/*		FALSE32																 */
/*				_LoadVFLCxt is failed										 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
PRIVATE BOOL32 
_LoadVFLCxt(UINT32 nBank)
{
	VFLCxt	*pVFLCxt;
	UINT32 	 nBlkIdx;
	Buffer	*pBuf = NULL;
	UINT32	 nMinAge;
	UINT32	 nNewCxt = 0;
	INT32	 nFILRet;
	
	VFL_LOG_PRINT((TEXT("[VFL: IN] ++_LoadVFLCxt(nBank:%d)\r\n"), nBank));
	
	/* check current state			*/
	if (nBank >= BANKS_TOTAL)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  _LoadVFLCxt(nBank:%d) nBank Overflow!\r\n"), nBank));

		return FALSE32;
	}
	
	pVFLCxt = GET_VFLCxt(nBank);

	pBuf = _GetBuffer(BUF_MAIN_AND_SPARE);

	if (pBuf == NULL)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  _GetBuffer(nBank:%d) fail.\r\n"), nBank));

		return FALSE32;
	}	
	
	/* read bad info block list from last physical page - 2 */
	nFILRet = pFuncTbl->Read(nBank, WMR_BADTBL_VFLCXT_POFFSET, 0x1, enuLEFT_PLANE_BITMAP, pBuf->pData, NULL, FALSE32, FALSE32);

	if (nFILRet != FIL_SUCCESS)
	{
		BUF_Release(pBuf);
		return FALSE32;
	}

	WMR_MEMCPY((UINT8 *) pVFLCxt->aBadInfoBlk, pBuf->pData, sizeof(pVFLCxt->aBadInfoBlk));

	/* VFL block scan start			*/	
	nMinAge = 0xFFFFFFFF;	

	for (nBlkIdx = 0; nBlkIdx < VFL_INFO_SECTION_SIZE; nBlkIdx++)
	{
		UINT32 nPpn;
		UINT32 nAge;
		UINT8  cStatusMark;				

		if (pVFLCxt->aBadInfoBlk[nBlkIdx] == TRUE32)
			continue;
	
		nPpn = GET_Ppn(nBlkIdx + VFL_INFO_SECTION_START, VFL_CXT_POFFSET);
				
		/*VFL context write only left plane and 4 sector*/
		nFILRet = pFuncTbl->Read(nBank, nPpn, 0xF, 
								enuLEFT_PLANE_BITMAP, pBuf->pData, pBuf->pSpare, FALSE32, FALSE32);

		/* if this block was a run time bad, then this block should be marked as bad, */
		/* and uncorrectable ecc error should occur, so skip this block 			  */
		if (nFILRet == FIL_U_ECC_ERROR)
			continue;

		if (nFILRet != FIL_SUCCESS)
		{
			BUF_Release(pBuf);
			return FALSE32;
		}

		_GetVFLSpare(&nAge, &cStatusMark, pBuf->pSpare);
		
		#if (WMR_SLC_SUPPORT)
		if (IS_SLC)
		{
			/* if the status mark is 0xFFFE for example		   */
			/* , then whimory think that it is 1bit read error */
			/* so ignore that case							   */			
			if (cStatusMark != PAGE_INCOMPLETE && WMR_GetChkSum((UINT8 *)&cStatusMark, 1) > 1)
			{
				if (nMinAge >= nAge && nAge != 0)
				{
					nNewCxt = nBlkIdx;
					nMinAge = nAge;
				}
			}
			else
			{
				continue;
			}
		}
		#endif
		
		
		#if (WMR_MLC_SUPPORT)
		if (IS_MLC)
		{
			if (cStatusMark == PAGE_VALID)
			{
				if (nMinAge >= nAge && nAge != 0)
				{
					nNewCxt = nBlkIdx;
					nMinAge = nAge;
				}
			}
			else
			{
				continue;
			}
		}
		#endif		
	}

	nFILRet = pFuncTbl->Read(nBank, GET_Ppn(nNewCxt + VFL_INFO_SECTION_START, VFL_CXT_POFFSET), 
							 0xF, enuLEFT_PLANE_BITMAP, pBuf->pData, NULL, FALSE32, FALSE32);

	if (nFILRet != FIL_SUCCESS)
	{
		BUF_Release(pBuf);
		return FALSE32;
	}

	WMR_MEMCPY((UINT8 *)pVFLCxt, pBuf->pData, sizeof(VFLCxt));
	
	pVFLCxt->nCxtLocation = nNewCxt;

    #if (WMR_SUPPORT_META_WEAR_LEVEL)
    if (pVFLCxt->nGlobalCxtAge > gMaxGlobalAge)
    {
        gMaxGlobalAge = pVFLCxt->nGlobalCxtAge;
    }
    #endif

	BUF_Release(pBuf);

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --_LoadVFLCxt(nBank:%d)\r\n"), nBank));
	
	return TRUE32;

}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      _RemapBadBlk                                                         */
/* DESCRIPTION                                                               */
/*      This function remaps the block address if it is bad block. 			 */
/* PARAMETERS                                                                */
/*      nBank    [IN] 	bank number					                         */
/*      nSrcPbn  [IN] 	the physical source block address 		             */
/*      *pDesLPbn[OUT] 	the pointer of the remapped good block address       */
/*      *pDesRPbn[OUT] 	the pointer of the remapped good block address       */
/* RETURN VALUES                                                             */
/*		TRUE32																 */
/*				_RemapBadBlk is completed									 */
/*		FALSE32																 */
/*				_RemapBadBlk is failed										 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
PRIVATE BOOL32
_RemapBadBlk(UINT32 nBank, UINT32 nSrcPbn, UINT32 *pDesLPbn, UINT32 *pDesRPbn)
{
	VFLCxt *pVFLCxt;
	INT32  nIdx;
    UINT32  nByteOffset;
    UINT32  nBitOffset;
    UINT8   cValue;

	VFL_LOG_PRINT((TEXT("[VFL: IN] ++_RemapBadBlk(nBank:%d, nSrcPbn:%d)\r\n"), nBank, nSrcPbn));

	pVFLCxt = GET_VFLCxt(nBank);

    /* check nSrcPbn block is bad block or not */
    if (TWO_PLANE_PROGRAM == TRUE32)
    {
        BOOL32 bLeftRemap = FALSE32;
        BOOL32 bRightRemap = FALSE32;

        /* check the block is bad or not */
        nByteOffset = (nSrcPbn * 2 / BAD_MARK_COMPRESS_SIZE) / 8;
        nBitOffset = 7 - (nSrcPbn * 2 / BAD_MARK_COMPRESS_SIZE) % 8;

        cValue = *(pVFLCxt->aBadMark + nByteOffset);

        if (!((cValue >> nBitOffset) & 0x1))
        {
			for (nIdx = 0; nIdx < (pVFLCxt->nBadMapTableMaxIdx * 2); nIdx++)
			{
				if (pVFLCxt->aBadMapTable[nIdx] == (UINT16)(nSrcPbn * 2))
				{
					*pDesLPbn = RESERVED_SECTION_START * 2 + nIdx;
					bLeftRemap = TRUE32;
				}
				else if (pVFLCxt->aBadMapTable[nIdx] == (UINT16)(nSrcPbn * 2 + 1))
				{
					*pDesRPbn = RESERVED_SECTION_START * 2 + nIdx;
					bRightRemap = TRUE32;
				}

				if (bLeftRemap == TRUE32 && bRightRemap == TRUE32)
				{
					break;
				}
			}
        }

        if (bLeftRemap == FALSE32)
        {
            *pDesLPbn = nSrcPbn * 2;
        }

        if (bRightRemap == FALSE32)
        {
            *pDesRPbn = nSrcPbn * 2 + 1;
        }

		WMR_ASSERT(*pDesLPbn < (BLOCKS_PER_BANK * 2));
		WMR_ASSERT(*pDesRPbn < (BLOCKS_PER_BANK * 2));
    }
    else
    {
        BOOL32 bRemap = FALSE32;

        /* check the block is bad or not */
        nByteOffset = nSrcPbn / BAD_MARK_COMPRESS_SIZE / 8;
        nBitOffset = 7 - (nSrcPbn / BAD_MARK_COMPRESS_SIZE) % 8;

        cValue = *(pVFLCxt->aBadMark + nByteOffset);

        if (!((cValue >> nBitOffset) & 0x1))
        {
            for (nIdx = 0; nIdx < pVFLCxt->nBadMapTableMaxIdx; nIdx++)
            {
                if (pVFLCxt->aBadMapTable[nIdx] == (UINT16)nSrcPbn)
                {
                    *pDesLPbn = RESERVED_SECTION_START + nIdx;
                    bRemap = TRUE32;
                    break;
                }
            }
        }

        if (bRemap == FALSE32)
        {
            *pDesLPbn = nSrcPbn;
        }

		WMR_ASSERT(*pDesLPbn < BLOCKS_PER_BANK);
    }

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --_RemapBadBlk(nBank:%d, nSrcPbn:%d)\r\n"), nBank, nSrcPbn));

	return TRUE32;
}


/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      _GetPhysicalAddr	                                                 */
/* DESCRIPTION                                                               */
/*      This function returns the physical bank, block & page address of	 */
/*		the virtual page address											 */
/* PARAMETERS                                                                */
/*      nVpn     [IN] 	virtual page number			                         */
/*      pBank    [OUT] 	physical bank number		                         */
/*      pPbn     [OUT] 	physical block number		                         */
/*      pPOffset [OUT] 	physical page offset		                         */
/* RETURN VALUES                                                             */
/*		none																 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
PRIVATE VOID
_GetPhysicalAddr(UINT32 nVpn, UINT32 *pBank, UINT32 *pPbn, UINT32 *pPOffset)
{
	VFL_LOG_PRINT((TEXT("[VFL: IN] ++_GetPhysicalAddr(nVpn:%d)\r\n"), nVpn));
	
	WMR_ASSERT(nVpn < PAGES_TOTAL);
	WMR_ASSERT(nVpn >= PAGES_PER_SUBLK);

	if (IS_SUPPORT_INTERLEAVING)
	{
		#if (WMR_STDLIB_SUPPORT)
		*pBank = nVpn % BANKS_TOTAL;
		*pPbn = nVpn / PAGES_PER_SUBLK;
		*pPOffset = (nVpn / BANKS_TOTAL) % PAGES_PER_BLOCK;
		#else
		*pBank = REM(nVpn, BANKS_TOTAL_SHIFT);
		*pPbn = DIV(nVpn, PAGES_PER_SUBLK_SHIFT);
		*pPOffset = REM(DIV(nVpn, BANKS_TOTAL_SHIFT), PAGES_PER_BLOCK_SHIFT);
		#endif

	}
	else
	{	
		#if (WMR_STDLIB_SUPPORT)
		*pBank = nVpn / PAGES_PER_BANK;
		*pPbn = (nVpn / PAGES_PER_BLOCK) % BLOCKS_PER_BANK;
		*pPOffset = nVpn % PAGES_PER_BLOCK;
		#else
		*pBank = DIV(nVpn, PAGES_PER_BANK_SHIFT);
		*pPbn = REM(DIV(nVpn, PAGES_PER_BLOCK_SHIFT), BLOCKS_PER_BANK_SHIFT);
		*pPOffset = REM(nVpn, PAGES_PER_BLOCK_SHIFT);
		#endif
	}

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --_GetPhysicalAddr(nVpn:%d)\r\n"), nVpn));
	return;
}
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      VFL_Init                                                             */
/* DESCRIPTION                                                               */
/*      This function initializes VFL layer.							     */
/* PARAMETERS                                                                */
/*      none			                                                     */
/* RETURN VALUES                                                             */
/* 		VFL_SUCCESS                                                          */
/*            VFL_Init is completed.                                         */
/*      VFL_CRITICAL_ERROR                                                   */
/*            VFL_Init is failed    		                                 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32
VFL_Init(VOID)
{
	UINT32 nBankIdx;
	INT32  nRet;
	BOOL32 bRet;
	
	/* semaphore */
	WMR_CreateSM (WMR_SM_BML);
	WMR_AcquireSM(WMR_SM_BML);

	VFL_LOG_PRINT((TEXT("[VFL: IN] ++VFL_Init()\r\n")));

	/* init buffer manager			*/
	nRet = BUF_Init();

	if (nRet != BUF_SUCCESS)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR] BUF_Init error occurs!\r\n")));	

		WMR_ReleaseSM(WMR_SM_BML);
		WMR_DestroySM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}

    /* the size of VFLMeta must be 2048 bytes */
    WMR_ASSERT(sizeof(VFLMeta) == 2048);
	
	/* init VFL meta				*/
	if (pstVFLMeta == NULL)
	{
		pstVFLMeta = (VFLMeta *)WMR_MALLOC(sizeof(VFLMeta) * BANKS_TOTAL);

		if (pstVFLMeta == NULL)
		{
			VFL_ERR_PRINT((TEXT("[VFL:ERR]  there is no memory to initialize pstVFLCxt!\r\n")));	

			WMR_ReleaseSM(WMR_SM_BML);
			WMR_DestroySM(WMR_SM_BML);
			return VFL_CRITICAL_ERROR;
		}
	}

	/* init async operation manager	*/
	if (pstAsyncOp == NULL)
	{
		pstAsyncOp = (AsyncOp *)WMR_MALLOC(sizeof(AsyncOp) * BANKS_TOTAL);

		if (pstAsyncOp == NULL)
		{
			VFL_ERR_PRINT((TEXT("[VFL:ERR]  there is no memory to initialize pstAsyncOp!\r\n")));	

			WMR_ReleaseSM(WMR_SM_BML);
			WMR_DestroySM(WMR_SM_BML);
			return VFL_CRITICAL_ERROR;
		}
	}

    /* init VFL spare */
    if (pVFLSpare == NULL)
    {
        pVFLSpare = WMR_MALLOC(BYTES_PER_SPARE_SUPAGE);

        if (pVFLSpare == NULL)
        {
			VFL_ERR_PRINT((TEXT("[VFL:ERR]  there is no memory to initialize pVFLSpare!\r\n")));	

			WMR_ReleaseSM(WMR_SM_BML);
			WMR_DestroySM(WMR_SM_BML);
			return VFL_CRITICAL_ERROR;
        }
    }

	for (nBankIdx = 0; nBankIdx < BANKS_TOTAL; nBankIdx++)
	{
		AsyncOp *pAsyncOp = GET_AsyncOp(nBankIdx);
		pAsyncOp->eOpType = FLASH_OPERATION_NONE;
		pAsyncOp->pBuf = NULL;
	}

	/* get low level function table	*/
	pFuncTbl = FIL_GetFuncTbl();

	if (pFuncTbl == NULL)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  FIL_GetFuncTbl() fail!\r\n")));	

		WMR_ReleaseSM(WMR_SM_BML);
		WMR_DestroySM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}

	#if (WMR_RO_AREA_SUPPORT)	
	/* init RO AREA Meta */
	bRet = _ROInit();
	
	if (bRet == FALSE32)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  there is no memory _ROInit!\r\n")));	

		WMR_ReleaseSM(WMR_SM_BML);
		WMR_DestroySM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}
	#endif

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --VFL_Init()\r\n")));

	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BML);

	return VFL_SUCCESS;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      VFL_Open                                                             */
/* DESCRIPTION                                                               */
/*      This function opens VFL layer.									     */
/* PARAMETERS                                                                */
/*      none			                                                     */
/* RETURN VALUES                                                             */
/* 		VFL_SUCCESS                                                          */
/*            VFL_Open is completed.                                         */
/*      VFL_CRITICAL_ERROR                                                   */
/*            VFL_Open is failed    		                                 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32
VFL_Open(VOID)
{
	UINT32 nBankIdx;
	BOOL32 bRet;
	
	/* semaphore */
	WMR_AcquireSM(WMR_SM_BML);

	VFL_LOG_PRINT((TEXT("[VFL: IN] ++VFL_Open()\r\n")));

	for (nBankIdx = 0; nBankIdx < BANKS_TOTAL; nBankIdx++)
	{
		bRet = _LoadVFLCxt(nBankIdx);

		if (bRet == FALSE32)
		{
			VFL_ERR_PRINT((TEXT("[VFL:ERR]  _LoadVFLCxt(nBank:%d) fail!\r\n"), nBankIdx));	

			WMR_ReleaseSM(WMR_SM_BML);
			return VFL_CRITICAL_ERROR;
		}
	}

	#if (WMR_RO_AREA_SUPPORT )
	bRet = _ROOpen();
	
	if (bRet == FALSE32)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  _ROOpen() fail!\r\n")));	

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}
	#endif
	
	VFL_LOG_PRINT((TEXT("[VFL:OUT] --VFL_Open()\r\n")));

	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BML);

	return VFL_SUCCESS;

}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      VFL_Read                                                             */
/* DESCRIPTION                                                               */
/*      This function reads virtual page.								     */
/* PARAMETERS                                                                */
/*      nVpn		[IN]	virtual page number		                         */
/*      pBuf		[IN]	Buffer pointer			                         */
/*      bCleanCheck	[IN]	clean check or not		                         */
/* RETURN VALUES                                                             */
/* 		VFL_SUCCESS                                                          */
/*            VFL_Read is completed.                                         */
/*      VFL_CRITICAL_ERROR                                                   */
/*            VFL_Read is failed.    		                                 */
/*		VFL_U_ECC_ERROR														 */
/*			  ECC uncorrectable error occurs from FIL read function.		 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32
VFL_Read(UINT32 nVpn, Buffer *pBuf, BOOL32 bCleanCheck)
{
	UINT32	nBank;
	UINT32	nPbn;
	UINT32	nPOffset;
    UINT32	nLPbnRemapped;
    UINT32  nRPbnRemapped;
	UINT32	nPpnRemapped;
//	BOOL32	bRet;
	INT32	nFILRet;
#if (WMR_PROGRAM_PER_PLANE)
    BOOL32  bClean = TRUE32;
#endif

	
	/* semaphore */
	WMR_AcquireSM(WMR_SM_BML);

	VFL_LOG_PRINT((TEXT("[VFL: IN] ++VFL_Read(nVpn:%d,bCleanCheck:%d)\r\n"), nVpn, bCleanCheck));
	
	/* check virtual page address	*/
	if (nVpn >= PAGES_TOTAL || nVpn < PAGES_PER_SUBLK)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  VFL_Read(nVpn:%d) nVpn Overflow!\r\n"), nVpn));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}
	
	/* calculate physical address		*/
	_GetPhysicalAddr(nVpn, &nBank, &nPbn, &nPOffset);

	/* check previous write operation	*/
	/*  - tinyWhimory don't have write or erase operation */
	/*
	bRet = _DoSync(nBank);
	
	if (bRet == FALSE32)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  _DoSync(nBank:%d) fail!\r\n"), nBank));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}
	*/
	
	/* get remapped block address from VFL context	*/	
    _RemapBadBlk(nBank, nPbn, &nLPbnRemapped, &nRPbnRemapped);

    if (TWO_PLANE_PROGRAM == TRUE32)
    {
        /* same plane */
        if ((nRPbnRemapped > nLPbnRemapped) && ((nLPbnRemapped >> 1) == (nRPbnRemapped >> 1)))
        {
            #if (WMR_PROGRAM_PER_PLANE)        
            UINT32  nPlaneBitmap = enuNONE_PLANE_BITMAP;
            UINT32  nSectorBitmap = 0;
            UINT8   *pDataPtr = pBuf->pData;
            UINT8   *pSparePtr = pBuf->pSpare;
            #endif
            
            #if (WMR_PROGRAM_PER_PLANE)    
            if (PROGRAM_PER_PLANE == TRUE32)
            {
                pBuf->nBitmap &= (UINT32)FULL_SECTOR_BITMAP_PAGE;
                nSectorBitmap = pBuf->nBitmap;
    
                if(pBuf->nBitmap == (UINT32)LEFT_SECTOR_BITMAP_PAGE)
                {
                    nPlaneBitmap = enuLEFT_PLANE_BITMAP;
                }
                else if (pBuf->nBitmap == (UINT32)RIGHT_SECTOR_BITMAP_PAGE)
                {
                    nPlaneBitmap = enuRIGHT_PLANE_BITMAP;
                    nSectorBitmap = pBuf->nBitmap >> SECTORS_PER_PAGE;
                    if (pDataPtr != NULL)
                    {
                        pDataPtr += BYTES_PER_MAIN_PAGE;
                    }
                    if (pSparePtr != NULL)
                    {
                        pSparePtr += BYTES_PER_SPARE_PAGE;
                    }
                }
                else
                {
                    nPlaneBitmap = enuBOTH_PLANE_BITMAP;
                }
            }
            #endif 
			
            /* get remapped page address		*/
            nPpnRemapped = GET_Ppn((nLPbnRemapped >> 1), nPOffset);
            #if (WMR_PROGRAM_PER_PLANE)               
            if (PROGRAM_PER_PLANE == TRUE32)
            {
                nFILRet = pFuncTbl->Read(nBank, nPpnRemapped, nSectorBitmap, nPlaneBitmap, pDataPtr, 
                                         pSparePtr, FALSE32, bCleanCheck);
            }
            else
            {
                nFILRet = pFuncTbl->Read(nBank, nPpnRemapped, pBuf->nBitmap, enuBOTH_PLANE_BITMAP, pBuf->pData, 
                                     pBuf->pSpare, FALSE32, bCleanCheck);
            }
            #else
            nFILRet = pFuncTbl->Read(nBank, nPpnRemapped, pBuf->nBitmap, enuBOTH_PLANE_BITMAP, pBuf->pData, 
                                     pBuf->pSpare, FALSE32, bCleanCheck);
            #endif

#if (WMR_PROGRAM_PER_PLANE)
            if (PROGRAM_PER_PLANE == TRUE32)
            {
                if (nFILRet != FIL_SUCCESS_CLEAN)
                {
                    bClean = FALSE32;
                }
            }
#endif
        }
        /* different plane */
        else
        {
            nFILRet = FIL_SUCCESS;
            
            if ((pBuf->nBitmap & LEFT_SECTOR_BITMAP_PAGE) != 0)
            {
                /* get remapped page address		*/
                nPpnRemapped = GET_Ppn((nLPbnRemapped >> 1), nPOffset);

                nFILRet = pFuncTbl->Read(nBank, nPpnRemapped, pBuf->nBitmap, GET_PlaneBitmap(nLPbnRemapped), 
										pBuf->pData, pBuf->pSpare, FALSE32, bCleanCheck);
#if (WMR_PROGRAM_PER_PLANE)            
                if (PROGRAM_PER_PLANE == TRUE32)
                {
                    if (nFILRet != FIL_SUCCESS_CLEAN)
                    {
                        bClean = FALSE32;
                    }

					if (WMR_RETURN_MAJOR(nFILRet) > 0)
					{
						VFL_ERR_PRINT((TEXT("[VFL:ERR]  Read(nBank:%d) fail!\r\n"), nBank));
	
						WMR_ReleaseSM(WMR_SM_BML);
						return nFILRet;
					}	
                }
#endif
            }

            if ((pBuf->nBitmap & RIGHT_SECTOR_BITMAP_PAGE) != 0)
            {
				UINT8 *pDataPtr = pBuf->pData;
#if (WMR_PROGRAM_PER_PLANE)        
                UINT8 *pSparePtr = pBuf->pSpare;
#endif
                /* get remapped page address		*/
                nPpnRemapped = GET_Ppn((nRPbnRemapped >> 1), nPOffset);

				if (pDataPtr != NULL)
				{
					pDataPtr += BYTES_PER_MAIN_PAGE;
				}
#if (WMR_PROGRAM_PER_PLANE)
                if (PROGRAM_PER_PLANE == TRUE32)
                {
                    if (pSparePtr != NULL)
                    {
                        pSparePtr += BYTES_PER_SPARE_PAGE;
                    }
                }
#endif

#if (WMR_PROGRAM_PER_PLANE)
                if (PROGRAM_PER_PLANE == TRUE32)
                {
				nFILRet = pFuncTbl->Read(nBank, nPpnRemapped, (pBuf->nBitmap >> SECTORS_PER_PAGE), GET_PlaneBitmap(nRPbnRemapped), 
										pDataPtr, pSparePtr, FALSE32, bCleanCheck);
                }
                else
                {
    				nFILRet |= pFuncTbl->Read(nBank, nPpnRemapped, (pBuf->nBitmap >> SECTORS_PER_PAGE), GET_PlaneBitmap(nRPbnRemapped), 
										pDataPtr, pBuf->pSpare, FALSE32, bCleanCheck);

                }
#else
				nFILRet |= pFuncTbl->Read(nBank, nPpnRemapped, (pBuf->nBitmap >> SECTORS_PER_PAGE), GET_PlaneBitmap(nRPbnRemapped), 
										pDataPtr, pBuf->pSpare, FALSE32, bCleanCheck);
#endif

#if (WMR_PROGRAM_PER_PLANE)            
                if (PROGRAM_PER_PLANE == TRUE32)
                {
                    if (nFILRet != FIL_SUCCESS_CLEAN)
                    {
                        bClean = FALSE32;
                    }
                }
#endif
            }
        }
    }
    else
    {
	    /* get remapped page address		*/
	    nPpnRemapped = GET_Ppn(nLPbnRemapped, nPOffset);

        nFILRet = pFuncTbl->Read(nBank, nPpnRemapped, pBuf->nBitmap, enuLEFT_PLANE_BITMAP, pBuf->pData, 
                                 pBuf->pSpare, FALSE32, bCleanCheck);
    }
    
	if (WMR_RETURN_MAJOR(nFILRet) > 0)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  Read(nBank:%d) fail!\r\n"), nBank));

		WMR_ReleaseSM(WMR_SM_BML);
		return nFILRet;
	}	

#if (WMR_PROGRAM_PER_PLANE)
    if (PROGRAM_PER_PLANE == TRUE32)
    {
        if (bClean == TRUE32)
        {
            nFILRet = FIL_SUCCESS_CLEAN;
        }
        else
        {
            nFILRet = FIL_SUCCESS;
        }
    }
#endif
	VFL_LOG_PRINT((TEXT("[VFL:OUT] --VFL_Read(nVpn:%d,bCleanCheck:%d)\r\n"), nVpn, bCleanCheck));
	
	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BML);

	return nFILRet;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      VFL_Close                                                            */
/* DESCRIPTION                                                               */
/*      This function releases VFL layer.								     */
/* PARAMETERS                                                                */
/*      none			                                                     */
/* RETURN VALUES                                                             */
/*      none			                                                     */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
VOID
VFL_Close(VOID)
{
	
	VFL_LOG_PRINT((TEXT("[VFL: IN] ++VFL_Close()\r\n")));
	
	/* semaphore */
	WMR_AcquireSM(WMR_SM_BML);

	/* release buffer manager			*/
	BUF_Close();
	
	/* release VFL meta				*/
	if (pstVFLMeta != NULL)
	{
		WMR_FREE(pstVFLMeta);

		pstVFLMeta = NULL;
	}

	/* release async operation manager	*/
	if (pstAsyncOp != NULL)	
	{
		WMR_FREE(pstAsyncOp);
		
		pstAsyncOp = NULL;
	}

    /* release spare buffer */
    if (pVFLSpare != NULL)
    {
        WMR_FREE(pVFLSpare);

        pVFLSpare = NULL;
    }

	pFuncTbl = NULL;

	#if (WMR_RO_AREA_SUPPORT )
	_ROClose();
	#endif
	
	VFL_LOG_PRINT((TEXT("[VFL:OUT] --VFL_Close()\r\n")));

	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BML);
	WMR_DestroySM(WMR_SM_BML);

	return;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      VFL_RO_GetInfo                                                       */
/* DESCRIPTION                                                               */
/*      This function return max pages for partition                         */
/* PARAMETERS                                                                */
/*      nPartNo    [IN]    Partition No                                      */
/*      nMaxPages  [OUT]   Max Page Number                                   */
/* RETURN VALUES                                                             */
/* 		VFL_SUCCESS                                                          */
/*            VFL_RO_GetInfo is completed.                                   */
/*      VFL_CRITICAL_ERROR                                                   */
/*            VFL_RO_GetInfo is failed.                                      */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
#if (WMR_RO_AREA_SUPPORT)
INT32	
VFL_RO_GetInfo (UINT32 nPartNo, UINT32 *nMaxPages)
{
	/* semaphore */
	WMR_AcquireSM(WMR_SM_BML);

	VFL_LOG_PRINT((TEXT("[VFL: IN] ++VFL_RO_GetInfo(PartNo:%d)\r\n"),nPartNo));

	if (nPartNo >= MAX_RO_PARTS)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  VFL_RO_GetInfo(nPartNo:%d) input overflow!\r\n"), nPartNo));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}

	*nMaxPages = pstROAreaMeta->aPatTbl[nPartNo].nSizePB * PAGES_PER_BLOCK;

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --VFL_RO_GetInfo(PartNo:%d)\r\n"),nPartNo));

	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BML);

    return VFL_SUCCESS;
}
#endif

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      VFL_RO_Read                                                          */
/* DESCRIPTION                                                               */
/*      This function read page                                              */
/* PARAMETERS                                                                */
/*      nPartNo  [IN]    Partition No                                        */
/*      nLpn     [IN]    Logical Page No                                     */
/*      pBuf     [IN]    buffer to be read                                   */
/* RETURN VALUES                                                             */
/* 		VFL_SUCCESS                                                          */
/*            VFL_RO_Read is completed.                                      */
/*      VFL_CRITICAL_ERROR                                                   */
/*            VFL_RO_Read is failed.                                         */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
#if (WMR_RO_AREA_SUPPORT)
INT32	
VFL_RO_Read (UINT32 nPartNo, UINT32 nLpn, UINT8 *pBuf)
{
	UINT32	nBank, nPbn, nPOffset;
    UINT32	nLPbnRemapped, nRPbnRemapped, nPpnRemapped;

	Buffer *pInBuf = NULL;
	BOOL32	bCleanCheck = FALSE32;
//	BOOL32	bRet;
	INT32	nFILRet;

	/* semaphore */
	WMR_AcquireSM(WMR_SM_BML);

	VFL_LOG_PRINT((TEXT("[VFL: IN] ++VFL_RO_Read(PartNo:%d,Lpn:%d)\r\n"),nPartNo,nLpn));

	if (nPartNo >= MAX_RO_PARTS)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  VFL_RO_Read(nPartNo:%d) input overflow!\r\n"), nPartNo));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}

	if (nLpn/PAGES_PER_BLOCK >= pstROAreaMeta->aPatTbl[nPartNo].nSizePB)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  VFL_RO_Read(PartNo:%d,Lpn:%d) input overflow!\r\n"), nPartNo, nLpn));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}

	/* calculate physical address		*/
	_GetROPhysicalAddr(nPartNo, nLpn, &nBank, &nPbn, &nPOffset);

	/* check previous write operation	*/
	/*  - tinyWhimory don't have write or erase operation */
	/*
	bRet = _DoSync(nBank);

	if (bRet == FALSE32)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  _DoSync(nBank:%d) fail!\r\n"), nBank));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}
	*/

	/* initialize interal buffer */
	pInBuf = _GetBuffer(BUF_MAIN_ONLY);

	if (pInBuf == NULL)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  there is error on _GetBuffer!\r\n")));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}

	pInBuf->nBitmap = FULL_SECTOR_BITMAP_PAGE;

	/* 4 bytes align check */
	#if (WMR_ALIGN_CHECK)
	if((UINT32)pBuf % 4 == 0)
	{
		pInBuf->pData = pBuf;
	}
	else
	{
		/* nothing to do - use original buffer */
	}
	#else
	pInBuf->pData = pBuf;
	#endif

	/* get remapped block address from VFL context	*/	
    _RemapBadBlk(nBank, nPbn, &nLPbnRemapped, &nRPbnRemapped);

    if (TWO_PLANE_PROGRAM == TRUE32)
    {
        /* same plane */
        if ((nRPbnRemapped > nLPbnRemapped) && ((nLPbnRemapped >> 1) == (nRPbnRemapped >> 1)))
        {
            /* get remapped page address		*/
            nPpnRemapped = GET_Ppn((nLPbnRemapped >> 1), nPOffset);
            
            nFILRet = pFuncTbl->Read( nBank, nPpnRemapped, 
									  pInBuf->nBitmap, enuBOTH_PLANE_BITMAP, 
									  pInBuf->pData, pInBuf->pSpare, 
									  FALSE32, bCleanCheck );
        }
        /* different plane */
        else
        {
			UINT8 *pDataPtr = pInBuf->pData;
            
            /* get remapped page address		*/
            nPpnRemapped = GET_Ppn((nLPbnRemapped >> 1), nPOffset);

            nFILRet = pFuncTbl->Read( nBank, nPpnRemapped, 
									  pInBuf->nBitmap, GET_PlaneBitmap(nLPbnRemapped), 
									  pInBuf->pData, pInBuf->pSpare, 
									  FALSE32, bCleanCheck );

            /* get remapped page address		*/
            nPpnRemapped = GET_Ppn((nRPbnRemapped >> 1), nPOffset);

			if (pDataPtr != NULL)
			{
				pDataPtr += BYTES_PER_MAIN_PAGE;
			}

			nFILRet |= pFuncTbl->Read( nBank, nPpnRemapped, 
									   (pInBuf->nBitmap >> SECTORS_PER_PAGE), GET_PlaneBitmap(nRPbnRemapped), 
									   pDataPtr, pInBuf->pSpare, 
									   FALSE32, bCleanCheck );
        }
    }
    else
    {
	    /* get remapped page address		*/
	    nPpnRemapped = GET_Ppn(nLPbnRemapped, nPOffset);

        nFILRet = pFuncTbl->Read( nBank, nPpnRemapped, 
								  pInBuf->nBitmap, enuLEFT_PLANE_BITMAP, 
								  pInBuf->pData, pInBuf->pSpare, 
								  FALSE32, bCleanCheck );
    }
    
	if (WMR_RETURN_MAJOR(nFILRet) > 0)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  Read(nBank:%d) fail!\r\n"), nBank));

		BUF_Release(pInBuf);
		WMR_ReleaseSM(WMR_SM_BML);
		return nFILRet;
	}	

	#if (WMR_ALIGN_CHECK)
	if ((UINT32)pBuf % 4 > 0)
	{
		WMR_MEMCPY(pBuf, pInBuf->pData, BYTES_PER_SECTOR * SECTORS_PER_SUPAGE);
	}
	#endif

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --VFL_RO_Read(PartNo:%d,Lpn:%d)\r\n"),nPartNo,nLpn));

	BUF_Release(pInBuf);	
	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BML);

    return nFILRet;
}
#endif

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      VFL_RO_ReadPL                                                        */
/* DESCRIPTION                                                               */
/*      This function read physical page                                     */
/* PARAMETERS                                                                */
/*      nPartNo  [IN]    Partition No                                        */
/*      nPPO     [IN]    Physical Page No                                    */
/*      pBuf     [IN]    buffer to be read                                   */
/* RETURN VALUES                                                             */
/* 		VFL_SUCCESS                                                          */
/*            VFL_RO_ReadPL is completed.                                    */
/*      VFL_CRITICAL_ERROR                                                   */
/*            VFL_RO_ReadPL is failed.                                       */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
#if (WMR_RO_AREA_SUPPORT)
INT32	
VFL_RO_ReadPL (UINT32 nPartNo, UINT32 nPPO, UINT8 *pBuf)
{
	UINT32  nLpn;
	UINT32	nBank, nPbn, nPOffset, nPlaneBitmap;
    UINT32	nLPbnRemapped, nRPbnRemapped, nPbnRemapped, nPpnRemapped;

	Buffer *pInBuf = NULL;
	BOOL32	bCleanCheck = FALSE32;
	BOOL32	bRet;
	INT32	nFILRet;

	VFL_LOG_PRINT((TEXT("[VFL: IN] ++VFL_RO_ReadPL(PartNo:%d,Ppo:%d)\r\n"),nPartNo,nPPO));
	
	/* semaphore */
	WMR_AcquireSM(WMR_SM_BML);

    if (TWO_PLANE_PROGRAM != TRUE32)
   	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  VFL_RO_ReadPL isn't supported for ONE_PLANE_NAND !\r\n")));   	

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
   	}

	if (nPartNo >= MAX_RO_PARTS)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  VFL_RO_ReadPL(nPartNo:%d) input overflow!\r\n"), nPartNo));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}

	/* calculate the logical page number and plane */
	nLpn = nPPO >> 1;
	nPlaneBitmap = (nPPO & 0x1) ? enuRIGHT_PLANE_BITMAP : enuLEFT_PLANE_BITMAP;

	if (nLpn/PAGES_PER_BLOCK >= pstROAreaMeta->aPatTbl[nPartNo].nSizePB)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  VFL_RO_ReadPL(PartNo:%d,Lpn:%d) input overflow!\r\n"), nPartNo, nLpn));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}

	/* calculate physical address		*/
	_GetROPhysicalAddr(nPartNo, nLpn, &nBank, &nPbn, &nPOffset);
	
	/* check previous write operation	*/
	/*  - tinyWhimory don't have write or erase operation */
	/*
	bRet = _DoSync(nBank);
	
	if (bRet == FALSE32)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  _DoSync(nBank:%d) fail!\r\n"), nBank));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}
	*/

	/* initialize interal buffer */
	pInBuf = _GetBuffer(BUF_MAIN_ONLY);

	if (pInBuf == NULL)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  there is error on _GetBuffer!\r\n")));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}

	pInBuf->nBitmap = FULL_SECTOR_BITMAP_PAGE;

	/* 4 bytes align check */
	#if (WMR_ALIGN_CHECK)
	if((UINT32)pBuf % 4 == 0)
	{
		pInBuf->pData = pBuf;
	}
	else
	{
		/* nothing to do - use original buffer */
	}
	#else
	pInBuf->pData = pBuf;
	#endif

	/* get remapped block address from VFL context	*/	
    _RemapBadBlk(nBank, nPbn, &nLPbnRemapped, &nRPbnRemapped);

	nPbnRemapped = (nPlaneBitmap == enuLEFT_PLANE_BITMAP) ? nLPbnRemapped : nRPbnRemapped;
	

    /* get remapped page address		*/
    nPpnRemapped = GET_Ppn((nPbnRemapped >> 1), nPOffset);

    nFILRet = pFuncTbl->Read( nBank, nPpnRemapped, 
								  pInBuf->nBitmap, GET_PlaneBitmap(nPbnRemapped), 
								  pInBuf->pData, pInBuf->pSpare, 
								  FALSE32, bCleanCheck );

	if (WMR_RETURN_MAJOR(nFILRet) > 0)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  Read(nBank:%d) fail!\r\n"), nBank));

		BUF_Release(pInBuf);
		WMR_ReleaseSM(WMR_SM_BML);
		return nFILRet;
	}	

	#if (WMR_ALIGN_CHECK)
	if ((UINT32)pBuf % 4 > 0)
	{
		WMR_MEMCPY(pBuf, pInBuf->pData, BYTES_PER_SECTOR * SECTORS_PER_PAGE);
	}
	#endif

	BUF_Release(pInBuf);	
	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BML);

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --VFL_RO_ReadPL(PartNo:%d,Ppo:%d)\r\n"),nPartNo,nPPO));
	
    return nFILRet;
}
#endif

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      VFL_WMRArea_Read                                                     */
/* DESCRIPTION                                                               */
/*      This function read page                                              */
/* PARAMETERS                                                                */
/*      nPpn     [ IN]    Physical Page No                                   */
/*								( Because Pbn of WMRArea is 0,				 */
/*									nPpn become Physical Page No of Bank )	 */
/*      pBuf     [OUT]    buffer to be read                                  */
/* RETURN VALUES                                                             */
/* 		VFL_SUCCESS                                                          */
/*            VFL_WMRArea_Read is completed.                                 */
/*      VFL_CRITICAL_ERROR                                                   */
/*            VFL_WMRArea_Read is failed.                                    */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32	
VFL_WMRArea_Read (UINT32 nPpn, UINT8 *pBuf)
{
	BOOL32	bCleanCheck = FALSE32;
	Buffer *pInBuf = NULL;

//	BOOL32	bRet;
	INT32	nFILRet;

	/* semaphore */
	WMR_AcquireSM(WMR_SM_BML);

	VFL_LOG_PRINT((TEXT("[VFL: IN] ++VFL_WMRArea_Read(Ppn:%d)\r\n"),nPpn));

	/* input parameter check */
	if (nPpn >= WMR_BADTBL_VFLCXT_POFFSET)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  VFL_WMRArea_Read(Ppn:%d) input overflow!\r\n"),nPpn));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}
	
	if (pBuf == NULL)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  pBuf == NULL!\r\n")));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}

	/* check previous write operation	*/	
	/*  - tinyWhimory don't have write or erase operation */
	/* 
	bRet = _DoSync(0); 

	if (bRet == FALSE32)
	{
		VFL_ERR_PRINT((TEXT("[VFL:ERR]  _DoSync(nBank:0) fail!\r\n")));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}
	*/

	/* initialize interal buffer */
	pInBuf = _GetBuffer(BUF_MAIN_ONLY);

	if (pInBuf == NULL)
	{
		BUF_Release(pInBuf);

		VFL_ERR_PRINT((TEXT("[VFL:ERR]  there is error on _GetBuffer!\r\n")));

		WMR_ReleaseSM(WMR_SM_BML);
		return VFL_CRITICAL_ERROR;
	}

	pInBuf->nBitmap = FULL_SECTOR_BITMAP_PAGE;

	/* 4 bytes align check */
	#if (WMR_ALIGN_CHECK)
	if((UINT32)pBuf % 4 == 0)
	{
		pInBuf->pData = pBuf;
	}
	else
	{
		/* nothing to do - use original buffer */
	}
	#else
	pInBuf->pData = pBuf;
	#endif

    /* read page */
    nFILRet = pFuncTbl->Read( 0, nPpn, pInBuf->nBitmap, enuLEFT_PLANE_BITMAP, pInBuf->pData, 
                                 pInBuf->pSpare, FALSE32, bCleanCheck);

	if (WMR_RETURN_MAJOR(nFILRet) > 0)
	{
		BUF_Release(pInBuf);

		VFL_ERR_PRINT((TEXT("[VFL:ERR]  Read(nBank:0) fail!\r\n")));

		WMR_ReleaseSM(WMR_SM_BML);
		return nFILRet;
	}	

	#if (WMR_ALIGN_CHECK)
	if ((UINT32)pBuf % 4 > 0)
	{
		WMR_MEMCPY(pBuf, pInBuf->pData, BYTES_PER_SECTOR * SECTORS_PER_SUPAGE);
	}
	#endif

	BUF_Release(pInBuf);

	VFL_LOG_PRINT((TEXT("[VFL:OUT] --VFL_WMRArea_Read(Ppn:%d)\r\n"),nPpn));

	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BML);

    return nFILRet;
}

