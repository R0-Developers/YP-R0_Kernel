/*****************************************************************************/
/*                                                                           */
/* COMPONENT   : Rainbow				                                     */
/* MODULE      : Virtual Flash Layer                                         */
/* NAME    	   : VFL Buffer Management                                       */
/* FILE        : VFLBuffer.c                                                 */
/* PURPOSE     : This file contains routines for managing buffers which      */
/*              whimory uses. 						                         */
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
/*   13-JUL-2005 [Jaesung Jung] : separate from vfl.c & reorganize code      */
/*   06-SEP-2005 [Jaesung Jung] : fix from code inspection	  	 			 */
/*   01-JAN-2007 [Inhwan Choi ] : 4k page nand support                       */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Header file inclusions                                                    */
/*****************************************************************************/
#include "WMRConfig.h"
#include "WMRTypes.h"
#include "WMROAM.h"
#include "VFLBuffer.h"

/*****************************************************************************/
/* Debug Print #defines                                                      */
/*****************************************************************************/
#define     BUF_ERR_PRINT(x)            WMR_RTL_PRINT(x)
#define     BUF_RTL_PRINT(x)            WMR_RTL_PRINT(x)

//#define 	BUF_LOG_MSG_ON
#if defined(BUF_LOG_MSG_ON)
#define     BUF_LOG_PRINT(x)            WMR_RTL_PRINT(x)
#else
#define     BUF_LOG_PRINT(x)
#endif

//#define 	BUF_INF_MSG_ON
#if defined(BUF_INF_MSG_ON)
#define     BUF_INF_PRINT(x)            WMR_RTL_PRINT(x)
#else
#define     BUF_INF_PRINT(x)
#endif

/*****************************************************************************/
/* Buffer management context structure definition                            */
/*****************************************************************************/
typedef struct {
	Buffer	*pBufferList;		/* the list of all the buffers				 */
	Buffer	 stWrittenBuffer;	/* doubly linked list of buffers that are BUF_WRITTEN */
	UINT32 	 nFreeBuffers;		/* the count of free buffers				 */
} BUFCxt;

/*****************************************************************************/
/* Static variables definitions                                              */
/*****************************************************************************/
PRIVATE BUFCxt		*pstBufferCxt = NULL;

/*****************************************************************************/
/* Code Implementation                                                       */
/*****************************************************************************/
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BUF_Init                                                             */
/* DESCRIPTION                                                               */
/*      This function initializes the buffer manager & the buffer pool.	     */
/* PARAMETERS                                                                */
/*      none			                                                     */
/* RETURN VALUES                                                             */
/* 		BUF_SUCCESS                                                          */
/*            BUF_Init is completed.                                         */
/*      BUF_CRITICAL_ERROR                                                   */
/*            BUF_Init is failed    		                                 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
INT32
BUF_Init(VOID)
{
	UINT32 	nBufIdx;
	Buffer *pBufIdx;
	BOOL32	bInitBuf = TRUE32;
	
	/* semaphore */
	WMR_CreateSM (WMR_SM_BUF);
	WMR_AcquireSM(WMR_SM_BUF);


	BUF_LOG_PRINT((TEXT("[BUF: IN] ++BUF_Init(VOID)\r\n")));
	
	/* Check buffer context was initialized */	
	if (pstBufferCxt != NULL)
	{
		BUF_LOG_PRINT((TEXT("[BUF:OUT] --BUF_Init(VOID) nRe=0x%x\r\n"), BUF_SUCCESS));
		bInitBuf = FALSE32;
		goto B_INIT_BUF;
	}
	
	/* Initialize buffer context */
	pstBufferCxt = (BUFCxt *)WMR_MALLOC(sizeof(BUFCxt));

	if (pstBufferCxt == NULL)
	{
		BUF_ERR_PRINT((TEXT("[BUF:ERR]  BUF_Init(VOID) BUFCxt Init Failure\r\n")));

		WMR_ReleaseSM(WMR_SM_BUF);
		WMR_DestroySM(WMR_SM_BUF);
        return BUF_CRITICAL_ERROR;		

	}
	
	/* Initialize buffer list */
	pstBufferCxt->pBufferList = (Buffer *)WMR_MALLOC(sizeof(Buffer) * WMR_NUM_BUFFERS);

	if (pstBufferCxt->pBufferList == NULL)
	{
		BUF_ERR_PRINT((TEXT("[BUF:ERR]  BUF_Init(VOID) BUFCxt->pBufferList Init Failure\r\n")));

		WMR_ReleaseSM(WMR_SM_BUF);
		WMR_DestroySM(WMR_SM_BUF);
        return BUF_CRITICAL_ERROR;
	}
	
B_INIT_BUF:
	
	pBufIdx = pstBufferCxt->pBufferList;
	
	for (nBufIdx = 0; nBufIdx < WMR_NUM_BUFFERS; nBufIdx++)
	{
		if (bInitBuf == TRUE32)
		{
			pBufIdx->pData = (UINT8 *)WMR_MALLOC(BYTES_PER_MAIN_SUPAGE + BYTES_PER_SPARE_SUPAGE);
		}
		
		pBufIdx->pSpare = pBufIdx->pData + BYTES_PER_MAIN_SUPAGE;
		
		if (pBufIdx->pData == NULL || pBufIdx->pSpare == NULL)
		{
			BUF_ERR_PRINT((TEXT("[BUF:ERR]  BUF_Init(VOID) pBufIdx->pData, pBufIdx->pSpare Init Failure\r\n")));

			WMR_ReleaseSM(WMR_SM_BUF);
			WMR_DestroySM(WMR_SM_BUF);
	        return BUF_CRITICAL_ERROR;
		}
		
		pBufIdx->pDataBak = pBufIdx->pData;
		pBufIdx->eStatus = BUF_FREE;
		
		pBufIdx++;
	}

	pstBufferCxt->stWrittenBuffer.pNext = &(pstBufferCxt->stWrittenBuffer);
	pstBufferCxt->stWrittenBuffer.pPrev = &(pstBufferCxt->stWrittenBuffer);

	pstBufferCxt->nFreeBuffers = WMR_NUM_BUFFERS;
	
	BUF_LOG_PRINT((TEXT("[BUF:OUT] --BUF_Init(VOID) nRe=0x%x\r\n"), BUF_SUCCESS));

	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BUF);

    return BUF_SUCCESS;
	
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BUF_Get	                                                             */
/* DESCRIPTION                                                               */
/*      This function returns a new free buffer. 						     */
/*		if there is not a free buffer, this function calls the sync function */
/*		and generates a new free buffer.									 */
/* PARAMETERS                                                                */
/*		eType			[IN]	buffer type									 */
/*		pBusyBank		[OUT]	BankNo that is busy and hold a buffer		 */
/* RETURN VALUES                                                             */
/* 		Buffer	                                                             */
/*            BUF_Get is completed.                                          */
/*      NULL			                                                     */
/*            BUF_Get is failed.    		                                 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
Buffer *
BUF_Get(BUFType eType, UINT32 *pBusyBank)
{
	UINT32 	 nBufIdx;
	Buffer	*pBufRet;

	/* semaphore */
	WMR_AcquireSM(WMR_SM_BUF);

	BUF_LOG_PRINT((TEXT("[BUF: IN] ++BUF_Get(eType = %d)\r\n"), eType));

	/* Check buffer context was initialized */	
	if (pstBufferCxt == NULL)
	{
		BUF_ERR_PRINT((TEXT("[BUF:ERR]  BUF_Get(eType = %d) Buffer manager is not initialized\r\n"), eType));

		WMR_ReleaseSM(WMR_SM_BUF);
		return NULL;
	} 
	
	/* Check the number of free buffers, and                                 */
	/* inform caller about it for generating a new free buffer if there is not */
	if (pstBufferCxt->nFreeBuffers == 0)
	{
		UINT32 	 nBank;

		nBank = pstBufferCxt->stWrittenBuffer.pNext->nBank;

		WMR_ASSERT(pstBufferCxt->stWrittenBuffer.pNext != &(pstBufferCxt->stWrittenBuffer));

		if ( pBusyBank != NULL )
		{
			*pBusyBank = nBank;
		}

		WMR_ReleaseSM(WMR_SM_BUF);
		return NULL;	/* not error */
	}
	
	/* Find a free buffer from the buffer pool. */
	pBufRet = pstBufferCxt->pBufferList;
	
	for (nBufIdx = 0; nBufIdx < WMR_NUM_BUFFERS; nBufIdx++, pBufRet++)
	{
		if (pBufRet->eStatus == BUF_FREE)
		{			
			break;
		}
	}

	if (nBufIdx >= WMR_NUM_BUFFERS)
	{
		BUF_ERR_PRINT((TEXT("[BUF:ERR]  BUF_Get(eType = %d) Can't find a free buffer\r\n"), eType));

		WMR_ReleaseSM(WMR_SM_BUF);
        return NULL;
	}

	switch(eType)
	{
		case BUF_MAIN_AND_SPARE :
			WMR_MEMSET(pBufRet->pSpare, 0xFF, BYTES_PER_SPARE_SUPAGE);			
			break;

		case BUF_MAIN_ONLY :
			pBufRet->pSpare = NULL;
			break;

		case BUF_SPARE_ONLY :
			pBufRet->pData = NULL;
			WMR_MEMSET(pBufRet->pSpare, 0xFF, BYTES_PER_SPARE_SUPAGE);			
			break;

		default :
			WMR_ASSERT(FALSE32);
			break;
	}
	/* Change the buffer status & bitmap */
	pBufRet->eStatus = BUF_ALLOCATED;
	pBufRet->nBitmap = 0;

	/* Decrease the number of free buffers */
	pstBufferCxt->nFreeBuffers--;
	
	BUF_LOG_PRINT((TEXT("[BUF:OUT] --BUF_Get(eType = %d)\r\n"), eType));

	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BUF);

	return pBufRet;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BUF_Release                                                          */
/* DESCRIPTION                                                               */
/*      This function releases the buffer to the buffer pool.			     */
/* PARAMETERS                                                                */
/*      pBuf			[IN]	                                             */
/*				buffer pointer												 */
/* RETURN VALUES                                                             */
/*		none																 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
VOID
BUF_Release(Buffer *pBuf)
{
	/* semaphore */
	WMR_AcquireSM(WMR_SM_BUF);

	BUF_LOG_PRINT((TEXT("[BUF: IN] ++BUF_Release(pBuf = %x)\r\n"), pBuf));
	
	if (pBuf == NULL || pBuf->eStatus == BUF_AUX)
	{
		WMR_ReleaseSM(WMR_SM_BUF);
		return;
	}
	
	WMR_ASSERT((pBuf - pstBufferCxt->pBufferList) >= 0 &&
		  	  (pBuf - pstBufferCxt->pBufferList) < WMR_NUM_BUFFERS);

	WMR_ASSERT(pBuf->eStatus == BUF_ALLOCATED || pBuf->eStatus == BUF_WRITTEN);

	/* Remove the buffer pointer from the written buffer queue */
	if (pBuf->eStatus == BUF_WRITTEN)
	{
		WMR_ASSERT(pBuf->pPrev != NULL);
		WMR_ASSERT(pBuf->pNext != NULL);

		pBuf->pPrev->pNext = pBuf->pNext;
		pBuf->pNext->pPrev = pBuf->pPrev;
	}
	
	/* Reinitialize the buffer */
	pBuf->pData = pBuf->pDataBak;
	pBuf->pSpare = pBuf->pDataBak + BYTES_PER_MAIN_SUPAGE;
	
	pBuf->eStatus = BUF_FREE;

	pstBufferCxt->nFreeBuffers++;	

	WMR_ASSERT(pstBufferCxt->nFreeBuffers <= WMR_NUM_BUFFERS);

	BUF_LOG_PRINT((TEXT("[BUF:OUT] --BUF_Release(pBuf = %x)\r\n"), pBuf));

	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BUF);

	return;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BUF_Reget                                                            */
/* DESCRIPTION                                                               */
/*      This function changes the type of the buffer.					     */
/* PARAMETERS                                                                */
/*      pBuf			[IN/OUT]                                             */
/*				buffer pointer												 */
/*		eType			[IN]												 */
/*				buffer type													 */
/* RETURN VALUES                                                             */
/*		none																 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
VOID
BUF_Reget(Buffer *pBuf, BUFType eType)
{
	/* semaphore */
	WMR_AcquireSM(WMR_SM_BUF);

	BUF_LOG_PRINT((TEXT("[BUF: IN] ++BUF_Reget(pBuf = %x, eType = %d)\r\n"), pBuf, eType));
	
	WMR_ASSERT(pBuf != NULL);
	
	switch(eType)
	{
		case BUF_MAIN_AND_SPARE :
			pBuf->pData = pBuf->pDataBak;
			pBuf->pSpare = pBuf->pDataBak + BYTES_PER_MAIN_SUPAGE;
			break;
			
		case BUF_MAIN_ONLY :
			pBuf->pData = pBuf->pDataBak;			
			pBuf->pSpare = NULL;
			break;
			
		case BUF_SPARE_ONLY :
			pBuf->pData = NULL;
			pBuf->pSpare = pBuf->pDataBak + BYTES_PER_MAIN_SUPAGE;			
			break;
	
		default :
			/* do nothing */	
			WMR_ASSERT(FALSE32);
			break;
	}
	BUF_LOG_PRINT((TEXT("[BUF:OUT] --BUF_Reget(pBuf = %x, eType = %d)\r\n"), pBuf, eType));

	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BUF);

	return ;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BUF_MarkAsWritten                                                    */
/* DESCRIPTION                                                               */
/*      This function changes the status of buffer to written			     */
/*		& adds the pointer of buffer to written buffer list.				 */
/* PARAMETERS                                                                */
/*      pBuf			[IN]		                                         */
/*				buffer pointer												 */
/* RETURN VALUES                                                             */
/*		none																 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
VOID
BUF_MarkAsWritten(Buffer *pBuf)
{
	Buffer *pBufListTail = pstBufferCxt->stWrittenBuffer.pPrev;

	/* semaphore */
	WMR_AcquireSM(WMR_SM_BUF);

	WMR_ASSERT((pBuf - pstBufferCxt->pBufferList) >= 0 &&
		   (pBuf - pstBufferCxt->pBufferList) < WMR_NUM_BUFFERS);

	WMR_ASSERT(pBuf->eStatus == BUF_ALLOCATED);

	pBuf->pPrev = pBufListTail;
	pBuf->pNext = pBufListTail->pNext;
	WMR_ASSERT(pBufListTail->pNext != NULL);
	pBufListTail->pNext->pPrev = pBuf;
	pBufListTail->pNext = pBuf;

	pBuf->eStatus = BUF_WRITTEN;

	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BUF);

	return;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      BUF_Close                                                            */
/* DESCRIPTION                                                               */
/*      This function releases buffer manager context.					     */
/* PARAMETERS                                                                */
/*      none			                                                     */
/* RETURN VALUES                                                             */
/* 		none		                                                         */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
VOID
BUF_Close(VOID)
{
	UINT32 	nBufIdx;
	Buffer *pBufIdx;
		
	/* semaphore */
	WMR_AcquireSM(WMR_SM_BUF);

	BUF_LOG_PRINT((TEXT("[BUF: IN] ++BUF_Close(VOID)\r\n")));
	
	if (pstBufferCxt != NULL)
	{
		if (pstBufferCxt->pBufferList != NULL)
		{
			pBufIdx = pstBufferCxt->pBufferList;
			
			for (nBufIdx = 0; nBufIdx < WMR_NUM_BUFFERS; nBufIdx++)
			{				
				WMR_FREE(pBufIdx->pDataBak);

				pBufIdx->pData = NULL;
				pBufIdx->pSpare = NULL;

				pBufIdx++;
			}
			
			WMR_FREE(pstBufferCxt->pBufferList);

			pstBufferCxt->pBufferList = NULL;
		}
		

		WMR_FREE(pstBufferCxt);

		pstBufferCxt = NULL;
	}
		
	BUF_LOG_PRINT((TEXT("[BUF:OUT] --BUF_Close(VOID)\r\n")));

	/* semaphore */
	WMR_ReleaseSM(WMR_SM_BUF);
	WMR_DestroySM(WMR_SM_BUF);

    return;	
}