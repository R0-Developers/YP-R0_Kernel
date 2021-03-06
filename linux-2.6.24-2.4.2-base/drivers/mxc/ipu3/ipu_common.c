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
 * @file ipu_common.c
 *
 * @brief This file contains the IPU driver common API functions.
 *
 * @ingroup IPU
 */
#include <linux/types.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/arch/ipu.h>
#include <asm/arch/clock.h>

#include "ipu_prv.h"
#include "ipu_regs.h"
#include "ipu_param_mem.h"

struct ipu_irq_node {
	irqreturn_t(*handler) (int, void *);	/*!< the ISR */
	const char *name;	/*!< device associated with the interrupt */
	void *dev_id;		/*!< some unique information for the ISR */
	__u32 flags;		/*!< not used */
};

/* Globals */
struct clk *g_ipu_clk;
bool g_ipu_clk_enabled;
struct clk *g_di_clk[2];
struct clk *g_ipu_csi_clk;
unsigned char g_dc_di_assignment[10];
int g_ipu_irq[2];
int g_ipu_hw_rev;
bool g_sec_chan_en[21];
uint32_t g_channel_init_mask;
uint32_t g_channel_enable_mask;
DEFINE_SPINLOCK(ipu_lock);
struct device *g_ipu_dev;

static struct ipu_irq_node ipu_irq_list[IPU_IRQ_COUNT];
static const char driver_name[] = "mxc_ipu";

static int ipu_dc_use_count;
static int ipu_dp_use_count;
static int ipu_dmfc_use_count;
static int ipu_ic_use_count;
static int ipu_rot_use_count;
static int ipu_di_use_count[2];

/* for power gating */
static uint32_t ipu_conf_reg;
static uint32_t ic_conf_reg;
static uint32_t ipu_cha_db_mode_reg[4];
static uint32_t ipu_cha_cur_buf_reg[4];
static uint32_t idma_enable_reg[2];
static uint32_t buf_ready_reg[8];

u32 *ipu_cm_reg;
u32 *ipu_idmac_reg;
u32 *ipu_dp_reg;
u32 *ipu_ic_reg;
u32 *ipu_dc_reg;
u32 *ipu_dc_tmpl_reg;
u32 *ipu_dmfc_reg;
u32 *ipu_di_reg[2];
u32 *ipu_cpmem_base;
u32 *ipu_tpmem_base;
u32 *ipu_disp_base[2];

/* Static functions */
static irqreturn_t ipu_irq_handler(int irq, void *desc);

static inline uint32_t channel_2_dma(ipu_channel_t ch, ipu_buffer_t type)
{
	return ((uint32_t) ch >> (6 * type)) & 0x3F;
};

#define idma_is_valid(ch)	(ch != NO_DMA)
#define idma_mask(ch)		(idma_is_valid(ch) ? (1UL << (ch & 0x1F)) : 0)
#define idma_is_set(reg, dma)	(__raw_readl(reg(dma)) & idma_mask(dma))

/*!
 * This function is called by the driver framework to initialize the IPU
 * hardware.
 *
 * @param	dev	The device structure for the IPU passed in by the
 *			driver framework.
 *
 * @return      Returns 0 on success or negative error code on error
 */
static int ipu_probe(struct platform_device *pdev)
{
	struct resource *res;
	struct mxc_ipu_config *plat_data = pdev->dev.platform_data;
	unsigned long ipu_base;
	u32 reg;

	spin_lock_init(&ipu_lock);

	g_ipu_dev = &pdev->dev;

	/* Register IPU interrupts */
	g_ipu_irq[0] = platform_get_irq(pdev, 0);
	if (g_ipu_irq[0] < 0)
		return -EINVAL;

	if (request_irq(g_ipu_irq[0], ipu_irq_handler, 0, pdev->name, 0) != 0) {
		dev_err(g_ipu_dev, "request SYNC interrupt failed\n");
		return -EBUSY;
	}
	/* Some platforms have 2 IPU interrupts */
	g_ipu_irq[1] = platform_get_irq(pdev, 1);
	if (g_ipu_irq[1] >= 0) {
		if (request_irq
		    (g_ipu_irq[1], ipu_irq_handler, 0, pdev->name, 0) != 0) {
			dev_err(g_ipu_dev, "request ERR interrupt failed\n");
			return -EBUSY;
		}
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (IS_ERR(res))
		return -ENODEV;

	ipu_base = res->start;
	ipu_cm_reg = ioremap(ipu_base + IPU_CM_REG_BASE, PAGE_SIZE);
	ipu_ic_reg = ioremap(ipu_base + IPU_IC_REG_BASE, PAGE_SIZE);
	ipu_idmac_reg = ioremap(ipu_base + IPU_IDMAC_REG_BASE, PAGE_SIZE);
	/* DP Registers are accessed thru the SRM */
	ipu_dp_reg = ioremap(ipu_base + IPU_SRM_REG_BASE, PAGE_SIZE);
	ipu_dc_reg = ioremap(ipu_base + IPU_DC_REG_BASE, PAGE_SIZE);
	ipu_dmfc_reg = ioremap(ipu_base + IPU_DMFC_REG_BASE, PAGE_SIZE);
	ipu_di_reg[0] = ioremap(ipu_base + IPU_DI0_REG_BASE, PAGE_SIZE);
	ipu_di_reg[1] = ioremap(ipu_base + IPU_DI1_REG_BASE, PAGE_SIZE);
	ipu_cpmem_base = ioremap(ipu_base + IPU_CPMEM_REG_BASE, PAGE_SIZE);
	ipu_tpmem_base = ioremap(ipu_base + IPU_TPM_REG_BASE, SZ_64K);
	ipu_dc_tmpl_reg = ioremap(ipu_base + IPU_DC_TMPL_REG_BASE, SZ_128K);
	ipu_disp_base[1] = ioremap(ipu_base + IPU_DISP1_BASE, SZ_4K);

	dev_dbg(g_ipu_dev, "IPU CM Regs = %p\n", ipu_cm_reg);
	dev_dbg(g_ipu_dev, "IPU IC Regs = %p\n", ipu_ic_reg);
	dev_dbg(g_ipu_dev, "IPU IDMAC Regs = %p\n", ipu_idmac_reg);
	dev_dbg(g_ipu_dev, "IPU DP Regs = %p\n", ipu_dp_reg);
	dev_dbg(g_ipu_dev, "IPU DC Regs = %p\n", ipu_dc_reg);
	dev_dbg(g_ipu_dev, "IPU DMFC Regs = %p\n", ipu_dmfc_reg);
	dev_dbg(g_ipu_dev, "IPU DI0 Regs = %p\n", ipu_di_reg[0]);
	dev_dbg(g_ipu_dev, "IPU DI1 Regs = %p\n", ipu_di_reg[1]);
	dev_dbg(g_ipu_dev, "IPU CPMem = %p\n", ipu_cpmem_base);
	dev_dbg(g_ipu_dev, "IPU TPMem = %p\n", ipu_tpmem_base);
	dev_dbg(g_ipu_dev, "IPU DC Template Mem = %p\n", ipu_dc_tmpl_reg);
	dev_dbg(g_ipu_dev, "IPU Display Region 1 Mem = %p\n", ipu_disp_base[1]);

	/* Enable IPU and CSI clocks */
	/* Get IPU clock freq */
	g_ipu_clk = clk_get(&pdev->dev, "ipu_clk");
	dev_dbg(g_ipu_dev, "ipu_clk = %lu\n", clk_get_rate(g_ipu_clk));

	clk_enable(g_ipu_clk);

	g_di_clk[0] = plat_data->di_clk[0];
	g_di_clk[1] = plat_data->di_clk[1];

	__raw_writel(0x807FFFFF, IPU_MEM_RST);
	while (__raw_readl(IPU_MEM_RST) & 0x80000000) ;

	_ipu_init_dc_mappings();

	/* Enable error interrupts by default */
	__raw_writel(0xFFFFFFFF, IPU_INT_CTRL(5));
	__raw_writel(0xFFFFFFFF, IPU_INT_CTRL(6));
	__raw_writel(0xFFFFFFFF, IPU_INT_CTRL(9));
	__raw_writel(0xFFFFFFFF, IPU_INT_CTRL(10));

	/* DMFC Init */
	_ipu_dmfc_init();

	/* Set sync refresh channels as high priority */
	__raw_writel(0x18800000L, IDMAC_CHA_PRI(0));

	/* Set MCU_T to divide MCU access window into 2 */
	__raw_writel(0x00400000L | (IPU_MCU_T_DEFAULT << 18), IPU_DISP_GEN);

	reg = __raw_readl(IPU_PM);
	reg &= ~(0x7f << 7);
	reg |= 0x20 << 7;
	__raw_writel(reg, IPU_PM);

	clk_disable(g_ipu_clk);

	register_ipu_device();

	return 0;
}

int ipu_remove(struct platform_device *pdev)
{
	if (g_ipu_irq[0])
		free_irq(g_ipu_irq[0], 0);
	if (g_ipu_irq[1])
		free_irq(g_ipu_irq[1], 0);

	clk_put(g_ipu_clk);

	iounmap(ipu_cm_reg);
	iounmap(ipu_ic_reg);
	iounmap(ipu_idmac_reg);
	iounmap(ipu_dc_reg);
	iounmap(ipu_dp_reg);
	iounmap(ipu_dmfc_reg);
	iounmap(ipu_di_reg[0]);
	iounmap(ipu_di_reg[1]);
	iounmap(ipu_cpmem_base);
	iounmap(ipu_tpmem_base);
	iounmap(ipu_dc_tmpl_reg);
	iounmap(ipu_disp_base[1]);

	return 0;
}

void ipu_dump_registers(void)
{
	printk(KERN_DEBUG "IPU_CONF = \t0x%08X\n", __raw_readl(IPU_CONF));
	printk(KERN_DEBUG "IDMAC_CONF = \t0x%08X\n", __raw_readl(IDMAC_CONF));
	printk(KERN_DEBUG "IDMAC_CHA_EN1 = \t0x%08X\n",
	       __raw_readl(IDMAC_CHA_EN(0)));
	printk(KERN_DEBUG "IDMAC_CHA_EN2 = \t0x%08X\n",
	       __raw_readl(IDMAC_CHA_EN(32)));
	printk(KERN_DEBUG "IDMAC_CHA_PRI1 = \t0x%08X\n",
	       __raw_readl(IDMAC_CHA_PRI(0)));
	printk(KERN_DEBUG "IDMAC_CHA_PRI2 = \t0x%08X\n",
	       __raw_readl(IDMAC_CHA_PRI(32)));
	printk(KERN_DEBUG "IDMAC_BAND_EN1 = \t0x%08X\n",
	       __raw_readl(IDMAC_BAND_EN(0)));
	printk(KERN_DEBUG "IDMAC_BAND_EN2 = \t0x%08X\n",
	       __raw_readl(IDMAC_BAND_EN(32)));
	printk(KERN_DEBUG "IPU_CHA_DB_MODE_SEL0 = \t0x%08X\n",
	       __raw_readl(IPU_CHA_DB_MODE_SEL(0)));
	printk(KERN_DEBUG "IPU_CHA_DB_MODE_SEL1 = \t0x%08X\n",
	       __raw_readl(IPU_CHA_DB_MODE_SEL(32)));
	printk(KERN_DEBUG "DMFC_WR_CHAN = \t0x%08X\n",
	       __raw_readl(DMFC_WR_CHAN));
	printk(KERN_DEBUG "DMFC_WR_CHAN_DEF = \t0x%08X\n",
	       __raw_readl(DMFC_WR_CHAN_DEF));
	printk(KERN_DEBUG "DMFC_DP_CHAN = \t0x%08X\n",
	       __raw_readl(DMFC_DP_CHAN));
	printk(KERN_DEBUG "DMFC_DP_CHAN_DEF = \t0x%08X\n",
	       __raw_readl(DMFC_DP_CHAN_DEF));
	printk(KERN_DEBUG "DMFC_IC_CTRL = \t0x%08X\n",
	       __raw_readl(DMFC_IC_CTRL));
	printk(KERN_DEBUG "IPU_FS_PROC_FLOW1 = \t0x%08X\n",
	       __raw_readl(IPU_FS_PROC_FLOW1));
	printk(KERN_DEBUG "IPU_FS_PROC_FLOW2 = \t0x%08X\n",
	       __raw_readl(IPU_FS_PROC_FLOW2));
	printk(KERN_DEBUG "IPU_FS_PROC_FLOW3 = \t0x%08X\n",
	       __raw_readl(IPU_FS_PROC_FLOW3));
}

/*!
 * This function is called to initialize a logical IPU channel.
 *
 * @param       channel Input parameter for the logical channel ID to init.
 *
 * @param       params  Input parameter containing union of channel
 *                      initialization parameters.
 *
 * @return      Returns 0 on success or negative error code on fail
 */
int32_t ipu_init_channel(ipu_channel_t channel, ipu_channel_params_t *params)
{
	int ret = 0;
	uint32_t ipu_conf;
	uint32_t reg;
	unsigned long lock_flags;

	dev_dbg(g_ipu_dev, "init channel = %d\n", IPU_CHAN_ID(channel));

	/* re-enable error interrupts every time a channel is initialized */
	__raw_writel(0xFFFFFFFF, IPU_INT_CTRL(5));
	__raw_writel(0xFFFFFFFF, IPU_INT_CTRL(6));
	__raw_writel(0xFFFFFFFF, IPU_INT_CTRL(9));
	__raw_writel(0xFFFFFFFF, IPU_INT_CTRL(10));

	ipu_conf = __raw_readl(IPU_CONF);
	if (ipu_conf == 0) {
		g_ipu_clk_enabled = true;
		clk_enable(g_ipu_clk);
	}

	spin_lock_irqsave(&ipu_lock, lock_flags);

	if (g_channel_init_mask & (1L << IPU_CHAN_ID(channel))) {
		dev_dbg(g_ipu_dev, "Warning: channel already initialized %d\n",
			IPU_CHAN_ID(channel));
	}

	switch (channel) {
	case MEM_PRP_VF_MEM:
		ipu_ic_use_count++;
		reg = __raw_readl(IPU_FS_PROC_FLOW1);
		__raw_writel(reg | FS_VF_IN_VALID, IPU_FS_PROC_FLOW1);

		if (params->mem_prp_vf_mem.graphics_combine_en)
			g_sec_chan_en[IPU_CHAN_ID(channel)] = true;

		_ipu_ic_init_prpvf(params, false);
		break;
	case MEM_ROT_VF_MEM:
		ipu_ic_use_count++;
		ipu_rot_use_count++;
		_ipu_ic_init_rotate_vf(params);
		break;
	case MEM_PRP_ENC_MEM:
		ipu_ic_use_count++;
		reg = __raw_readl(IPU_FS_PROC_FLOW1);
		__raw_writel(reg | FS_ENC_IN_VALID, IPU_FS_PROC_FLOW1);
		_ipu_ic_init_prpenc(params, false);
		break;
	case MEM_ROT_ENC_MEM:
		ipu_ic_use_count++;
		ipu_rot_use_count++;
		_ipu_ic_init_rotate_enc(params);
		break;
	case MEM_PP_MEM:
		if (params->mem_pp_mem.graphics_combine_en)
			g_sec_chan_en[IPU_CHAN_ID(channel)] = true;
		_ipu_ic_init_pp(params);
		ipu_ic_use_count++;
		break;
	case MEM_ROT_PP_MEM:
		_ipu_ic_init_rotate_pp(params);
		ipu_ic_use_count++;
		ipu_rot_use_count++;
		break;
	case MEM_DC_SYNC:
		if (params->mem_dc_sync.di > 1) {
			ret = -EINVAL;
			goto err;
		}

		g_dc_di_assignment[1] = params->mem_dc_sync.di;
		_ipu_dc_init(1, params->mem_dc_sync.di,
			     params->mem_dc_sync.interlaced);
		ipu_di_use_count[params->mem_dc_sync.di]++;
		ipu_dc_use_count++;
		ipu_dmfc_use_count++;
		break;
	case MEM_BG_SYNC:
		if (params->mem_dp_bg_sync.di > 1) {
			ret = -EINVAL;
			goto err;
		}

		g_dc_di_assignment[5] = params->mem_dp_bg_sync.di;
		_ipu_dp_init(channel, params->mem_dp_bg_sync.in_pixel_fmt,
			     params->mem_dp_bg_sync.out_pixel_fmt);
		_ipu_dc_init(5, params->mem_dp_bg_sync.di,
			     params->mem_dp_bg_sync.interlaced);
		ipu_di_use_count[params->mem_dp_bg_sync.di]++;
		ipu_dc_use_count++;
		ipu_dp_use_count++;
		ipu_dmfc_use_count++;
		break;
	case MEM_FG_SYNC:
		_ipu_dp_init(channel, params->mem_dp_fg_sync.in_pixel_fmt,
			     params->mem_dp_fg_sync.out_pixel_fmt);
		ipu_dc_use_count++;
		ipu_dp_use_count++;
		ipu_dmfc_use_count++;
		break;
	case DIRECT_ASYNC0:
		if (params->direct_async.di > 1) {
			ret = -EINVAL;
			goto err;
		}

		g_dc_di_assignment[8] = params->direct_async.di;
		_ipu_dc_init(8, params->direct_async.di, false);
		ipu_di_use_count[params->direct_async.di]++;
		ipu_dc_use_count++;
		break;
	case DIRECT_ASYNC1:
		if (params->direct_async.di > 1) {
			ret = -EINVAL;
			goto err;
		}

		g_dc_di_assignment[9] = params->direct_async.di;
		_ipu_dc_init(9, params->direct_async.di, false);
		ipu_di_use_count[params->direct_async.di]++;
		ipu_dc_use_count++;
		break;
	default:
		dev_err(g_ipu_dev, "Missing channel initialization\n");
		break;
	}

	/* Enable IPU sub module */
	g_channel_init_mask |= 1L << IPU_CHAN_ID(channel);
	if (ipu_ic_use_count == 1)
		ipu_conf |= IPU_CONF_IC_EN;
	if (ipu_rot_use_count == 1)
		ipu_conf |= IPU_CONF_ROT_EN;
	if (ipu_dc_use_count == 1)
		ipu_conf |= IPU_CONF_DC_EN;
	if (ipu_dp_use_count == 1)
		ipu_conf |= IPU_CONF_DP_EN;
	if (ipu_dmfc_use_count == 1)
		ipu_conf |= IPU_CONF_DMFC_EN;
	if (ipu_di_use_count[0] == 1) {
		ipu_conf |= IPU_CONF_DI0_EN;
		clk_enable(g_di_clk[0]);
	}
	if (ipu_di_use_count[1] == 1) {
		ipu_conf |= IPU_CONF_DI1_EN;
		clk_enable(g_di_clk[1]);
	}

	__raw_writel(ipu_conf, IPU_CONF);

err:
	spin_unlock_irqrestore(&ipu_lock, lock_flags);
	return ret;
}
EXPORT_SYMBOL(ipu_init_channel);

/*!
 * This function is called to uninitialize a logical IPU channel.
 *
 * @param       channel Input parameter for the logical channel ID to uninit.
 */
void ipu_uninit_channel(ipu_channel_t channel)
{
	unsigned long lock_flags;
	uint32_t reg;
	uint32_t in_dma, out_dma = 0;
	uint32_t ipu_conf;

	if ((g_channel_init_mask & (1L << IPU_CHAN_ID(channel))) == 0) {
		dev_dbg(g_ipu_dev, "Channel already uninitialized %d\n",
			IPU_CHAN_ID(channel));
		return;
	}

	/* Make sure channel is disabled */
	/* Get input and output dma channels */
	in_dma = channel_2_dma(channel, IPU_OUTPUT_BUFFER);
	out_dma = channel_2_dma(channel, IPU_VIDEO_IN_BUFFER);

	if (idma_is_set(IDMAC_CHA_EN, in_dma) ||
	    idma_is_set(IDMAC_CHA_EN, out_dma)) {
		dev_err(g_ipu_dev,
			"Channel %d is not disabled, disable first\n",
			IPU_CHAN_ID(channel));
		return;
	}

	spin_lock_irqsave(&ipu_lock, lock_flags);

	/* Reset the double buffer */
	reg = __raw_readl(IPU_CHA_DB_MODE_SEL(in_dma));
	__raw_writel(reg & ~idma_mask(in_dma), IPU_CHA_DB_MODE_SEL(in_dma));
	reg = __raw_readl(IPU_CHA_DB_MODE_SEL(out_dma));
	__raw_writel(reg & ~idma_mask(out_dma), IPU_CHA_DB_MODE_SEL(out_dma));

	g_sec_chan_en[IPU_CHAN_ID(channel)] = false;

	switch (channel) {
	case MEM_PRP_VF_MEM:
		ipu_ic_use_count--;
		_ipu_ic_uninit_prpvf();
		reg = __raw_readl(IPU_FS_PROC_FLOW1);
		__raw_writel(reg & ~FS_VF_IN_VALID, IPU_FS_PROC_FLOW1);
		break;
	case MEM_ROT_VF_MEM:
		ipu_rot_use_count--;
		ipu_ic_use_count--;
		_ipu_ic_uninit_rotate_vf();
		break;
	case MEM_PRP_ENC_MEM:
		ipu_ic_use_count--;
		_ipu_ic_uninit_prpenc();
		reg = __raw_readl(IPU_FS_PROC_FLOW1);
		__raw_writel(reg & ~FS_ENC_IN_VALID, IPU_FS_PROC_FLOW1);
		break;
	case MEM_ROT_ENC_MEM:
		ipu_rot_use_count--;
		ipu_ic_use_count--;
		_ipu_ic_uninit_rotate_enc();
		break;
	case MEM_PP_MEM:
		ipu_ic_use_count--;
		_ipu_ic_uninit_pp();
		break;
	case MEM_ROT_PP_MEM:
		ipu_rot_use_count--;
		ipu_ic_use_count--;
		_ipu_ic_uninit_rotate_pp();
		break;
	case MEM_DC_SYNC:
		_ipu_dc_uninit(1);
		ipu_di_use_count[g_dc_di_assignment[1]]--;
		ipu_dc_use_count--;
		ipu_dmfc_use_count--;
		break;
	case MEM_BG_SYNC:
		_ipu_dp_uninit(channel);
		_ipu_dc_uninit(5);
		ipu_di_use_count[g_dc_di_assignment[5]]--;
		ipu_dc_use_count--;
		ipu_dp_use_count--;
		ipu_dmfc_use_count--;
		break;
	case MEM_FG_SYNC:
		_ipu_dp_uninit(channel);
		ipu_dc_use_count--;
		ipu_dp_use_count--;
		ipu_dmfc_use_count--;
		break;
	case DIRECT_ASYNC0:
		_ipu_dc_uninit(8);
		ipu_di_use_count[g_dc_di_assignment[8]]--;
		ipu_dc_use_count--;
		break;
	case DIRECT_ASYNC1:
		_ipu_dc_uninit(9);
		ipu_di_use_count[g_dc_di_assignment[9]]--;
		ipu_dc_use_count--;
		break;
	default:
		break;
	}

	g_channel_init_mask &= ~(1L << IPU_CHAN_ID(channel));

	ipu_conf = __raw_readl(IPU_CONF);

	if (ipu_ic_use_count == 0)
		ipu_conf &= ~IPU_CONF_IC_EN;
	if (ipu_rot_use_count == 0)
		ipu_conf &= ~IPU_CONF_ROT_EN;
	if (ipu_dc_use_count == 0)
		ipu_conf &= ~IPU_CONF_DC_EN;
	if (ipu_dp_use_count == 0)
		ipu_conf &= ~IPU_CONF_DP_EN;
	if (ipu_dmfc_use_count == 0)
		ipu_conf &= ~IPU_CONF_DMFC_EN;
	if (ipu_di_use_count[0] == 0) {
		ipu_conf &= ~IPU_CONF_DI0_EN;
		clk_disable(g_di_clk[0]);
	}
	if (ipu_di_use_count[1] == 0) {
		ipu_conf &= ~IPU_CONF_DI1_EN;
		clk_disable(g_di_clk[1]);
	}

	__raw_writel(ipu_conf, IPU_CONF);

	spin_unlock_irqrestore(&ipu_lock, lock_flags);

	if (ipu_conf == 0) {
		clk_disable(g_ipu_clk);
		g_ipu_clk_enabled = false;
	}

	WARN_ON(ipu_ic_use_count < 0);
	WARN_ON(ipu_rot_use_count < 0);
	WARN_ON(ipu_dc_use_count < 0);
	WARN_ON(ipu_dp_use_count < 0);
	WARN_ON(ipu_dmfc_use_count < 0);
}
EXPORT_SYMBOL(ipu_uninit_channel);

/*!
 * This function is called to initialize a buffer for logical IPU channel.
 *
 * @param       channel         Input parameter for the logical channel ID.
 *
 * @param       type            Input parameter which buffer to initialize.
 *
 * @param       pixel_fmt       Input parameter for pixel format of buffer.
 *                              Pixel format is a FOURCC ASCII code.
 *
 * @param       width           Input parameter for width of buffer in pixels.
 *
 * @param       height          Input parameter for height of buffer in pixels.
 *
 * @param       stride          Input parameter for stride length of buffer
 *                              in pixels.
 *
 * @param       rot_mode        Input parameter for rotation setting of buffer.
 *                              A rotation setting other than
 *                              IPU_ROTATE_VERT_FLIP
 *                              should only be used for input buffers of
 *                              rotation channels.
 *
 * @param       phyaddr_0       Input parameter buffer 0 physical address.
 *
 * @param       phyaddr_1       Input parameter buffer 1 physical address.
 *                              Setting this to a value other than NULL enables
 *                              double buffering mode.
 *
 * @param       u		private u offset for additional cropping,
 *				zero if not used.
 *
 * @param       v		private v offset for additional cropping,
 *				zero if not used.
 *
 * @return      Returns 0 on success or negative error code on fail
 */
int32_t ipu_init_channel_buffer(ipu_channel_t channel, ipu_buffer_t type,
				uint32_t pixel_fmt,
				uint16_t width, uint16_t height,
				uint32_t stride,
				ipu_rotate_mode_t rot_mode,
				dma_addr_t phyaddr_0, dma_addr_t phyaddr_1,
				uint32_t u, uint32_t v)
{
	unsigned long lock_flags;
	uint32_t reg;
	uint32_t dma_chan;
	uint32_t burst_size;

	dma_chan = channel_2_dma(channel, type);
	if (!idma_is_valid(dma_chan))
		return -EINVAL;

	if (stride < width * bytes_per_pixel(pixel_fmt))
		stride = width * bytes_per_pixel(pixel_fmt);
	if (stride % 4) {
		dev_err(g_ipu_dev,
			"Stride not 32-bit aligned, stride = %d\n", stride);
		return -EINVAL;
	}
	/* IC channels' width must be multiple of 8 pixels     */
	if ((IPU_CHAN_ID(channel) <= 6) && (width % 8)) {
		dev_err(g_ipu_dev, "Width must be 8 pixel multiple\n");
		return -EINVAL;
	}

	/* Build parameter memory data for DMA channel */
	_ipu_ch_param_init(dma_chan, pixel_fmt, width, height, stride, u, v, 0,
			   phyaddr_0, phyaddr_1);
	if (rot_mode)
		_ipu_ch_param_set_rotation(dma_chan, rot_mode);

	/* IC and ROT channels have restriction of 8 or 16 pix burst length */
	if ((channel == MEM_PRP_ENC_MEM) || (channel == MEM_PRP_VF_MEM) ||
	    (channel == MEM_PP_MEM)) {
		if ((width % 16) == 0)
			_ipu_ch_param_set_burst_size(dma_chan, 16);
		else
			_ipu_ch_param_set_burst_size(dma_chan, 8);
	} else if ((channel == MEM_ROT_ENC_MEM) ||
		   (channel == MEM_ROT_VF_MEM) || (channel == MEM_ROT_PP_MEM)) {
		_ipu_ch_param_set_burst_size(dma_chan, 8);
		_ipu_ch_param_set_block_mode(dma_chan);
	}

	_ipu_dmfc_set_wait4eot(dma_chan, width);

	if (_ipu_chan_is_interlaced(channel))
		_ipu_ch_param_set_interlaced_scan(dma_chan);

	burst_size = _ipu_ch_param_get_burst_size(dma_chan);
	_ipu_ic_idma_init(dma_chan, width, height, burst_size, rot_mode);

	if (idma_is_set(IDMAC_CHA_PRI, dma_chan))
		_ipu_ch_param_set_high_priority(dma_chan);

	_ipu_ch_param_dump(dma_chan);

	spin_lock_irqsave(&ipu_lock, lock_flags);

	reg = __raw_readl(IPU_CHA_DB_MODE_SEL(dma_chan));
	if (phyaddr_1)
		reg |= idma_mask(dma_chan);
	else
		reg &= ~idma_mask(dma_chan);
	__raw_writel(reg, IPU_CHA_DB_MODE_SEL(dma_chan));

	/* Reset to buffer 0 */
	__raw_writel(idma_mask(dma_chan), IPU_CHA_CUR_BUF(dma_chan));

	spin_unlock_irqrestore(&ipu_lock, lock_flags);

	return 0;
}
EXPORT_SYMBOL(ipu_init_channel_buffer);

/*!
 * This function is called to update the physical address of a buffer for
 * a logical IPU channel.
 *
 * @param       channel         Input parameter for the logical channel ID.
 *
 * @param       type            Input parameter which buffer to initialize.
 *
 * @param       bufNum          Input parameter for buffer number to update.
 *                              0 or 1 are the only valid values.
 *
 * @param       phyaddr         Input parameter buffer physical address.
 *
 * @return      This function returns 0 on success or negative error code on
 *              fail. This function will fail if the buffer is set to ready.
 */
int32_t ipu_update_channel_buffer(ipu_channel_t channel, ipu_buffer_t type,
				  uint32_t bufNum, dma_addr_t phyaddr)
{
	uint32_t reg;
	int ret = 0;
	unsigned long lock_flags;
	uint32_t dma_chan = channel_2_dma(channel, type);

	if (dma_chan == IDMA_CHAN_INVALID)
		return -EINVAL;

	spin_lock_irqsave(&ipu_lock, lock_flags);

	if (bufNum == 0)
		reg = __raw_readl(IPU_CHA_BUF0_RDY(dma_chan));
	else
		reg = __raw_readl(IPU_CHA_BUF1_RDY(dma_chan));

	if ((reg & idma_mask(dma_chan)) == 0)
		_ipu_ch_param_set_buffer(dma_chan, bufNum, phyaddr);
	else
		ret = -EACCES;

	spin_unlock_irqrestore(&ipu_lock, lock_flags);
	return ret;
}
EXPORT_SYMBOL(ipu_update_channel_buffer);

/*!
 * This function is called to set a channel's buffer as ready.
 *
 * @param       channel         Input parameter for the logical channel ID.
 *
 * @param       type            Input parameter which buffer to initialize.
 *
 * @param       bufNum          Input parameter for which buffer number set to
 *                              ready state.
 *
 * @return      Returns 0 on success or negative error code on fail
 */
int32_t ipu_select_buffer(ipu_channel_t channel, ipu_buffer_t type,
			  uint32_t bufNum)
{
	uint32_t dma_chan = channel_2_dma(channel, type);

	if (dma_chan == IDMA_CHAN_INVALID)
		return -EINVAL;

	if (bufNum == 0) {
		/*Mark buffer 0 as ready. */
		__raw_writel(idma_mask(dma_chan), IPU_CHA_BUF0_RDY(dma_chan));
	} else {
		/*Mark buffer 1 as ready. */
		__raw_writel(idma_mask(dma_chan), IPU_CHA_BUF1_RDY(dma_chan));
	}
	return 0;
}
EXPORT_SYMBOL(ipu_select_buffer);

static int proc_dest_sel[] =
    { 0, 1, 1, 3, 5, 5, 4, 7, 8, 9, 10, 11, 12, 14, 15 };
static int proc_src_sel[] = { 0, 6, 7, 6, 7, 8, 5, };
static int disp_src_sel[] = { 0, 6, 7, 8, 3, 4, 5, };

/*!
 * This function links 2 channels together for automatic frame
 * synchronization. The output of the source channel is linked to the input of
 * the destination channel.
 *
 * @param       src_ch          Input parameter for the logical channel ID of
 *                              the source channel.
 *
 * @param       dest_ch         Input parameter for the logical channel ID of
 *                              the destination channel.
 *
 * @return      This function returns 0 on success or negative error code on
 *              fail.
 */
int32_t ipu_link_channels(ipu_channel_t src_ch, ipu_channel_t dest_ch)
{
	int retval = 0;
	unsigned long lock_flags;
	uint32_t fs_proc_flow1;
	uint32_t fs_proc_flow2;
	uint32_t fs_disp_flow1;

	spin_lock_irqsave(&ipu_lock, lock_flags);

	fs_proc_flow1 = __raw_readl(IPU_FS_PROC_FLOW1);
	fs_proc_flow2 = __raw_readl(IPU_FS_PROC_FLOW2);
	fs_disp_flow1 = __raw_readl(IPU_FS_DISP_FLOW1);

	switch (src_ch) {
	case MEM_PP_MEM:
		fs_proc_flow2 &= ~FS_PP_DEST_SEL_MASK;
		fs_proc_flow2 |=
		    proc_dest_sel[IPU_CHAN_ID(dest_ch)] <<
		    FS_PP_DEST_SEL_OFFSET;
		break;
	case MEM_ROT_PP_MEM:
		fs_proc_flow2 &= ~FS_PP_ROT_DEST_SEL_MASK;
		fs_proc_flow2 |=
		    proc_dest_sel[IPU_CHAN_ID(dest_ch)] <<
		    FS_PP_ROT_DEST_SEL_OFFSET;
		break;
	case MEM_PRP_ENC_MEM:
		fs_proc_flow2 &= ~FS_PRPENC_DEST_SEL_MASK;
		fs_proc_flow2 |=
		    proc_dest_sel[IPU_CHAN_ID(dest_ch)] <<
		    FS_PRPENC_DEST_SEL_OFFSET;
		break;
	case MEM_ROT_ENC_MEM:
		fs_proc_flow2 &= ~FS_PRPENC_ROT_DEST_SEL_MASK;
		fs_proc_flow2 |=
		    proc_dest_sel[IPU_CHAN_ID(dest_ch)] <<
		    FS_PRPENC_ROT_DEST_SEL_OFFSET;
		break;
	case MEM_PRP_VF_MEM:
		fs_proc_flow2 &= ~FS_PRPVF_DEST_SEL_MASK;
		fs_proc_flow2 |=
		    proc_dest_sel[IPU_CHAN_ID(dest_ch)] <<
		    FS_PRPVF_DEST_SEL_OFFSET;
		break;
	case MEM_ROT_VF_MEM:
		fs_proc_flow2 &= ~FS_PRPVF_ROT_DEST_SEL_MASK;
		fs_proc_flow2 |=
		    proc_dest_sel[IPU_CHAN_ID(dest_ch)] <<
		    FS_PRPVF_ROT_DEST_SEL_OFFSET;
		break;
	default:
		retval = -EINVAL;
		goto err;
	}

	switch (dest_ch) {
	case MEM_PP_MEM:
		fs_proc_flow1 &= ~FS_PP_SRC_SEL_MASK;
		fs_proc_flow1 |=
		    proc_src_sel[IPU_CHAN_ID(src_ch)] << FS_PP_SRC_SEL_OFFSET;
		break;
	case MEM_ROT_PP_MEM:
		fs_proc_flow1 &= ~FS_PP_ROT_SRC_SEL_MASK;
		fs_proc_flow1 |=
		    proc_src_sel[IPU_CHAN_ID(src_ch)] <<
		    FS_PP_ROT_SRC_SEL_OFFSET;
		break;
	case MEM_PRP_ENC_MEM:
		fs_proc_flow1 &= ~FS_PRP_SRC_SEL_MASK;
		fs_proc_flow1 |=
		    proc_src_sel[IPU_CHAN_ID(src_ch)] << FS_PRP_SRC_SEL_OFFSET;
		break;
	case MEM_ROT_ENC_MEM:
		fs_proc_flow1 &= ~FS_PRPENC_ROT_SRC_SEL_MASK;
		fs_proc_flow1 |=
		    proc_src_sel[IPU_CHAN_ID(src_ch)] <<
		    FS_PRPENC_ROT_SRC_SEL_OFFSET;
		break;
	case MEM_PRP_VF_MEM:
		fs_proc_flow1 &= ~FS_PRP_SRC_SEL_MASK;
		fs_proc_flow1 |=
		    proc_src_sel[IPU_CHAN_ID(src_ch)] << FS_PRP_SRC_SEL_OFFSET;
		break;
	case MEM_ROT_VF_MEM:
		fs_proc_flow1 &= ~FS_PRPVF_ROT_SRC_SEL_MASK;
		fs_proc_flow1 |=
		    proc_src_sel[IPU_CHAN_ID(src_ch)] <<
		    FS_PRPVF_ROT_SRC_SEL_OFFSET;
		break;
	case MEM_DC_SYNC:
		fs_disp_flow1 &= ~FS_DC1_SRC_SEL_MASK;
		fs_disp_flow1 |=
		    disp_src_sel[IPU_CHAN_ID(src_ch)] << FS_DC1_SRC_SEL_OFFSET;
		break;
	case MEM_BG_SYNC:
		fs_disp_flow1 &= ~FS_DP_SYNC0_SRC_SEL_MASK;
		fs_disp_flow1 |=
		    disp_src_sel[IPU_CHAN_ID(src_ch)] <<
		    FS_DP_SYNC0_SRC_SEL_OFFSET;
		break;
	case MEM_FG_SYNC:
		fs_disp_flow1 &= ~FS_DP_SYNC1_SRC_SEL_MASK;
		fs_disp_flow1 |=
		    disp_src_sel[IPU_CHAN_ID(src_ch)] <<
		    FS_DP_SYNC1_SRC_SEL_OFFSET;
		break;
	case MEM_DC_ASYNC:
		fs_disp_flow1 &= ~FS_DC2_SRC_SEL_MASK;
		fs_disp_flow1 |=
		    disp_src_sel[IPU_CHAN_ID(src_ch)] << FS_DC2_SRC_SEL_OFFSET;
		break;
	case MEM_BG_ASYNC0:
		fs_disp_flow1 &= ~FS_DP_ASYNC0_SRC_SEL_MASK;
		fs_disp_flow1 |=
		    disp_src_sel[IPU_CHAN_ID(src_ch)] <<
		    FS_DP_ASYNC0_SRC_SEL_OFFSET;
		break;
	case MEM_FG_ASYNC0:
		fs_disp_flow1 &= ~FS_DP_ASYNC1_SRC_SEL_MASK;
		fs_disp_flow1 |=
		    disp_src_sel[IPU_CHAN_ID(src_ch)] <<
		    FS_DP_ASYNC1_SRC_SEL_OFFSET;
		break;
	default:
		retval = -EINVAL;
		goto err;
	}

	__raw_writel(fs_proc_flow1, IPU_FS_PROC_FLOW1);
	__raw_writel(fs_proc_flow2, IPU_FS_PROC_FLOW2);
	__raw_writel(fs_disp_flow1, IPU_FS_DISP_FLOW1);

err:
	spin_unlock_irqrestore(&ipu_lock, lock_flags);
	return retval;
}
EXPORT_SYMBOL(ipu_link_channels);

/*!
 * This function unlinks 2 channels and disables automatic frame
 * synchronization.
 *
 * @param       src_ch          Input parameter for the logical channel ID of
 *                              the source channel.
 *
 * @param       dest_ch         Input parameter for the logical channel ID of
 *                              the destination channel.
 *
 * @return      This function returns 0 on success or negative error code on
 *              fail.
 */
int32_t ipu_unlink_channels(ipu_channel_t src_ch, ipu_channel_t dest_ch)
{
	int retval = 0;
	unsigned long lock_flags;
	uint32_t fs_proc_flow1;
	uint32_t fs_proc_flow2;
	uint32_t fs_disp_flow1;

	spin_lock_irqsave(&ipu_lock, lock_flags);

	fs_proc_flow1 = __raw_readl(IPU_FS_PROC_FLOW1);
	fs_proc_flow2 = __raw_readl(IPU_FS_PROC_FLOW2);
	fs_disp_flow1 = __raw_readl(IPU_FS_DISP_FLOW1);

	switch (src_ch) {
	case MEM_PP_MEM:
		fs_proc_flow2 &= ~FS_PP_DEST_SEL_MASK;
		break;
	case MEM_ROT_PP_MEM:
		fs_proc_flow2 &= ~FS_PP_ROT_DEST_SEL_MASK;
		break;
	case MEM_PRP_ENC_MEM:
		fs_proc_flow2 &= ~FS_PRPENC_DEST_SEL_MASK;
		break;
	case MEM_ROT_ENC_MEM:
		fs_proc_flow2 &= ~FS_PRPENC_ROT_DEST_SEL_MASK;
		break;
	case MEM_PRP_VF_MEM:
		fs_proc_flow2 &= ~FS_PRPVF_DEST_SEL_MASK;
		break;
	case MEM_ROT_VF_MEM:
		fs_proc_flow2 &= ~FS_PRPVF_ROT_DEST_SEL_MASK;
		break;
	default:
		retval = -EINVAL;
		goto err;
	}

	switch (dest_ch) {
	case MEM_PP_MEM:
		fs_proc_flow1 &= ~FS_PP_SRC_SEL_MASK;
		break;
	case MEM_ROT_PP_MEM:
		fs_proc_flow1 &= ~FS_PP_ROT_SRC_SEL_MASK;
		break;
	case MEM_PRP_ENC_MEM:
		fs_proc_flow1 &= ~FS_PRP_SRC_SEL_MASK;
		break;
	case MEM_ROT_ENC_MEM:
		fs_proc_flow1 &= ~FS_PRPENC_ROT_SRC_SEL_MASK;
		break;
	case MEM_PRP_VF_MEM:
		fs_proc_flow1 &= ~FS_PRP_SRC_SEL_MASK;
		break;
	case MEM_ROT_VF_MEM:
		fs_proc_flow1 &= ~FS_PRPVF_ROT_SRC_SEL_MASK;
		break;
	case MEM_DC_SYNC:
		fs_disp_flow1 &= ~FS_DC1_SRC_SEL_MASK;
		break;
	case MEM_BG_SYNC:
		fs_disp_flow1 &= ~FS_DP_SYNC0_SRC_SEL_MASK;
		break;
	case MEM_FG_SYNC:
		fs_disp_flow1 &= ~FS_DP_SYNC1_SRC_SEL_MASK;
		break;
	case MEM_DC_ASYNC:
		fs_disp_flow1 &= ~FS_DC2_SRC_SEL_MASK;
		break;
	case MEM_BG_ASYNC0:
		fs_disp_flow1 &= ~FS_DP_ASYNC0_SRC_SEL_MASK;
		break;
	case MEM_FG_ASYNC0:
		fs_disp_flow1 &= ~FS_DP_ASYNC1_SRC_SEL_MASK;
		break;
	default:
		retval = -EINVAL;
		goto err;
	}

	__raw_writel(fs_proc_flow1, IPU_FS_PROC_FLOW1);
	__raw_writel(fs_proc_flow2, IPU_FS_PROC_FLOW2);
	__raw_writel(fs_disp_flow1, IPU_FS_DISP_FLOW1);

err:
	spin_unlock_irqrestore(&ipu_lock, lock_flags);
	return retval;
}
EXPORT_SYMBOL(ipu_unlink_channels);

/*!
 * This function check whether a logical channel was enabled.
 *
 * @param       channel         Input parameter for the logical channel ID.
 *
 * @return      This function returns 1 while request channel is enabled or
 *              0 for not enabled.
 */
int32_t ipu_is_channel_busy(ipu_channel_t channel)
{
	uint32_t reg;
	uint32_t in_dma;
	uint32_t out_dma;

	out_dma = channel_2_dma(channel, IPU_OUTPUT_BUFFER);
	in_dma = channel_2_dma(channel, IPU_VIDEO_IN_BUFFER);

	reg = __raw_readl(IDMAC_CHA_EN(in_dma));
	if(reg & idma_mask(in_dma))
		return 1;
	reg = __raw_readl(IDMAC_CHA_EN(out_dma));
	if(reg & idma_mask(out_dma))
		return 1;
	return 0;
}
EXPORT_SYMBOL(ipu_is_channel_busy);

/*!
 * This function enables a logical channel.
 *
 * @param       channel         Input parameter for the logical channel ID.
 *
 * @return      This function returns 0 on success or negative error code on
 *              fail.
 */
int32_t ipu_enable_channel(ipu_channel_t channel)
{
	uint32_t reg;
	unsigned long lock_flags;
	uint32_t in_dma;
	uint32_t out_dma;

	if (g_channel_enable_mask & (1L << IPU_CHAN_ID(channel))) {
		dev_dbg(g_ipu_dev, "Warning: channel already enabled %d\n",
			IPU_CHAN_ID(channel));
	}

	/* Get input and output dma channels */
	out_dma = channel_2_dma(channel, IPU_OUTPUT_BUFFER);
	in_dma = channel_2_dma(channel, IPU_VIDEO_IN_BUFFER);

	spin_lock_irqsave(&ipu_lock, lock_flags);

	if (idma_is_valid(in_dma)) {
		reg = __raw_readl(IDMAC_CHA_EN(in_dma));
		__raw_writel(reg | idma_mask(in_dma), IDMAC_CHA_EN(in_dma));
	}
	if (idma_is_valid(out_dma)) {
		reg = __raw_readl(IDMAC_CHA_EN(out_dma));
		__raw_writel(reg | idma_mask(out_dma), IDMAC_CHA_EN(out_dma));
	}

	if ((channel == MEM_DC_SYNC) || (channel == MEM_BG_SYNC) ||
	    (channel == MEM_FG_SYNC))
		_ipu_dp_dc_enable(channel);

	if (IPU_CHAN_ID(channel) <= IPU_CHAN_ID(MEM_PP_MEM))
		_ipu_ic_enable_task(channel);

	spin_unlock_irqrestore(&ipu_lock, lock_flags);

	g_channel_enable_mask |= 1L << IPU_CHAN_ID(channel);
	return 0;
}
EXPORT_SYMBOL(ipu_enable_channel);

/*!
 * This function disables a logical channel.
 *
 * @param       channel         Input parameter for the logical channel ID.
 *
 * @param       wait_for_stop   Flag to set whether to wait for channel end
 *                              of frame or return immediately.
 *
 * @return      This function returns 0 on success or negative error code on
 *              fail.
 */
int32_t ipu_disable_channel(ipu_channel_t channel, bool wait_for_stop)
{
	uint32_t reg;
	unsigned long lock_flags;
	uint32_t in_dma;
	uint32_t out_dma;
	uint32_t timeout;

	if ((g_channel_enable_mask & (1L << IPU_CHAN_ID(channel))) == 0) {
		dev_dbg(g_ipu_dev, "Channel already disabled %d\n",
			IPU_CHAN_ID(channel));
		return 0;
	}

	/* Get input and output dma channels */
	out_dma = channel_2_dma(channel, IPU_OUTPUT_BUFFER);
	in_dma = channel_2_dma(channel, IPU_VIDEO_IN_BUFFER);

	if ((idma_is_valid(in_dma) &&
		!idma_is_set(IDMAC_CHA_EN, in_dma))
		&& (idma_is_valid(out_dma) &&
		!idma_is_set(IDMAC_CHA_EN, out_dma)))
		return -EINVAL;

	if ((channel == MEM_BG_SYNC) || (channel == MEM_FG_SYNC) ||
	    (channel == MEM_DC_SYNC)) {
		_ipu_dp_dc_disable(channel, false);
	} else if (wait_for_stop) {
		timeout = 40;
		while (idma_is_set(IDMAC_CHA_BUSY, in_dma) ||
		       idma_is_set(IDMAC_CHA_BUSY, out_dma) ||
		       (_ipu_channel_status(channel) == TASK_STAT_ACTIVE)) {
			timeout--;
			msleep(10);
			if (timeout == 0) {
				ipu_dump_registers();
				break;
			}
		}
		dev_dbg(g_ipu_dev, "timeout = %d * 10ms\n", 40 - timeout);
	}

	spin_lock_irqsave(&ipu_lock, lock_flags);

	/* Disable IC task */
	if (IPU_CHAN_ID(channel) < IPU_CHAN_ID(MEM_DC_SYNC))
		_ipu_ic_disable_task(channel);

	/* Disable DMA channel(s) */
	if (idma_is_valid(in_dma)) {
		reg = __raw_readl(IDMAC_CHA_EN(in_dma));
		__raw_writel(reg & ~idma_mask(in_dma), IDMAC_CHA_EN(in_dma));
		__raw_writel(idma_mask(in_dma), IPU_CHA_CUR_BUF(in_dma));
	}
	if (idma_is_valid(out_dma)) {
		reg = __raw_readl(IDMAC_CHA_EN(out_dma));
		__raw_writel(reg & ~idma_mask(out_dma), IDMAC_CHA_EN(out_dma));
		__raw_writel(idma_mask(out_dma), IPU_CHA_CUR_BUF(out_dma));
	}

	/* Set channel buffers NOT to be ready */
	__raw_writel(0xF0000000, IPU_GPR); /* write one to clear */
	if (idma_is_valid(in_dma)) {
		if (idma_is_set(IPU_CHA_BUF0_RDY, in_dma)) {
			__raw_writel(idma_mask(in_dma),
				     IPU_CHA_BUF0_RDY(in_dma));
		}
		if (idma_is_set(IPU_CHA_BUF1_RDY, in_dma)) {
			__raw_writel(idma_mask(in_dma),
				     IPU_CHA_BUF1_RDY(in_dma));
		}
	}
	if (idma_is_valid(out_dma)) {
		if (idma_is_set(IPU_CHA_BUF0_RDY, out_dma)) {
			__raw_writel(idma_mask(out_dma),
				     IPU_CHA_BUF0_RDY(out_dma));
		}
		if (idma_is_set(IPU_CHA_BUF1_RDY, out_dma)) {
			__raw_writel(idma_mask(out_dma),
				     IPU_CHA_BUF1_RDY(out_dma));
		}
	}
	__raw_writel(0x0, IPU_GPR); /* write one to set */

	spin_unlock_irqrestore(&ipu_lock, lock_flags);

	g_channel_enable_mask &= ~(1L << IPU_CHAN_ID(channel));

	return 0;
}
EXPORT_SYMBOL(ipu_disable_channel);

void ipu_wait_interrupt(int irq, int ms)
{
	int timeout = ms;

	__raw_writel(IPUIRQ_2_MASK(irq),
			IPUIRQ_2_STATREG(irq));
	while ((__raw_readl(IPUIRQ_2_STATREG(irq)) &
				IPUIRQ_2_MASK(irq)) == 0) {
		msleep(2);
		timeout -= 2;
		if (timeout <= 0) {
			dev_err(g_ipu_dev, "Did not got ipu irq %d within %dms\n", irq, ms);
			break;
		}
	}
}

static irqreturn_t ipu_irq_handler(int irq, void *desc)
{
	int i;
	uint32_t line;
	irqreturn_t result = IRQ_NONE;
	uint32_t int_stat;
	const int err_reg[] = { 5, 6, 9, 10, 0 };
	const int int_reg[] = { 1, 2, 3, 4, 11, 12, 13, 14, 15, 0 };

	if (g_ipu_irq[1]) {
		disable_irq(g_ipu_irq[0]);
		disable_irq(g_ipu_irq[1]);
	}

	for (i = 0;; i++) {
		if (err_reg[i] == 0)
			break;
		int_stat = __raw_readl(IPU_INT_STAT(err_reg[i]));
		int_stat &= __raw_readl(IPU_INT_CTRL(err_reg[i]));
		if (int_stat) {
			__raw_writel(int_stat, IPU_INT_STAT(err_reg[i]));
			dev_err(g_ipu_dev,
				"IPU Error - IPU_INT_STAT_%d = 0x%08X\n",
				err_reg[i], int_stat);

			/* Disable interrupts so we only get error once */
			int_stat =
			    __raw_readl(IPU_INT_CTRL(err_reg[i])) & ~int_stat;
			__raw_writel(int_stat, IPU_INT_CTRL(err_reg[i]));
		}
	}

	for (i = 0;; i++) {
		if (int_reg[i] == 0)
			break;
		int_stat = __raw_readl(IPU_INT_STAT(int_reg[i]));
		int_stat &= __raw_readl(IPU_INT_CTRL(int_reg[i]));
		__raw_writel(int_stat, IPU_INT_STAT(int_reg[i]));
		while ((line = ffs(int_stat)) != 0) {
			line--;
			int_stat &= ~(1UL << line);
			line += (int_reg[i] - 1) * 32;
			result |=
			    ipu_irq_list[line].handler(line,
						       ipu_irq_list[line].
						       dev_id);
		}
	}

	if (g_ipu_irq[1]) {
		enable_irq(g_ipu_irq[0]);
		enable_irq(g_ipu_irq[1]);
	}
	return result;
}

/*!
 * This function enables the interrupt for the specified interrupt line.
 * The interrupt lines are defined in \b ipu_irq_line enum.
 *
 * @param       irq             Interrupt line to enable interrupt for.
 *
 */
void ipu_enable_irq(uint32_t irq)
{
	uint32_t reg;
	unsigned long lock_flags;

	if (!g_ipu_clk_enabled)
		clk_enable(g_ipu_clk);

	spin_lock_irqsave(&ipu_lock, lock_flags);

	reg = __raw_readl(IPUIRQ_2_CTRLREG(irq));
	reg |= IPUIRQ_2_MASK(irq);
	__raw_writel(reg, IPUIRQ_2_CTRLREG(irq));

	spin_unlock_irqrestore(&ipu_lock, lock_flags);
	if (!g_ipu_clk_enabled)
		clk_disable(g_ipu_clk);
}
EXPORT_SYMBOL(ipu_enable_irq);

/*!
 * This function disables the interrupt for the specified interrupt line.
 * The interrupt lines are defined in \b ipu_irq_line enum.
 *
 * @param       irq             Interrupt line to disable interrupt for.
 *
 */
void ipu_disable_irq(uint32_t irq)
{
	uint32_t reg;
	unsigned long lock_flags;

	if (!g_ipu_clk_enabled)
		clk_enable(g_ipu_clk);
	spin_lock_irqsave(&ipu_lock, lock_flags);

	reg = __raw_readl(IPUIRQ_2_CTRLREG(irq));
	reg &= ~IPUIRQ_2_MASK(irq);
	__raw_writel(reg, IPUIRQ_2_CTRLREG(irq));

	spin_unlock_irqrestore(&ipu_lock, lock_flags);
	if (!g_ipu_clk_enabled)
		clk_disable(g_ipu_clk);
}
EXPORT_SYMBOL(ipu_disable_irq);

/*!
 * This function clears the interrupt for the specified interrupt line.
 * The interrupt lines are defined in \b ipu_irq_line enum.
 *
 * @param       irq             Interrupt line to clear interrupt for.
 *
 */
void ipu_clear_irq(uint32_t irq)
{
	if (!g_ipu_clk_enabled)
		clk_enable(g_ipu_clk);

	__raw_writel(IPUIRQ_2_MASK(irq), IPUIRQ_2_STATREG(irq));

	if (!g_ipu_clk_enabled)
		clk_disable(g_ipu_clk);
}
EXPORT_SYMBOL(ipu_clear_irq);

/*!
 * This function returns the current interrupt status for the specified
 * interrupt line. The interrupt lines are defined in \b ipu_irq_line enum.
 *
 * @param       irq             Interrupt line to get status for.
 *
 * @return      Returns true if the interrupt is pending/asserted or false if
 *              the interrupt is not pending.
 */
bool ipu_get_irq_status(uint32_t irq)
{
	uint32_t reg;

	if (!g_ipu_clk_enabled)
		clk_enable(g_ipu_clk);

	reg = __raw_readl(IPUIRQ_2_STATREG(irq));

	if (!g_ipu_clk_enabled)
		clk_disable(g_ipu_clk);

	if (reg & IPUIRQ_2_MASK(irq))
		return true;
	else
		return false;
}
EXPORT_SYMBOL(ipu_get_irq_status);

/*!
 * This function registers an interrupt handler function for the specified
 * interrupt line. The interrupt lines are defined in \b ipu_irq_line enum.
 *
 * @param       irq             Interrupt line to get status for.
 *
 * @param       handler         Input parameter for address of the handler
 *                              function.
 *
 * @param       irq_flags       Flags for interrupt mode. Currently not used.
 *
 * @param       devname         Input parameter for string name of driver
 *                              registering the handler.
 *
 * @param       dev_id          Input parameter for pointer of data to be
 *                              passed to the handler.
 *
 * @return      This function returns 0 on success or negative error code on
 *              fail.
 */
int ipu_request_irq(uint32_t irq,
		    irqreturn_t(*handler) (int, void *),
		    uint32_t irq_flags, const char *devname, void *dev_id)
{
	unsigned long lock_flags;

	BUG_ON(irq >= IPU_IRQ_COUNT);

	spin_lock_irqsave(&ipu_lock, lock_flags);

	if (ipu_irq_list[irq].handler != NULL) {
		dev_err(g_ipu_dev,
			"handler already installed on irq %d\n", irq);
		spin_unlock_irqrestore(&ipu_lock, lock_flags);
		return -EINVAL;
	}

	ipu_irq_list[irq].handler = handler;
	ipu_irq_list[irq].flags = irq_flags;
	ipu_irq_list[irq].dev_id = dev_id;
	ipu_irq_list[irq].name = devname;

	spin_unlock_irqrestore(&ipu_lock, lock_flags);

	ipu_enable_irq(irq);	/* enable the interrupt */

	return 0;
}
EXPORT_SYMBOL(ipu_request_irq);

/*!
 * This function unregisters an interrupt handler for the specified interrupt
 * line. The interrupt lines are defined in \b ipu_irq_line enum.
 *
 * @param       irq             Interrupt line to get status for.
 *
 * @param       dev_id          Input parameter for pointer of data to be passed
 *                              to the handler. This must match value passed to
 *                              ipu_request_irq().
 *
 */
void ipu_free_irq(uint32_t irq, void *dev_id)
{
	ipu_disable_irq(irq);	/* disable the interrupt */

	if (ipu_irq_list[irq].dev_id == dev_id)
		ipu_irq_list[irq].handler = NULL;
}
EXPORT_SYMBOL(ipu_free_irq);

uint32_t _ipu_channel_status(ipu_channel_t channel)
{
	uint32_t stat = 0;
	uint32_t task_stat_reg = __raw_readl(IPU_PROC_TASK_STAT);

	switch (channel) {
	case MEM_PRP_VF_MEM:
		stat = (task_stat_reg & TSTAT_VF_MASK) >> TSTAT_VF_OFFSET;
		break;
	case MEM_ROT_VF_MEM:
		stat =
		    (task_stat_reg & TSTAT_VF_ROT_MASK) >> TSTAT_VF_ROT_OFFSET;
		break;
	case MEM_PRP_ENC_MEM:
		stat = (task_stat_reg & TSTAT_ENC_MASK) >> TSTAT_ENC_OFFSET;
		break;
	case MEM_ROT_ENC_MEM:
		stat =
		    (task_stat_reg & TSTAT_ENC_ROT_MASK) >>
		    TSTAT_ENC_ROT_OFFSET;
		break;
	case MEM_PP_MEM:
		stat = (task_stat_reg & TSTAT_PP_MASK) >> TSTAT_PP_OFFSET;
		break;
	case MEM_ROT_PP_MEM:
		stat =
		    (task_stat_reg & TSTAT_PP_ROT_MASK) >> TSTAT_PP_ROT_OFFSET;
		break;

	default:
		stat = TASK_STAT_IDLE;
		break;
	}
	return stat;
}

int32_t ipu_swap_channel(ipu_channel_t from_ch, ipu_channel_t to_ch)
{
	uint32_t reg;
	unsigned long lock_flags;

	int from_dma = channel_2_dma(from_ch, IPU_INPUT_BUFFER);
	int to_dma = channel_2_dma(to_ch, IPU_INPUT_BUFFER);

	/*enable target channel*/
	spin_lock_irqsave(&ipu_lock, lock_flags);

	reg = __raw_readl(IDMAC_CHA_EN(to_dma));
	__raw_writel(reg | idma_mask(to_dma), IDMAC_CHA_EN(to_dma));

	g_channel_enable_mask |= 1L << IPU_CHAN_ID(to_ch);

	spin_unlock_irqrestore(&ipu_lock, lock_flags);

	/*switch dp dc*/
	_ipu_dp_dc_disable(from_ch, true);

	/*disable source channel*/
	spin_lock_irqsave(&ipu_lock, lock_flags);
	reg = __raw_readl(IDMAC_CHA_EN(from_dma));
	__raw_writel(reg & ~idma_mask(from_dma), IDMAC_CHA_EN(from_dma));
	__raw_writel(idma_mask(from_dma), IPU_CHA_CUR_BUF(from_dma));
	spin_unlock_irqrestore(&ipu_lock, lock_flags);

	g_channel_enable_mask &= ~(1L << IPU_CHAN_ID(from_ch));

	return 0;
}
EXPORT_SYMBOL(ipu_swap_channel);

uint32_t bytes_per_pixel(uint32_t fmt)
{
	switch (fmt) {
	case IPU_PIX_FMT_GENERIC:	/*generic data */
	case IPU_PIX_FMT_RGB332:
	case IPU_PIX_FMT_YUV420P:
	case IPU_PIX_FMT_YUV422P:
		return 1;
		break;
	case IPU_PIX_FMT_RGB565:
	case IPU_PIX_FMT_YUYV:
	case IPU_PIX_FMT_UYVY:
		return 2;
		break;
	case IPU_PIX_FMT_BGR24:
	case IPU_PIX_FMT_RGB24:
		return 3;
		break;
	case IPU_PIX_FMT_GENERIC_32:	/*generic data */
	case IPU_PIX_FMT_BGR32:
	case IPU_PIX_FMT_BGRA32:
	case IPU_PIX_FMT_RGB32:
	case IPU_PIX_FMT_RGBA32:
	case IPU_PIX_FMT_ABGR32:
		return 4;
		break;
	default:
		return 1;
		break;
	}
	return 0;
}
EXPORT_SYMBOL(bytes_per_pixel);

ipu_color_space_t format_to_colorspace(uint32_t fmt)
{
	switch (fmt) {
	case IPU_PIX_FMT_RGB666:
	case IPU_PIX_FMT_RGB565:
	case IPU_PIX_FMT_BGR24:
	case IPU_PIX_FMT_RGB24:
	case IPU_PIX_FMT_BGR32:
	case IPU_PIX_FMT_BGRA32:
	case IPU_PIX_FMT_RGB32:
	case IPU_PIX_FMT_RGBA32:
	case IPU_PIX_FMT_ABGR32:
		return RGB;
		break;

	default:
		return YCbCr;
		break;
	}
	return RGB;
}

static int ipu_suspend(struct platform_device *pdev, pm_message_t state)
{
	if (g_ipu_clk_enabled) {
		/* save and disable enabled channels*/
		idma_enable_reg[0] = __raw_readl(IDMAC_CHA_EN(0));
		idma_enable_reg[1] = __raw_readl(IDMAC_CHA_EN(32));
		while ((__raw_readl(IDMAC_CHA_BUSY(0)) & idma_enable_reg[0])
			|| (__raw_readl(IDMAC_CHA_BUSY(32)) & idma_enable_reg[1])) {
			/* disable channel not busy already */
			uint32_t chan_should_disable, timeout = 1000, time = 0;

			chan_should_disable =
				__raw_readl(IDMAC_CHA_BUSY(0)) ^ idma_enable_reg[0];
			__raw_writel((~chan_should_disable) & idma_enable_reg[0],
					IDMAC_CHA_EN(0));
			chan_should_disable =
				__raw_readl(IDMAC_CHA_BUSY(1)) ^ idma_enable_reg[1];
			__raw_writel((~chan_should_disable) & idma_enable_reg[1],
					IDMAC_CHA_EN(32));
			msleep(2);
			time += 2;
			if (time >= timeout)
				return -1;
		}
		__raw_writel(0, IDMAC_CHA_EN(0));
		__raw_writel(0, IDMAC_CHA_EN(32));

		/* save double buffer select regs */
		ipu_cha_db_mode_reg[0] = __raw_readl(IPU_CHA_DB_MODE_SEL(0));
		ipu_cha_db_mode_reg[1] = __raw_readl(IPU_CHA_DB_MODE_SEL(32));
		ipu_cha_db_mode_reg[2] = __raw_readl(IPU_ALT_CHA_DB_MODE_SEL(0));
		ipu_cha_db_mode_reg[3] = __raw_readl(IPU_ALT_CHA_DB_MODE_SEL(32));

		/* save current buffer regs */
		ipu_cha_cur_buf_reg[0] = __raw_readl(IPU_CHA_CUR_BUF(0));
		ipu_cha_cur_buf_reg[1] = __raw_readl(IPU_CHA_CUR_BUF(32));
		ipu_cha_cur_buf_reg[2] = __raw_readl(IPU_ALT_CUR_BUF0);
		ipu_cha_cur_buf_reg[3] = __raw_readl(IPU_ALT_CUR_BUF1);

		/* save sub-modules status and disable all */
		ic_conf_reg = __raw_readl(IC_CONF);
		__raw_writel(0, IC_CONF);
		ipu_conf_reg = __raw_readl(IPU_CONF);
		__raw_writel(0, IPU_CONF);

		/* save buf ready regs */
		buf_ready_reg[0] = __raw_readl(IPU_CHA_BUF0_RDY(0));
		buf_ready_reg[1] = __raw_readl(IPU_CHA_BUF0_RDY(32));
		buf_ready_reg[2] = __raw_readl(IPU_CHA_BUF1_RDY(0));
		buf_ready_reg[3] = __raw_readl(IPU_CHA_BUF1_RDY(32));
		buf_ready_reg[4] = __raw_readl(IPU_ALT_CHA_BUF0_RDY(0));
		buf_ready_reg[5] = __raw_readl(IPU_ALT_CHA_BUF0_RDY(32));
		buf_ready_reg[6] = __raw_readl(IPU_ALT_CHA_BUF1_RDY(0));
		buf_ready_reg[7] = __raw_readl(IPU_ALT_CHA_BUF1_RDY(32));

		mxc_pg_enable(pdev);
	}

	return 0;
}

static int ipu_resume(struct platform_device *pdev)
{
	if (g_ipu_clk_enabled) {
		mxc_pg_disable(pdev);

		/* restore buf ready regs */
		__raw_writel(buf_ready_reg[0], IPU_CHA_BUF0_RDY(0));
		__raw_writel(buf_ready_reg[1], IPU_CHA_BUF0_RDY(32));
		__raw_writel(buf_ready_reg[2], IPU_CHA_BUF1_RDY(0));
		__raw_writel(buf_ready_reg[3], IPU_CHA_BUF1_RDY(32));
		__raw_writel(buf_ready_reg[4], IPU_ALT_CHA_BUF0_RDY(0));
		__raw_writel(buf_ready_reg[5], IPU_ALT_CHA_BUF0_RDY(32));
		__raw_writel(buf_ready_reg[6], IPU_ALT_CHA_BUF1_RDY(0));
		__raw_writel(buf_ready_reg[7], IPU_ALT_CHA_BUF1_RDY(32));

		/* re-enable sub-modules*/
		__raw_writel(ipu_conf_reg, IPU_CONF);
		__raw_writel(ic_conf_reg, IC_CONF);

		/* restore double buffer select regs */
		__raw_writel(ipu_cha_db_mode_reg[0], IPU_CHA_DB_MODE_SEL(0));
		__raw_writel(ipu_cha_db_mode_reg[1], IPU_CHA_DB_MODE_SEL(32));
		__raw_writel(ipu_cha_db_mode_reg[2], IPU_ALT_CHA_DB_MODE_SEL(0));
		__raw_writel(ipu_cha_db_mode_reg[3], IPU_ALT_CHA_DB_MODE_SEL(32));

		/* restore current buffer select regs */
		__raw_writel(~(ipu_cha_cur_buf_reg[0]), IPU_CHA_CUR_BUF(0));
		__raw_writel(~(ipu_cha_cur_buf_reg[1]), IPU_CHA_CUR_BUF(32));
		__raw_writel(~(ipu_cha_cur_buf_reg[2]), IPU_ALT_CUR_BUF0);
		__raw_writel(~(ipu_cha_cur_buf_reg[3]), IPU_ALT_CUR_BUF1);

		/* restart idma channel*/
		__raw_writel(idma_enable_reg[0], IDMAC_CHA_EN(0));
		__raw_writel(idma_enable_reg[1], IDMAC_CHA_EN(32));

	} else {
		clk_enable(g_ipu_clk);
		_ipu_dmfc_init();
		_ipu_init_dc_mappings();

		/* Set sync refresh channels as high priority */
		__raw_writel(0x18800000L, IDMAC_CHA_PRI(0));
		clk_disable(g_ipu_clk);
	}

	return 0;
}

/*!
 * This structure contains pointers to the power management callback functions.
 */
static struct platform_driver mxcipu_driver = {
	.driver = {
		   .name = "mxc_ipu",
		   },
	.probe = ipu_probe,
	.remove = ipu_remove,
	.suspend_late = ipu_suspend,
	.resume_early = ipu_resume,
};

int32_t __init ipu_gen_init(void)
{
	int32_t ret;

	ret = platform_driver_register(&mxcipu_driver);
	return 0;
}

subsys_initcall(ipu_gen_init);

static void __exit ipu_gen_uninit(void)
{
	platform_driver_unregister(&mxcipu_driver);
}

module_exit(ipu_gen_uninit);
