/*****************************************************************************/
/*                                                                           */
/* COMPONENT   : Rainbow				                                     */
/* MODULE      : Example			                                         */
/* NAME    	   : Example					                                 */
/* FILE        : WMRRFS.c		                                             */
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
/*   10-MAR-2007 [Junho   Jang] : first writing                              */
/*                                                                           */
/*****************************************************************************/
#include "WMRRFS.h"


/*****************************************************************************/
/* Static function prototypes                                                */
/*****************************************************************************/


/*****************************************************************************/
/* Debug Print #defines                                                      */
/*****************************************************************************/
#define 	EXAM_INF_MSG_ON

#define     EXAM_ERR_PRINT(x)            WMR_RTL_PRINT(x)
#define     EXAM_RTL_PRINT(x)            WMR_RTL_PRINT(x)

#if defined(EXAM_LOG_MSG_ON)
#define     EXAM_LOG_PRINT(x)            WMR_DBG_PRINT(x)
#else
#define     EXAM_LOG_PRINT(x)
#endif

#if defined(EXAM_INF_MSG_ON)
#define     EXAM_INF_PRINT(x)            WMR_DBG_PRINT(x)
#else
#define     EXAM_INF_PRINT(x)
#endif

/*****************************************************************************/
/* Code Implementation                                                       */
/*****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BML_WMRArea_Read                                                     */
/* DESCRIPTION                                                               */
/*      This function read data of WMR_AREA                                  */
/* PARAMETERS                                                                */
/*      nPpn		[ IN]    Physical Page No                                */
/*      pBuf		[OUT]    buffer to be read                               */
/* RETURN VALUES                                                             */
/* 		WMR_SUCCESS                                                          */
/*            BML_WMRArea_Read is completed.                                 */
/*      WMR_CRITICAL_ERROR                                                   */
/*            BML_WMRArea_Read is failed.                                    */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32	
BML_WMRArea_Read (UINT32  nPpn, UINT8 *pBuf)
{
	return VFL_WMRArea_Read(nPpn,pBuf);
}

#if (WMR_RO_AREA_SUPPORT)
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BML_Open                                                             */
/* DESCRIPTION                                                               */
/*      This function init and open VFL layer of eWhimory.                   */
/* PARAMETERS                                                                */
/*                                                                           */
/* RETURN VALUES                                                             */
/* 		WMR_SUCCESS                                                          */
/*            BML_Open is completed.                                         */
/*      WMR_CRITICAL_ERROR                                                   */
/*            BML_Open is failed.                                            */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32	
BML_Open (VOID)
{
	return WMR_LOW_Init();
}
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BML_Close                                                            */
/* DESCRIPTION                                                               */
/*      This function close ewhimory and free memory                         */
/* PARAMETERS                                                                */
/*                                                                           */
/* RETURN VALUES                                                             */
/* 		WMR_SUCCESS                                                          */
/*            BML_Close is completed.                                        */
/*      WMR_CRITICAL_ERROR                                                   */
/*            BML_Close is failed.                                           */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
VOID	
BML_Close (VOID)
{
	WMR_LOW_Close();
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BML_GetInfo                                                          */
/* DESCRIPTION                                                               */
/*      This function erase all data of RO Partition                         */
/* PARAMETERS                                                                */
/*      nROPartNo	[ IN]    Partition No                                    */
/*      nMaxPages	[OUT]    Partition Size (Max Pages of Partition)         */
/* RETURN VALUES                                                             */
/* 		WMR_SUCCESS                                                          */
/*            BML_GetInfo is completed.                                      */
/*      WMR_CRITICAL_ERROR                                                   */
/*            BML_GetInfo is failed.                                         */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32 
BML_GetInfo	(UINT32  nROPartNo, UINT32 *nMaxPages)
{
	return VFL_RO_GetInfo(nROPartNo,nMaxPages);
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BML_Mread                                                            */
/* DESCRIPTION                                                               */
/*      This function read pages of RO Partition                             */
/* PARAMETERS                                                                */
/*      nROPartNo	[ IN]    Partition No                                    */
/*      nLpn		[ IN]    Logical Page No                                 */
/*		nPages		[ IN]    nubmer of pages to be read                      */
/*      pBuf		[OUT]    buffer to be read                               */
/* RETURN VALUES                                                             */
/* 		WMR_SUCCESS                                                          */
/*            BML_Mread is completed.                                        */
/*      WMR_CRITICAL_ERROR                                                   */
/*            BML_Mread is failed.                                           */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32 
BML_Mread (UINT32 nROPartNo, UINT32 nLPN, UINT32 nPages, UINT8 *pBuf)
{
	UINT32	nIdxPage;

	INT32	nRet;

	for ( nIdxPage = 0; nIdxPage < nPages; nIdxPage++ )
	{
		nRet = VFL_RO_Read( nROPartNo, nIdxPage + nLPN, pBuf + nIdxPage*BYTES_PER_MAIN_SUPAGE );

		if ( nRet != WMR_SUCCESS )
		{
			return WMR_CRITICAL_ERROR;
		}
	}

	return WMR_SUCCESS;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BML_MreadPL                                                          */
/* DESCRIPTION                                                               */
/*      This function read phsical pages of RO Partition                     */
/*      It is used for 4K Page/size NAND and 4K Block Device Driver          */
/* PARAMETERS                                                                */
/*      nROPartNo	[ IN]    Partition No                                    */
/*      nPPO		[ IN]    Physical Page Offset                            */
/*		nPages		[ IN]    nubmer of pages to be read                      */
/*      pBuf		[OUT]    buffer to be read                               */
/* RETURN VALUES                                                             */
/* 		WMR_SUCCESS                                                          */
/*            BML_Mread is completed.                                        */
/*      WMR_CRITICAL_ERROR                                                   */
/*            BML_Mread is failed.                                           */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32 
BML_MreadPL (UINT32 nROPartNo, UINT32 nPPO, UINT32 nPages, UINT8 *pBuf)
{
	UINT32	nIdxPage;

	INT32	nRet;

	for ( nIdxPage = 0; nIdxPage < nPages; nIdxPage++ )
	{
		nRet = VFL_RO_ReadPL( nROPartNo, nIdxPage + nPPO, pBuf + nIdxPage*BYTES_PER_MAIN_PAGE );

		if ( nRet != WMR_SUCCESS )
		{
			return WMR_CRITICAL_ERROR;
		}
	}

	return WMR_SUCCESS;
}

#endif


/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BML_GetDevInfo                                                       */
/* DESCRIPTION                                                               */
/*      This function give information of NAND device.                       */
/* PARAMETERS                                                                */
/*      stLLDSpec  [OUT]    NAND information                                 */
/* RETURN VALUES                                                             */
/* 		WMR_SUCCESS                                                          */
/*            BML_GetDevInfo is completed.                                   */
/*      WMR_CRITICAL_ERROR                                                   */
/*            BML_GetDevInfo is failed.                                      */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32 
BML_GetDevInfo (WMRLLDSpec *stLLDSpec)
{
    stLLDSpec->nPgsPerBlock	= PAGES_PER_BLOCK;
    stLLDSpec->nBlksPerNand	= BLOCKS_PER_BANK;	

    stLLDSpec->nNumOfPlane		= TWO_PLANE_PROGRAM + 1;
	stLLDSpec->nSctsPerPgNAND	= SECTORS_PER_PAGE;

	stLLDSpec->nSctsPerPgLLD	= SECTORS_PER_PAGE * stLLDSpec->nNumOfPlane;

	return WMR_SUCCESS;
}
