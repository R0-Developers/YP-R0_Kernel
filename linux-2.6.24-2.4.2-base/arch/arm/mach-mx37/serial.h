/*
 * Copyright 2004-2008 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifndef __ARCH_ARM_MACH_MX37_SERIAL_H__
#define __ARCH_ARM_MACH_MX37_SERIAL_H__

#include <asm/arch/mxc_uart.h>

/* UART 1 configuration */
/*!
 * This option allows to choose either an interrupt-driven software controlled
 * hardware flow control (set this option to 0) or hardware-driven hardware
 * flow control (set this option to 1).
 */
/* UART used as wakeup source */
#define UART1_HW_FLOW           0
/*!
 * This specifies the threshold at which the CTS pin is deasserted by the
 * RXFIFO. Set this value in Decimal to anything from 0 to 32 for
 * hardware-driven hardware flow control. Read the HW spec while specifying
 * this value. When using interrupt-driven software controlled hardware
 * flow control set this option to -1.
 */
#define UART1_UCR4_CTSTL        16
/*!
 * This is option to enable (set this option to 1) or disable DMA data transfer
 */
#define UART1_DMA_ENABLE        0
/*!
 * Specify the size of the DMA receive buffer. The minimum buffer size is 512
 * bytes. The buffer size should be a multiple of 256.
 */
#define UART1_DMA_RXBUFSIZE     1024
/*!
 * Specify the MXC UART's Receive Trigger Level. This controls the threshold at
 * which a maskable interrupt is generated by the RxFIFO. Set this value in
 * Decimal to anything from 0 to 32. Read the HW spec while specifying this
 * value.
 */
#define UART1_UFCR_RXTL         16
/*!
 * Specify the MXC UART's Transmit Trigger Level. This controls the threshold at
 * which a maskable interrupt is generated by the TxFIFO. Set this value in
 * Decimal to anything from 0 to 32. Read the HW spec while specifying this
 * value.
 */
#define UART1_UFCR_TXTL         16
/* UART 2 configuration */
#define UART2_HW_FLOW           1
#define UART2_UCR4_CTSTL        16
#define UART2_DMA_ENABLE        1
#define UART2_DMA_RXBUFSIZE     512
#define UART2_UFCR_RXTL         16
#define UART2_UFCR_TXTL         16
/* UART 3 configuration */
#define UART3_HW_FLOW           1
#define UART3_UCR4_CTSTL        16
#define UART3_DMA_ENABLE        1
#define UART3_DMA_RXBUFSIZE     1024
#define UART3_UFCR_RXTL         16
#define UART3_UFCR_TXTL         16
/*
 * UART Chip level Configuration that a user may not have to edit. These
 * configuration vary depending on how the UART module is integrated with
 * the ARM core
 */
/*
 * Is the MUXED interrupt output sent to the ARM core
 */
#define INTS_NOTMUXED           0
#define INTS_MUXED              1
/* UART 1 configuration */
/*!
 * This define specifies whether the muxed ANDed interrupt line or the
 * individual interrupts from the UART port is integrated with the ARM core.
 * There exists a define like this for each UART port. Valid values that can
 * be used are \b INTS_NOTMUXED or \b INTS_MUXED.
 */
#define UART1_MUX_INTS          INTS_MUXED
/*!
 * This define specifies the transmitter interrupt number or the interrupt
 * number of the ANDed interrupt in case the interrupts are muxed. There exists
 * a define like this for each UART port.
 */
#define UART1_INT1              MXC_INT_UART1
/*!
 * This define specifies the receiver interrupt number. If the interrupts of
 * the UART are muxed, then we specify here a dummy value -1. There exists a
 * define like this for each UART port.
 */
#define UART1_INT2              -1
/*!
 * This specifies the master interrupt number. If the interrupts of the UART
 * are muxed, then we specify here a dummy value of -1. There exists a define
 * like this for each UART port.
 */
#define UART1_INT3              -1
/*!
 * This specifies if the UART is a shared peripheral. It holds the shared
 * peripheral number if it is shared or -1 if it is not shared. There exists
 * a define like this for each UART port.
 */
#define UART1_SHARED_PERI       -1
/* UART 2 configuration */
#define UART2_MUX_INTS          INTS_MUXED
#define UART2_INT1              MXC_INT_UART2
#define UART2_INT2              -1
#define UART2_INT3              -1
#define UART2_SHARED_PERI       -1
/* UART 3 configuration */
#define UART3_MUX_INTS          INTS_MUXED
#define UART3_INT1              MXC_INT_UART3
#define UART3_INT2              -1
#define UART3_INT3              -1
#define UART3_SHARED_PERI       SPBA_UART3

#endif				/* __ARCH_ARM_MACH_MX37_SERIAL_H__ */
