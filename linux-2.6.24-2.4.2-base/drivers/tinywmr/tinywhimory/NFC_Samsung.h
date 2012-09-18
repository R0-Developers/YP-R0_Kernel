/*==================================================================================================

    Module Name:  NFC_Samsung.h    
    
 ===================================================================================================
    Copyright (C) 2004-2008 Freescale Semiconductor, Inc. All Rights Reserved
    THIS SOURCE CODE IS CONFIDENTIAL AND PROPRIETARY AND MAY NOT
    BE USED OR DISTRIBUTED WITHOUT THE WRITTEN PERMISSION OF
    Freescale Semiconductor, Inc.

 */


/* include */
#ifndef __SAMSUNG_NAND_H__
#define __SAMSUNG_NAND_H__



#include <asm/arch/mx37.h>
#include "mx37_registers.h"

//#define NULL ((VOID *)0)

/* define */
#define NAND_STATUS_READY_BIT (0x40)
#define NAND_STATUS_ERROR_BIT (0x1)
#define NAND_PLANE0_STATUS_ERROR_BIT (0x2)
#define NAND_PLANE1_STATUS_ERROR_BIT (0x4)


#define RAM_BUFFER_SIZE (0x200)
#define RAM_BUFFER_COUNT (4)
/* define max chips select */
#define NAND_MAX_CS	(4)

/* define block size */
#define NAND_PAGE_ADDRESS_WIDTH   0x7   /* 7 for 128 pages per block */

/* define number of address cycle*/
#define NAND_ADDRESS_CYCLE 5

/***********************************************************/
/* NAND Flash Command Set for 8-bit I/O                    */
/***********************************************************/
#define NAND_CMD_READ              (0x0000)
#define NAND_CMD_READ_CONFIRM      (0x0030)
#define NAND_CMD_READ_COPY_CONFIRM (0x0035)
#define NAND_CMD_PAGE_PROG         (0x0080)
#define NAND_CMD_PAGE_PROG_CONFIRM (0x0010)
#define NAND_CMD_ERASE             (0x0060)
#define NAND_CMD_ERASE_CONFIRM     (0x00D0)
#define NAND_CMD_READID            (0x0090)
#define NAND_CMD_RESET             (0x00FF)
#define NAND_CMD_READ_STATUS       (0x0070)
#define NAND_CMD_2PAGE_PROG         (0x0081)
#define NAND_CMD_2PAGE_READ              (0x0060)
#define NAND_CMD_PAGE_DUMMY_PROG_CONFIRM (0x0011)
#define NAND_CMD_READ_STATUS_CE0		(0x00F1)
#define NAND_CMD_READ_STATUS_CE1		(0x00F2)


#define TIMEOUT_NFC_BUSY 0xff0000



/* Internal NFC REGs Addresses */
#define NANDFC_BASE_ADDR 0x7FFF0000
#define EMIV2_IP_BASE_ADDR 0xe3fd8000


#define NAND_SUBPAGE_NUM 8

#define NAND_PAGE_NUM   128
#define NAND_BLOCKS_NUM_PER_BANK  8192
#define NAND_BANK_NUM 2


/* Internal NFC REGs Addresses */
/* IP registers: */
#define NFC_WR_PROTECT		(IO_ADDRESS(EMIV2_IP_BASE_ADDR + 0x3000))
#define UNLOCK_BLK_ADD0	(IO_ADDRESS(EMIV2_IP_BASE_ADDR + 0x3004))
#define UNLOCK_BLK_ADD1	(IO_ADDRESS(EMIV2_IP_BASE_ADDR + 0x3008))
#define UNLOCK_BLK_ADD2	(IO_ADDRESS(EMIV2_IP_BASE_ADDR + 0x300C))
#define UNLOCK_BLK_ADD3	(IO_ADDRESS(EMIV2_IP_BASE_ADDR + 0x3010))
#define NFC_CONFIG2		  	(IO_ADDRESS(EMIV2_IP_BASE_ADDR + 0x3014))
#define NFC_IPC				(IO_ADDRESS(EMIV2_IP_BASE_ADDR + 0x3018))
#define AXI_ERR_ADDR		(IO_ADDRESS(EMIV2_IP_BASE_ADDR + 0x301C))

/* EMIV defines */
#define IPLCK				(IO_ADDRESS(EMI_REGISTERS_BASE_ADDR + 0xf00))
#define EICS					(IO_ADDRESS(EMI_REGISTERS_BASE_ADDR + 0xf04))
#define CCM_CBCDR4			(IO_ADDRESS(CCM_BASE_ADDR + CCM_CBCDR4_OFFSET))
#define CCM_CAMR			(IO_ADDRESS(CCM_BASE_ADDR + CCM_CAMR_OFFSET))

/* AXI registers: */
#define NAND_ADD_CMD		(IO_ADDRESS(NANDFC_BASE_ADDR + 0x1e00))
#define NFC_CONFIG1			(IO_ADDRESS(NANDFC_BASE_ADDR + 0x1e04))
#define ECC_STATUS_RESULT	(IO_ADDRESS(NANDFC_BASE_ADDR + 0x1e08))
#define LAUNCH_NFC			(IO_ADDRESS(NANDFC_BASE_ADDR + 0x1e0c))
#define NFC_RAM				(IO_ADDRESS(NANDFC_BASE_ADDR))
#define NFC_MAB0_BASE		(NFC_RAM+0x000)	//  main area buffer0 (0x7FFF0000 - 0x7FFF01FE)
#define NFC_MAB1_BASE		(NFC_RAM+0x200)	//  main area buffer1 (0x7FFF0200 - 0x7FFF03FE)
#define NFC_MAB2_BASE		(NFC_RAM+0x400)	//  main area buffer2 (0x7FFF0400 - 0x7FFF05FE)
#define NFC_MAB3_BASE		(NFC_RAM+0x600)	//  main area buffer3 (0x7FFF0600 - 0x7FFF07FE)
#define NFC_MAB4_BASE		(NFC_RAM+0x800)	//  main area buffer4 (0x7FFF0800 - 0x7FFF09FE)
#define NFC_MAB5_BASE		(NFC_RAM+0xA00)	//  main area buffer5 (0x7FFF0A00 - 0x7FFF0BFE)
#define NFC_MAB6_BASE		(NFC_RAM+0xC00)	//  main area buffer6 (0x7FFF0C00 - 0x7FFF0DFE)
#define NFC_MAB7_BASE		(NFC_RAM+0xE00)	//  main area buffer7 (0x7FFF0E00 - 0x7FFF0FFE)

#define NFC_SAB0_BASE		(NFC_RAM+0x1000)	//  spare area buffer0 (0x7FFF1000 - 0x7FFF103E)
#define NFC_SAB1_BASE		(NFC_RAM+0x1040)	//  spare area buffer1 (0x7FFF1040 - 0x7FFF107E)
#define NFC_SAB2_BASE		(NFC_RAM+0x1080)	//  spare area buffer2 (0x7FFF1080 - 0x7FFF10BE)
#define NFC_SAB3_BASE		(NFC_RAM+0x10C0)	//  spare area buffer3 (0x7FFF10C0 - 0x7FFF10FE)
#define NFC_SAB4_BASE		(NFC_RAM+0x1100)	//  spare area buffer4 (0x7FFF1100 - 0x7FFF113E)
#define NFC_SAB5_BASE		(NFC_RAM+0x1140)	//  spare area buffer5 (0x7FFF1140 - 0x7FFF117E)
#define NFC_SAB6_BASE		(NFC_RAM+0x1180)	//  spare area buffer6 (0x7FFF1180 - 0x7FFF11BE)
#define NFC_SAB7_BASE		(NFC_RAM+0x11C0)	//  spare area buffer7 (0x7FFF11C0 - 0x7FFF11FE)

#define	NFC_NO_ERR				0x00
#define NFC_SB_1BIT_ECC_ERR		0x01
#define NFC_SB_2BIT_ECC_ERR		0x02
#define NFC_PB_1BIT_ECC_ERR		0x04
#define NFC_PB_2BIT_ECC_ERR		0x08
#define NFC_ERASE_ERR			0x10
#define NFC_WRITE_ERR			0x20
#define NFC_READ_ERR			0x40
#define NFC_ILLEGAL_ACCESS		0x80

/* Operation Type */
#define NFC_CMD_RD_PB_ECC		0x0000
#define NFC_CMD_RD_PB_NOECC		0x0100
#define NFC_CMD_RD_SB_ECC		0x0003
#define NFC_CMD_RD_SB_NOECC		0x0103
#define NFC_CMD_WR_PB_ECC		0x0085
#define NFC_CMD_WR_PB_NOECC		0x0185
#define NFC_CMD_WR_SB_ECC		0x008A
#define NFC_CMD_WR_SB_NOECC		0x018A

#define	ECC_WRITE				1
#define NOECC_WRITE				0
#define ECC_READ				1
#define NOECC_READ				0
#define ECC_COPY				1
#define NOECC_COPY				0

// NFC Latency & Wait states defenitions

#define LAT_RD32_MEM 3
#define LAT_WR32_REG 3
#define LAT_WR32_MEM 2
#define LAT_WR16_MEM 2

#define LAT_WR16 2
#define LAT_WR32 3
#define LAT_RD16 3
#define LAT_RD32 4
// read 16 reg,mem & read 32 mem & read after read are all same wait state
// in order not to change  defines in all tests
//i left these defines here
#define LAT_RD16_REG 3
#define LAT_RD16_REG_AftRE 3
#define LAT_RD16_MEM 3
#define LAT_RD16_MEM_AftRE 3
#define LAT_RD32_MEM_AftRE 3
#define LAT_RD32_REG 4
#define LAT_RD32_REG_AftRE 4

#define WS_RD32_MEM 0

#define WS_WR16 0
#define WS_WR32 0
#define WS_RD16 0
#define WS_RD32 0


#define CCM_CBCDR6	(IO_ADDRESS(CCM_BASE_ADDR+CCM_CBCDR6_OFFSET))
#define CCM_CBCDR7	(IO_ADDRESS(CCM_BASE_ADDR+CCM_CBCDR7_OFFSET))
#define CCM_CCDR	(IO_ADDRESS(CCM_BASE_ADDR+CCM_CCDR_OFFSET))


/* i.MX37 */
#define reg8_read(addr)       *((UINT8 *)(addr))
#define reg16_read(addr)      *((UINT16 *)(addr))
#define reg32_read(addr)      *(UINT32 *)((addr))
#define reg8_write(addr,val)  *((UINT8 *)(addr)) = (val)
#define reg16_write(addr,val) *((UINT16 *)(addr)) = (val)
#define reg32_write(addr,val) *((UINT32 *)(addr)) = (val)

/* Micro */
//Modifed by Alex for RB signal which is emulated by GPIO
//#define NFC_BUSY_CHIP0  (!( *(volatile unsigned long *)NFC_IPC & (0x1 << 29)))  // 1-busy;0-idle
#define NFC_BUSY_CHIP0  (!(( *(volatile unsigned long *)(IO_ADDRESS(GPIO2_BASE_ADDR + GPIO_DR0_OFFSET))) & (0x1 << 7)))  // 1-busy;0-idle
#define NFC_BUSY_CHIP1  (!(( *(volatile unsigned long *)(IO_ADDRESS(GPIO3_BASE_ADDR + GPIO_DR0_OFFSET))) & (0x1 << 10)))  // 1-busy;0-idle
#define NFC_BUSY_CHIP2  (!(( *(volatile unsigned long *)(IO_ADDRESS(GPIO1_BASE_ADDR + GPIO_DR0_OFFSET))) & (0x1 << 3)))  // 1-busy;0-idle
#define NFC_BUSY_CHIP3  (!(( *(volatile unsigned long *)(IO_ADDRESS(GPIO3_BASE_ADDR + GPIO_DR0_OFFSET))) & (0x1 << 4)))  // 1-busy;0-idle

/* If use two plane operation */
#define USE_TWO_PLANE (FALSE32)
#define RB_TO_NFC (0)
#define Is_First_Board 	(0)

typedef enum {
	NAND_OUT_ID		= 0x0,
	NAND_OUT_PAGE_DATA,
	NAND_OUT_STATUS
} eNandDataOutputType;



INT32 NAND_Init(VOID);
INT32 NAND_Init_Refresh(VOID);
VOID NAND_Read_ID(UINT32 nBank, UINT8 *pMID, UINT8 *pDID, UINT8 *pHID);
VOID  NAND_Reset(UINT32 nBank);
INT32 NAND_Read(UINT32 nBank,	UINT32 nPpn,	UINT32 nSctBitmap, UINT32 nPlaneBitmap,
					UINT8 *pDBuf,	UINT8 *pSBuf,	BOOL32 bECCIn,	BOOL32 bCleanCheck);
VOID  NAND_Write(UINT32 nBank,	UINT32 nPpn,	UINT32 nSctBitmap, UINT32 nPlaneBitmap,
					UINT8 *pDBuf,	UINT8 *pSBuf);

VOID  NAND_Erase(UINT32 nBank,	UINT32 nPbn,	UINT32 nPlaneBitmap);
INT32  NAND_Sync(UINT32 nBank, UINT32 *nPlaneBitmap);
#endif
