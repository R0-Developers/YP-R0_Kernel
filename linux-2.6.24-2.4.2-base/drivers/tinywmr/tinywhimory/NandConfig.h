#ifndef __NAND_CONFIG_H_
#define __NAND_CONFIG_H_

/* CORE TYPE */
#define TCC79X

/* EV BOARD TYPE */
#define _BOARD_VERSION_TCC7901_010_

/* OS TYPE */
#define LINUX_NAND

/* DRIVER TYPE */
#define KERNEL_DRIVER

/* DEVICE TYPE */
//#define NAND_8BIT_ONLY

/* DEFAULT */
#define NAND_INCLUDE

/* DEBUGGING */
//#define NAND_DRV_UART_DEBUG
//#define NAND_DRV_UART_MEASURE

/* Use Virtual Address ? */
#ifdef KERNEL_DRIVER
#define USE_V_ADDRESS
#define PRINTF printk
#else
#define PRINTF printf
#endif

#endif /* __NAND_CONFIG_H_ */

