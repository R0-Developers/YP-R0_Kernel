/*
 * Copyright 2005-2008 Freescale Semiconductor, Inc. All Rights Reserved.
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
 * @file mx37_lpmodes.c
 *
 * @brief Driver for the Freescale Semiconductor MXC low power modes setup.
 *
 * MX37 is designed to play and video with minimal power consumption.
 * This driver enables the platform to enter and exit audio and video low
 * power modes.
 *
 * @ingroup PM
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <asm/arch/clock.h>
#include <asm/arch/hardware.h>
#include <linux/regulator/regulator-platform.h>
#include "crm_regs.h"

#define ARM_LP_CLK  200000000
#define GP_LPM_VOLTAGE 850000
#define LP_LPM_VOLTAGE 1050000
#define GP_NORMAL_VOLTAGE 1000000
#define LP_NORMAL_VOLTAGE 1200000

static int org_cpu_rate;
int lp_video_mode;
int lp_audio_mode;
static struct device *lpmode_dev;

//#define PLL_266
DEFINE_SPINLOCK(lpmode_lock);

#if defined(CONFIG_MACH_MX37_BIGBANG)
extern void mx37_set_gp_voltage(int uV);
extern void mx37_set_lp_voltage(int uV);
#endif

void enter_lp_video_mode(void)
{
	int ret = 0;

	struct clk *p_clk;
	struct clk *tclk;
	struct clk *vmode_parent_clk;
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	struct regulator *gp_core;
#endif

	tclk = clk_get(NULL, "main_bus_clk");
	vmode_parent_clk = clk_get(NULL, "pll2");
	p_clk = clk_get_parent(tclk);

	if (p_clk != vmode_parent_clk) {
		clk_set_parent(tclk, vmode_parent_clk);

		clk_set_rate(clk_get(NULL, "axi_a_clk"), 133000000);
		clk_set_rate(clk_get(NULL, "axi_b_clk"), 66500000);
		clk_set_rate(clk_get(NULL, "axi_c_clk"), 166000000);
		clk_set_rate(clk_get(NULL, "emi_core_clk"), 133000000);
		clk_set_rate(clk_get(NULL, "nfc_clk"), 26600000);
		clk_set_rate(clk_get(NULL, "ahb_clk"), 133000000);
	}

	/* move VPU clock to source from the emi_core_clk */
	tclk = clk_get(NULL, "vpu_clk");
	vmode_parent_clk = clk_get(NULL, "emi_core_clk");
	if (clk_get_parent(tclk) != vmode_parent_clk)
		clk_set_parent(tclk, vmode_parent_clk);

	tclk = clk_get(NULL, "vpu_core_clk");
	if (clk_get_parent(tclk) != vmode_parent_clk)
		clk_set_parent(tclk, vmode_parent_clk);

	tclk = clk_get(NULL, "arm_axi_clk");
	if (clk_get_parent(tclk) != vmode_parent_clk)
		clk_set_parent(tclk, vmode_parent_clk);

	tclk = clk_get(NULL, "ddr_clk");
	vmode_parent_clk = clk_get(NULL, "axi_c_clk");
	if (clk_get_parent(tclk) != vmode_parent_clk)
		clk_set_parent(tclk, vmode_parent_clk);

	/* disable PLL3 */
	tclk = clk_get(NULL, "pll3");
	if (tclk->usecount == 1)
		clk_disable(tclk);

	tclk = clk_get(NULL, "cpu_clk");
	org_cpu_rate = clk_get_rate(tclk);

	ret = clk_set_rate(tclk, ARM_LP_CLK);
	if (ret != 0)
		printk(KERN_DEBUG "cannot set CPU clock rate\n");

	/* Set the voltage to 0.8v for the GP domain. */
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	gp_core = regulator_get(NULL, "DCDC1");

	ret = regulator_set_voltage(gp_core, GP_LPM_VOLTAGE);
	if (ret < 0)
		printk(KERN_DEBUG "COULD NOT SET GP VOLTAGE!!!\n");
#else
	mx37_set_gp_voltage(GP_LPM_VOLTAGE);
#endif

	lp_video_mode = 1;
}

void exit_lp_video_mode(void)
{
	int ret = 0;
	static struct clk *tclk;
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	struct regulator *gp_core;
#endif

	/*Set the voltage to 0.8v for the GP domain. */
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	gp_core = regulator_get(NULL, "DCDC1");

	ret = regulator_set_voltage(gp_core, GP_NORMAL_VOLTAGE);
	if (ret < 0)
		printk(KERN_DEBUG "COULD NOT SET GP VOLTAGE!!!!\n");
#else
	mx37_set_gp_voltage(GP_NORMAL_VOLTAGE);
	udelay(100);
#endif

	tclk = clk_get(NULL, "cpu_clk");

	ret = clk_set_rate(tclk, org_cpu_rate);
	if (ret != 0)
		printk(KERN_DEBUG "cannot set CPU clock rate\n");

	lp_video_mode = 0;
}

//#define CLK_API
//#define PLL_266
void enter_lp_audio_mode(void)
{
	int ret = 0;

#ifdef CLK_API
	struct clk *p_clk;
	struct clk *tclk;
	struct clk *amode_parent_clk;
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	struct regulator *gp_core;
	struct regulator *lp_core;
#endif
	unsigned long flags;
	tclk = clk_get(NULL, "ipu_clk");
	if (clk_get_usecount(tclk) != 0) {
		printk(KERN_INFO
		       "Cannot enter AUDIO LPM mode - display is still active\n");
		return;
	}

	tclk = clk_get(NULL, "cpu_clk");
	org_cpu_rate = clk_get_rate(tclk);

	ret = clk_set_rate(tclk, ARM_LP_CLK);
	if (ret != 0)
		printk(KERN_DEBUG "cannot set CPU clock rate\n");

	/* Set the voltage to 0.8v for the GP domain. */
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	gp_core = regulator_get(NULL, "DCDC1");

	if (gp_core != NULL) {
		ret = regulator_set_voltage(gp_core, GP_LPM_VOLTAGE);
		if (ret < 0)
			printk(KERN_DEBUG "COULD NOT SET GP VOLTAGE!!!!!\n");
	}
#else
	mx37_set_gp_voltage(GP_LPM_VOLTAGE);
//	udelay(100);
#endif

#ifndef PLL_266
	spin_lock_irqsave(&lpmode_lock, flags);

	tclk = clk_get(NULL, "periph_apm_clk");
	amode_parent_clk = clk_get(NULL, "lp_apm");
	p_clk = clk_get_parent(tclk);

	/* Make sure osc_clk is the parent of lp_apm. */
	clk_set_parent(amode_parent_clk, clk_get(NULL, "osc"));

	/* Set the parent of periph_apm_clk to be lp_apm */
	clk_set_parent(tclk, amode_parent_clk);
	amode_parent_clk = tclk;

	tclk = clk_get(NULL, "main_bus_clk");
	p_clk = clk_get_parent(tclk);


	/* Set the parent of main_bus_clk to be periph_apm_clk */
	clk_set_parent(tclk, amode_parent_clk);

//	clk_set_rate(clk_get(NULL, "axi_a_clk"), 24000000);
//	clk_set_rate(clk_get(NULL, "axi_b_clk"), 24000000);
//	clk_set_rate(clk_get(NULL, "axi_c_clk"), 24000000);
	clk_set_rate(clk_get(NULL, "emi_core_clk"), 24000000);
//	clk_set_rate(clk_get(NULL, "nfc_clk"), 12000000);
	clk_set_rate(clk_get(NULL, "ahb_clk"), 24000000);

	udelay(5);
	amode_parent_clk = clk_get(NULL, "emi_core_clk");

	tclk = clk_get(NULL, "arm_axi_clk");
	p_clk = clk_get_parent(tclk);
	if (p_clk != amode_parent_clk) {
		clk_set_parent(tclk, amode_parent_clk);
	}

#if RANJANI
	tclk = clk_get(NULL, "vpu_clk");
	p_clk = clk_get_parent(tclk);
	if (p_clk != amode_parent_clk) {
		clk_set_parent(tclk, amode_parent_clk);
	}

	tclk = clk_get(NULL, "vpu_core_clk");
	p_clk = clk_get_parent(tclk);
	if (p_clk != amode_parent_clk) {
		clk_set_parent(tclk, amode_parent_clk);
	}
	amode_parent_clk = clk_get(NULL, "lp_apm");

	/* disable PLL3 */
	tclk = clk_get(NULL, "pll3");
	if (tclk->usecount == 1)
		clk_disable(tclk);
#endif
#if RANJANI
	/* disable PLL2 */
	tclk = clk_get(NULL, "pll2");
	if (tclk->usecount == 1)
		clk_disable(tclk);
#endif
spin_unlock_irqrestore(&lpmode_lock, flags);

#else //PLL_266
	//set the individual dividers.
	{
	struct clk *ahb_clk = clk_get(NULL, "ahb_clk");
	struct clk *emi_core_clk = clk_get(NULL, "emi_core_clk");
	struct clk *emi_intr_clk = clk_get(NULL, "emi_intr_clk");

	printk("setting ahb to 33MHz\n");
	spin_lock_irqsave(&lpmode_lock, flags);

#if RANJANI
		clk_set_rate(emi_core_clk, clk_round_rate(emi_core_clk,33000000));

		clk_set_rate(ahb_clk, clk_round_rate(ahb_clk,33000000));
		clk_set_rate(emi_intr_clk, clk_round_rate(emi_intr_clk, 33000000));
#else
		clk_set_rate(emi_core_clk, clk_round_rate(emi_core_clk,16000000));

		clk_set_rate(ahb_clk, clk_round_rate(ahb_clk,16000000));

#endif
	spin_unlock_irqrestore(&lpmode_lock, flags);

	}
#endif
	/* Set the voltage to 1.0v for the LP domain. */
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	lp_core = regulator_get(NULL, "DCDC4");

	if (lp_core != NULL) {
		ret = regulator_set_voltage(lp_core, LP_LPM_VOLTAGE);
		if (ret < 0)
			printk(KERN_DEBUG "COULD NOT SET GP VOLTAGE!!!!!!\n");
	}
#else
	mx37_set_lp_voltage(LP_LPM_VOLTAGE);
	udelay(100);
#endif

#else //CLK_API

#if !defined(CONFIG_MACH_MX37_BIGBANG)
	struct regulator *gp_core;
	struct regulator *lp_core;
#endif

	struct clk *tclk;
	unsigned int reg;
	unsigned int ccgr5_reg;

	tclk = clk_get(NULL, "ipu_clk");
	if (clk_get_usecount(tclk) != 0) {
		printk(KERN_INFO
		       "Cannot enter AUDIO LPM mode - display is still active\n");
		return;
	}

	//Set the CPU rate.
	tclk = clk_get(NULL, "cpu_clk");
	org_cpu_rate = clk_get_rate(tclk);

	ret = clk_set_rate(tclk, ARM_LP_CLK);
	if (ret != 0)
		printk(KERN_DEBUG "cannot set CPU clock rate\n");

	//Set the CPU voltage

	/* Set the voltage to 0.8v for the GP domain. */
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	gp_core = regulator_get(NULL, "DCDC1");

	if (gp_core != NULL) {
		ret = regulator_set_voltage(gp_core, GP_LPM_VOLTAGE);
		if (ret < 0)
			printk(KERN_DEBUG "COULD NOT SET GP VOLTAGE!!!!!\n");
	}
#else
	mx37_set_gp_voltage(GP_LPM_VOLTAGE);
	udelay(100);
#endif
	//Set the parent of peripheral_apm_clk to be lpapm
	tclk = clk_get(NULL, "periph_apm_clk");
#ifdef PARENT_LPAPM

	clk_set_parent(tclk, clk_get(NULL, "lp_apm"));
#else
	clk_set_parent(tclk, clk_get(NULL, "pll1_sw_clk"));
#endif
	//Set the LP clocks
	//Set ahb_max to 0x01
	*(unsigned int*)(MXC_CCM_CCGR0) |=0x02000000;
	// Make sure all emi clocks are ON.
	ccgr5_reg = *(unsigned int*)(MXC_CCM_CCGR5);
	*(unsigned int*)(MXC_CCM_CCGR5)|=0x3fc00000;
 	/* disable handshake of HSC&IPU with CCM, HSC&IPU clock
 	gates should be closed*/
	*(unsigned int*)(MXC_CCM_CCDR) |=0x20000;

 	/* Set the parent of main_bus clock to be periph apm */
 	reg = *(unsigned int*)(MXC_CCM_CBCDR6);
	reg |=0x10;
	*(unsigned int*)(MXC_CCM_CBCDR6)=reg;
	//set the load dividers bit.
	*(unsigned int*)(MXC_CCM_CCDR) |=0x1;
	while((*(unsigned int*)(MXC_CCM_CCDR)) & 0x1) {
		printk("main_bus_clk\n");
	}

#ifdef PARENT_LPAPM
	//Set the AXI divider to 1
	reg = __raw_readl(MXC_CCM_CBCDR3);
	reg &= ~MXC_CCM_CBCDR3_AXI_A_PODF_MASK;
	reg |= 0 << MXC_CCM_CBCDR3_AXI_A_PODF_OFFSET;
	__raw_writel(reg, MXC_CCM_CBCDR3);

	//loop on the load dividers bit.
	*(unsigned int*)(MXC_CCM_CCDR) |=0x1;
	while((*(unsigned int*)(MXC_CCM_CCDR)) & 0x1) {
		printk("axi, emi\n");
	}

	//Set the EMI divider to 1.
	reg = __raw_readl(MXC_CCM_CBCDR6);
	reg &= ~MXC_CCM_CBCDR6_EMI_PODF_MASK;
	reg |= 0 << MXC_CCM_CBCDR6_EMI_PODF_OFFSET;
	__raw_writel(reg, MXC_CCM_CBCDR6);

	//loop on the load dividers bit.
	*(unsigned int*)(MXC_CCM_CCDR) |=0x1;
	while((*(unsigned int*)(MXC_CCM_CCDR)) & 0x1) {
		printk("axi, emi\n");
	}

	//Set the AHB_Divider to 1.
	reg = __raw_readl(MXC_CCM_CBCDR2);
	reg &= ~MXC_CCM_CBCDR2_AHB_PODF_MASK;
	reg |= 0 << MXC_CCM_CBCDR2_AHB_PODF_OFFSET;
	__raw_writel(reg, MXC_CCM_CBCDR2);
#else
	//Set the AXI divider to 8
	reg = __raw_readl(MXC_CCM_CBCDR3);
	reg &= ~MXC_CCM_CBCDR3_AXI_A_PODF_MASK;
	reg |= 7 << MXC_CCM_CBCDR3_AXI_A_PODF_OFFSET;
	__raw_writel(reg, MXC_CCM_CBCDR3);

	//loop on the load dividers bit.
	*(unsigned int*)(MXC_CCM_CCDR) |=0x1;
	while((*(unsigned int*)(MXC_CCM_CCDR)) & 0x1) {
		printk("axi, emi\n");
	}

	//Set the EMI divider to 8.
	reg = __raw_readl(MXC_CCM_CBCDR6);
	reg &= ~MXC_CCM_CBCDR6_EMI_PODF_MASK;
	reg |= 7 << MXC_CCM_CBCDR6_EMI_PODF_OFFSET;
	__raw_writel(reg, MXC_CCM_CBCDR6);

	//loop on the load dividers bit.
	*(unsigned int*)(MXC_CCM_CCDR) |=0x1;
	while((*(unsigned int*)(MXC_CCM_CCDR)) & 0x1) {
		printk("axi, emi\n");
	}

	//Set the AHB_Divider to 8
	reg = __raw_readl(MXC_CCM_CBCDR2);
	reg &= ~MXC_CCM_CBCDR2_AHB_PODF_MASK;
	reg |= 7 << MXC_CCM_CBCDR2_AHB_PODF_OFFSET;
	__raw_writel(reg, MXC_CCM_CBCDR2);
#endif
	*(unsigned int*)(MXC_CCM_CCGR5) = ccgr5_reg;
	*(unsigned int*)(MXC_CCM_CCDR) &= ~0x30000;


//Set main_bus_clk parent to be periph-apm... just to get all the clock rates correct.
clk_set_parent(clk_get(NULL, "main_bus_clk"), tclk);
	//Set the LP voltage
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	lp_core = regulator_get(NULL, "DCDC4");

	if (lp_core != NULL) {
		ret = regulator_set_voltage(lp_core, LP_LPM_VOLTAGE);
		if (ret < 0)
			printk(KERN_DEBUG "COULD NOT SET GP VOLTAGE!!!!!!\n");
	}
#else
	mx37_set_lp_voltage(LP_LPM_VOLTAGE);
	udelay(100);
#endif

#endif
	lp_audio_mode = 1;
//printk("Entered lpapm\n");
}

void exit_lp_audio_mode(void)
{
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	struct regulator *gp_core;
	struct regulator *lp_core;
#endif
	struct clk *tclk;

#ifdef CLK_API
	struct clk *p_clk;
	struct clk *rmode_parent_clk;
	int ret;
	unsigned long flags;

	lp_audio_mode = 0;
	/* Set the voltage to 1.2v for the LP domain. */
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	lp_core = regulator_get(NULL, "DCDC4");

	if (lp_core != NULL) {
		ret = regulator_set_voltage(lp_core, LP_NORMAL_VOLTAGE);
		if (ret < 0)
			printk(KERN_DEBUG "COULD NOT SET GP VOLTAGE!!!!!!\n");
	}
#else
	mx37_set_lp_voltage(LP_NORMAL_VOLTAGE);
	udelay(100);
#endif


#ifndef PLL_266
	spin_lock_irqsave(&lpmode_lock, flags);

	rmode_parent_clk = clk_get(NULL, "pll2");
#if RANJANI
	clk_enable(rmode_parent_clk);
#endif
	tclk = clk_get(NULL, "main_bus_clk");
//	p_clk = clk_get_parent(tclk);

	/* Set the dividers before setting the parent clock. */
	clk_set_rate(clk_get(NULL, "ahb_clk"), 4800000);

//	clk_set_rate(clk_get(NULL, "axi_a_clk"), 4800000);
//	clk_set_rate(clk_get(NULL, "axi_b_clk"), 4000000);
//	clk_set_rate(clk_get(NULL, "axi_c_clk"), 6000000);
	clk_set_rate(clk_get(NULL, "emi_core_clk"), 4800000);
//	clk_set_rate(clk_get(NULL, "nfc_clk"), 1600000);


	/* Set the parent of main_bus_clk to be pll2 */
	clk_set_parent(tclk, rmode_parent_clk);

	spin_unlock_irqrestore(&lpmode_lock, flags);

#else //end PLL_266
{
	struct clk *ahb_clk = clk_get(NULL, "ahb_clk");
	struct clk *emi_core_clk = clk_get(NULL, "emi_core_clk");
	struct clk *emi_intr_clk = clk_get(NULL, "emi_intr_clk");

	printk("setting ahb to 133MHz\n");
	clk_set_rate(ahb_clk, clk_round_rate(ahb_clk,130000000));
	clk_set_rate(emi_core_clk, clk_round_rate(emi_core_clk,130000000));
	clk_set_rate(emi_intr_clk, clk_round_rate(emi_intr_clk, 66000000));
}

#endif


	/* Set the voltage to 1.0v for the GP domain. */
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	gp_core = regulator_get(NULL, "DCDC1");

	ret = regulator_set_voltage(gp_core, GP_NORMAL_VOLTAGE);
	if (ret < 0)
		printk(KERN_DEBUG "COULD NOT SET GP VOLTAGE!!!!\n");
#else
	mx37_set_gp_voltage(GP_NORMAL_VOLTAGE);
	udelay(100);
#endif

	tclk = clk_get(NULL, "cpu_clk");

	ret = clk_set_rate(tclk, org_cpu_rate);
	if (ret != 0)
		printk(KERN_DEBUG "cannot set CPU clock rate\n");

#else //CLK_API
	unsigned int ccgr5_reg, reg;
	int ret;

	lp_audio_mode = 0;
	/* Set the voltage to 1.2v for the LP domain. */
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	lp_core = regulator_get(NULL, "DCDC4");

	if (lp_core != NULL) {
		ret = regulator_set_voltage(lp_core, LP_NORMAL_VOLTAGE);
		if (ret < 0)
			printk(KERN_DEBUG "COULD NOT SET GP VOLTAGE!!!!!!\n");
	}
#else
	mx37_set_lp_voltage(LP_NORMAL_VOLTAGE);
	udelay(100);
#endif

	//Set the LP clocks.
	//Enable EMI clocks
	ccgr5_reg =*(unsigned int*)(MXC_CCM_CCGR5);
	*(unsigned int*)(MXC_CCM_CCGR5)|=0x3fc00000;

	/* disable handshake of HSC&IPU with CCM, HSC&IPU clock gates should be closed*/
	*(unsigned int*)(MXC_CCM_CCDR) |=0x20000;

	//Set AHB divider to 5.
	reg = __raw_readl(MXC_CCM_CBCDR2);
	reg &= ~MXC_CCM_CBCDR2_AHB_PODF_MASK;
	reg |= 4 << MXC_CCM_CBCDR2_AHB_PODF_OFFSET;
	__raw_writel(reg, MXC_CCM_CBCDR2);

	//Set the AXI divider to 5
	reg = __raw_readl(MXC_CCM_CBCDR3);
	reg &= ~MXC_CCM_CBCDR3_AXI_A_PODF_MASK;
	reg |= 4 << MXC_CCM_CBCDR3_AXI_A_PODF_OFFSET;
	__raw_writel(reg, MXC_CCM_CBCDR3);

	//Set the EMI divider to 5.
	reg = __raw_readl(MXC_CCM_CBCDR6);
	reg &= ~MXC_CCM_CBCDR6_EMI_PODF_MASK;
	reg |= 4 << MXC_CCM_CBCDR6_EMI_PODF_OFFSET;
	__raw_writel(reg, MXC_CCM_CBCDR6);

	//Set the Main bus clock parent to be pll2.
	reg = *(unsigned int*)(MXC_CCM_CBCDR6);
	reg &= ~0x10;
	*(unsigned int*)(MXC_CCM_CBCDR6) = reg;

	//loop on the load dividers bit.
	*(unsigned int*)(MXC_CCM_CCDR) |=0x1;
	while((*(unsigned int*)(MXC_CCM_CCDR)) & 0x1);

//set the parent of main_bus_clk to be pll2, just to get all the rates correct.
	tclk = clk_get(NULL, "main_bus_clk");
	clk_set_parent(tclk, clk_get(NULL, "pll2"));

	//Set the GP back to 532MHz.
	/* Set the voltage to 1.0v for the GP domain. */
#if !defined(CONFIG_MACH_MX37_BIGBANG)
	gp_core = regulator_get(NULL, "DCDC1");

	ret = regulator_set_voltage(gp_core, GP_NORMAL_VOLTAGE);
	if (ret < 0)
		printk(KERN_DEBUG "COULD NOT SET GP VOLTAGE!!!!\n");
#else
	mx37_set_gp_voltage(GP_NORMAL_VOLTAGE);
	udelay(100);
#endif

	tclk = clk_get(NULL, "cpu_clk");

	ret = clk_set_rate(tclk, org_cpu_rate);
	if (ret != 0)
		printk(KERN_DEBUG "cannot set CPU clock rate\n");

#endif
//printk("Exit lpapm\n");

}

static ssize_t lp_curr_mode(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	if (lp_video_mode)
		return sprintf(buf, "in lp_video_mode\n");
	else if (lp_audio_mode)
		return sprintf(buf, "in lp_audio_mode\n");
	else
		return sprintf(buf, "in normal mode\n");
}

static ssize_t set_lp_mode(struct device *dev,
			   struct device_attribute *attr,
			   const char *buf, size_t size)
{
//	printk(KERN_DEBUG "In set_lp_mode() \n");

	if (strstr(buf, "enable_lp_video") != NULL) {
		if (!lp_video_mode)
			enter_lp_video_mode();
	} else if (strstr(buf, "disable_lp_video") != NULL) {
		if (lp_video_mode)
			exit_lp_video_mode();
	} else if (strstr(buf, "enable_lp_audio") != NULL) {
		if (!lp_audio_mode)
			enter_lp_audio_mode();
	} else if (strstr(buf, "disable_lp_audio") != NULL) {
		if (lp_audio_mode)
			exit_lp_audio_mode();
	}
	return size;
}

static DEVICE_ATTR(lp_modes, 0644, lp_curr_mode, set_lp_mode);

/*!
 * This is the probe routine for the lp_mode driver.
 *
 * @param   pdev   The platform device structure
 *
 * @return         The function returns 0 on success
 *
 */
static int __devinit mx37_lpmode_probe(struct platform_device *pdev)
{
	u32 res = 0;
	lpmode_dev = &pdev->dev;

	res = sysfs_create_file(&lpmode_dev->kobj, &dev_attr_lp_modes.attr);
	if (res) {
		printk(KERN_ERR
		       "lpmode_dev: Unable to register sysdev entry for lpmode_dev");
		return res;
	}

	if (res != 0) {
		printk(KERN_ERR "lpmode_dev: Unable to start");
		return res;
	}
	lp_video_mode = 0;
	lp_audio_mode = 0;

	return 0;
}

static struct platform_driver mx37_lpmode_driver = {
	.driver = {
		   .name = "mx37_lpmode",
		   },
	.probe = mx37_lpmode_probe,
};

/*!
 * Initialise the mx37_lpmode_driver.
 *
 * @return  The function always returns 0.
 */

static int __init lpmode_init(void)
{
	if (platform_driver_register(&mx37_lpmode_driver) != 0) {
		printk(KERN_ERR "mx37_lpmode_driver register failed\n");
		return -ENODEV;
	}

	printk(KERN_INFO "LPMode driver module loaded\n");
	return 0;
}

static void __exit lpmode_cleanup(void)
{
	sysfs_remove_file(&lpmode_dev->kobj, &dev_attr_lp_modes.attr);

	/* Unregister the device structure */
	platform_driver_unregister(&mx37_lpmode_driver);
}

module_init(lpmode_init);
module_exit(lpmode_cleanup);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("LPMode driver");
MODULE_LICENSE("GPL");
