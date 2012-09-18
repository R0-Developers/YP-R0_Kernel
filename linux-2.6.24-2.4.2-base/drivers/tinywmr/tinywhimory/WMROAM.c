/*****************************************************************************/
/*                                                                           */
/* COMPONENT   : Rainbow				                                     */
/* MODULE      : OS Adaptation Module                                        */
/* FILE        : WMROAM.c                                                    */
/* PURPOSE     : This file contains exported functions for 					 */
/*				 OS Adaptation Module.     				                     */
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
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/mm.h>
//#include <asm-mips/semaphore.h>
//#include <asm-mips/pgtable.h>
#include <asm/semaphore.h>
#include <asm/pgtable.h>
#include <asm/semaphore.h>
#include <asm/pgtable.h>
//#include <asm/sizes.h>

#include "WMRConfig.h"
#include "WMRTypes.h"
#include "WMROAM.h"


/*****************************************************************************/
/* Local typedefs                                                            */
/*****************************************************************************/
UINT8 	aTempSBuf[512];
UINT8 	aMakeSECCBuf[512];
UINT8 	aTempEccBuf[10];

/*****************************************************************************/
/* constant definitions                                                		 */
/*****************************************************************************/
#define SZ_128K 	0x00020000

/*****************************************************************************/
/* Static variables definitions                                              */
/*****************************************************************************/
/* static struct semaphore bml_sem, buf_sem; */
#ifdef CONFIG_RFS_TINYBML

struct semaphore bml_sem;
int bml_debug_sema = 0;

EXPORT_SYMBOL(bml_sem);
EXPORT_SYMBOL(bml_debug_sema);

#else
static struct semaphore bml_sem;
static int bml_debug_sema = 0;
#endif

static struct semaphore buf_sem;
static int buf_debug_sema = 0;

/*
static DECLARE_MUTEX(buf_sem);
static int buf_debug_sema = 0;
*/

PRIVATE const UINT8 aChecksumTable[256]  =
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
/*      WMR_GetChkSum                                                        */
/* DESCRIPTION                                                               */
/*      This function returns the sum of 0 count of pBuf.				     */
/* PARAMETERS                                                                */
/*      pBuf    [IN] 	the pointer of buffer	                             */
/*      nSize	[IN] 	the size of buffer		                             */
/* RETURN VALUES                                                             */
/*		the sum of 0 count													 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
UINT16
WMR_GetChkSum(UINT8 *pBuf, UINT32 nSize)
{
	register UINT32 nIdx;
	register const UINT8 *pD8;
	register UINT8 *pCheckSum;
	register UINT32 nSum = 0;
	
	pD8 = pBuf;

	pCheckSum = (UINT8 *)aChecksumTable;

	if(nSize < 512)
	{
		for (nIdx = 0; nIdx < nSize; nIdx++)
		{
			nSum += *(pCheckSum + *(pD8++));
		}
	}
	else
	{
		for (nIdx = 0; nIdx < nSize / 8; nIdx++)
		{
			nSum += *(pCheckSum + *(pD8++));
			nSum += *(pCheckSum + *(pD8++));
			nSum += *(pCheckSum + *(pD8++));
			nSum += *(pCheckSum + *(pD8++));
			nSum += *(pCheckSum + *(pD8++));
			nSum += *(pCheckSum + *(pD8++));
			nSum += *(pCheckSum + *(pD8++));
			nSum += *(pCheckSum + *(pD8++));
		}
	}

	return (UINT16)nSum;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      WMR_GetRelatedChkSum                                                 */
/* DESCRIPTION                                                               */
/*      This function returns the sum of 0 count of pBuf1 & pBuf2.		     */
/* PARAMETERS                                                                */
/*      pBuf1   [IN] 	the pointer of buffer	                             */
/*      pBuf2   [IN] 	the pointer of buffer	                             */
/*      nSize	[IN] 	the size of buffer		                             */
/* RETURN VALUES                                                             */
/*		the sum of 1 count													 */
/* NOTES                                                                     */
/*                                                                           */
/*****************************************************************************/
#if (WMR_MLC_SUPPORT)
UINT32
WMR_GetRelatedChkSum(UINT8 *pBuf1, UINT8 *pBuf2, UINT32	nSize)
{
	UINT32 nIdx;
	UINT32 nSum = 0;

	for (nIdx = 0; nIdx < nSize; nIdx++)
	{
		UINT8 nValue0 = pBuf1[nIdx];
		UINT8 nValue2 = pBuf2[nIdx];
		UINT32 j;

		for (j = 0; j < 8; j++)
		{
			UINT8 nBit0 = (nValue0 >> j) & 0x1;
			UINT8 nBit2 = (nValue2 >> j) & 0x1;

			if (!nBit0 && !nBit2) 
			{
				nSum += 3;
			}
			else if (nBit0 && !nBit2)
			{
				nSum += 2;
			}
			else if (!nBit0 && nBit2)
			{
				nSum += 1;
			}
		}
		
	}

	return nSum;
}
#endif

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      _free_vmem                                                           */
/* DESCRIPTION                                                               */
/*      This function frees memory for Whimory                               */
/*                                                                           */
/* PARAMETERS                                                                */
/*      addr       [IN]                                                      */
/*            address to be free                                             */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      0 is success, -1 is error                                            */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called by function that wants to free memory        */
/*                                                                           */
/*****************************************************************************/
static inline int _free_vmem(void *addr)
{
	if( ((u32)addr >= VMALLOC_START) && ((u32)addr <= VMALLOC_END))
	{
		vfree(addr);
		return 0;
	}
	
	return -1;
}
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      _alloc_vmem                                                          */
/* DESCRIPTION                                                               */
/*      This function allocates memory for Whimory                           */
/*                                                                           */
/* PARAMETERS                                                                */
/*      size       [IN]                                                      */
/*            Size to be allocated                                           */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      Pointer of allocated memory                                          */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called by function that wants to use memory         */
/*                                                                           */
/*****************************************************************************/
static INLINE void *_alloc_vmem(unsigned long size)
{
	return vmalloc(size);
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      OAM_Malloc                                                           */
/* DESCRIPTION                                                               */
/*      This function allocates memory for Whimory                           */
/*                                                                           */
/* PARAMETERS                                                                */
/*      nSize       [IN]                                                     */
/*            Size to be allocated                                           */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      Pointer of allocated memory                                          */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called by function that wants to use memory         */
/*                                                                           */
/*****************************************************************************/
void *
OAM_Malloc(UINT32 nSize)
{
	void * ret;
	
	if (nSize > SZ_128K)
		ret = _alloc_vmem(nSize);
	else
		ret = kmalloc(nSize, GFP_KERNEL);
		
	return ret;
}
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      OAM_Free                                                             */
/* DESCRIPTION                                                               */
/*      This function allocates memory for Whimory                           */
/*                                                                           */
/* PARAMETERS                                                                */
/*      pMem       [IN]                                                      */
/*            address to be free                                             */
/*                                                                           */
/* RETURN VALUES                                                             */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called by function that wants to free memory        */
/*                                                                           */
/*****************************************************************************/
void
OAM_Free(void *pMem)
{
	if (_free_vmem(pMem))
		kfree(pMem);
		
	return;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*  OAM_Assert                                                               */
/* DESCRIPTION                                                               */
/*     assert check									 	                     */
/*                                                                           */
/* PARAMETERS                                                                */
/*  bVal    check boolean value									             */
/*  szFile  file name												         */
/*  nLine   line count											             */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      none                                                                 */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is used when system can not support debug              */
/*  print function                                                           */
/*                                                                           */
/*****************************************************************************/
VOID
OAM_Assert(BOOL32 bVal, const UINT8 *szFile, UINT32 nLine)
{
	if(!bVal)
	{
		WMR_RTL_PRINT((TEXT("[FTL:ERR]  OAM_Assert Error [%s:%d]!\r\n"), szFile, nLine));
		WMR_RTL_PRINT((TEXT("\n <log P1=\"101\" P2=\"WMROAM\" P3=\"\" P4=\"OAM_Assert()\" P5=\"OAM_Assert Error [%x:%d]\" P6=\"\" P7=\"\" />\n"), szFile, nLine));
	}

}
	
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      OAM_Pa2Va                                                            */
/* DESCRIPTION                                                               */
/*      This function remap physical I/O address to virtual address          */
/*                                                                           */
/* PARAMETERS                                                                */
/*      nPAddr      [IN]                                                     */
/*            address to be mapped                                           */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      virtual address                                                      */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called to access physical address                   */
/*                                                                           */
/*****************************************************************************/
UINT32
OAM_Pa2Va(UINT32 nPAddr)
{
	unsigned long ioaddr;

	ioaddr = (unsigned long) ioremap(nPAddr, SZ_128K);

	return ioaddr;
}

/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      OAM_CreateSM                                                         */
/* DESCRIPTION                                                               */
/*      This function create semaphore and initialize.                       */
/*                                                                           */
/* PARAMETERS                                                                */
/*      nHandle     [IN]                                                     */
/*            semaphore number                                               */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      alwayers TRUE                                                        */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called to init semaphore                            */
/*                                                                           */
/*****************************************************************************/
BOOL32
OAM_CreateSM(UINT32 nHandle)
{
	if ( nHandle == WMR_SM_BUF )
	{
		buf_debug_sema = 0;
		sema_init(&buf_sem,1);
	}
	else if ( nHandle == WMR_SM_BML )
	{
		//#ifndef CONFIG_RFS_TINYBML
		bml_debug_sema = 0;
		sema_init(&bml_sem,1);
		//#endif
	}	
		
	return TRUE32;
}
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      OAM_DestroySM                                                        */
/* DESCRIPTION                                                               */
/*      This function remove semaphore.                                      */
/*                                                                           */
/* PARAMETERS                                                                */
/*      nHandle     [IN]                                                     */
/*            semaphore number                                               */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      alwayers TRUE                                                        */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called to remove semaphore                          */
/*                                                                           */
/*****************************************************************************/
BOOL32
OAM_DestroySM(UINT32 nHandle)
{
	if ( nHandle == WMR_SM_BUF )	
		buf_debug_sema = 0;
	else if ( nHandle == WMR_SM_BML )
		bml_debug_sema = 0;
					
	return TRUE32;
}
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      OAM_AcquireSM                                                        */
/* DESCRIPTION                                                               */
/*      This function lock semaphore.                                        */
/*                                                                           */
/* PARAMETERS                                                                */
/*      nHandle     [IN]                                                     */
/*            semaphore number                                               */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      alwayers TRUE                                                        */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called to lock semaphore                            */
/*                                                                           */
/*****************************************************************************/
BOOL32
OAM_AcquireSM(UINT32 nHandle)
{
	if ( nHandle == WMR_SM_BUF )
	{	
		down(&buf_sem);
		buf_debug_sema ++;
	}
	else if ( nHandle == WMR_SM_BML )
	{
		down(&bml_sem);
		bml_debug_sema ++;		
	}
	
	return TRUE32;
}
/*****************************************************************************/
/*                                                                           */
/* NAME                                                                      */
/*      OAM_ReleaseSM                                                        */
/* DESCRIPTION                                                               */
/*      This function unlock semaphore.                                      */
/*                                                                           */
/* PARAMETERS                                                                */
/*      nHandle     [IN]                                                     */
/*            semaphore number                                               */
/*                                                                           */
/* RETURN VALUES                                                             */
/*      alwayers TRUE                                                        */
/*                                                                           */
/* NOTES                                                                     */
/*      This function is called to unlock semaphore                          */
/*                                                                           */
/*****************************************************************************/
BOOL32
OAM_ReleaseSM(UINT32 nHandle)
{
	if ( nHandle == WMR_SM_BUF )
	{	
		up(&buf_sem);
		buf_debug_sema --;	
	}
	else if ( nHandle == WMR_SM_BML )
	{	
		up(&bml_sem);
		bml_debug_sema --;
	}		
		
	return TRUE32;
}

/************************************************
INT32
xsr_get_bml_semaphore(void)
{
	return bml_debug_sema;
}


INT32
xsr_get_buf_semaphore(void)
{
	return buf_debug_sema;
}
******************************************************/

VOID
OAM_Debug(VOID  *pFmt, ...)
{
#if 0
        va_list args;
        uint i;
        char printbuffer[512];

        va_start (args, pFmt);

        /* For this to work, printbuffer must be larger than
         * anything we ever want to print.
         */
        i = vsprintf (printbuffer, pFmt, args);
        va_end (args);

        /* Print the string */
        printk("%s", printbuffer);
#endif
}

