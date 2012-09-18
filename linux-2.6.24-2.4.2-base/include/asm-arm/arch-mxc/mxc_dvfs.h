/*
 * Copyright 2008 Freescale Semiconductor, Inc. All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*!
 * @defgroup DVFS Dynamic Voltage and Frequency Scaling (DVFS) Driver
 */

/*!
 * @file arch-mxc/mxc_dvfs.h
 *
 * @brief This file contains the DVFS configuration structure definition.
 *
 *
 * @ingroup DVFS
 */

#ifndef __ASM_ARCH_MXC_DVFS_H__
#define __ASM_ARCH_MXC_DVFS_H__

#ifdef __KERNEL__

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/workqueue.h>
#include <linux/device.h>

#define MXC_DVFS_PER_BASE	IO_ADDRESS(GPC_BASE_ADDR + 0x1C4)

/* DVFS PER */
#define MXC_DVFS_PER_LTR0	(MXC_DVFS_PER_BASE)
#define MXC_DVFS_PER_LTR1	(MXC_DVFS_PER_BASE + 0x04)
#define MXC_DVFS_PER_LTR2	(MXC_DVFS_PER_BASE + 0x08)
#define MXC_DVFS_PER_LTR3	(MXC_DVFS_PER_BASE + 0x0C)
#define MXC_DVFS_PER_LTBR0	(MXC_DVFS_PER_BASE + 0x10)
#define MXC_DVFS_PER_LTBR1	(MXC_DVFS_PER_BASE + 0x14)
#define MXC_DVFS_PER_PMCR0	(MXC_DVFS_PER_BASE + 0x18)
#define MXC_DVFS_PER_PMCR1	(MXC_DVFS_PER_BASE + 0x1C)

#define MXC_DVFSLTR0_UPTHR_MASK		0x0FC00000
#define MXC_DVFSLTR0_UPTHR_OFFSET	22
#define MXC_DVFSLTR0_DNTHR_MASK		0x003F0000
#define MXC_DVFSLTR0_DNTHR_OFFSET	16
#define MXC_DVFSLTR0_DIV3CK_MASK	0x00000006
#define MXC_DVFSLTR0_DIV3CK_OFFSET	1

#define MXC_DVFSLTR1_PNCTHR_MASK	0x0000003F
#define MXC_DVFSLTR1_PNCTHR_OFFSET	0
#define MXC_DVFSLTR1_DNCNT_MASK		0x003FC000
#define MXC_DVFSLTR1_DNCNT_OFFSET	14
#define MXC_DVFSLTR1_UPCNT_MASK		0x00003FC0
#define MXC_DVFSLTR1_UPCNT_OFFSET	6
#define MXC_DVFSLTR1_LTBRSR		0x800000
#define MXC_DVFSLTR1_LTBRSH		0x400000

#define MXC_DVFSLTR2_EMAC_MASK		0x000001FF
#define MXC_DVFSLTR2_EMAC_OFFSET	0

#define MXC_DVFSPMCR0_UDCS		0x8000000
#define MXC_DVFSPMCR0_DVFEV		0x800000
#define MXC_DVFSPMCR0_DVFIS		0x400000
#define MXC_DVFSPMCR0_LBMI		0x200000
#define MXC_DVFSPMCR0_LBFL		0x100000
#define MXC_DVFSPMCR0_LBFC_MASK		0xC0000
#define MXC_DVFSPMCR0_LBFC_OFFSET	18
#define MXC_DVFSPMCR0_FSVAIM		0x00008000
#define MXC_DVFSPMCR0_FSVAI_MASK	0x00006000
#define MXC_DVFSPMCR0_FSVAI_OFFSET	13
#define MXC_DVFSPMCR0_WFIM		0x00000400
#define MXC_DVFSPMCR0_WFIM_OFFSET	10
#define MXC_DVFSPMCR0_DVFEN		0x00000010

#define MXC_DVFSPMCR1_P1INM		0x00100000
#define MXC_DVFSPMCR1_P1ISM		0x00080000
#define MXC_DVFSPMCR1_P1IFM		0x00040000
#define MXC_DVFSPMCR1_P4PM		0x00020000
#define MXC_DVFSPMCR1_P2PM		0x00010000

#define MXC_GPC_BASE			IO_ADDRESS(GPC_BASE_ADDR)
#define MXC_GPC_CNTR			(MXC_GPC_BASE + 0x0)
#define MXC_GPC_PGR			(MXC_GPC_BASE + 0x4)
#define MXC_GPC_VCR			(MXC_GPC_BASE + 0x8)

#define MXC_GPCCNTR_GPCIRQM		0x00200000
#define MXC_GPCCNTR_GPCIRQ		0x00100000
#define MXC_GPCCNTR_DVFS1CR		0x00020000
#define MXC_GPCCNTR_ADU			0x00008000
#define MXC_GPCCNTR_STRT		0x00004000
#define MXC_GPCCNTR_FUPD		0x00002000
#define MXC_GPCCNTR_HTRI_MASK		0x0000000F
#define MXC_GPCCNTR_HTRI_OFFSET		0

#define MXC_GPCVCR_VINC_MASK		0x00020000
#define MXC_GPCVCR_VINC_OFFSET		17
#define MXC_GPCVCR_VCNTU_MASK		0x00010000
#define MXC_GPCVCR_VCNTU_OFFSET		16
#define MXC_GPCVCR_VCNT_MASK		0x00007FFF
#define MXC_GPCVCR_VCNT_OFFSET		0

/*
 * Frequency increase threshold. Increase frequency change request
 * will be sent if DVFS counter value will be more than this value.
 */
#define DVFS_UPTHR		(31 << MXC_DVFSLTR0_UPTHR_OFFSET)
#define DVFS_UPTHR_LOW		(8 << MXC_DVFSLTR0_UPTHR_OFFSET)

/*
 * Frequency decrease threshold. Decrease frequency change request
 * will be sent if DVFS counter value will be less than this value.
 */
#define DVFS_DNTHR		(4 << MXC_DVFSLTR0_DNTHR_OFFSET)
#define DVFS_DNTHR_LOW		(0 << MXC_DVFSLTR0_DNTHR_OFFSET)

/*
 * With the CKIH clocked at 22579200 Hz,
 * this setting yields a DIV_3_CLK of 2.75 kHz.
 */
#define DVFS_DIV3CK		(3 << MXC_DVFSLTR0_DIV3CK_OFFSET)

/*
 * DNCNT defines the amount of times the down threshold should be exceeded
 * before DVFS will trigger frequency decrease request.
 */
#define DVFS_DNCNT		(5 << MXC_DVFSLTR1_DNCNT_OFFSET)

/*
 * UPCNT defines the amount of times the up threshold should be exceeded
 * before DVFS will trigger frequency increase request.
 */
#define DVFS_UPCNT		(5 << MXC_DVFSLTR1_UPCNT_OFFSET)

/*
 * Panic threshold. Panic frequency change request
 * will be sent if DVFS counter value will be more than this value.
 */
#define DVFS_PNCTHR		(63 << MXC_DVFSLTR1_PNCTHR_OFFSET)
#define DVFS_PNCTHR_LOW		(20 << MXC_DVFSLTR1_PNCTHR_OFFSET)

#define UPTHR(a)	a << MXC_DVFSLTR0_UPTHR_OFFSET
#define DNTHR(a)	a << MXC_DVFSLTR0_DNTHR_OFFSET
#define PANICTHR(a)	a << MXC_DVFSLTR1_PNCTHR_OFFSET
#define UPCNT(a)	a << MXC_DVFSLTR1_UPCNT_OFFSET;
#define DNCNT(a)	a << MXC_DVFSLTR1_DNCNT_OFFSET;
#define EMAC(a)		a << MXC_DVFSLTR2_EMAC_OFFSET

/* EMAC defines how many samples are included in EMA calculation */
#define DVFS_EMAC		(0x10 << MXC_DVFSLTR2_EMAC_OFFSET)

/*
 * DVFS structure
 */
struct dvfs_per_wp {
	int upthr;
	int downthr;
	int panicthr;
	int upcnt;
	int downcnt;
	int emac;
};

/*!
 * This structure is used to define the dvfs controller's platform
 * data. It includes the regulator name string and DVFS clock name string.
 */
struct mxc_dvfsper_data {
	/** Regulator name string */
	char *reg_id;
	/* DVFS clock name string */
	char *clk_id;
	/* GPC control reg address */
	unsigned int gpc_cntr_reg_addr;
	/* DVFS interrupt status bit */
	unsigned int dvfs_intr;
	/* DVFS enable bit */
	u32 dvfs_enable_bit;
	/* DVFS ADU bit */
	int gpc_adu;
	/* VAI mask */
	u32 vai_mask;
	/* VAI offset */
	int vai_offset;
	/* Mask DVFS interrupt */
	u32 irq_mask;
	/* ARM interrrupt bit */
	u32 gpc_irq_bit;
	/* Div3 clock offset. */
	u32 div3_offset;
	/*div3 clock mask. */
	u32 div3_mask;
	/*div3 clock divider */
	u32 div3_div;
	/* LP voltage - high setpoint*/
	u32 lp_high;
	/* LP voltage - low setpoint*/
	u32 lp_low;
};

/*!
 * This function is called to put the DVFS in a low power state.
 *
 * @param   id   The DVFS device id.
 */
void dvfs_suspend(int id);
/*!
 * This function is called to resume the DVFS from a low power state.
 *
 * @param   id   The DVFS device id.
 */
void dvfs_resume(int id);

#endif				/* __KERNEL__ */

#endif				/* __ASM_ARCH_MXC_DVFS_H__ */
