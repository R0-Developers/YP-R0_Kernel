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
 * @file dvfs_per.c
 *
 * @brief A simplied driver for the Freescale Semiconductor MXC DVFS module.
 *
 * Upon initialization, the DVFS driver initializes the DVFS hardware
 * sets up driver nodes attaches to the DVFS interrupt and initializes internal
 * data structures. When the DVFS interrupt occurs the driver checks the cause
 * of the interrupt (lower frequency, increase frequency or emergency) and
 * changes the CPU voltage according to translation table that is loaded into
 * the driver.
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
#include <linux/sysdev.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/regulator/regulator.h>
#include <linux/input.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>

#include <asm/arch/clock.h>
#include <asm/arch/hardware.h>
#include <asm/arch/mxc_dvfs.h>
#include <asm/arch/mxc_dptc.h>

#define DRIVER_NAME "DVFSPER"
//#define DVFS_SW_WORKAROUND
int dvfs_per_is_active;
int dvfs_per_low_freq;
static int dvfs_per_suspended;
static volatile int freq_increased;
static int cur_setpoint;
static struct delayed_work dvfs_per_work;
static struct clk *dvfs_clk;
static struct clk *main_bus_clk;
static struct clk *pll2;
static struct clk *lpapm;
static struct clk *cpu_clk;
static struct clk *axi_b_clk;
static struct clk *ahb_clk;
static struct clk *ddr_hf_clk;

struct dvfsper_device *dvfsper_device_data;
/* DVFS platform data pointer */
struct mxc_dvfsper_data *dvfsper_platform_data;
struct timeval prev_intr;
extern int set_high_bus_freq(int high_bus_speed);


/*!
 * In case the MXC device has multiple DVFS modules, this structure is used to
 * store information specific to each DVFS module.
 */
struct dvfsper_device {
	/* DVFS delayed work */
	struct delayed_work dvfs_work;
	/* DVFS regulator */
	struct regulator *dvfs_reg;
	/* DVFS clock */
	struct clk *dvfs_clk;
	/* The interrupt number used by the DVFS device */
	int irq;
};

struct dvfs_per_wp dvfs_per_setpoint[] = {{33, 5, 33, 10, 10, 0x10},
                                          {12, 0, 33, 10, 10, 0x10},
					  /* When LP is at 24MHz */
					  {8, 0, 10, 5, 5, 0x2E},};

int start_dvfs_per(void);
void stop_dvfs_per(void);
void force_freq_change(void);
void dump_dvfs_per_regs(void);
void restart_dvfs_per(void);
void disable_dvfs_per(void);
extern void mx37_set_lp_voltage(int uV);

enum {
	FSVAI_FREQ_NOCHANGE = 0x0,
	FSVAI_FREQ_INCREASE,
	FSVAI_FREQ_DECREASE,
	FSVAI_FREQ_EMERG,
};

#define LOW_BUS_FREQ 24000000

#define LP_LPM_VOLTAGE 1000000
#define LP_NORMAL_VOLTAGE 1200000
#define LP_LOWFREQ_VOLTAGE 1050000


DEFINE_SPINLOCK(mxc_dvfs_per_lock);

static void dvfs_per_load_config(void)
{
	u32 reg;

	reg = __raw_readl(MXC_DVFS_PER_LTR0);
	reg &= ~MXC_DVFSLTR0_UPTHR_MASK;
	reg &= ~MXC_DVFSLTR0_DNTHR_MASK;
	reg |= UPTHR(dvfs_per_setpoint[cur_setpoint].upthr);
	reg |= DNTHR(dvfs_per_setpoint[cur_setpoint].downthr);
	__raw_writel(reg, MXC_DVFS_PER_LTR0);

	reg = __raw_readl(MXC_DVFS_PER_LTR1);
	reg &= ~MXC_DVFSLTR1_PNCTHR_MASK;
	reg &= ~MXC_DVFSLTR1_DNCNT_MASK;
	reg &= ~MXC_DVFSLTR1_UPCNT_MASK;
	reg |= DNCNT(dvfs_per_setpoint[cur_setpoint].downcnt);
	reg |= UPCNT(dvfs_per_setpoint[cur_setpoint].upcnt);
	reg |= PANICTHR(dvfs_per_setpoint[cur_setpoint].panicthr);
	__raw_writel(reg, MXC_DVFS_PER_LTR1);

	reg = EMAC(dvfs_per_setpoint[cur_setpoint].emac);
	__raw_writel(reg, MXC_DVFS_PER_LTR2);
}

/*!
 * This function is called for module initialization.
 * It sets up the DVFS hardware.
 * It sets default values for DVFS thresholds and counters. The default
 * values was chosen from a set of different reasonable values. They was tested
 * and the default values in the driver gave the best results.
 * More work should be done to find optimal values.
 *
 * @return   0 if successful; non-zero otherwise.
 *
 */
static int init_dvfs_per_controller(void)
{
	u32 reg;

	reg = __raw_readl(MXC_DVFS_PER_LTR0);
	/* DIV3CLK */
	reg &= ~dvfsper_platform_data->div3_mask;
	reg |= (dvfsper_platform_data->div3_div <<
		  dvfsper_platform_data->div3_offset);
	__raw_writel(reg, MXC_DVFS_PER_LTR0);

	reg = __raw_readl(MXC_DVFS_PER_LTR1);
	/* Set load tracking buffer register source */
	reg &= ~MXC_DVFSLTR1_LTBRSR;
	reg &= ~MXC_DVFSLTR1_LTBRSH;
	__raw_writel(reg, MXC_DVFS_PER_LTR1);

	/* Enable all the peripheral signals, but VPU and IPU panic*/
	__raw_writel(0x30000, MXC_DVFS_PER_PMCR1);
	/* Disable weighted load tracking signals */
	__raw_writel(0, MXC_DVFS_PER_LTR3);

	reg = __raw_readl(MXC_DVFS_PER_PMCR0);
	reg &= ~MXC_DVFSPMCR0_DVFEV;
	reg |= MXC_DVFSPMCR0_LBMI;
	__raw_writel(reg, MXC_DVFS_PER_PMCR0);

	/* DVFS loading config */
	dvfs_per_load_config();
	return 0;
}

static irqreturn_t dvfs_per_irq(int irq, void *dev_id)
{
	u32 reg;

	/* Check if DVFS1 (PER) id requesting for freqency/voltage update */
	if ((__raw_readl(MXC_GPC_CNTR) & MXC_GPCCNTR_DVFS1CR) == 0)
		return IRQ_NONE;
	/* Mask DVFS irq */
	reg = __raw_readl(MXC_DVFS_PER_PMCR0);
	/* FSVAIM=1 */
	reg |= MXC_DVFSPMCR0_FSVAIM;
	__raw_writel(reg, MXC_DVFS_PER_PMCR0);
	/* Mask GPC1 irq */
	reg = __raw_readl(MXC_GPC_CNTR);
	reg |= MXC_GPCCNTR_GPCIRQM | 0x1000000;
	__raw_writel(reg, MXC_GPC_CNTR);

	reg = __raw_readl(MXC_DVFS_PER_PMCR0);
	if (reg & MXC_DVFSPMCR0_LBFL) {
		/* clear LBFL */
		reg = (reg & ~MXC_DVFSPMCR0_LBFL);
		reg |= MXC_DVFSPMCR0_LBFL;
		__raw_writel(reg, MXC_DVFS_PER_PMCR0);
	}
	schedule_delayed_work(&dvfs_per_work, 0);
	return IRQ_HANDLED;
}

static void dvfs_per_workqueue_handler(struct work_struct *work)
{
	u32 fsvai;
	u32 reg;
	u32 ret;
	u32 rate;
	/* Check DVFS frequency adjustment interrupt status */
	reg = __raw_readl(MXC_DVFS_PER_PMCR0);
	fsvai = (reg & MXC_DVFSPMCR0_FSVAI_MASK) >> MXC_DVFSPMCR0_FSVAI_OFFSET;
	/* Check FSVAI, FSVAI=0 is error */
	if (fsvai == FSVAI_FREQ_NOCHANGE) {
		/* Do nothing. Freq change is not required */
		goto END;
	}
	/* If FSVAI indicate freq down. */
	if (fsvai == FSVAI_FREQ_DECREASE) {
		reg = __raw_readl(MXC_DVFS_PER_PMCR0);
		reg &= ~MXC_DVFSPMCR0_UDCS;
		__raw_writel(reg, MXC_DVFS_PER_PMCR0);

	if (cpu_is_mx51()) {
		/*Change the DDR freq to 133Mhz. */
		clk_set_rate(ddr_hf_clk, clk_round_rate(ddr_hf_clk, 133000000));
	}

#ifndef DVFS_SW_WORKAROUND
		/* Set the peripheral divider */
		reg = __raw_readl(MXC_GPC_CNTR);
		reg |= MXC_GPCCNTR_FUPD;
		__raw_writel(reg, MXC_GPC_CNTR);
		reg = __raw_readl(MXC_GPC_VCR);
		reg &= ~(MXC_GPCVCR_VINC_MASK | MXC_GPCVCR_VCNTU_MASK |
		      MXC_GPCVCR_VCNT_MASK);
		reg |= (1 << MXC_GPCVCR_VCNTU_OFFSET) |
			(20 << MXC_GPCVCR_VCNT_OFFSET);
		__raw_writel(reg, MXC_GPC_VCR);
		reg = __raw_readl(MXC_GPC_CNTR);
		reg |= MXC_GPCCNTR_STRT;
		__raw_writel(reg, MXC_GPC_CNTR);
		while (__raw_readl(MXC_GPC_CNTR) & 0x4000) {
			udelay(10);
		}
#else
		//Set the frequencies manually
		rate = clk_get_rate(axi_b_clk);
		clk_set_rate(axi_b_clk, clk_round_rate(axi_b_clk, rate/2));

		rate = clk_get_rate(ahb_clk);
		clk_set_rate(ahb_clk, clk_round_rate(ahb_clk, rate/2));
#endif
		if (clk_get_rate(main_bus_clk) == LOW_BUS_FREQ) {
			cur_setpoint = 2;
		} else {
			if (cpu_is_mx37())
				;//dptc_suspend(DPTC_LP_ID);
			cur_setpoint = 1;
#ifndef DVFS_SW_WORKAROUND
			clk_set_parent(main_bus_clk, pll2);
#endif
		}
#ifndef DVFS_SW_WORKAROUND
		/* Drop the LP domain voltage */
		mx37_set_lp_voltage(LP_LOWFREQ_VOLTAGE);
		udelay(100);
#endif
		dvfs_per_load_config();
	} else if ((fsvai == FSVAI_FREQ_INCREASE) ||
			(fsvai == FSVAI_FREQ_EMERG)) {
		reg = __raw_readl(MXC_DVFS_PER_PMCR0);
		reg |= MXC_DVFSPMCR0_UDCS;
		__raw_writel(reg, MXC_DVFS_PER_PMCR0);
#ifndef DVFS_SW_WORKAROUND
		/* Increase the LP domain voltage first. */
		mx37_set_lp_voltage(LP_NORMAL_VOLTAGE);
		udelay(100);
#endif

	if (cpu_is_mx51()) {
		/*Change the DDR freq to 133Mhz. */
		clk_set_rate(ddr_hf_clk, clk_round_rate(ddr_hf_clk, 200000000));
	}

#ifndef DVFS_SW_WORKAROUND
		reg = __raw_readl(MXC_GPC_CNTR);
		reg |= MXC_GPCCNTR_FUPD;
		__raw_writel(reg, MXC_GPC_CNTR);
		reg = __raw_readl(MXC_GPC_VCR);
		reg &= ~(MXC_GPCVCR_VINC_MASK | MXC_GPCVCR_VCNTU_MASK |
		      MXC_GPCVCR_VCNT_MASK);
		reg |= (1 << MXC_GPCVCR_VINC_OFFSET |
			1 << MXC_GPCVCR_VCNTU_OFFSET |
			20 << MXC_GPCVCR_VCNT_OFFSET);
		__raw_writel(reg, MXC_GPC_VCR);
		reg = __raw_readl(MXC_GPC_CNTR);
		reg &= ~MXC_GPCCNTR_ADU;
		reg |= MXC_GPCCNTR_STRT;

		__raw_writel(reg, MXC_GPC_CNTR);
		while (__raw_readl(MXC_GPC_CNTR) & 0x4000) {
			udelay(10);
		}
#else
		//Set the frequencies manually
		rate = clk_get_rate(axi_b_clk);
		clk_set_rate(axi_b_clk, clk_round_rate(axi_b_clk, 130000000));
		rate = clk_get_rate(ahb_clk);
		clk_set_rate(ahb_clk, clk_round_rate(ahb_clk, 130000000));
#endif

		if (clk_get_rate(main_bus_clk) == LOW_BUS_FREQ) {
			cur_setpoint = 2;
		} else {
			cur_setpoint = 0;
			if (cpu_is_mx37())
				;//dptc_resume(DPTC_LP_ID);
#ifndef DVFS_SW_WORKAROUND
			clk_set_parent(main_bus_clk, clk_get(NULL, "pll2"));
#endif
		}
		dvfs_per_load_config();
		freq_increased = 1;
	}

END:
//	dump_dvfs_per_regs();

	if (dvfs_per_is_active) {
		reg = __raw_readl(MXC_DVFS_PER_PMCR0);
		/* Enable dVFS interrupt */
		/* FSVAIM=0 */
		reg &= ~MXC_DVFSPMCR0_FSVAI_MASK;
		reg |= FSVAI_FREQ_NOCHANGE;
		reg = (reg & ~MXC_DVFSPMCR0_FSVAIM);
		__raw_writel(reg, MXC_DVFS_PER_PMCR0);
		/*Unmask GPC1 IRQ */
		reg = __raw_readl(MXC_GPC_CNTR);
		reg &= ~MXC_GPCCNTR_GPCIRQM;
		__raw_writel(reg, MXC_GPC_CNTR);
	}

}

int start_dvfs_per(void)
{
	u32 reg, flags;

	if (dvfs_per_is_active)
		return 0;

	spin_lock_irqsave(&mxc_dvfs_per_lock, flags);

	clk_enable(dvfs_clk);

	cur_setpoint = 0;
	init_dvfs_per_controller();

	/* config reg GPC_CNTR */
	reg = __raw_readl(MXC_GPC_CNTR);
	/* ADU=0, select PER domain */
	reg &= ~MXC_GPCCNTR_ADU;
	__raw_writel(reg, MXC_GPC_CNTR);

	reg = __raw_readl(MXC_DVFS_PER_PMCR0);
	/* Select ARM domain */
	reg |= MXC_DVFSPMCR0_DVFIS;
	/* Set the UDCS bit */
	reg |= MXC_DVFSPMCR0_UDCS;
	/* Enable DVFS interrupt */
	/* FSVAIM=0 */
	reg &= ~MXC_DVFSPMCR0_FSVAIM;
	/*Set the FSVAI to no_freq_change */
	reg &= ~MXC_DVFSPMCR0_FSVAI_MASK;
	reg |= FSVAI_FREQ_NOCHANGE << MXC_DVFSPMCR0_FSVAI_OFFSET;
	__raw_writel(reg, MXC_DVFS_PER_PMCR0);

	/* config reg GPC_CNTR */
	reg = __raw_readl(MXC_GPC_CNTR);
	/* GPCIRQ=1, select ARM IRQ */
	reg |= MXC_GPCCNTR_GPCIRQ;
	reg &= ~MXC_GPCCNTR_GPCIRQM;
	__raw_writel(reg, MXC_GPC_CNTR);

	/* Enable DVFS */
	reg = __raw_readl(MXC_DVFS_PER_PMCR0);
	reg |= MXC_DVFSPMCR0_DVFEN;
	__raw_writel(reg, MXC_DVFS_PER_PMCR0);

	dvfs_per_is_active = 1;
	spin_unlock_irqrestore(&mxc_dvfs_per_lock, flags);

	printk(KERN_DEBUG "DVFS PER is started\n");

	return 0;
}

void restart_dvfs_per()
{
	u32 reg;

	if (clk_get_rate(main_bus_clk) == 24000000)
		cur_setpoint = 2;
	else
		cur_setpoint = 0;

	/* Remove weights. */
	__raw_writel(0, MXC_DVFS_PER_LTR3);
	reg = __raw_readl(MXC_DVFS_PER_PMCR1);
	reg &= ~0xf;
	__raw_writel(reg, MXC_DVFS_PER_PMCR1);

	dvfs_per_load_config();

	/*Unmask GPC1 IRQ */
	reg = __raw_readl(MXC_GPC_CNTR);
	reg &= ~MXC_GPCCNTR_GPCIRQM;
	__raw_writel(reg, MXC_GPC_CNTR);

	reg = __raw_readl(MXC_DVFS_PER_PMCR0);
	/* Enable FVFS interrupt */
	/* FSVAIM=0 */
	reg &= ~MXC_DVFSPMCR0_FSVAI_MASK;
	reg |= FSVAI_FREQ_NOCHANGE;
	reg = (reg & ~MXC_DVFSPMCR0_FSVAIM);
	reg |= MXC_DVFSPMCR0_DVFEN;
	__raw_writel(reg, MXC_DVFS_PER_PMCR0);
	dvfs_per_is_active = 1;

}

/*!
 * This function disables the DVFS module.
 */
void stop_dvfs_per(void)
{
	u32 reg = 0;
	u32 flags;
	if (dvfs_per_is_active) {
		dvfs_per_is_active = 0;
		if (cur_setpoint != 0) {
			force_freq_change();
		}
		spin_lock_irqsave(&mxc_dvfs_per_lock, flags);

		/* Mask dvfs irq, disable DVFS */
		reg = __raw_readl(MXC_DVFS_PER_PMCR0);
		/* FSVAIM=1 */
		reg |= MXC_DVFSPMCR0_FSVAIM;
		reg = (reg & ~MXC_DVFSPMCR0_DVFEN);
		__raw_writel(reg, MXC_DVFS_PER_PMCR0);

		spin_unlock_irqrestore(&mxc_dvfs_per_lock, flags);
		clk_disable(dvfs_clk);
	}

}

void disable_dvfs_per(void)
{
	dvfs_per_is_active = 0;
	if (cur_setpoint != 0) {
		force_freq_change();
	}
}

void force_freq_change(void)
{
	u32 reg;
	int rate;
	u32 flags;

#ifndef DVFS_SW_WORKAROUND
	/* Increase the LP domain voltage first. */
	mx37_set_lp_voltage(LP_NORMAL_VOLTAGE);
	udelay(100);
#endif

	freq_increased = 0;
	spin_lock_irqsave(&mxc_dvfs_per_lock, flags);

//	printk("force_freq_change\n");
	reg = __raw_readl(MXC_DVFS_PER_PMCR0);
	reg |= MXC_DVFSPMCR0_UDCS;
	__raw_writel(reg, MXC_DVFS_PER_PMCR0);

	if (cpu_is_mx51()) {
		/*Change the DDR freq to 133Mhz. */
		clk_set_rate(ddr_hf_clk, clk_round_rate(ddr_hf_clk, 200000000));
	}

#ifndef DVFS_SW_WORKAROUND
	reg = __raw_readl(MXC_GPC_CNTR);
	reg |= MXC_GPCCNTR_FUPD;
	__raw_writel(reg, MXC_GPC_CNTR);
	reg = __raw_readl(MXC_GPC_VCR);
	reg &= ~(MXC_GPCVCR_VINC_MASK | MXC_GPCVCR_VCNTU_MASK |
	      MXC_GPCVCR_VCNT_MASK);
	reg |= (1 << MXC_GPCVCR_VINC_OFFSET |
		1 << MXC_GPCVCR_VCNTU_OFFSET |
		20 << MXC_GPCVCR_VCNT_OFFSET);
	__raw_writel(reg, MXC_GPC_VCR);
	reg = __raw_readl(MXC_GPC_CNTR);
	reg &= ~MXC_GPCCNTR_ADU;
	reg |= MXC_GPCCNTR_STRT;

	__raw_writel(reg, MXC_GPC_CNTR);
	while (__raw_readl(MXC_GPC_CNTR) & 0x4000) {
		udelay(10);
	}
	freq_increased = 1;

#else
	//Set the frequencies manually
	rate = clk_get_rate(axi_b_clk);
	clk_set_rate(axi_b_clk, clk_round_rate(axi_b_clk, 130000000));
	rate = clk_get_rate(ahb_clk);
	clk_set_rate(ahb_clk, clk_round_rate(ahb_clk, 130000000));
#endif
#ifndef DVFS_SW_WORKAROUND
	clk_set_parent(main_bus_clk, clk_get(NULL, "pll2"));
#endif

	spin_unlock_irqrestore(&mxc_dvfs_per_lock, flags);

}

void dump_dvfs_per_regs()
{
	struct timeval cur;
 	u32 diff;
	if (prev_intr.tv_sec == 0)
		do_gettimeofday(&prev_intr);
	else {
		do_gettimeofday(&cur);
		diff = (cur.tv_sec - prev_intr.tv_sec)*1000000
			 + (cur.tv_usec - prev_intr.tv_usec);
		prev_intr = cur;
	}
	if (diff < 30000)
		printk("diff = %d\n", diff);

	printk("LTRO = 0x%08x\n", __raw_readl(MXC_DVFS_PER_LTR0));
	printk("LTR1 = 0x%08x\n", __raw_readl(MXC_DVFS_PER_LTR1));
	printk("LTR2 = 0x%08x\n", __raw_readl(MXC_DVFS_PER_LTR2));
	printk("LTR3 = 0x%08x\n", __raw_readl(MXC_DVFS_PER_LTR3));
	printk("LBTR0 = 0x%08x\n", __raw_readl(MXC_DVFS_PER_LTBR0));
	printk("LBTR1 = 0x%08x\n", __raw_readl(MXC_DVFS_PER_LTBR1));
	printk("PMCR0 = 0x%08x\n", __raw_readl(MXC_DVFS_PER_PMCR0));
	printk("PMCR1 = 0x%08x\n", __raw_readl(MXC_DVFS_PER_PMCR1));

}

/*!
 * Enable DVFS Peripheral
 *
 */
int dvfs_enable(struct device *dev)
{
	if (dvfs_per_is_active)
		return 0;
	return (start_dvfs_per());
}


static ssize_t dvfsper_enable_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t size)
{
	if (strstr(buf, "1") != NULL) {
		if (dvfs_enable(dev) != 0)
			printk(KERN_ERR "Failed to start DVFS\n");
	} else if (strstr(buf, "0") != NULL)
		stop_dvfs_per();

	return size;
}

static ssize_t dvfsper_status_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	int size = 0;

	if (dvfs_per_is_active)
		size = sprintf(buf, "DVFS PER is enabled\n");
	else
		size = sprintf(buf, "DVFS PEr is disabled\n");

	return size;
}

static DEVICE_ATTR(enable, 0644, dvfsper_status_show, dvfsper_enable_store);

/*!
 * This is the probe routine for the DVFS PER driver.
 *
 * @param   pdev   The platform device structure
 *
 * @return         The function returns 0 on success
 *
 */
static int __devinit mxc_dvfsper_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct resource *res;
	struct mxc_dvfsper_data *dvfsper_data = pdev->dev.platform_data;

	if (dvfsper_data == NULL) {
		printk(KERN_ERR "DVFS: Pointer to DVFS data is NULL\
				not started\n");
		return -1;
	}

	/* Set driver data */
	platform_set_drvdata(pdev, dvfsper_device_data);

	dvfsper_platform_data = pdev->dev.platform_data;
	dvfsper_device_data = kzalloc(sizeof(struct dvfsper_device), GFP_KERNEL);
	if (!dvfsper_device_data)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		ret = -ENODEV;
		goto err1;
	}

	/*
	 * Request the DVFSPER interrupt
	 */
	dvfsper_device_data->irq = platform_get_irq(pdev, 0);
	if (dvfsper_device_data->irq < 0) {
		ret = dvfsper_device_data->irq;
		goto err1;
	}

	ret =
	    request_irq(dvfsper_device_data->irq, dvfs_per_irq, IRQF_SHARED,
			pdev->name, &pdev->dev);
	if (ret) {
		printk(KERN_ERR "DVFS: Unable to attach to DVFS interrupt\n");
		goto err1;
	}

	INIT_DELAYED_WORK(&dvfs_per_work, dvfs_per_workqueue_handler);
	main_bus_clk = clk_get(NULL, "main_bus_clk");
	pll2 = clk_get(NULL, "pll2");
	lpapm = clk_get(NULL, "lp_apm");
	cpu_clk = clk_get(NULL, "cpu_clk");
	ahb_clk = clk_get(NULL, "ahb_clk");
	axi_b_clk = clk_get(NULL, "axi_b_clk");
	if (cpu_is_mx51())
		ddr_hf_clk = clk_get(NULL, "ddr_hf_clk");

	dvfsper_device_data->dvfs_clk = clk_get(NULL, dvfsper_data->clk_id);
	dvfs_clk = dvfsper_device_data->dvfs_clk;

	clk_enable(dvfsper_device_data->dvfs_clk);
	ret = init_dvfs_per_controller();
	if (ret) {
		printk(KERN_ERR "DVFS_PER: Unable to initialize DVFS PER");
		goto err1;
	}
	clk_disable(dvfsper_device_data->dvfs_clk);
	ret = sysfs_create_file(&pdev->dev.kobj, &dev_attr_enable.attr);

	if (ret) {
		printk(KERN_ERR
		       "DVFS: Unable to register sysdev entry for dvfs");
		goto err1;
	}

	return 0;
err1:
	dev_err(&pdev->dev, "Failed to probe DVFS\n");
	kfree(dvfsper_device_data);

	return ret;
}


/*!
 * This function is called to put DVFS in a low power state.
 *
 * @param   pdev  the device structure
 * @param   state the power state the device is entering
 *
 * @return  The function always returns 0.
 */
static int mxc_dvfs_suspend(struct platform_device *pdev,
					pm_message_t state)
{
	if (dvfs_per_is_active) {
		stop_dvfs_per();
		dvfs_per_suspended = 1;
	}
	return 0;
}

/*!
 * This function is called to resume the DVFS from a low power state.
 *
 * @param   dev   the device structure
 * @param   level the stage in device suspension process that we want the
 *                device to be put in
 *
 * @return  The function always returns 0.
 */
static int mxc_dvfs_resume(struct platform_device *pdev)
{
	if (dvfs_per_suspended) {
		dvfs_per_suspended = 0;
		return start_dvfs_per();
	}

	return 0;
}


static struct platform_driver mxc_dvfsper_driver = {
	.driver = {
		   .name = "mxc_dvfsper",
		   .owner = THIS_MODULE,
		   },
	.probe = mxc_dvfsper_probe,
	.suspend = mxc_dvfs_suspend,
	.resume = mxc_dvfs_resume,
};

static int __init dvfs_per_init(void)
{
	int err = 0;

	if (platform_driver_register(&mxc_dvfsper_driver) != 0) {
		printk(KERN_ERR "mxc_dvfsper_driver register failed\n");
		return -ENODEV;
	}
	printk(KERN_INFO "DVFS PER driver module loaded\n");

	return err;
}

static void __exit dvfs_per_cleanup(void)
{
	stop_dvfs_per();

	/* release the DVFS interrupt */
	free_irq(dvfsper_device_data->irq, NULL);

	clk_put(dvfs_clk);
	clk_put(main_bus_clk);
	clk_put(pll2);
	clk_put(lpapm);
	clk_put(cpu_clk);
	clk_put(ahb_clk);
	clk_put(axi_b_clk);
	if (cpu_is_mx51())
		clk_put(ddr_hf_clk);

	/* Unregister the device structure */
	platform_driver_unregister(&mxc_dvfsper_driver);

}

module_init(dvfs_per_init);
module_exit(dvfs_per_cleanup);

MODULE_AUTHOR("Freescale Semiconductor, Inc.");
MODULE_DESCRIPTION("DVFS PERIPHERAL  driver");
MODULE_LICENSE("GPL");
