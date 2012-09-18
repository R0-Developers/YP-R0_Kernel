/*==================================================================================================

    Module Name:  NFC_Samsung.c

 ===================================================================================================
    Copyright (C) 2004-2008 Freescale Semiconductor, Inc. All Rights Reserved
    THIS SOURCE CODE IS CONFIDENTIAL AND PROPRIETARY AND MAY NOT
    BE USED OR DISTRIBUTED WITHOUT THE WRITTEN PERMISSION OF
    Freescale Semiconductor, Inc.

 */

//#define MJXU

/* include */
#include "WMRConfig.h"
#include "WMRTypes.h"
#include "WMROAM.h"

#include "NFC_Samsung.h"
#include "FIL.h"
#include <../arch/arm/mach-mx37/iomux.h>


/*****************************************************************************/
/* Debug Print #defines                                                      */
/*****************************************************************************/
//#define     NAND_INF_MSG_ON

#ifndef MJXU
#define     NAND_ERR_PRINT(x)            WMR_RTL_PRINT(x)
#define     NAND_RTL_PRINT(x)            WMR_RTL_PRINT(x)
#define		USE_MACRO
#define		USE_ASM
#else
#define     NAND_ERR_PRINT(x)
#define     NAND_RTL_PRINT(x)
#define		USE_MACRO
#define		USE_ASM


#endif

#if defined(NAND_LOG_MSG_ON)
#define     NAND_LOG_PRINT(x)            WMR_DBG_PRINT(x)
#else
#define     NAND_LOG_PRINT(x)
#endif

#if defined(NAND_INF_MSG_ON)
#define     NAND_INF_PRINT(x)            WMR_DBG_PRINT(x)
#else
#define     NAND_INF_PRINT(x)
#endif


/*****************************************************************************/
/* Device type context definitions                                           */
/*****************************************************************************/
typedef struct
{
	UINT8	nDevID;								/* Device ID				 */
	UINT8	nHidID;								/* Hidden ID				 */
	UINT32	nNumOfBlocks;						/* Number of Blocks			 */
	UINT32 	nPagesPerBlock;						/* Pages per block			 */
	UINT32  nSectorsPerPage;					/* Sectors per page			 */
	BOOL32	b2XProgram;							/* 2 plane program			 */
	BOOL32	b2XRead;							/* 2 plane read			 	 */
	BOOL32	b2XReadStatus;						/* 2 plane read status	 	 */
	BOOL32	bDualDie;							/* internal interleaving	 */
	BOOL32	bMLC;								/* MLC						 */
} DEVInfo;

const DEVInfo stDEVInfo[] = {
		/*****************************************************************************/
		/* Device ID																 */
		/*	   Hidden ID															 */
		/*			 Blocks															 */
		/*				   Pages per block											 */
		/*						Sectors per page									 */
		/*						   2X program										 */
		/*						   			2X read									 */
		/*											 2x status 						 */
		/*													 internal Interleaving	 */
		/*												      			MLC			 */
		/*****************************************************************************/
#if (WMR_SLC_SUPPORT)
		/* SLC NAND ID TABLE */
		{0xF1, 0x80, 1024,  64, 4, FALSE32, FALSE32, FALSE32, FALSE32, FALSE32},	/* 1Gb SLC(K9F1G08) Mono */
		{0xF1, 0x00, 1024,  64, 4, FALSE32, FALSE32, FALSE32, FALSE32, FALSE32},	/* 1Gb SLC(K9F1G08) Mono B-die */
		{0xDA, 0xC1, 2048,  64, 4, FALSE32, FALSE32, FALSE32, FALSE32, FALSE32},	/* 2Gb SLC(K9K2G08) DDP  */
		{0xDA, 0x80, 2048,  64, 4, FALSE32, FALSE32, FALSE32, FALSE32, FALSE32},	/* 2Gb SLC(K9F2G08) Mono */
		{0xDA, 0x10, 2048,  64, 4, TRUE32,  FALSE32, FALSE32, FALSE32, FALSE32},	/* 2Gb SLC(K9F2G08) Mono A-die */
		{0xDC, 0xC1, 4096,  64, 4, FALSE32, FALSE32, FALSE32, TRUE32,  FALSE32},	/* 4Gb SLC(K9K4G08) DDP  */
		{0xDC, 0x10, 4096,  64, 4, TRUE32,  FALSE32, FALSE32, FALSE32, FALSE32},	/* 4Gb SLC(K9F4G08) Mono */
		{0xD3, 0x51, 8192,  64, 4, TRUE32,  FALSE32, FALSE32, TRUE32,  FALSE32},	/* 8Gb SLC(K9K8G08) DDP  */
		{0xD3, 0x10, 4096,  64, 8, TRUE32,  TRUE32,  TRUE32,  FALSE32, FALSE32},	/* 8Gb SLC(K9F8G08) Mono  */
		{0xD5, 0x51, 8192,  64, 8, TRUE32,  TRUE32,  TRUE32,  TRUE32,  FALSE32},	/* 16Gb SLC(K9KAG08) DDP  */
#endif
#if (WMR_MLC_SUPPORT)
		/* MLC NAND ID TABLE */
		{0xDC, 0x14, 2048, 128, 4, TRUE32,  FALSE32, FALSE32, FALSE32, TRUE32},    /* 4Gb MLC(K9G4G08) Mono */
		{0xD3, 0x55, 4096, 128, 4, TRUE32,  FALSE32, FALSE32, TRUE32,  TRUE32},	   /* 8Gb MLC(K9L8G08) DDP  */
		{0xD3, 0x14, 4096, 128, 4, TRUE32,  FALSE32, FALSE32, FALSE32, TRUE32},	   /* 8Gb MLC(K9G8G08) Mono */
		{0xD5, 0x55, 8192, 128, 4, TRUE32,  FALSE32, FALSE32, TRUE32,  TRUE32},	   /* 16Gb MLC(K9LAG08) DDP  */
		{0xD5, 0x14, 4096, 128, 8, FALSE32, FALSE32, FALSE32, FALSE32, TRUE32},	   /* 16Gb MLC(K9GAG08) Mono */
		{0xD7, 0x55, 8192, 128, 8, FALSE32, FALSE32, FALSE32, TRUE32,  TRUE32},	   /* 32Gb MLC(K9LBG08) DDP  */
		/* SLC NAND ID TABLE */
		{0xF1, 0x80, 1024,  64, 4, FALSE32, FALSE32, FALSE32, FALSE32, FALSE32},	/* 1Gb SLC(K9F1G08) Mono */
		{0xF1, 0x00, 1024,  64, 4, FALSE32, FALSE32, FALSE32, FALSE32, FALSE32},	/* 1Gb SLC(K9F1G08) Mono B-die */
		{0xDA, 0xC1, 2048,  64, 4, FALSE32, FALSE32, FALSE32, FALSE32, FALSE32},	/* 2Gb SLC(K9K2G08) DDP  */
		{0xDA, 0x80, 2048,  64, 4, FALSE32, FALSE32, FALSE32, FALSE32, FALSE32},	/* 2Gb SLC(K9F2G08) Mono */
		{0xDA, 0x10, 2048,  64, 4, TRUE32,  FALSE32, FALSE32, FALSE32, FALSE32},	/* 2Gb SLC(K9F2G08) Mono A-die */
		{0xDC, 0xC1, 4096,  64, 4, FALSE32, FALSE32, FALSE32, TRUE32,  FALSE32},	/* 4Gb SLC(K9K4G08) DDP  */
		{0xDC, 0x10, 4096,  64, 4, TRUE32,  FALSE32, FALSE32, FALSE32, FALSE32},	/* 4Gb SLC(K9F4G08) Mono */
		{0xD3, 0x51, 8192,  64, 4, TRUE32,  FALSE32, FALSE32, TRUE32,  FALSE32},	/* 8Gb SLC(K9K8G08) DDP  */
		{0xD3, 0x10, 4096,  64, 8, TRUE32,  TRUE32,  TRUE32,  FALSE32, FALSE32},	/* 8Gb SLC(K9F8G08) Mono  */
		{0xD5, 0x51, 8192,  64, 8, TRUE32,  TRUE32,  TRUE32,  TRUE32,  FALSE32},	/* 16Gb SLC(K9KAG08) DDP  */
		{0xD7, 0xD5, 8192, 128, 8, FALSE32, FALSE32, FALSE32, TRUE32,  TRUE32},		/* 32Gb MLC(K9LBG08) DDP D-die */
#endif
};


/*****************************************************************************/
/* PRIVATE variables definitions                                             */
/*****************************************************************************/
BOOL32			bInternalInterleaving 	= TRUE32;
PRIVATE 	BOOL32			aNeedSync[WMR_MAX_DEVICE * 2];


#define NAND_PAGE_SIZE  4096

// {hyunkwon] 42nm MLC NAND
#if (WMR_4ECC_SUPPORT)
#define NAND_SPAGE_SIZE  		128
#define NAND_ECC_CODE_SIZE 	9
#define NAND_ECC_ERROR_LIMIT	0x04
#endif

#if (WMR_8ECC_SUPPORT)
#define NAND_SPAGE_SIZE  		218
#define NAND_ECC_CODE_SIZE 	18
#define NAND_ECC_ERROR_LIMIT	0x08
#endif

/* This structure should be modified by Samsung's define */
#if (WMR_4ECC_SUPPORT)
typedef struct {
	UINT8	cBadMark;			/* 1 bytes bad mark */
	UINT8	cCleanMark;			/* 1 Byte clean mark */
	UINT8	aSECC[9];			/* 9 Bytes Spare ECC code by RS ECC */
	UINT8	cReserved0;
	INT32 	aSpareData[5];		/* 20 bytes Spare data --> H/W Ecc coverage */
	UINT32  nReserved1[2];
	UINT8 	aMECC[8][9];		/* 8*9 Bytes Main ECC code by RS ECC */
} MLCSpareCxt4K;
#endif

#if (WMR_8ECC_SUPPORT)
typedef struct {
	UINT8	cBadMark;			/* 1 bytes bad mark */
	UINT8	cCleanMark;			/* 1 Byte clean mark */
	UINT8	aSECC[9];			/* 9 Bytes Spare ECC code by RS ECC */
	UINT8	cReserved0;
	INT32 	aSpareData[5];		/* 20 bytes Spare data --> H/W Ecc coverage */
	UINT32  nReserved1[2];
	UINT8 	aMECC[8][18];		/* 8*18 Bytes Main ECC code by RS ECC */
} MLCSpareCxt4K;
#endif


typedef struct {
	UINT32  AppData;
	UINT8	WC2;
	UINT8	BI;
	UINT8	Reserved;
	UINT8	ECC[NAND_ECC_CODE_SIZE];
} MLCSpareCxt512Byte;


PRIVATE VOID FSL_NFC_MemorySet_32Bytes(VOID *pAddress, UINT8 nData, UINT32 nByteSize);
PRIVATE VOID FSL_NFC_MemoryCopy_32Bytes(void *pSourceAddress, void *pTargetAddress, UINT32 nByteSize);
PRIVATE VOID FSL_NFC_MemorySet_218(VOID *pAddress, UINT8 nData, UINT32 nByteSize);
PRIVATE VOID FSL_NFC_MemoryCopy_218(void *pSourceAddress, void *pTargetAddress, UINT32 nByteSize);


PRIVATE UINT32 _IsAllFF(UINT8* pBuf, UINT32 nSize)
{
	register UINT32 nIdx;
	register UINT32 nLoop;
	UINT32 *pBuf32;

	pBuf32 = (UINT32 *)pBuf;
	nLoop = nSize / sizeof(UINT32);

	for (nIdx = nLoop; nIdx > 0; nIdx--)
	{
		if(*pBuf32++ != 0xFFFFFFFF)
		{
			return FALSE32;
		}
	}

	return TRUE32;
}

#ifndef USE_ASM
PRIVATE VOID FSL_NFC_MemorySet_32Bytes(VOID *pAddress, UINT8 nData, UINT32 nByteSize)
{
  UINT32 i;

  for(i = 0; i < nByteSize/4; i++)
     *(UINT32 *)((UINT32 *)pAddress+i) = (nData<<24) |( nData<<16) | (nData<<8) | nData;;
}


PRIVATE VOID FSL_NFC_MemoryCopy_32Bytes(void *pSourceAddress, void *pTargetAddress, UINT32 nByteSize)
{
	UINT32 i;

	for(i=0; i< nByteSize/4; i++)
		*(UINT32 *)((UINT32 *)pTargetAddress+i) = *(UINT32 *)((UINT32 *)pSourceAddress+i);
}

PRIVATE VOID FSL_NFC_MemorySet_218(VOID *pAddress, UINT8 nData, UINT32 nByteSize)
{
  UINT32 i;

  for(i = 0; i < nByteSize; i++)
     *(UINT8 *)((UINT8 *)pAddress+i) = nData;;
}


PRIVATE VOID FSL_NFC_MemoryCopy_218(void *pSourceAddress, void *pTargetAddress, UINT32 nByteSize)
{
	UINT32 i;

	for(i=0; i< nByteSize; i++)
		*(UINT8 *)((UINT8 *)pTargetAddress+i) = *(UINT8 *)((UINT8 *)pSourceAddress+i);
}

#endif


#ifdef USE_MACRO
#define FSL_NFC_Command_Input(a) {\
						*(volatile UINT32 *)NAND_ADD_CMD  = (a); \
						*(volatile UINT32 *)LAUNCH_NFC = 0x0001;	  \
						while (!(*(volatile UINT32 *)(NFC_IPC) >> 31)); \
						reg32_write(NFC_IPC,reg32_read(NFC_IPC)&0x7FFFFFFF);\
						}
#else
PRIVATE INT32 FSL_NFC_Command_Input(UINT16 nCommand)
{
	INT32 eRetVal = FIL_SUCCESS;

	*(volatile UINT32 *)NAND_ADD_CMD  = nCommand;
	*(volatile UINT32 *)LAUNCH_NFC = 0x0001;

	while (!(*(volatile UINT32 *)(NFC_IPC) >> 31));
	reg32_write(NFC_IPC,reg32_read(NFC_IPC)&0x7FFFFFFF);

  	return eRetVal;
}
#endif


#ifdef USE_MACRO
#define FSL_NFC_Address_Input(a)	{\
					    *(volatile UINT32 *)NAND_ADD_CMD = ((a)<<16); \
					    *(volatile UINT32 *)LAUNCH_NFC = 0x0002; \
						while (!(*(volatile UINT32 *)(NFC_IPC) >> 31)); \
						reg32_write(NFC_IPC,reg32_read(NFC_IPC)&0x7FFFFFFF); \
						}
#else
PRIVATE INT32 FSL_NFC_Address_Input(UINT32 nAddress)
{
	INT32 eRetVal = FIL_SUCCESS;

   *(volatile UINT32 *)NAND_ADD_CMD = (nAddress<<16);
   *(volatile UINT32 *)LAUNCH_NFC = 0x0002;

	while (!(*(volatile UINT32 *)(NFC_IPC) >> 31));
	reg32_write(NFC_IPC,reg32_read(NFC_IPC)&0x7FFFFFFF);

	return eRetVal;

}
#endif



#ifdef USE_MACRO
#define FSL_NFC_Data_Input() {\
						*(volatile UINT32 *)LAUNCH_NFC = 0x0004; \
						while (!(*(volatile UINT32 *)(NFC_IPC) >> 31)); \
						reg32_write(NFC_IPC,reg32_read(NFC_IPC)&0x7FFFFFFF); \
						}
#else
PRIVATE INT32 FSL_NFC_Data_Input(VOID)
{
	INT32 eRetVal = FIL_SUCCESS;

	*(volatile UINT32 *)LAUNCH_NFC = 0x0004;
	while (!(*(volatile UINT32 *)(NFC_IPC) >> 31));
	reg32_write(NFC_IPC,reg32_read(NFC_IPC)&0x7FFFFFFF);

  	return eRetVal;

}
#endif




#ifdef USE_MACRO
#define FSL_NFC_Data_Output(a) {\
						switch (a) { \
							case NAND_OUT_PAGE_DATA: \
								*(volatile UINT32 *)LAUNCH_NFC = 0x0008;\
								break;\
							case NAND_OUT_ID: \
								*(volatile UINT32 *)LAUNCH_NFC = 0x0010;\
								break;\
							case NAND_OUT_STATUS: \
								*(volatile UINT32 *)LAUNCH_NFC = 0x0020;\
								break;\
							default: \
								return FIL_CRITICAL_ERROR;\
						}\
						while (!(*(volatile UINT32 *)(NFC_IPC) >> 31));\
						reg32_write(NFC_IPC,reg32_read(NFC_IPC)&0x7FFFFFFF);\
						}

#else
PRIVATE INT32 FSL_NFC_Data_Output(eNandDataOutputType eOutputType)
{
	INT32 eRetVal = FIL_SUCCESS;

	switch (eOutputType) {
		case NAND_OUT_PAGE_DATA:
			*(volatile UINT32 *)LAUNCH_NFC = 0x0008;
			break;
		case NAND_OUT_ID:
			*(volatile UINT32 *)LAUNCH_NFC = 0x0010;
			break;
		case NAND_OUT_STATUS:
			*(volatile UINT32 *)LAUNCH_NFC = 0x0020;
			break;
		default:
			return FIL_CRITICAL_ERROR;
	}

	while (!(*(volatile UINT32 *)(NFC_IPC) >> 31));
	reg32_write(NFC_IPC,reg32_read(NFC_IPC)&0x7FFFFFFF);

	return eRetVal;
}
#endif


#ifdef USE_MACRO
#define FSL_NFC_SetConfiguration1(a,b,c,d) {\
						reg32_write(NFC_CONFIG1, (((a%4) << 8) | ((b) << 4) | ((c) << 2) | (d)));\
						*(volatile unsigned long *)NFC_WR_PROTECT = 0x20004 | ((a%4) << 20);\
}

#else
PRIVATE VOID FSL_NFC_SetConfiguration1(UINT32 nBank, UINT32 nRBA, BOOL32 bNFCRst, BOOL32 bSPR_EN)
{
	UINT32 nNewConfig;

	nNewConfig = ((nBank << 8) | (nRBA << 4) | (bNFCRst << 2) | (bSPR_EN));
	reg32_write(NFC_CONFIG1, nNewConfig);
	*(volatile unsigned long *)NFC_WR_PROTECT = 0x20004 | (nBank << 20);
}
#endif

/*
NFC_CONFIG1   4-6 bit, nBufferNum range from 0 to 7

000 - Use 1st 1/2K of the internal ram.
001 - Use 2nd 1/2K of the internal ram
010 - Use 3rd 1/2K of the internal ram
011- Use 4th 1/2K of the internal ram
100- Use 5th1/2K of the internal ram
101- Use 6th 1/2K of the internal ram
110- Use 7th 1/2K of the internal ram
111- Use 8th 1/2K of the internal ram


LAUNCH_NFC
3-5 bit FDO
001 - One page data out
010 - Flash ID data out
100 - Flash status data out

2 bit FDI
0 - No nand flash data input operation
1 - Enable nand flash data input operation

1 bit FADD
0 - No nand flash address input operation
1 - Enable nand flash address input operation


0 bit FCMD
0 - No nand flash command input operation
1 - Enable nand flash command input operation

*/

PRIVATE VOID FSL_NFC_Address_Cycle(UINT32 nPageNum, BOOL32 bSendColumnAddr, BOOL32 bSendRowAddr)
{
	/* 4k Page, 5 cycles */
	UINT32 nColumnAddr = 0;
	UINT32 nRowAddr = 0;

	nRowAddr = nPageNum;

	if (bSendColumnAddr == TRUE32) {
		FSL_NFC_Address_Input(nColumnAddr & 0xFF);
		FSL_NFC_Address_Input((nColumnAddr >> 8) & 0xFF);
	}

	if (bSendRowAddr == TRUE32) {
		FSL_NFC_Address_Input(nRowAddr & 0xFF);
		FSL_NFC_Address_Input((nRowAddr >> 8) & 0xFF);
		FSL_NFC_Address_Input((nRowAddr >> 16) & 0xFF);
	}

}



PRIVATE INT32 FSL_NFC_Check_ECC(VOID)
{
	UINT32 nECCState, nError;
	UINT32 nNoSubpages = 1;
	INT32 eRetVal = FIL_SUCCESS;
	UINT8 nECCBitMask, nErrLimit;

	nECCBitMask = 0xF;
	nErrLimit = NAND_ECC_ERROR_LIMIT;

	nNoSubpages = NAND_SUBPAGE_NUM;

	nECCState = reg32_read(ECC_STATUS_RESULT);
	do {
		nError = nECCState & nECCBitMask;
		if (nError > nErrLimit)
			return FIL_U_ECC_ERROR;

		nECCState >>= 4;
	} while (--nNoSubpages);

	return eRetVal;
}


PRIVATE VOID FSL_NFC_Read_Done(UINT32 nBank)
{
	int nTimeout = 100000; //TIMEOUT_NFC_BUSY;	// i'm not sure this timeout value

	if (nBank == 0)
	{
		while(NFC_BUSY_CHIP0 && (nTimeout-- > 0));
	}
	else if (nBank == 1)
	{
		while(NFC_BUSY_CHIP1 && (nTimeout-- > 0));
	}
	else if (nBank == 2)
	{
		while(NFC_BUSY_CHIP2 && (nTimeout-- > 0));
	}
	else if (nBank == 3)
	{
		while(NFC_BUSY_CHIP3 && (nTimeout-- > 0));
	}

	if(nTimeout <= 0)
	{
		NAND_ERR_PRINT(("[ERROR] FSL_NFC_Read_Done() Timeout!!!\n"));
		NAND_Init_Refresh(); // 2009.9.23 by Trial
	}
}

PRIVATE UINT32 FSL_NFC_Read_Status(UINT32 nBank)
{
	UINT32 nStatus;

	FSL_NFC_SetConfiguration1(nBank, 0, FALSE32, FALSE32);
	FSL_NFC_Command_Input(NAND_CMD_READ_STATUS);
  	FSL_NFC_Data_Output(NAND_OUT_STATUS);

	nStatus = (*(volatile UINT32 *)(NFC_MAB0_BASE)) & 0xFF ;

	return nStatus;
}

PRIVATE INT32 FSL_NFC_Wait_Ready(UINT32 nBank, UINT32 *nStatus, UINT32 nTimeOut, BOOL32 bUseInt)
{
	INT32 i;
	INT32 eRetVal = FIL_SUCCESS;

	if ((nBank > 1) || (nBank < 0))
		return FIL_CRITICAL_ERROR;

	/* Check the R/B */
	if (nBank == 0) {
		for (i = 0; i < nTimeOut; i++) {
		    if (!NFC_BUSY_CHIP0)
		    	break;
		}

		if (NFC_BUSY_CHIP0)
		{
			eRetVal = FIL_TIMEOUT;
			return eRetVal;
		}
	} else if (nBank == 1) {
		for (i = 0; i < nTimeOut; i++) {
		    if (!NFC_BUSY_CHIP1)
		    	break;
		}

		if (NFC_BUSY_CHIP1)
		{
			eRetVal = FIL_TIMEOUT;
			return eRetVal;
		}
	} else if (nBank == 2) {
		for (i = 0; i < nTimeOut; i++) {
		    if (!NFC_BUSY_CHIP2)
		    	break;
		}

		if (NFC_BUSY_CHIP2)
		{
			eRetVal = FIL_TIMEOUT;
			return eRetVal;
		}
	} else if (nBank == 3) {
		for (i = 0; i < nTimeOut; i++) {
		    if (!NFC_BUSY_CHIP3)
		    	break;
		}

		if (NFC_BUSY_CHIP3)
		{
			eRetVal = FIL_TIMEOUT;
			return eRetVal;
		}
	}
	*nStatus = FSL_NFC_Read_Status(nBank);

	return eRetVal;
}

/* For internal interleave use only */
PRIVATE INT32 FSL_NFC_Wait_Internal_Ready(UINT32 nBank, UINT32 *nStatus, UINT32 nTimeOut)
{
	INT32 eRetVal = FIL_SUCCESS;
	UINT16 nCommand;
	INT32 i;
	UINT32 nRBank;

	nRBank = nBank/2;

	FSL_NFC_SetConfiguration1(nRBank,0, FALSE32, FALSE32);
	if ((nBank % 2) == 0) {
		/* Check CE0 */
		nCommand = NAND_CMD_READ_STATUS_CE0;
	} else {
		/* Check CE1 */
		nCommand = NAND_CMD_READ_STATUS_CE1;
	}

	for (i = 0; i < nTimeOut; i++) {
		FSL_NFC_Command_Input(nCommand);
		FSL_NFC_Data_Output(NAND_OUT_STATUS);
		*nStatus = (*(volatile UINT32 *)(NFC_MAB0_BASE)) & 0xFF ;
		if (*nStatus & NAND_STATUS_READY_BIT)
			break;
	}

	if (i == nTimeOut)
		return FIL_TIMEOUT;

	return eRetVal;
}


PRIVATE VOID FSL_NFC_Block_Unlock(UINT32 nBank, UINT16 nStartBlockNum, UINT16 nEndBlockNum)
{
	int nData2write;
	int nNewCs;
	nNewCs = (0x3 & nBank)<<20;   //CS
	nData2write = 0x00020004 | nNewCs;

	*(volatile UINT32 *)(UNLOCK_BLK_ADD0 + 0x4*nBank) = (nEndBlockNum<<16)|nStartBlockNum;
	*(volatile UINT32 *)NFC_WR_PROTECT = nData2write;
}

PRIVATE VOID FSL_NFC_Enable_ECC(BOOL32 bEnable)
{
	UINT32 nECCMask;

	if (bEnable == TRUE32) {
		nECCMask = 0xFFFFFFFF;
		reg32_write(NFC_CONFIG2,reg32_read(NFC_CONFIG2) | (0x1 << 3));
	} else {
		nECCMask = ~(0x1 << 3);
		*(volatile UINT32 *)NFC_CONFIG2 &= nECCMask;
	}

}

PRIVATE BOOL32 FSL_NFC_IS_ALL_FF(UINT32 *pSBuf)
{
	UINT32 nIndex;
	for (nIndex = 0; nIndex < ((NAND_SPAGE_SIZE/NAND_SUBPAGE_NUM)>>2); nIndex++) {
		if(*(pSBuf+nIndex) != 0xFFFFFFFF)
			return FALSE32;
	}
	return TRUE32;

}

PRIVATE INT32 FSL_NFC_Read_FullPage(UINT32 nBank, UINT32 nPpn, UINT32 nSctBitmap, UINT8 *pDBuf, UINT8 *pSBuf, BOOL32 isEnableEcc, BOOL32 bCleanCheck)
{
	INT32 eRetVal = FIL_SUCCESS;
	UINT32 nRBank;
	UINT32 nIndex, nInc, nLen;
	MLCSpareCxt512Byte *pSparePriv;
	MLCSpareCxt4K *pSpare;
	UINT32 * pSpare2;
	UINT32 *pMainArea;
	UINT32 *pDBufTemp;
	BOOL32	bRetry = FALSE32;	/* Try to read again */


	if (bInternalInterleaving == TRUE32) {
		nRBank = nBank/2;
		if(nBank % 2 == 1)
		{
			nPpn += PAGES_PER_BANK;
		}
	} else {
		nRBank = nBank;
	}

_B_RetryRead:
	FSL_NFC_SetConfiguration1(nRBank, 0, FALSE32, FALSE32);

	FSL_NFC_Enable_ECC(isEnableEcc);
	FSL_NFC_Command_Input(NAND_CMD_READ);
	FSL_NFC_Address_Cycle(nPpn, TRUE32, TRUE32);
	FSL_NFC_Command_Input(NAND_CMD_READ_CONFIRM);

	//NAND_Sync(nBank, &nSyncRet);
	FSL_NFC_Read_Done(nRBank);
	FSL_NFC_Data_Output(NAND_OUT_PAGE_DATA);


	if (isEnableEcc)
	{
		/* Check ECC if correct */
		if(FIL_U_ECC_ERROR == FSL_NFC_Check_ECC())
		{
			if (bRetry == FALSE32)
			{
				bRetry = TRUE32;
				goto _B_RetryRead;
			}
			return FIL_U_ECC_ERROR;
		}
	}


	if (pDBuf != NULL) {
		if (nSctBitmap == 0xFF) {
			/* Full Page read */
			FSL_NFC_MemoryCopy_32Bytes((void*)NFC_MAB0_BASE, (void*)pDBuf, NAND_PAGE_SIZE);
		} else {
			/* Dummy Random read, Copy Only need Area */
			pMainArea = (UINT32 *)NFC_MAB0_BASE;
			pDBufTemp = (UINT32 *)pDBuf;
			nInc = NFC_MAB1_BASE - NFC_MAB0_BASE;
			nLen = NAND_PAGE_SIZE/NAND_SUBPAGE_NUM;

			for (nIndex = 0; nIndex < NAND_SUBPAGE_NUM; nIndex++) {
				if ((nSctBitmap >> nIndex) & 0x01)
					FSL_NFC_MemoryCopy_32Bytes((void*)pMainArea, (void*)pDBufTemp, nLen);
				pMainArea += (nInc>>2);
				pDBufTemp += (nInc>>2);
			}
		}
	}

	if (pSBuf != NULL) {
		pSpare = (MLCSpareCxt4K *)pSBuf;
		pSparePriv = (MLCSpareCxt512Byte *)NFC_SAB0_BASE;
		nInc = NFC_SAB1_BASE - NFC_SAB0_BASE;

		for (nIndex = 0; nIndex < NAND_SUBPAGE_NUM; nIndex ++) {
			if (nIndex == 0) {
				pSpare->cBadMark = pSparePriv->AppData & 0xFF;
				pSpare->cCleanMark = (pSparePriv->AppData & 0xFF00) >> 8;
			}
			if((nIndex > 0) && (nIndex < 6)) {
				pSpare->aSpareData[nIndex-1] = pSparePriv->AppData;
			}
			pSparePriv = (UINT8 *)pSparePriv + nInc;
		}

		if(bCleanCheck) {
			/* Only need to check if Spare all 0xFF */
			pSpare2 = (UINT32 *)NFC_SAB0_BASE;

			for(nIndex = 0; nIndex < NAND_SUBPAGE_NUM; nIndex++) {
				if (!FSL_NFC_IS_ALL_FF(pSpare2)) {
					/* As ECC is checked, it just mean the page is used */
					return FIL_SUCCESS;
				}
				pSpare2 += (nInc>>2);
			}
			return FIL_SUCCESS_CLEAN;
		}
	}

	return eRetVal;
}


PRIVATE INT32 FSL_NFC_Write_FullPage(UINT32 nBank, UINT32 nPpn, UINT32 nSctBitmap, UINT8 *pDBuf, UINT8 *pSBuf, BOOL32 isEnableEcc)
{
	INT32 eRetVal = FIL_SUCCESS;
	UINT32 nRBank;
	UINT32 nIndex, nInc, nLen;
	MLCSpareCxt512Byte *pSparePriv;
	MLCSpareCxt4K *pSpare;
	UINT32 *pMainArea;
	UINT32 *pDBufTemp;


	if(pDBuf == NULL)
		return FIL_CRITICAL_ERROR;

	if (bInternalInterleaving == TRUE32) {
		nRBank = nBank/2;
		if(nBank % 2 == 1)
		{
			nPpn += PAGES_PER_BANK;
		}
	} else {
		nRBank = nBank;
	}

	FSL_NFC_SetConfiguration1(nRBank, 0, FALSE32, FALSE32);
	FSL_NFC_Enable_ECC(isEnableEcc);

	if (nSctBitmap == 0xFF) {
		FSL_NFC_MemoryCopy_32Bytes((void*)pDBuf, (void*)NFC_MAB0_BASE, NAND_PAGE_SIZE);
	} else {
		/* Copy Only need Area to Main buffer */
		pMainArea = (UINT32 *)NFC_MAB0_BASE;
		pDBufTemp = (UINT32 *)pDBuf;
		nInc = NFC_MAB1_BASE - NFC_MAB0_BASE;
		nLen = NAND_PAGE_SIZE/NAND_SUBPAGE_NUM;

		for (nIndex = 0; nIndex < NAND_SUBPAGE_NUM; nIndex++) {
			if ((nSctBitmap >> nIndex) & 0x01)
				FSL_NFC_MemoryCopy_32Bytes((void*)pDBufTemp, (void*)pMainArea, nLen);
			pMainArea += (nInc>>2);
			pDBufTemp += (nInc>>2);
		}
	}

	//FSL_NFC_MemoryCopy((void*)pDBuf, (void*)NFC_MAB0_BASE, NAND_PAGE_SIZE);

	if(pSBuf != NULL) {
		pSpare = (MLCSpareCxt4K *)pSBuf;
		pSpare->cCleanMark = 0x00;
		pSparePriv = (MLCSpareCxt512Byte *)NFC_SAB0_BASE;
		pSparePriv->AppData = pSpare->cBadMark | (pSpare->cCleanMark << 8);
		nInc = NFC_SAB1_BASE - NFC_SAB0_BASE;
		nLen = NAND_SPAGE_SIZE/NAND_SUBPAGE_NUM;
		for (nIndex = 0; nIndex < 5; nIndex++) {
			pSparePriv = (UINT8 *)pSparePriv + nInc;
			pSparePriv->AppData = pSpare->aSpareData[nIndex];
		}
		//FSL_NFC_MemoryCopy((void*)pSBuf, (void*)NFC_SAB0_BASE, NAND_SPAGE_SIZE);
	}

	FSL_NFC_Command_Input(NAND_CMD_PAGE_PROG);
	FSL_NFC_Address_Cycle(nPpn,TRUE32,TRUE32);
	FSL_NFC_Data_Input();
	FSL_NFC_Command_Input(NAND_CMD_PAGE_PROG_CONFIRM);

	return eRetVal;
}

PRIVATE INT32 _Read_ID(UINT32 nBank, UINT8 *pDID, UINT8 *pHID)
{
	UINT8 nMID, nDID, nHID;
	UINT32 nScanIdx;
	//INT32 eRetVal = FIL_SUCCESS;

/*
	FSL_NFC_SetConfiguration1(nBank, 0, FALSE32, FALSE32);
	FSL_NFC_Command_Input(NAND_CMD_READID);
	FSL_NFC_Address_Input(0);
	FSL_NFC_Data_Output(NAND_OUT_ID);

	nResult  = *(volatile UINT32 *)(NFC_MAB0_BASE);
	nMID = (UINT8)nResult;
	nDID = (UINT8)(nResult >> 8);
	nHID = (UINT8)(nResult >> 16);
*/
	NAND_Read_ID(nBank, &nMID, &nDID, &nHID);

	/* check empty bank */
	if ( nMID == nDID || nDID == nHID )
	{
		*pDID = 0x00;
		*pHID = 0x00;

		return FIL_CRITICAL_ERROR;
	}

	for(nScanIdx = 0; nScanIdx < sizeof(stDEVInfo)/sizeof(DEVInfo); nScanIdx++)
	{
		if((nMID == (UINT8)0xEC) && (nDID == stDEVInfo[nScanIdx].nDevID) && (nHID == stDEVInfo[nScanIdx].nHidID))
		{
			*pDID = nDID;
			*pHID = nHID;

			NAND_LOG_PRINT(("=======================\r\n"));
			NAND_LOG_PRINT(("[FIL:MSG] _Read_ID(%d)\r\n", nBank));
			NAND_LOG_PRINT(("[FIL:MSG] MID = %X\r\n", nMID));
			NAND_LOG_PRINT(("[FIL:MSG] DID = %X\r\n", nDID));
			NAND_LOG_PRINT(("[FIL:MSG] HID = %X\r\n", nHID));
			NAND_LOG_PRINT(("=======================\r\n"));

			NAND_LOG_PRINT (("[FIL:MSG] Bank %d Detect\r\n", nBank));
			return nScanIdx;
		}
	}

	FSL_NFC_MemorySet_32Bytes(((VOID *)NFC_MAB0_BASE), 0, 512);

	*pDID = 0x00;
	*pHID = 0x00;

	return FIL_CRITICAL_ERROR;
}




/*********************************************************************************
NAND API
*********************************************************************************/
/* For external interleave only support: Write/Erase */
/* For NAND_Sync(), external interleave sync is called by VFL layer,
    internal interleave NAND_Sync is called inside Read/Write/Erase */


VOID NAND_Read_ID(UINT32 nBank, UINT8 *pMID, UINT8 *pDID, UINT8 *pHID)
{
	UINT32 nResult=0;

	FSL_NFC_SetConfiguration1(nBank, 0, FALSE32, FALSE32);
	FSL_NFC_Command_Input(NAND_CMD_READID);
	FSL_NFC_Address_Input(0);
	FSL_NFC_Data_Output(NAND_OUT_ID);

	nResult  = *(volatile UINT32 *)(NFC_MAB0_BASE);
	*pMID = (UINT8)nResult;
	*pDID = (UINT8)(nResult >> 8);
	*pHID = (UINT8)(nResult >> 16);
}

INT32 NAND_Init_Refresh(VOID)
{
	/* Set GPIO2_7 mode - for RB0 Signal*/
	(*(volatile unsigned long *)(GPIO2_BASE_ADDR + GPIO_GDIR0_OFFSET)) &= ~(0x1 << 7);
	/* Set GPIO3_10 mode - for RB1 Signal*/
	(*(volatile unsigned long *)(GPIO3_BASE_ADDR + GPIO_GDIR0_OFFSET)) &= ~(0x1 << 10);
	/* Set GPIO1_3 mode - for RB2 Signal*/
	(*(volatile unsigned long *)(GPIO1_BASE_ADDR + GPIO_GDIR0_OFFSET)) &= ~(0x1 << 3);
	/* Set GPIO3_4 mode - for RB3 Signal*/	// 1. MUX 2. PAD
	reg32_write((IOMUXC_BASE_ADDR + 0x144), 0x04);
	(*(volatile unsigned long *)(GPIO3_BASE_ADDR + GPIO_GDIR0_OFFSET)) &= ~(0x1 << 4);
	/* Set NFC R/B to GPIO to avoid limitaion about interleave */
	reg32_write((IOMUXC_BASE_ADDR + 0x80),0x4);

	FSL_NFC_SetConfiguration1(0, 0, TRUE32, FALSE32);

	return FIL_SUCCESS;
}

INT32 NAND_Init(VOID)
{
	UINT32 nChipNum;
	UINT32 nScanIdx = 0, nCompIdx = 0;
	UINT8 nDevID, nHiddenID;
	UINT32 nDevCnt = 0;
	//UINT32 nTmp;
	BOOL32 bComp = FALSE32;
	BOOL32 bRet;

	/* Set clock for NFC */
	reg32_write(CCM_CBCDR7,(reg32_read(CCM_CBCDR7)& 0xfffffff8) | 0x00000003);
	reg32_write(CCM_CCDR,reg32_read(CCM_CCDR)| 0x00000001);

	/* Config NFC */
	reg32_write(NFC_IPC,0x00000001);
	/* Default ECC enabled */
#if (WMR_4ECC_SUPPORT)
	*(volatile UINT32 *)NFC_CONFIG2 = 0x0040114A;
#endif
#if (WMR_8ECC_SUPPORT)
	*(volatile UINT32 *)NFC_CONFIG2 = 0x006D110A;
#endif
	reg32_write(NFC_IPC,0x00000000);

	/* Set GPIO2_7 mode - for RB0 Signal*/
	(*(volatile unsigned long *)(GPIO2_BASE_ADDR + GPIO_GDIR0_OFFSET)) &= ~(0x1 << 7);

	/* Set GPIO3_10 mode - for RB1 Signal*/
	(*(volatile unsigned long *)(GPIO3_BASE_ADDR + GPIO_GDIR0_OFFSET)) &= ~(0x1 << 10);

	/* Set GPIO1_3 mode - for RB2 Signal*/
	(*(volatile unsigned long *)(GPIO1_BASE_ADDR + GPIO_GDIR0_OFFSET)) &= ~(0x1 << 3);

	/* Set GPIO3_4 mode - for RB3 Signal*/	// 1. MUX 2. PAD
	reg32_write((IOMUXC_BASE_ADDR + 0x144), 0x04);
	(*(volatile unsigned long *)(GPIO3_BASE_ADDR + GPIO_GDIR0_OFFSET)) &= ~(0x1 << 4);

	/* Set NFC R/B to GPIO to avoid limitaion about interleave */
	reg32_write((IOMUXC_BASE_ADDR + 0x80),0x4);


	/* Unlock all area in all chips */
	for (nChipNum = 0; nChipNum < NAND_MAX_CS; nChipNum++) {
		FSL_NFC_Block_Unlock(nChipNum, 0, (NAND_BLOCKS_NUM_PER_BANK - 1));
	}

	for (nChipNum = 0; nChipNum < NAND_MAX_CS; nChipNum++) {
		/* Also reset NFC */
		FSL_NFC_SetConfiguration1(nChipNum, 0, TRUE32, FALSE32);
		FSL_NFC_Command_Input(NAND_CMD_RESET);
	}

	for (nChipNum = 0; nChipNum < NAND_MAX_CS; nChipNum++) {
		nScanIdx = _Read_ID(nChipNum, &nDevID, &nHiddenID);

		if (nScanIdx == FIL_CRITICAL_ERROR)
		{
			if (nDevID == 0x00 && nCompIdx == 0)
			{
				NAND_ERR_PRINT(("[ERROR] not found NAND flash\n"));
				return FIL_CRITICAL_ERROR;
			}
			nScanIdx = nCompIdx;
			break;
		}
		if ((nCompIdx != nScanIdx) && (bComp))
		{
			NAND_ERR_PRINT(("[ERROR] different NAND flashs\n"));
			return FIL_CRITICAL_ERROR;
		}

		nCompIdx = nScanIdx;
		bComp = TRUE32;
		nDevCnt ++;
	}

	if(stDEVInfo[nScanIdx].bMLC)
	{
		SET_DevType(WMR_MLC);
		//IS_MLC = 1;
	}
	else
	{
		//SET_DevType(WMR_SLC);
		//IS_SLC = 1;
		SET_DevType(WMR_MLC);
	}

// [Hyunkwon] K9PDG08U5D Àû¿ë : chip count = 4
//	if(stDEVInfo[nScanIdx].bDualDie && nDevCnt <= 2)
	if(stDEVInfo[nScanIdx].bDualDie && nDevCnt <= 4)
	{
		/* multi chip dual die (DDP) */
		BLOCKS_PER_BANK = stDEVInfo[nScanIdx].nNumOfBlocks / 2;
		BANKS_TOTAL = nDevCnt * 2;
		bInternalInterleaving = TRUE32;
	}
	else
	{
		BLOCKS_PER_BANK = stDEVInfo[nScanIdx].nNumOfBlocks;
		BANKS_TOTAL = nDevCnt;
		bInternalInterleaving = FALSE32;
	}

	SECTORS_PER_PAGE = stDEVInfo[nScanIdx].nSectorsPerPage;

	TWO_PLANE_PROGRAM = stDEVInfo[nScanIdx].b2XProgram;

	if(TWO_PLANE_PROGRAM == TRUE32)
	{
		BLOCKS_PER_BANK /= 2;
	}

// [Hyunkwon] : Use only one plane
///////////////////////////////////////////////////////////////////////////////////////
#if 1
    /* 16Gbit MLC x 4 = QDP --> use only one plane and reduce super block size for performance  */
    if ( (nDevID == 0xD7 && nHiddenID == 0x55) && nDevCnt == 2 )
    {
        if ( TWO_PLANE_PROGRAM == TRUE32 ) /* always TRUE32 */
        {
            BLOCKS_PER_BANK  *= 2;

            TWO_PLANE_PROGRAM = FALSE32;

            TWO_PLANE_READ    = FALSE32;
        }
    }
#endif
///////////////////////////////////////////////////////////////////////////////////////

	#if (WMR_PROGRAM_PER_PLANE)
/* MLC 4K NAND only uses PROGRAM_PER_PLANE */
/*	if(TWO_PLANE_PROGRAM == TRUE32) */
	if(TWO_PLANE_PROGRAM == TRUE32 &&
		SECTORS_PER_PAGE == 8 &&
		stDEVInfo[nScanIdx].bMLC )
	{
		PROGRAM_PER_PLANE = TRUE32;
	}
    else
    {
		PROGRAM_PER_PLANE = FALSE32;
    }
	#endif


	TWO_PLANE_READ = stDEVInfo[nScanIdx].b2XRead;

	TWO_PLANE_READ_STATUS = stDEVInfo[nScanIdx].b2XReadStatus;

	PAGES_PER_BLOCK = stDEVInfo[nScanIdx].nPagesPerBlock;

	/* DDP */
	if (bInternalInterleaving)
	{
		for (nChipNum = 0; nChipNum < nDevCnt; nChipNum++)
		{
			aNeedSync[nChipNum * 2] = FALSE32;
			aNeedSync[nChipNum * 2 + 1] = FALSE32;
		}
	}

	#if (WMR_MLC_SUPPORT)
	/* initialize buffer for ecc generation */
//	WMR_MEMSET(aMakeSECCBuf, 0xFF, BYTES_PER_SECTOR);
	#endif

#if (WMR_MLC_LSB_RECOVERY)
	MLC_LSB_CLASS = GetMlcClass( stDEVInfo[nScanIdx].nDevID,
								 stDEVInfo[nScanIdx].nHidID);
#endif

	CalcGlobal();

	if (BANKS_TOTAL <= 4)	/* except K9HCG08U5D */
	{
		/* Set clock for NFC */
		reg32_write(CCM_CBCDR7,(reg32_read(CCM_CBCDR7)& 0xfffffff8) | 0x00000001); /* Experimental value */
		reg32_write(CCM_CCDR,reg32_read(CCM_CCDR)| 0x00000001);
	}
	else
	{
		reg32_write(CCM_CBCDR7,(reg32_read(CCM_CBCDR7)& 0xfffffff8) | 0x00000003);
		reg32_write(CCM_CCDR,reg32_read(CCM_CCDR)| 0x00000001);
	}

	/*************************************************************************/
	/* display the result of FSL_NAND_Init                                       */
	/*************************************************************************/
	NAND_INF_PRINT(("=======================\n"));
	NAND_INF_PRINT(("[INFO] tinywmr : NFC_NAND_Init()\n"));
	NAND_INF_PRINT(("[INFO] BANKS_TOTAL = %d\n", BANKS_TOTAL));
	NAND_INF_PRINT(("[INFO] BLOCKS_PER_BANK = %d\n", BLOCKS_PER_BANK));
	NAND_INF_PRINT(("[INFO] SUPPORT_INTERLEAVING = %d\n", IS_SUPPORT_INTERLEAVING));
	NAND_INF_PRINT(("[INFO] SUBLKS_TOTAL = %d\n", SUBLKS_TOTAL));
	NAND_INF_PRINT(("[INFO] PAGES_PER_SUBLK = %d\n", PAGES_PER_SUBLK));
	NAND_INF_PRINT(("[INFO] PAGES_PER_BANK = %d\n", PAGES_PER_BANK));
	NAND_INF_PRINT(("[INFO] SECTORS_PER_PAGE = %d\n", SECTORS_PER_PAGE));
	NAND_INF_PRINT(("[INFO] SECTORS_PER_SUPAGE = %d\n", SECTORS_PER_SUPAGE));
	NAND_INF_PRINT(("[INFO] SECTORS_PER_SUBLK = %d\n", SECTORS_PER_SUBLK));
	NAND_INF_PRINT(("[INFO] USER_SECTORS_TOTAL = %d\n", USER_SECTORS_TOTAL));
	NAND_INF_PRINT(("[INFO] ADDRESS_CYCLE = %d\n", DEV_ADDR_CYCLE));
	NAND_INF_PRINT(("[INFO] TWO_PLANE_READ = %d\n", TWO_PLANE_READ));
	NAND_INF_PRINT(("[INFO] TWO_PLANE_READ_STATUS = %d\n", TWO_PLANE_READ_STATUS));
	NAND_INF_PRINT(("[INFO] TWO_PLANE_PROGRAM = %d\n", TWO_PLANE_PROGRAM));
	NAND_INF_PRINT(("[INFO] CCM_CBCDR7 = 0x%x\n", reg32_read(CCM_CBCDR7)));
	#if (WMR_PROGRAM_PER_PLANE)
	NAND_INF_PRINT(("[INFO] PROGRAM_PER_PLANE = %d\n", PROGRAM_PER_PLANE));
	#endif
	#if (WMR_MLC_LSB_RECOVERY)
	NAND_INF_PRINT(("[INFO] MLC_LSB_RECOVERY:MLC_LSB_CLASS = %d\n", MLC_LSB_CLASS));
	#endif
	NAND_INF_PRINT(("=======================\n"));


#ifdef MJXU
	/* define value for Samsung */

	if (bInternalInterleaving == TRUE32) {
		BANKS_TOTAL = 4;
		PAGES_PER_BANK = 128*4096;
		BLOCKS_PER_BANK = 4096;
	} else {
		BANKS_TOTAL = 2;
		PAGES_PER_BANK = 128*8192;
		BLOCKS_PER_BANK = 8192;
	}

#endif

	return FIL_SUCCESS;
}


/* For read, not two-plane, no internal/external interleave is supported */

INT32 NAND_Read	 (UINT32 nBank,	UINT32 nPpn, UINT32 nSctBitmap, UINT32 nPlaneBitmap, UINT8 *pDBuf,	UINT8 *pSBuf, BOOL32 bECCIn, BOOL32 bCleanCheck)

{

	INT32 eRetVal = FIL_SUCCESS;


	if(nBank >= BANKS_TOTAL|| nPpn >= PAGES_PER_BANK || (pDBuf == NULL && pSBuf == NULL))
	{
		/* Error parameter */
		NAND_ERR_PRINT((TEXT("[FIL:ERR]  parameter overflow on NAND_Read!\r\n")));
		WMR_ASSERT(FALSE32);
		return FIL_CRITICAL_ERROR;
	}

	if ((nPlaneBitmap == enuBOTH_PLANE_BITMAP) && (USE_TWO_PLANE)) {
		/* Two plane operation */
	} else {
		/* Single plane operation */
		eRetVal = FSL_NFC_Read_FullPage(nBank, nPpn, nSctBitmap, pDBuf, pSBuf, !bECCIn, bCleanCheck);
	}

	if ((eRetVal != FIL_SUCCESS) && (eRetVal != FIL_SUCCESS_CLEAN))
	{
		if (eRetVal == FIL_U_ECC_ERROR)
		{
			NAND_ERR_PRINT(("[NAND_Read] Fail nBank = %d, nPpn = %d Error is FIL_U_ECC_ERROR\n", nBank, nPpn));
		}
		else
		{
		NAND_ERR_PRINT(("[NAND_Read] Fail nBank = %d, nPpn = %d \n", nBank, nPpn));
	}
	}

	return eRetVal;


}

VOID NAND_Write (UINT32 nBank, UINT32 nPpn, UINT32 nSctBitmap, UINT32 nPlaneBitmap, UINT8 *pDBuf, UINT8 *pSBuf)

{

	INT32 eRetVal = FIL_SUCCESS;
	BOOL32 bECCIn = TRUE32;

	if(nBank >= BANKS_TOTAL|| nPpn >= PAGES_PER_BANK || (pDBuf == NULL && pSBuf == NULL))
	{
		/* Error parameter */
		NAND_ERR_PRINT((TEXT("[FIL:ERR]  parameter overflow on NAND_Write!\r\n")));
		WMR_ASSERT(FALSE32);
		return;
	}


	if ((nPlaneBitmap == enuBOTH_PLANE_BITMAP) && (USE_TWO_PLANE)) {
		/* Two plane operation */
	} else {
		/* Single plane operation */
		eRetVal = FSL_NFC_Write_FullPage(nBank, nPpn, nSctBitmap, pDBuf, pSBuf, bECCIn);
	}

	return;

}


INT32 NAND_Sync(UINT32 nBank, UINT32 *nPlaneBitmap)
{
	INT32 eRetVal = FIL_SUCCESS;
	UINT32 nStatus = 0;

	if (nBank >= BANKS_TOTAL)
	{
		NAND_ERR_PRINT((TEXT("[FIL:ERR]  parameter overflow on NAND_Sync!\r\n")));
		WMR_ASSERT(FALSE32);
		return FIL_CRITICAL_ERROR;
	}

	WMR_ASSERT(nPlaneBitmap != NULL);
	NAND_LOG_PRINT((TEXT("[FIL : IN] ++NAND_Sync() nBank = %d, nPlaneBitmap = %d\r\n"), nBank, nPlaneBitmap));

	if (bInternalInterleaving == TRUE32) {
		/* Check internal RB status */
		eRetVal = FSL_NFC_Wait_Internal_Ready(nBank, &nStatus, TIMEOUT_NFC_BUSY);
		if(nStatus & NAND_PLANE0_STATUS_ERROR_BIT) {
			/* Left plane is error */
			*nPlaneBitmap = enuLEFT_PLANE_BITMAP;
			return FIL_CRITICAL_ERROR;
		} else if (nStatus & NAND_PLANE1_STATUS_ERROR_BIT) {
			/* Right plane is error */
			*nPlaneBitmap = enuRIGHT_PLANE_BITMAP;
			return FIL_CRITICAL_ERROR;
		}

	} else {
		/* Check RB */
		eRetVal = FSL_NFC_Wait_Ready(nBank, &nStatus, TIMEOUT_NFC_BUSY, FALSE32);

		if(nStatus & NAND_STATUS_ERROR_BIT)
			return FIL_CRITICAL_ERROR;
	}

	return eRetVal;
}


VOID NAND_Erase (UINT32 nBank,	UINT32 nPbn,	UINT32 nPlaneBitmap)
{

	UINT32 nRBank;
	UINT32 nBitmap;
	BOOL32 isEnableEcc = TRUE32;
	UINT32 nPpn;


	NAND_LOG_PRINT((TEXT("NAND_Erase start bank = %d, block = %d\r\n"), nBank, nPbn));

	if (nBank >= BANKS_TOTAL || nPbn >= BLOCKS_PER_BANK)
	{
		NAND_ERR_PRINT((TEXT("[FIL:ERR]  parameter overflow on NAND_Erase!\r\n")));
		WMR_ASSERT(FALSE32);
		return;
	}

	if (bInternalInterleaving == TRUE32) {
		nRBank = nBank/2;
		if(nBank % 2 == 1)
		{
			nPbn += BLOCKS_PER_BANK;
		}
	} else {
		nRBank = nBank;
	}

	nPpn = nPbn << NAND_PAGE_ADDRESS_WIDTH;

	FSL_NFC_SetConfiguration1(nRBank,0, FALSE32, FALSE32);
	FSL_NFC_Enable_ECC(isEnableEcc);
	FSL_NFC_Command_Input(NAND_CMD_ERASE);
	FSL_NFC_Address_Cycle(nPpn ,FALSE32,TRUE32);
	FSL_NFC_Command_Input(NAND_CMD_ERASE_CONFIRM);

	NAND_Sync(nBank, &nBitmap);

}


VOID NAND_Reset(UINT32 nBank)
{
	//INT32 eRetVal = FIL_SUCCESS;
	//UINT32 nNewConfig1;

	FSL_NFC_SetConfiguration1(nBank,0, FALSE32, FALSE32);
	FSL_NFC_Command_Input(NAND_CMD_RESET);

	return;
}

#if 0
/***********************************************/

/*************** Test code ***********************/


BOOL32	SS_CmpWMRConfig	(WMRGlobalConfig stWMRGloalConfig)
{
	return TRUE32;
}
BOOL32	SS_CompDevID		(UINT8 nBank, UINT8 nMID, UINT8 nDID, UINT8 nHID, UINT32 *pIDIdx )
{
	return TRUE32;
}

BOOL32	SS_CalcDeviceInfo	(INT32(*_Read_ID)(UINT32 nBank, UINT8 *pDID, UINT8 *pHID),
										WMRGlobalConfig 	 stGblCfg,
										UINT32 				 nDevIDIdx,
										UINT32 				 nDevCnt,
										BOOL32 				*bInternalInterleaving
									)
{
	return TRUE32;
}


VOID	SS_CalcGlobal			(WMRGlobalConfig stWMRGloalConfig, BOOL32 bInternalInterleaving)
{
	return;
}


PRIVATE UINT32 _IsAllSame(UINT8* pBuf, UINT32 nValue, UINT32 nSize)
{
	register UINT32 nIdx;
	register UINT32 nLoop;
	UINT32 *pBuf32;

	pBuf32 = (UINT32 *)pBuf;
	nLoop = nSize / sizeof(UINT32);

	for (nIdx = nLoop; nIdx > 0; nIdx--)
	{
		if(*pBuf32++ != nValue)
		{
			return FALSE32;
		}
	}

	return TRUE32;
}


INT32 main (VOID)
{
	UINT32 nBank;
	UINT32 nPpn;
	UINT32 nBitMask;
	UINT32 tmp = 0;
	INT32 eRetVal = FIL_SUCCESS;
	UINT32 eBitMap;
	UINT32 tmpIndex;
	UINT32 tmpIndex2;
	UINT32 nBlockNum;
	UINT8 *tmpData;
	UINT32 *tmpRecord;
	UINT32 nMark = 0;
	UINT32 nPrev = 0;
	UINT32 nBlkNum = 0;
	UINT8 DBufR_WRITE[NAND_PAGE_SIZE+3];
	UINT8 SBufR_WRITE[NAND_SPAGE_SIZE+3];
	UINT8 DBufR_READ[NAND_PAGE_SIZE+3];
	UINT8 SBufR_READ[NAND_SPAGE_SIZE+3];
	UINT8 SBufR2[NAND_SPAGE_SIZE+3];
	UINT8 Record[4000+15];
	UINT8 nRamData, nRamData2;
	UINT32 nRamData3;

	UINT32 *pDBufR_WRITE;
	UINT32 *pSBufR_WRITE;
	UINT32 *pDBufR_READ;
	UINT32 *pSBufR_READ;
	UINT32 *pSBufR2;
	UINT32 *pRecord;
	MLCSpareCxt4K sSpare;


	/* Make 4 bytes alignment */
	pDBufR_WRITE = (UINT32 *)(((UINT32)DBufR_WRITE + 3)&(~0x3));
	pSBufR_WRITE = (UINT32 *)(((UINT32)SBufR_WRITE + 3)&(~0x3));
	pDBufR_READ =  (UINT32 *)(((UINT32)DBufR_READ + 3)&(~0x3));
	pSBufR_READ =  (UINT32 *)(((UINT32)SBufR_READ + 3)&(~0x3));
	pSBufR2 =  (UINT32 *)(((UINT32)SBufR2 + 3)&(~0x3));
	pRecord =  (UINT32 *)(((UINT32)Record + 3)&(~0x3));



	FSL_NFC_MemorySet((VOID *)pDBufR_WRITE, 0x00, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pSBufR_WRITE, 0x00, NAND_SPAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pDBufR_READ, 0x00, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pSBufR_READ, 0x00, NAND_SPAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pRecord, 0x00, 1000*4);
	FSL_NFC_MemorySet((VOID *)pSBufR2, 0x00, NAND_SPAGE_SIZE);



	FSL_NFC_MemorySet((VOID *)pSBufR2, 0x11, NAND_SPAGE_SIZE);
	FSL_NFC_MemoryCopy((VOID *)pSBufR2, (VOID *)pSBufR_WRITE, NAND_SPAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pSBufR2, 0x00, NAND_SPAGE_SIZE);



	nPpn = 34;
	nBitMask = 0x0A;

	eRetVal = NAND_Init();

	tmpRecord = (UINT32 *)pRecord;
	FSL_NFC_MemorySet((VOID *)pDBufR_WRITE, 0x00, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pSBufR_WRITE, 0x00, NAND_SPAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pDBufR_READ, 0x00, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pSBufR_READ, 0x00, NAND_SPAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pRecord, 0x00, 1000*4);


	//nBlockNum = nPpn >> 7;
	//NAND_Erase(0, nBlockNum, enuNONE_PLANE_BITMAP);
	//eRetVal = NAND_Read(0, nPpn, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,(UINT8 *)SBufR, FALSE32, FALSE32);

	//eRetVal = NAND_Read(0, nPpn, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,(UINT8 *)SBufR, TRUE32, FALSE32);
	//eRetVal = NAND_Read(0, nPpn, 0 ,enuNONE_PLANE_BITMAP, NULL,(UINT8 *)SBufR2, TRUE32, FALSE32);

	/* All erease */
	for (tmpIndex = 0; tmpIndex < BLOCKS_PER_BANK; tmpIndex++) {
		NAND_Erase(0, tmpIndex, enuNONE_PLANE_BITMAP);
		NAND_Erase(1, tmpIndex, enuNONE_PLANE_BITMAP);
		NAND_Erase(2, tmpIndex, enuNONE_PLANE_BITMAP);
		NAND_Erase(3, tmpIndex, enuNONE_PLANE_BITMAP);
		eRetVal = NAND_Sync(0, &eBitMap);
	}

	FSL_NFC_MemorySet((VOID *)pDBufR_WRITE, 0x11, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pSBufR_WRITE, 0x22, NAND_SPAGE_SIZE);


	for (tmpIndex = 0; tmpIndex < 10000; tmpIndex++) {
			//nRamData = tmpIndex%0xFF;
			//nRamData2 = (tmpIndex+1)%0xFF;
			//FSL_NFC_MemorySet((VOID *)pDBufR_WRITE, nRamData, NAND_PAGE_SIZE);
			//FSL_NFC_MemorySet((VOID *)pSBufR_WRITE, nRamData2, NAND_SPAGE_SIZE);
			NAND_Write(0, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_WRITE, (UINT8 *)pSBufR_WRITE);
			NAND_Write(1, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_WRITE, (UINT8 *)pSBufR_WRITE);
			NAND_Write(2, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_WRITE, (UINT8 *)pSBufR_WRITE);
			NAND_Write(3, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_WRITE, (UINT8 *)pSBufR_WRITE);
			eRetVal = NAND_Sync(0, &eBitMap);
			eRetVal = NAND_Sync(1, &eBitMap);
			eRetVal = NAND_Sync(2, &eBitMap);
			eRetVal = NAND_Sync(3, &eBitMap);
	}

	FSL_NFC_MemorySet((VOID *)pDBufR_READ, 0x00, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pSBufR_READ, 0x00, NAND_SPAGE_SIZE);


	for (tmpIndex = 0; tmpIndex < 10000; tmpIndex++) {
			NAND_Read(0, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_READ, NULL, FALSE32, FALSE32);
			NAND_Read(1, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_READ, NULL, FALSE32, FALSE32);
			NAND_Read(2, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_READ, NULL, FALSE32, FALSE32);
			NAND_Read(3, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_READ, NULL, FALSE32, FALSE32);
	}

	FSL_NFC_MemorySet((VOID *)pDBufR_READ, 0xFF, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pSBufR_READ, 0xFF, NAND_SPAGE_SIZE);


	for (tmpIndex = 0; tmpIndex < 10000; tmpIndex++) {
			nRamData = tmpIndex%0xFF;
			nRamData2 = (tmpIndex+1)%0xFF;
			nRamData3 = nRamData | (nRamData << 8) | (nRamData << 16) |(nRamData << 24);

			FSL_NFC_MemorySet((VOID *)pDBufR_READ, 0xFF, NAND_PAGE_SIZE);
			NAND_Read(0, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_READ,(UINT8 *)pSBufR_READ, FALSE32, FALSE32);
			if (!_IsAllSame((UINT8 *)pDBufR_READ, nRamData3, NAND_PAGE_SIZE))
				while(1);
			FSL_NFC_MemorySet((VOID *)pDBufR_READ, 0xFF, NAND_PAGE_SIZE);
			NAND_Read(1, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_READ,(UINT8 *)pSBufR_READ, FALSE32, FALSE32);
			if (!_IsAllSame((UINT8 *)pDBufR_READ, nRamData3, NAND_PAGE_SIZE))
				while(1);
			FSL_NFC_MemorySet((VOID *)pDBufR_READ, 0xFF, NAND_PAGE_SIZE);
			NAND_Read(2, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_READ,(UINT8 *)pSBufR_READ, FALSE32, FALSE32);
			if (!_IsAllSame((UINT8 *)pDBufR_READ, nRamData3, NAND_PAGE_SIZE))
				while(1);
			FSL_NFC_MemorySet((VOID *)pDBufR_READ, 0xFF, NAND_PAGE_SIZE);
			NAND_Read(3, tmpIndex, 0xFF ,enuNONE_PLANE_BITMAP, (UINT8 *)pDBufR_READ,(UINT8 *)pSBufR_READ, FALSE32, FALSE32);
			if (!_IsAllSame((UINT8 *)pDBufR_READ, nRamData3, NAND_PAGE_SIZE))
				while(1);

	}

	FSL_NFC_MemorySet((VOID *)pDBufR_READ, 0xFF, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)pSBufR_READ, 0xFF, NAND_SPAGE_SIZE);

#if 0
	for (tmpIndex = 3456; tmpIndex < 3459; tmpIndex++) {
	FSL_NFC_MemorySet((VOID *)DBufR, 0x00, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)SBufR, 0x00, NAND_SPAGE_SIZE);
		eRetVal = NAND_Read(0, tmpIndex, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,(UINT8 *)SBufR, TRUE32, FALSE32);


		if (eRetVal == FIL_SUCCESS) {
			tmpData = (UINT8 *)DBufR;
			for(tmpIndex2 = 0; tmpIndex2 < NAND_PAGE_SIZE; tmpIndex2++ ) {
				if(*tmpData != 0xFF)
					while(1);
				tmpData++;
			}
			tmpData = (UINT8 *)SBufR;
			for(tmpIndex2 = 0; tmpIndex2 < NAND_SPAGE_SIZE; tmpIndex2++ ) {
				if(*tmpData != 0xFF)
					while(1);
				tmpData++;
			}
		} else {
			if (eRetVal == FIL_U_ECC_ERROR)
				nMark = tmpIndex | (0x1 << 30);
			else
				nMark = tmpIndex;

			if ((nMark >> 7) == (nPrev >> 7)) /* Same block */
				continue;
			nPrev = nMark;
			*tmpRecord = nMark;
			tmpRecord ++;
		}
	}

	for (tmpIndex = 3423; tmpIndex < 3434; tmpIndex++) {
		FSL_NFC_MemorySet((VOID *)DBufR, 0x00, NAND_PAGE_SIZE);
		FSL_NFC_MemorySet((VOID *)SBufR, 0x00, NAND_SPAGE_SIZE);
		eRetVal = NAND_Read(1, tmpIndex, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,(UINT8 *)SBufR, TRUE32, FALSE32);


		if (eRetVal == FIL_SUCCESS) {
			tmpData = (UINT8 *)DBufR;
			for(tmpIndex2 = 0; tmpIndex2 < NAND_PAGE_SIZE; tmpIndex2++ ) {
				if(*tmpData != 0xFF)
					while(1);
				tmpData++;
			}

			tmpData = (UINT8 *)SBufR;
			for(tmpIndex2 = 0; tmpIndex2 < NAND_SPAGE_SIZE; tmpIndex2++ ) {
				if(*tmpData != 0xFF)
					while(1);
				tmpData++;
			}
		} else {
			if (eRetVal == FIL_U_ECC_ERROR)
				nMark = tmpIndex | (0x1 << 31)| (0x1 << 30);
			else
				nMark = tmpIndex | (0x1 << 31);

			if ((nMark >> 7) == (nPrev >> 7)) /* Same block */
				continue;
			nPrev = nMark;
			*tmpRecord = nMark;
			tmpRecord ++;
		}
	}

	for (tmpIndex = 2344; tmpIndex < 2355; tmpIndex++) {
		FSL_NFC_MemorySet((VOID *)DBufR, 0x99, NAND_PAGE_SIZE);
		FSL_NFC_MemorySet((VOID *)SBufR, 0x00, NAND_SPAGE_SIZE);
		FSL_NFC_MemorySet((VOID *)(&sSpare), 0x00, NAND_SPAGE_SIZE);
		//eRetVal = NAND_Read(0, tmpIndex, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,(UINT8 *)SBufR, TRUE32, FALSE32);

		sSpare.cBadMark = 0x11;
		sSpare.cCleanMark = 0x22;
		sSpare.aSpareData[0] = 0x11111111;
		sSpare.aSpareData[1] = 0x22222222;
		sSpare.aSpareData[2] = 0x33333333;
		sSpare.aSpareData[3] = 0x44444444;
		sSpare.aSpareData[4] = 0x55555555;

		FSL_NFC_MemoryCopy((VOID *)(&sSpare), (VOID *)SBufR, NAND_SPAGE_SIZE);
		NAND_Write(0, tmpIndex, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR, (UINT8 *)SBufR);

		FSL_NFC_MemorySet((VOID *)SBufR, 0x00, NAND_SPAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)DBufR, 0x00, NAND_PAGE_SIZE);
		eRetVal = NAND_Read(0, tmpIndex, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,(UINT8 *)SBufR, TRUE32, FALSE32);

		NAND_Erase(0, tmpIndex >> 7, enuNONE_PLANE_BITMAP);
	FSL_NFC_MemorySet((VOID *)SBufR, 0x00, NAND_SPAGE_SIZE);
		FSL_NFC_MemorySet((VOID *)DBufR, 0x00, NAND_PAGE_SIZE);
		eRetVal = NAND_Read(0, tmpIndex, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,(UINT8 *)SBufR, TRUE32, FALSE32);
	}


	eRetVal = NAND_Read(1, nPpn, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,(UINT8 *)SBufR, TRUE32, FALSE32);
	eRetVal = NAND_Read(0, nPpn, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,(UINT8 *)SBufR, TRUE32, FALSE32);

	FSL_NFC_MemorySet((VOID *)DBufR, 0x44, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)SBufR, 0x33, NAND_SPAGE_SIZE);

	NAND_Write(1, nPpn, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,NULL);
	NAND_Write(0, nPpn, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,NULL);

	eRetVal = NAND_Sync(1, &eBitMap);
	eRetVal = NAND_Sync(0, &eBitMap);

	FSL_NFC_MemorySet((VOID *)DBufR, 0x00, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)SBufR, 0x00, NAND_SPAGE_SIZE);

	eRetVal = NAND_Read(1, nPpn, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,(UINT8 *)SBufR, TRUE32, FALSE32);
	eRetVal = NAND_Read(0, nPpn, 0 ,enuNONE_PLANE_BITMAP, (UINT8 *)DBufR,(UINT8 *)SBufR, TRUE32, FALSE32);



	FSL_NFC_Erase(1, nPpn);
	FSL_NFC_Erase(0, nPpn);
	while(!(FSL_NFC_Erase_Done(1) == FIL_SUCCESS));
	while(!(FSL_NFC_Erase_Done(0) == FIL_SUCCESS));


	eRetVal = FSL_NFC_Read(1, nPpn, NAND_NONE_PLANE_BITMAP, (UINT8 *)DBufR, (UINT8 *)SBufR, TRUE32);
	eRetVal = FSL_NFC_Read(0, nPpn, NAND_NONE_PLANE_BITMAP, (UINT8 *)DBufR, (UINT8 *)SBufR, TRUE32);
	while(!(FSL_NFC_Read_Done(0, (UINT8 *)DBufR, (UINT8 *)SBufR, TRUE32) == FIL_SUCCESS));
	while(!(FSL_NFC_Read_Done(1, (UINT8 *)DBufR, (UINT8 *)SBufR, TRUE32) == FIL_SUCCESS));

	FSL_NFC_MemorySet((VOID *)DBufR, 0x33, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)SBufR, 0x44, NAND_SPAGE_SIZE);

	eRetVal = FSL_NFC_Write(1, nPpn, NAND_NONE_PLANE_BITMAP, (UINT8 *)DBufR, NULL ,TRUE32);
	eRetVal = FSL_NFC_Write(0, nPpn, NAND_NONE_PLANE_BITMAP, (UINT8 *)DBufR, NULL ,TRUE32);
	hile(!(_NFC_Write_Done(1) == FIL_SUCCESS));
	while(!(_NFC_Write_Done(0) == FIL_SUCCESS));

	FSL_NFC_MemorySet((VOID *)DBufR, 0x00, NAND_PAGE_SIZE);
	FSL_NFC_MemorySet((VOID *)SBufR, 0x00, NAND_SPAGE_SIZE);

	FSL_NFC_Read(1, nPpn, NAND_NONE_PLANE_BITMAP, (UINT8 *)DBufR, (UINT8 *)SBufR, TRUE32);
	FSL_NFC_Read(0, nPpn, NAND_NONE_PLANE_BITMAP, (UINT8 *)DBufR, (UINT8 *)SBufR, TRUE32);
	while(!(FSL_NFC_Read_Done(0, (UINT8 *)DBufR, (UINT8 *)SBufR, TRUE32) == FIL_SUCCESS));
	while(!(FSL_NFC_Read_Done(1, (UINT8 *)DBufR, (UINT8 *)SBufR, TRUE32) == FIL_SUCCESS));

	tmp = 1000;
#endif
	return 0;
}
#endif

