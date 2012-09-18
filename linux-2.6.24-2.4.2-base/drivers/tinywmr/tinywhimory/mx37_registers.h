//#################################################				
//#                                                                          				
//#                    Marley Offset instances memory map                   				
//#                     Freescale Confidential Proprietary                    				
//# Purpose:                                                                 				
//#   This file is used by the ARM GAS, ARM GCC				
//#   and verilog Simulator to decode the memory map      				
//# Description:				
//#   This file lists instances offset memory location       				
//#                                                                          				
//# Initial version writen By                                                				
//#                              				
//#                                                                          				
//# Date                                                                     				
//# 16 Feb 2006                                                              				
//#                                                                          				
//# Revisions                                                                				
//#                                                                          				
//#################################################				
				
//include				
				
				
//#########################################				
//# BOOT ROM                              				
//#########################################				
#define BOOTROM1_ADDR_BOT_OFFSET	0x00000000	//  boot rom section 1 bottom address		
//#########################################				
//# WDOG                                  				
//#########################################				
#define WDOG_WCR_OFFSET	0x00	//  16bit watchdog control reg		
#define WDOG_WSR_OFFSET	0x02	//  16bit watchdog service reg		
#define WDOG_WRSR_OFFSET	0x04	//  16bit watchdog reset status reg		
#define WDOG_WICR_OFFSET	0x06	// Watchdog Interrupt Control Register		
//#########################################				
//# GPT				
//#########################################				
#define GPT_GPTCR_OFFSET	0x00	//  32bit timer 1 control reg		
#define GPT_GPTPR_OFFSET	0x04	//  32bit timer 1 prescaler reg		
#define GPT_GPTSR_OFFSET	0x08	//  32bit timer 1 compare reg		
#define GPT_GPTIR_OFFSET	0x0C	//  32bit timer 1 capture reg		
#define GPT_GPTOCR1_OFFSET	0x10	//  32bit timer 1 counter reg		
#define GPT_GPTOCR2_OFFSET	0x14	//  32bit timer 1 status reg		
#define GPT_GPTOCR3_OFFSET	0x18	// 32 bit read & write		
#define GPT_GPTICR1_OFFSET	0x1C	// 32 bit read		
#define GPT_GPTICR2_OFFSET	0x20	// 32 bit read		
#define GPT_GPTCNT_OFFSET	0x24	// 32 bit read		
//#########################################				
//# EPIT 				
//#########################################				
#define EPIT_EPITCR_OFFSET	0x00	//  32bit timer 3 control reg		
#define EPIT_EPITSR_OFFSET	0x04	//  32bit timer 3 prescaler reg		
#define EPIT_EPITLR_OFFSET	0x08	//  32bit timer 3 compare reg		
#define EPIT_EPITCMPR_OFFSET	0x0C	//  32bit timer 3 capture reg		
#define EPIT_EPITCNR_OFFSET	0x10	//  32bit timer 3 counter reg		
//#########################################				
//# PWM                                   				
//#########################################				
#define PWM_PWMCR_OFFSET	0x00	//  32bit pwm control reg		
#define PWM_PWMSR_OFFSET	0x04	//  32bit pwm sample reg		
#define PWM_PWMIR_OFFSET	0x08	//  32bit pwm period reg		
#define PWM_PWMSAR_OFFSET	0x0C	//  32bit pwm counter reg		
#define PWM_PWMPR_OFFSET	0x10	//  32bit pwm test reg		
#define PWM_PWMCNR_OFFSET	0x14			
//#########################################				
//# KPP                                   				
//#########################################				
#define KPP_KPCR_OFFSET	0x00	//  16bit kpp keypad control reg		
#define KPP_KPSR_OFFSET	0x02	//  16bit kpp keypad status reg		
#define KPP_KDDR_OFFSET	0x04	//  16bit kpp keypad data directon reg		
#define KPP_KPDR_OFFSET	0x06	//  16bit kpp keypad data reg		
//#########################################				
//# I2C 				
//#########################################				
#define I2C_IADR_OFFSET	0x00	//  16bit i2c address reg		
#define I2C_IFDR_OFFSET	0x04	//  16bit i2c frequency divider reg		
#define I2C_I2CR_OFFSET	0x08	//  16bit i2c control reg		
#define I2C_I2SR_OFFSET	0x0C	//  16bit i2c status reg		
#define I2C_I2DR_OFFSET	0x10	//  16bit i2c data i/o reg		
//#########################################				
//# GPIO  				
//#########################################				
#define GPIO_DR0_OFFSET	0x00	//  32bit gpio pta data direction reg		
#define GPIO_GDIR0_OFFSET	0x04	//  32bit gpio pta output config 1 reg		
#define GPIO_PSR0_OFFSET	0x08	//  32bit gpio pta output config 2 reg		
#define GPIO_ICR1_OFFSET	0x0C	//  32bit gpio pta input config A1 reg		
#define GPIO_ICR2_OFFSET	0x10	//  32bit gpio pta input config A2 reg		
#define GPIO_IMR_OFFSET	0x14	//  32bit gpio pta input config B1 reg		
#define GPIO_ISR_OFFSET	0x18	// GPIO Interrupt Status Register		
#define GPIO_EDGE_SEL_OFFSET	0x1C	// GPIO Edge Detect Register		
//#########################################				
//# CSPI   				
//#########################################				
#define CSPI_RXDATA_OFFSET	0x00	//  32bit CSPI receive data reg		
#define CSPI_TXDATA_OFFSET	0x04	//  32bit CSPI transmit data reg		
#define CSPI_CONREG_OFFSET	0x08	//  32bit CSPI control reg		
#define CSPI_CONFIG_OFFSET	0x0C			
#define CSPI_INTREG_OFFSET	0x10	//  32bit CSPI interrupt stat/ctr reg		
#define CSPI_DMAREG_OFFSET	0x14	//  32bit CSPI test reg		
#define CSPI_STATREG_OFFSET	0x18	//  32bit CSPI sample period ctrl reg		
#define CSPI_PERIODREG_OFFSET	0x1C	//  32bit CSPI dma ctrl reg		
#define CSPI_TESTREG_OFFSET	0x20	//  32bit CSPI soft reset reg		
#define CSPI_MSG0REG_OFFSET	0x24			
#define CSPI_MSG1REG_OFFSET	0x28			
#define CSPI_MSG2REG_OFFSET	0x2c			
#define CSPI_MSG3REG_OFFSET	0x30			
#define CSPI_MSG4REG_OFFSET	0x34			
#define CSPI_MSG5REG_OFFSET	0x38			
#define CSPI_MSG6REG_OFFSET	0x3c			
#define CSPI_MSG7REG_OFFSET	0x40			
#define CSPI_MSG8REG_OFFSET	0x44			
#define CSPI_MSG9REG_OFFSET	0x48			
#define CSPI_MSG10REG_OFFSET	0x4c			
#define CSPI_MSG11REG_OFFSET	0x50			
#define CSPI_MSG12REG_OFFSET	0x54			
#define CSPI_MSG13REG_OFFSET	0x58			
#define CSPI_MSG14REG_OFFSET	0x5c			
#define CSPI_MSG15REG_OFFSET	0x60			
//#########################################				
//# OWIRE                               				
//#########################################				
				
#define OWIRE_CTRL_OFFSET	0x00	//  16bit owire control reg		
#define OWIRE_TIME_DIV_OFFSET	0x02	//  16bit owire time divider reg		
#define OWIRE_RESET_OFFSET	0x04	//  16bit owire reset reg		
#define OWIRE_COMMAND_OFFSET	0x06			
#define OWIRE_TX_RX_OFFSET	0x08			
#define OWIRE_INTERRUPT_OFFSET	0x0A			
#define OWIRE_INTERRUPT_EN_OFFSET	0x0C			
//#########################################				
//# eSDHC  				
//#########################################				
#define ESDHC_DSADDR_OFFSET	0x00	//  32bit SDHC control reg 		
#define ESDHC_BLKATTR_OFFSET	0x04	//  32bit SDHC status reg		
#define ESDHC_CMDARG_OFFSET	0x08	//  32bit SDHC clock rate reg		
#define ESDHC_XFERTYP_OFFSET	0x0C	//  32bit SDHC cmd/data control reg		
#define ESDHC_CMDRSP0_OFFSET	0x10	//  32bit SDHC response time out reg		
#define ESDHC_CMDRSP1_OFFSET	0x14	//  32bit SDHC read time out reg		
#define ESDHC_CMDRSP2_OFFSET	0x18	//  32bit SDHC block length reg		
#define ESDHC_CMDRSP3_OFFSET	0x1C	//  32bit SDHC number of blocks reg		
#define ESDHC_DATPORT_OFFSET	0x20	//  32bit SDHC revision number reg		
#define ESDHC_PRSSTATE_OFFSET	0x24	//  32bit SDHC interrupt mask reg		
#define ESDHC_PROCTL_OFFSET	0x28	//  32bit SDHC command code reg		
#define ESDHC_SYSCTRL_OFFSET	0x2C	//  32bit SDHC argument (high+low) reg		
#define ESDHC_IRQSTAT_OFFSET	0x30	//  32bit SDHC response fifo reg		
#define ESDHC_IRQSTATEN_OFFSET	0x34	//  32bit SDHC buffer access reg		
#define ESDHC_IRQSIGEN_OFFSET	0x38	//  32bit SDHC remaining NUM reg		
#define ESDHC_AUTOC12ERR_OFFSET	0x3C	//  32bit SDHC remaining block bytes  reg		
#define ESDHC_HOSTCAPBLT_OFFSET	0x40			
#define ESDHC_WML_OFFSET	0x44			
#define ESDHC_FEVTR_OFFSET	0x50			
#define ESDHC_HOSTVER_OFFSET	0xfc			
//#########################################				
//# SIM                                  				
//#########################################				
#define SIM_PORT1_CNTL_OFFSET	0x00			
#define SIM_SETUP_OFFSET	0x04			
#define SIM_PORT1_DETECT_OFFSET	0x08			
#define SIM_PORT1_XMT_BUF_OFFSET	0x0C			
#define SIM_PORT1_RCV_BUF_OFFSET	0x10			
#define SIM_PORT0_CNTL_OFFSET	0x14			
#define SIM_CNTL_OFFSET	0x18			
#define SIM_CLOCK_SELECT_OFFSET	0x1C			
#define SIM_RCV_THRESHOLD_OFFSET	0x20			
#define SIM_ENABLE_OFFSET	0x24			
#define SIM_XMT_STATUS_OFFSET	0x28			
#define SIM_RCV_STATUS_OFFSET	0x2C			
#define SIM_INT_MASK_OFFSET	0x30			
#define SIM_PORT0_XMT_BUF_OFFSET	0x34			
#define SIM_PORT0_RCV_BUF_OFFSET	0x38			
#define SIM_PORT0_DETECT_OFFSET	0x3C			
#define SIM_DATA_FORMAT_OFFSET	0x40			
#define SIM_XMT_THRESHOLD_OFFSET	0x44			
#define SIM_GUARD_CNTL_OFFSET	0x48			
#define SIM_OD_CONFIG_OFFSET	0x4C			
#define SIM_RESET_CNTL_OFFSET	0x50			
#define SIM_CHAR_WAIT_OFFSET	0x54			
#define SIM_GPCNT_OFFSET	0x58			
#define SIM_DIVISOR_OFFSET	0x5C			
#define SIM_BWT_OFFSET	0x60			
#define SIM_BGT_OFFSET	0x64			
#define SIM_BWT_H_OFFSET	0x68			
#define SIM_XMT_FIFO_STAT_OFFSET	0x6C			
#define SIM_RCV_FIFO_CNT_OFFSET	0x70			
#define SIM_RCV_FIFO_WPTR_OFFSET	0x74			
#define SIM_RCV_FIFO_RPTR_OFFSET	0x78			
//#########################################				
//# SSI 				
//#########################################				
#define SSI_STX0_OFFSET	0x00	//  32bit SSI tx reg 0		
#define SSI_STX1_OFFSET	0x04	//  32bit SSI tx reg 1		
#define SSI_SRX0_OFFSET	0x08	//  32bit SSI rx reg 0		
#define SSI_SRX1_OFFSET	0x0C	//  32bit SSI rx reg 1		
#define SSI_SCR_OFFSET	0x10	//  32bit SSI control reg		
#define SSI_SISR_OFFSET	0x14	//  32bit SSI intr status reg		
#define SSI_SIER_OFFSET	0x18	//  32bit SSI intr enable reg		
#define SSI_STCR_OFFSET	0x1C	//  32bit SSI tx config reg		
#define SSI_SRCR_OFFSET	0x20	//  32bit SSI rx config reg		
#define SSI_STCCR_OFFSET	0x24	//  32bit SSI tx clock control reg		
#define SSI_SRCCR_OFFSET	0x28	//  32bit SSI rx clock control reg		
#define SSI_SFCSR_OFFSET	0x2C	//  32bit SSI fifo control/status reg		
#define SSI_STR_OFFSET	0x30	//  32bit SSI test reg		
#define SSI_SOR_OFFSET	0x34	//  32bit SSI option reg		
#define SSI_SACNT_OFFSET	0x38	//  32bit SSI ac97 control reg		
#define SSI_SACADD_OFFSET	0x3C	//  32bit SSI ac97 cmd addr reg		
#define SSI_SACDAT_OFFSET	0x40	//  32bit SSI ac97 cmd data reg		
#define SSI_SATAG_OFFSET	0x44	//  32bit SSI ac97 tag reg		
#define SSI_STMSK_OFFSET	0x48	//  32bit SSI tx time slot mask reg		
#define SSI_SRMSK_OFFSET	0x4C	//  32bit SSI rx time slot mask reg		
#define SSI_SACCST_OFFSET	0x50			
#define SSI_SACCEN_OFFSET	0x54			
#define SSI_SACCDIS_OFFSET	0x58			
//#########################################				
//# UART				
//#########################################				
#define UART_URXD_OFFSET	0x00	//  32bit UART receiver reg		
#define UART_UTXD_OFFSET	0x40	//  32bit UART transmitter reg		
#define UART_UCR1_OFFSET	0x80	//  32bit UART control 1 reg		
#define UART_UCR2_OFFSET	0x84	//  32bit UART control 2 reg		
#define UART_UCR3_OFFSET	0x88	//  32bit UART control 3 reg		
#define UART_UCR4_OFFSET	0x8C	//  32bit UART control 4 reg		
#define UART_UFCR_OFFSET	0x90	//  32bit UART fifo control reg		
#define UART_USR1_OFFSET	0x94	//  32bit UART status 1 reg		
#define UART_USR2_OFFSET	0x98	//  32bit UART status 2 reg		
#define UART_UESC_OFFSET	0x9C	//  32bit UART escape char reg		
#define UART_UTIM_OFFSET	0xA0	//  32bit UART escape timer reg		
#define UART_UBIR_OFFSET	0xA4	//  32bit UART BRM incremental reg		
#define UART_UBMR_OFFSET	0xA8	//  32bit UART BRM modulator reg		
#define UART_UBRC_OFFSET	0xAC	//  32bit UART baud rate count reg		
#define UART_ONEMS_OFFSET	0xB0	//  32bit UART one ms reg		
#define UART_UTS_OFFSET	0xB4	//  32bit UART test reg		
//#########################################				
//# AUDMUX                        				
//#########################################				
#define AUDMUX_PTCR1_OFFSET	0x00	// Port Timing Control Register 1		
#define AUDMUX_PDCR1_OFFSET	0x04	// Port Data Control Register 1		
#define AUDMUX_PTCR2_OFFSET	0x08	// Port Timing Control Register 2		
#define AUDMUX_PDCR2_OFFSET	0x0C	// Port Data Control Register 2		
#define AUDMUX_PTCR3_OFFSET	0x10	// Port Timing Control Register 3		
#define AUDMUX_PDCR3_OFFSET	0x14	// Port Data Control Register 3		
#define AUDMUX_PTCR4_OFFSET	0x18	// Port Timing Control Register 4		
#define AUDMUX_PDCR4_OFFSET	0x1C	// Port Data Control Register 4		
#define AUDMUX_PTCR5_OFFSET	0x20	// Port Timing Control Register 5		
#define AUDMUX_PDCR5_OFFSET	0x24	// Port Data Control Register 5		
#define AUDMUX_PTCR6_OFFSET	0x28	// Port Timing Control Register 6		
#define AUDMUX_PDCR6_OFFSET	0x2C	// Port Data Control Register 6		
#define AUDMUX_PTCR7_OFFSET	0x30	// Port Timing Control Register 7		
#define AUDMUX_PDCR7_OFFSET	0x34	// Port Data Control Register 7		
#define AUDMUX_CNMCR_OFFSET	0x38	// CE Bus Network Mode Control Register		
//#########################################				
//# SPBA                                				
//#########################################				
#define SPBA_PER0_START_ADDR_OFFSET	0x00	//  32bit gpio pta data direction reg		
#define SPBA_PER1_START_ADDR_OFFSET	0x4000	//  32bit gpio pta output config 1 reg		
#define SPBA_PER2_START_ADDR_OFFSET	0x8000	//  32bit gpio pta output config 2 reg		
#define SPBA_PER3_START_ADDR_OFFSET	0xC000			
#define SPBA_PER4_START_ADDR_OFFSET	0x10000			
#define SPBA_PER5_START_ADDR_OFFSET	0x14000			
#define SPBA_PER6_START_ADDR_OFFSET	0x18000			
#define SPBA_PER7_START_ADDR_OFFSET	0x1C000			
#define SPBA_PER8_START_ADDR_OFFSET	0x20000			
#define SPBA_PER9_START_ADDR_OFFSET	0x24000			
#define SPBA_PER10_START_ADDR_OFFSET	0x28000			
#define SPBA_PER11_START_ADDR_OFFSET	0x2C000			
#define SPBA_PER12_START_ADDR_OFFSET	0x30000			
#define SPBA_PER13_START_ADDR_OFFSET	0x34000			
#define SPBA_PER14_START_ADDR_OFFSET	0x38000			
#define SPBA_REG_ADDR_OFFSET	0x3C000			
#define SPBA_PRR0_OFFSET	0x00			
#define SPBA_PRR1_OFFSET	0x04			
#define SPBA_PRR2_OFFSET	0x08			
#define SPBA_PRR3_OFFSET	0x0C			
#define SPBA_PRR4_OFFSET	0x10			
#define SPBA_PRR5_OFFSET	0x14			
#define SPBA_PRR6_OFFSET	0x18			
#define SPBA_PRR7_OFFSET	0x1C			
#define SPBA_PRR8_OFFSET	0x20			
#define SPBA_PRR9_OFFSET	0x24			
#define SPBA_PRR10_OFFSET	0x28			
#define SPBA_PRR11_OFFSET	0x2C			
#define SPBA_PRR12_OFFSET	0x30			
#define SPBA_PRR13_OFFSET	0x34			
#define SPBA_PRR14_OFFSET	0x38			
//#########################################				
//# PATA                           				
//#########################################				
#define PATA_TIME_OFF_OFFSET	0x00			
#define PATA_TIME_ON_OFFSET	0x01			
#define PATA_TIME_1_OFFSET	0x02			
#define PATA_TIME_2W_OFFSET	0x03			
#define PATA_TIME_2R_OFFSET	0x04			
#define PATA_TIME_AX_OFFSET	0x05			
#define PATA_TIME_PIO_RDX_OFFSET	0x06			
#define PATA_TIME_4_OFFSET	0x07			
#define PATA_TIME_9_OFFSET	0x08			
#define PATA_TIME_M_OFFSET	0x09			
#define PATA_TIME_JN_OFFSET	0x0A			
#define PATA_TIME_D_OFFSET	0x0B			
#define PATA_TIME_K_OFFSET	0x0C			
#define PATA_TIME_ACK_OFFSET	0x0D			
#define PATA_TIME_ENV_OFFSET	0x0E			
#define PATA_TIME_RPX_OFFSET	0x0F			
#define PATA_TIME_ZAH_OFFSET	0x10			
#define PATA_TIME_MLIX_OFFSET	0x11			
#define PATA_TIME_DVH_OFFSET	0x12			
#define PATA_TIME_DZFS_OFFSET	0x13			
#define PATA_TIME_DVS_OFFSET	0x14			
#define PATA_TIME_CVH_OFFSET	0x15			
#define PATA_TIME_SS_OFFSET	0x16			
#define PATA_TIME_CYC_OFFSET	0x17			
#define PATA_FIFO_DATA_16_OFFSET	0x1c			
#define PATA_FIFO_DATA_32_OFFSET	0x18			
#define PATA_FIFO_FILL_OFFSET	0x20			
#define PATA_ATA_CONTROL_OFFSET	0x24			
#define PATA_INTERRUPT_PENDING_OFFSET	0x28			
#define PATA_INTERRUPT_ENABLE_OFFSET	0x2c			
#define PATA_INTERRUPT_CLEAR_OFFSET	0x30			
#define PATA_FIFO_ALARM_OFFSET	0x34			
#define PATA_DRIVE_DATA_OFFSET	0xA0			
#define PATA_DRIVE_FEATURES_OFFSET	0xA4			
#define PATA_DRIVE_SECTOR_COUNT_OFFSET	0xA8			
#define PATA_DRIVE_SECTOR_M_OFFSET	0xAC			
#define PATA_DRIVE_CYL_LOW_OFFSET	0xB0			
#define PATA_DRIVE_CYL_HIGH_OFFSET	0xB4			
#define PATA_DRIVE_DEV_HEAD_OFFSET	0xB8			
#define PATA_DRIVE_COMMAND_OFFSET	0xBC			
#define PATA_DRIVE_STATUS_OFFSET	0xC0			
#define PATA_DRIVE_ALT_STATUS_OFFSET	0xC4			
#define PATA_DRIVE_CONTROL_OFFSET	0xC8			
#define ATA_DCTR_OFFSET	0xD8	// req. by telma - not defined at the spec.		
#define ATA_DDTR_OFFSET	0xA0	// req. by telma - not defined at the spec.		
#define ATA_DFTR_OFFSET	0xA4	// req. by telma - not defined at the spec.		
#define ATA_DSCR_OFFSET	0xA8	// req. by telma - not defined at the spec.		
#define ATA_DSNR_OFFSET	0xAC	// req. by telma - not defined at the spec.		
#define ATA_DCLR_OFFSET	0xB0	// req. by telma - not defined at the spec.		
#define ATA_DCHR_OFFSET	0xB4	// req. by telma - not defined at the spec.		
#define ATA_DDHR_OFFSET	0xB8	// req. by telma - not defined at the spec.		
#define ATA_DCDR_OFFSET	0xBC	// req. by telma - not defined at the spec.		
//#########################################				
//# IIM   				
//#########################################				
#define IIM_STAT_OFFSET	0x0000			
#define IIM_STATM_OFFSET	0x0004			
#define IIM_ERR_OFFSET	0x0008			
#define IIM_EMASK_OFFSET	0x000C			
#define IIM_FCTL_OFFSET	0x0010			
#define IIM_UA_OFFSET	0x0014			
#define IIM_LA_OFFSET	0x0018			
#define IIM_SDAT_OFFSET	0x001C			
#define IIM_PREV_OFFSET	0x0020			
#define IIM_SREV_OFFSET	0x0024			
#define IIM_PRG_P_OFFSET	0x0028			
#define IIM_SCS0_OFFSET	0x002C			
#define IIM_SCS1_OFFSET	0x0030			
#define IIM_SCS2_OFFSET	0x0034			
#define IIM_SCS3_OFFSET	0x0038			
#define IIM_FBNSOP_OFFSET	0x003c			
#define IIM_FBNSRP_OFFSET	0X0040			
#define IIM_FBNSESP_OFFSET	0X0044			
#define IIM_TZWRLCK_OFFSET	0X0048			
#define IIM_FBAC0_OFFSET	0x0800			
#define IIM_HWV0_OFFSET	0x0804			
#define IIM_HWV1_OFFSET	0x0808			
#define IIM_HWV2_OFFSET	0x080C			
#define IIM_FB0_WORD4_OFFSET	0x0810			
#define IIM_FB0_WORD5_OFFSET	0x0814			
#define IIM_FB0_WORD6_OFFSET	0x0818			
#define IIM_FB0_WORD7_OFFSET	0x081c			
#define IIM_FB0_WORD8_OFFSET	0x0820			
#define IIM_FB0_WORD9_OFFSET	0x0824			
#define IIM_FB0_WORD10_OFFSET	0x0828			
#define IIM_FB0_WORD11_OFFSET	0x082c			
#define IIM_FB0_WORD12_OFFSET	0x0830			
#define IIM_FB0_WORD13_OFFSET	0x0834			
#define IIM_FB0_WORD14_OFFSET	0x0838			
#define IIM_FB0_WORD15_OFFSET	0x083c			
#define IIM_FB0_WORD16_OFFSET	0x0840			
#define IIM_FB0_WORD17_OFFSET	0x0844			
#define IIM_FB0_WORD18_OFFSET	0x0848			
#define IIM_FB0_WORD19_OFFSET	0x084c			
#define IIM_FB0_WORD20_OFFSET	0x0850			
#define IIM_FB0_WORD21_OFFSET	0x0854			
#define IIM_FB0_WORD22_OFFSET	0x0858			
#define IIM_FB0_WORD23_OFFSET	0x085c			
#define IIM_FB0_WORD24_OFFSET	0x0860			
#define IIM_FB0_WORD25_OFFSET	0x0864			
#define IIM_FB0_WORD26_OFFSET	0x0868			
#define IIM_FB0_WORD27_OFFSET	0x086c			
#define IIM_FB0_WORD28_OFFSET	0x0870			
#define IIM_FB0_WORD29_OFFSET	0x0874			
#define IIM_FB0_WORD30_OFFSET	0x0878			
#define IIM_FB0_WORD31_OFFSET	0x087c			
#define IIM_FBAC1_OFFSET	0x0c00			
#define IIM_FB1_WORD1_OFFSET	0x0c04			
#define IIM_FB1_WORD2_OFFSET	0x0c08			
#define IIM_FB1_WORD3_OFFSET	0x0c0c			
#define IIM_FB1_WORD4_OFFSET	0x0c10			
#define IIM_FB1_WORD5_OFFSET	0x0c14			
#define IIM_FB1_WORD6_OFFSET	0x0c18			
#define IIM_FB1_WORD7_OFFSET	0x0c1c			
#define IIM_FB1_WORD8_OFFSET	0x0c20			
#define IIM_FB1_WORD9_OFFSET	0x0c24			
#define IIM_FB1_WORD10_OFFSET	0x0c28			
#define IIM_FB1_WORD11_OFFSET	0x0c2c			
#define IIM_FB1_WORD12_OFFSET	0x0c30			
#define IIM_FB1_WORD13_OFFSET	0x0c34			
#define IIM_FB1_WORD14_OFFSET	0x0c38			
#define IIM_FB1_WORD15_OFFSET	0x0c3c			
#define IIM_FB1_WORD16_OFFSET	0x0c40			
#define IIM_FB1_WORD17_OFFSET	0x0c44			
#define IIM_FB1_WORD18_OFFSET	0x0c48			
#define IIM_FB1_WORD19_OFFSET	0x0c4c			
#define IIM_FB1_WORD20_OFFSET	0x0c50			
#define IIM_FB1_WORD21_OFFSET	0x0c54			
#define IIM_FB1_WORD22_OFFSET	0x0c58			
#define IIM_FB1_WORD23_OFFSET	0x0c5c			
#define IIM_FB1_WORD24_OFFSET	0x0c60			
#define IIM_FB1_WORD25_OFFSET	0x0c64			
#define IIM_FB1_WORD26_OFFSET	0x0c68			
#define IIM_FB1_WORD27_OFFSET	0x0c6c			
#define IIM_FB1_WORD28_OFFSET	0x0c70			
#define IIM_FB1_WORD29_OFFSET	0x0c74			
#define IIM_FB1_WORD30_OFFSET	0x0c78			
#define IIM_FB1_WORD31_OFFSET	0x0c7c			
#define IIM_FBAC2_OFFSET	0x1000			
#define IIM_FB2_WORD1_OFFSET	0x1004			
#define IIM_FB2_WORD2_OFFSET	0x1008			
#define IIM_FB2_WORD3_OFFSET	0x100c			
#define IIM_FB2_WORD4_OFFSET	0x1010			
#define IIM_FB2_WORD5_OFFSET	0x1014			
#define IIM_FB2_WORD6_OFFSET	0x1018			
#define IIM_FB2_WORD7_OFFSET	0x101c			
#define IIM_FB2_WORD8_OFFSET	0x1020			
#define IIM_FB2_WORD9_OFFSET	0x1024			
#define IIM_FB2_WORD10_OFFSET	0x1028			
#define IIM_FB2_WORD11_OFFSET	0x102c			
#define IIM_FB2_WORD12_OFFSET	0x1030			
#define IIM_FB2_WORD13_OFFSET	0x1034			
#define IIM_FB2_WORD14_OFFSET	0x1038			
#define IIM_FB2_WORD15_OFFSET	0x103c			
#define IIM_FB2_WORD16_OFFSET	0x1040			
#define IIM_FB2_WORD17_OFFSET	0x1044			
#define IIM_FB2_WORD18_OFFSET	0x1048			
#define IIM_FB2_WORD19_OFFSET	0x104c			
#define IIM_FB2_WORD20_OFFSET	0x1050			
#define IIM_FB2_WORD21_OFFSET	0x1054			
#define IIM_FB2_WORD22_OFFSET	0x1058			
#define IIM_FB2_WORD23_OFFSET	0x105c			
#define IIM_FB2_WORD24_OFFSET	0x1060			
#define IIM_FB2_WORD25_OFFSET	0x1064			
#define IIM_FB2_WORD26_OFFSET	0x1068			
#define IIM_FB2_WORD27_OFFSET	0x106c			
#define IIM_FB2_WORD28_OFFSET	0x1070			
#define IIM_FB2_WORD29_OFFSET	0x1074			
#define IIM_FB2_WORD30_OFFSET	0x1078			
#define IIM_FB2_WORD31_OFFSET	0x107c			
#define IIM_FBAC3_OFFSET	0x1400			
#define IIM_FB3_WORD1_OFFSET	0x1404			
#define IIM_FB3_WORD2_OFFSET	0x1408			
#define IIM_FB3_WORD3_OFFSET	0x140c			
#define IIM_FB3_WORD4_OFFSET	0x1410			
#define IIM_FB3_WORD5_OFFSET	0x1414			
#define IIM_FB3_WORD6_OFFSET	0x1418			
#define IIM_FB3_WORD7_OFFSET	0x141c			
#define IIM_FB3_WORD8_OFFSET	0x1420			
#define IIM_FB3_WORD9_OFFSET	0x1424			
#define IIM_FB3_WORD10_OFFSET	0x1428			
#define IIM_FB3_WORD11_OFFSET	0x142c			
#define IIM_FB3_WORD12_OFFSET	0x1430			
#define IIM_FB3_WORD13_OFFSET	0x1434			
#define IIM_FB3_WORD14_OFFSET	0x1438			
#define IIM_FB3_WORD15_OFFSET	0x143c			
#define IIM_FB3_WORD16_OFFSET	0x1440			
#define IIM_FB3_WORD17_OFFSET	0x1444			
#define IIM_FB3_WORD18_OFFSET	0x1448			
#define IIM_FB3_WORD19_OFFSET	0x144c			
#define IIM_FB3_WORD20_OFFSET	0x1450			
#define IIM_FB3_WORD21_OFFSET	0x1454			
#define IIM_FB3_WORD22_OFFSET	0x1458			
#define IIM_FB3_WORD23_OFFSET	0x145c			
#define IIM_FB3_WORD24_OFFSET	0x1460			
#define IIM_FB3_WORD25_OFFSET	0x1464			
#define IIM_FB3_WORD26_OFFSET	0x1468			
#define IIM_FB3_WORD27_OFFSET	0x146c			
#define IIM_FB3_WORD28_OFFSET	0x1470			
#define IIM_FB3_WORD29_OFFSET	0x1474			
#define IIM_FB3_WORD30_OFFSET	0x1478			
#define IIM_FB3_WORD31_OFFSET	0x147c			
//#########################################				
//# MSHC 				
//#########################################				
#define MSHC_COMMAND_REG_OFFSET	0x00			
#define MSHC_DATA_REG_OFFSET	0x08			
#define MSHC_STATUS_REG_OFFSET	0x10			
#define MSHC_SYSTEM_REG_OFFSET	0x18			
#define MSHC_DMA_START_ADD_OFFSET	0x20			
//#########################################				
//# DPLLIP                  				
//#########################################				
#define DPLLIP_DP_CTL_OFFSET	0x000			
#define DPLLIP_DP_CONFIG_OFFSET	0x004			
#define DPLLIP_DP_OP_OFFSET	0x008			
#define DPLLIP_DP_MFD_OFFSET	0x00C			
#define DPLLIP_DP_MFN_OFFSET	0x010			
#define DPLLIP_DP_MFNMINUS_OFFSET	0x014			
#define DPLLIP_DP_MFNPLUS_OFFSET	0x018			
#define DPLLIP_DP_HFS_OP_OFFSET	0x01C			
#define DPLLIP_DP_HFS_MFD_OFFSET	0x020			
#define DPLLIP_DP_HFS_MFN_OFFSET	0x024			
#define DPLLIP_DP_MFN_TOGC_OFFSET	0x028			
#define DPLLIP_DP_DESTAT_OFFSET	0x02C			
//#########################################				
//# IPU                 				
//#########################################				
#define IPU_REG_OFFSET	0x1E000000			
#define IPU_IPU_CONF_OFFSET	IPU_REG_OFFSET + 0x00000000			
#define IPU_SISG_CTRL0_OFFSET	IPU_REG_OFFSET + 0x00000004			
#define IPU_SISG_CTRL1_OFFSET	IPU_REG_OFFSET + 0x00000008			
#define IPU_SISG_SET_1_OFFSET	IPU_REG_OFFSET + 0x0000000C			
#define IPU_SISG_SET_2_OFFSET	IPU_REG_OFFSET + 0x00000010			
#define IPU_SISG_SET_3_OFFSET	IPU_REG_OFFSET + 0x00000014			
#define IPU_SISG_SET_4_OFFSET	IPU_REG_OFFSET + 0x00000018			
#define IPU_SISG_SET_5_OFFSET	IPU_REG_OFFSET + 0x0000001C			
#define IPU_SISG_SET_6_OFFSET	IPU_REG_OFFSET + 0x00000020			
#define IPU_SISG_CLR_1_OFFSET	IPU_REG_OFFSET + 0x00000024			
#define IPU_SISG_CLR_2_OFFSET	IPU_REG_OFFSET + 0x00000028			
#define IPU_SISG_CLR_3_OFFSET	IPU_REG_OFFSET + 0x0000002C			
#define IPU_SISG_CLR_4_OFFSET	IPU_REG_OFFSET + 0x00000030			
#define IPU_SISG_CLR_5_OFFSET	IPU_REG_OFFSET + 0x00000034			
#define IPU_SISG_CLR_6_OFFSET	IPU_REG_OFFSET + 0x00000038			
#define IPU_IPU_INT_CTRL_1_OFFSET	IPU_REG_OFFSET + 0x0000003C			
#define IPU_IPU_INT_CTRL_2_OFFSET	IPU_REG_OFFSET + 0x00000040			
#define IPU_IPU_INT_CTRL_3_OFFSET	IPU_REG_OFFSET + 0x00000044			
#define IPU_IPU_INT_CTRL_4_OFFSET	IPU_REG_OFFSET + 0x00000048			
#define IPU_IPU_INT_CTRL_5_OFFSET	IPU_REG_OFFSET + 0x0000004C			
#define IPU_IPU_INT_CTRL_6_OFFSET	IPU_REG_OFFSET + 0x00000050			
#define IPU_IPU_INT_CTRL_7_OFFSET	IPU_REG_OFFSET + 0x00000054			
#define IPU_IPU_INT_CTRL_8_OFFSET	IPU_REG_OFFSET + 0x00000058			
#define IPU_IPU_INT_CTRL_9_OFFSET	IPU_REG_OFFSET + 0x0000005C			
#define IPU_IPU_INT_CTRL_10_OFFSET	IPU_REG_OFFSET + 0x00000060			
#define IPU_IPU_INT_CTRL_11_OFFSET	IPU_REG_OFFSET + 0x00000064			
#define IPU_IPU_INT_CTRL_12_OFFSET	IPU_REG_OFFSET + 0x00000068			
#define IPU_IPU_INT_CTRL_13_OFFSET	IPU_REG_OFFSET + 0x0000006C			
#define IPU_IPU_INT_CTRL_14_OFFSET	IPU_REG_OFFSET + 0x00000070			
#define IPU_IPU_INT_CTRL_15_OFFSET	IPU_REG_OFFSET + 0x00000074			
#define IPU_IPU_SDMA_EVENT_1_OFFSET	IPU_REG_OFFSET + 0x00000078			
#define IPU_IPU_SDMA_EVENT_2_OFFSET	IPU_REG_OFFSET + 0x0000007C			
#define IPU_IPU_SDMA_EVENT_3_OFFSET	IPU_REG_OFFSET + 0x00000080			
#define IPU_IPU_SDMA_EVENT_4_OFFSET	IPU_REG_OFFSET + 0x00000084			
#define IPU_IPU_SDMA_EVENT_5_OFFSET	IPU_REG_OFFSET + 0x00000088			
#define IPU_IPU_SDMA_EVENT_6_OFFSET	IPU_REG_OFFSET + 0x0000008C			
#define IPU_IPU_SDMA_EVENT_7_OFFSET	IPU_REG_OFFSET + 0x00000090			
#define IPU_IPU_SDMA_EVENT_8_OFFSET	IPU_REG_OFFSET + 0x00000094			
#define IPU_IPU_SDMA_EVENT_9_OFFSET	IPU_REG_OFFSET + 0x00000098			
#define IPU_IPU_SDMA_EVENT_10_OFFSET	IPU_REG_OFFSET + 0x0000009C			
#define IPU_IPU_SRM_PRI1_OFFSET	IPU_REG_OFFSET + 0x000000A0			
#define IPU_IPU_SRM_PRI2_OFFSET	IPU_REG_OFFSET + 0x000000A4			
#define IPU_IPU_FS_PROC_FLOW1_OFFSET	IPU_REG_OFFSET + 0x000000A8			
#define IPU_IPU_FS_PROC_FLOW2_OFFSET	IPU_REG_OFFSET + 0x000000AC			
#define IPU_IPU_FS_PROC_FLOW3_OFFSET	IPU_REG_OFFSET + 0x000000B0			
#define IPU_IPU_FS_DISP_FLOW1_OFFSET	IPU_REG_OFFSET + 0x000000B4			
#define IPU_IPU_FS_DISP_FLOW2_OFFSET	IPU_REG_OFFSET + 0x000000B8			
#define IPU_IPU_SKIP_OFFSET	IPU_REG_OFFSET + 0x000000BC			
#define IPU_IPU_DISP_ALT_CONF_OFFSET	IPU_REG_OFFSET + 0x000000C0			
#define IPU_DP_CUR_POS_ALT_OFFSET	IPU_REG_OFFSET + 0x000000C4			
#define IPU_IPU_DISP_GEN_OFFSET	IPU_REG_OFFSET + 0x000000C8			
#define IPU_IPU_DISP_ALT1_OFFSET	IPU_REG_OFFSET + 0x000000CC			
#define IPU_IPU_DISP_ALT2_OFFSET	IPU_REG_OFFSET + 0x000000D0			
#define IPU_IPU_DISP_ALT3_OFFSET	IPU_REG_OFFSET + 0x000000D4			
#define IPU_IPU_DISP_ALT4_OFFSET	IPU_REG_OFFSET + 0x000000D8			
#define IPU_IPU_SNOOP_OFFSET	IPU_REG_OFFSET + 0x000000DC			
#define IPU_IPU_MEM_RST_OFFSET	IPU_REG_OFFSET + 0x000000E0			
#define IPU_IPU_PM_OFFSET	IPU_REG_OFFSET + 0x000000E4			
#define IPU_IPU_INT_STAT_1_OFFSET	IPU_REG_OFFSET + 0x000000E8			
#define IPU_IPU_INT_STAT_2_OFFSET	IPU_REG_OFFSET + 0x000000EC			
#define IPU_IPU_INT_STAT_3_OFFSET	IPU_REG_OFFSET + 0x000000F0			
#define IPU_IPU_INT_STAT_4_OFFSET	IPU_REG_OFFSET + 0x000000F4			
#define IPU_IPU_INT_STAT_5_OFFSET	IPU_REG_OFFSET + 0x000000F8			
#define IPU_IPU_INT_STAT_6_OFFSET	IPU_REG_OFFSET + 0x000000FC			
#define IPU_IPU_INT_STAT_7_OFFSET	IPU_REG_OFFSET + 0x00000100			
#define IPU_IPU_INT_STAT_8_OFFSET	IPU_REG_OFFSET + 0x00000104			
#define IPU_IPU_INT_STAT_9_OFFSET	IPU_REG_OFFSET + 0x00000108			
#define IPU_IPU_INT_STAT_10_OFFSET	IPU_REG_OFFSET + 0x0000010C			
#define IPU_IPU_INT_STAT_11_OFFSET	IPU_REG_OFFSET + 0x00000110			
#define IPU_IPU_INT_STAT_12_OFFSET	IPU_REG_OFFSET + 0x00000114			
#define IPU_IPU_INT_STAT_13_OFFSET	IPU_REG_OFFSET + 0x00000118			
#define IPU_IPU_INT_STAT_14_OFFSET	IPU_REG_OFFSET + 0x0000011C			
#define IPU_IPU_INT_STAT_15_OFFSET	IPU_REG_OFFSET + 0x00000120			
#define IPU_IPU_CUR_BUF_0_OFFSET	IPU_REG_OFFSET + 0x00000124			
#define IPU_IPU_CUR_BUF_1_OFFSET	IPU_REG_OFFSET + 0x00000128			
#define IPU_IPU_ALT_CUR_BUF_0_OFFSET	IPU_REG_OFFSET + 0x0000012C			
#define IPU_IPU_ALT_CUR_BUF_1_OFFSET	IPU_REG_OFFSET + 0x00000130			
#define IPU_IPU_SRM_STAT_OFFSET	IPU_REG_OFFSET + 0x00000134			
#define IPU_IPU_PROC_TASKS_STAT_OFFSET	IPU_REG_OFFSET + 0x00000138			
#define IPU_IPU_DISP_TASKS_STAT_OFFSET	IPU_REG_OFFSET + 0x0000013C			
#define IPU_IPU_CH_BUF0_RDY0_OFFSET	IPU_REG_OFFSET + 0x00000140			
#define IPU_IPU_CH_BUF0_RDY1_OFFSET	IPU_REG_OFFSET + 0x00000144			
#define IPU_IPU_CH_BUF1_RDY0_OFFSET	IPU_REG_OFFSET + 0x00000148			
#define IPU_IPU_CH_BUF1_RDY1_OFFSET	IPU_REG_OFFSET + 0x0000014C			
#define IPU_IPU_CH_DB_MODE_SEL_0_OFFSET	IPU_REG_OFFSET + 0x00000150			
#define IPU_IPU_CH_DB_MODE_SEL_1_OFFSET	IPU_REG_OFFSET + 0x00000154			
#define IPU_IPU_ALT_CH_BUF0_RDY0_OFFSET	IPU_REG_OFFSET + 0x00000158			
#define IPU_IPU_ALT_CH_BUF0_RDY1_OFFSET	IPU_REG_OFFSET + 0x0000015C			
#define IPU_IPU_ALT_CH_BUF1_RDY0_OFFSET	IPU_REG_OFFSET + 0x00000160			
#define IPU_IPU_ALT_CH_BUF1_RDY1_OFFSET	IPU_REG_OFFSET + 0x00000164			
#define IPU_IPU_ALT_CH_DB_MODE_SEL_0_OFFSET	IPU_REG_OFFSET + 0x00000168			
#define IPU_IPU_ALT_CH_DB_MODE_SEL_1_OFFSET	IPU_REG_OFFSET + 0x0000016C			
#define IPU_IDMAC_CONF_OFFSET	IPU_REG_OFFSET + 0x00008000			
#define IPU_IDMAC_CH_EN_1_OFFSET	IPU_REG_OFFSET + 0x00008004			
#define IPU_IDMAC_CH_EN_2_OFFSET	IPU_REG_OFFSET + 0x00008008			
#define IPU_IDMAC_SEP_ALPHA_OFFSET	IPU_REG_OFFSET + 0x0000800C			
#define IPU_IDMAC_ALT_SEP_ALPHA_OFFSET	IPU_REG_OFFSET + 0x00008010			
#define IPU_IDMAC_CH_PRI_1_OFFSET	IPU_REG_OFFSET + 0x00008014			
#define IPU_IDMAC_CH_PRI_2_OFFSET	IPU_REG_OFFSET + 0x00008018			
#define IPU_IDMAC_WM_EN_1_OFFSET	IPU_REG_OFFSET + 0x0000801C			
#define IPU_IDMAC_WM_EN_2_OFFSET	IPU_REG_OFFSET + 0x00008020			
#define IPU_IDMAC_LOCK_EN_2_OFFSET	IPU_REG_OFFSET + 0x00008024			
#define IPU_IDMAC_SUB_ADDR_0_OFFSET	IPU_REG_OFFSET + 0x00008028			
#define IPU_IDMAC_SUB_ADDR_1_OFFSET	IPU_REG_OFFSET + 0x0000802C			
#define IPU_IDMAC_SUB_ADDR_2_OFFSET	IPU_REG_OFFSET + 0x00008030			
#define IPU_IDMAC_BNDM_EN_1_OFFSET	IPU_REG_OFFSET + 0x00008034			
#define IPU_IDMAC_BNDM_EN_2_OFFSET	IPU_REG_OFFSET + 0x00008038			
#define IPU_IDMAC_SC_CORD_OFFSET	IPU_REG_OFFSET + 0x0000803C			
#define IPU_IDMAC_CH_BUSY_1_OFFSET	IPU_REG_OFFSET + 0x00008040			
#define IPU_IDMAC_CH_BUSY_2_OFFSET	IPU_REG_OFFSET + 0x00008044			
#define IPU_ISP_C0_OFFSET	IPU_REG_OFFSET + 0x00010000			
#define IPU_ISP_C1_OFFSET	IPU_REG_OFFSET + 0x00010004			
#define IPU_ISP_FS_OFFSET	IPU_REG_OFFSET + 0x00010008			
#define IPU_ISP_BI_OFFSET	IPU_REG_OFFSET + 0x0001000C			
#define IPU_ISP_OCO_OFFSET	IPU_REG_OFFSET + 0x00010010			
#define IPU_ISP_BPR1_OFFSET	IPU_REG_OFFSET + 0x00010014			
#define IPU_ISP_BPR2_OFFSET	IPU_REG_OFFSET + 0x00010018			
#define IPU_ISP_BPR3_OFFSET	IPU_REG_OFFSET + 0x0001001C			
#define IPU_ISP_CG_0_OFFSET	IPU_REG_OFFSET + 0x00010020			
#define IPU_ISP_CG_1_OFFSET	IPU_REG_OFFSET + 0x00010024			
#define IPU_ISP_ROC_0_OFFSET	IPU_REG_OFFSET + 0x00010028			
#define IPU_ISP_ROC_1_OFFSET	IPU_REG_OFFSET + 0x0001002C			
#define IPU_ISP_ROC_2_OFFSET	IPU_REG_OFFSET + 0x00010030			
#define IPU_ISP_RRO_0_OFFSET	IPU_REG_OFFSET + 0x00010034			
#define IPU_ISP_RRO_1_OFFSET	IPU_REG_OFFSET + 0x00010038			
#define IPU_ISP_RRO_2_OFFSET	IPU_REG_OFFSET + 0x0001003C			
#define IPU_ISP_RRO_3_OFFSET	IPU_REG_OFFSET + 0x00010040			
#define IPU_ISP_RRO_4_OFFSET	IPU_REG_OFFSET + 0x00010044			
#define IPU_ISP_RRO_5_OFFSET	IPU_REG_OFFSET + 0x00010048			
#define IPU_ISP_RRO_6_OFFSET	IPU_REG_OFFSET + 0x0001004C			
#define IPU_ISP_RRO_7_OFFSET	IPU_REG_OFFSET + 0x00010050			
#define IPU_ISP_GRO_0_OFFSET	IPU_REG_OFFSET + 0x00010054			
#define IPU_ISP_GRO_1_OFFSET	IPU_REG_OFFSET + 0x00010058			
#define IPU_ISP_GRO_2_OFFSET	IPU_REG_OFFSET + 0x0001005C			
#define IPU_ISP_GRO_3_OFFSET	IPU_REG_OFFSET + 0x00010060			
#define IPU_ISP_GRO_4_OFFSET	IPU_REG_OFFSET + 0x00010064			
#define IPU_ISP_GRO_5_OFFSET	IPU_REG_OFFSET + 0x00010068			
#define IPU_ISP_GRO_6_OFFSET	IPU_REG_OFFSET + 0x0001006C			
#define IPU_ISP_GRO_7_OFFSET	IPU_REG_OFFSET + 0x00010070			
#define IPU_ISP_BRO_0_OFFSET	IPU_REG_OFFSET + 0x00010074			
#define IPU_ISP_BRO_1_OFFSET	IPU_REG_OFFSET + 0x00010078			
#define IPU_ISP_BRO_2_OFFSET	IPU_REG_OFFSET + 0x0001007C			
#define IPU_ISP_BRO_3_OFFSET	IPU_REG_OFFSET + 0x00010080			
#define IPU_ISP_BRO_4_OFFSET	IPU_REG_OFFSET + 0x00010084			
#define IPU_ISP_BRO_5_OFFSET	IPU_REG_OFFSET + 0x00010088			
#define IPU_ISP_BRO_6_OFFSET	IPU_REG_OFFSET + 0x0001008C			
#define IPU_ISP_BRO_7_OFFSET	IPU_REG_OFFSET + 0x00010090			
#define IPU_ISP_GAMMA_C_0_OFFSET	IPU_REG_OFFSET + 0x00010094			
#define IPU_ISP_GAMMA_C_1_OFFSET	IPU_REG_OFFSET + 0x00010098			
#define IPU_ISP_GAMMA_C_2_OFFSET	IPU_REG_OFFSET + 0x0001009C			
#define IPU_ISP_GAMMA_C_3_OFFSET	IPU_REG_OFFSET + 0x000100A0			
#define IPU_ISP_GAMMA_C_4_OFFSET	IPU_REG_OFFSET + 0x000100A4			
#define IPU_ISP_GAMMA_C_5_OFFSET	IPU_REG_OFFSET + 0x000100A8			
#define IPU_ISP_GAMMA_C_6_OFFSET	IPU_REG_OFFSET + 0x000100AC			
#define IPU_ISP_GAMMA_C_7_OFFSET	IPU_REG_OFFSET + 0x000100B0			
#define IPU_ISP_GAMMA_S_0_OFFSET	IPU_REG_OFFSET + 0x000100B4			
#define IPU_ISP_GAMMA_S_1_OFFSET	IPU_REG_OFFSET + 0x000100B8			
#define IPU_ISP_GAMMA_S_2_OFFSET	IPU_REG_OFFSET + 0x000100BC			
#define IPU_ISP_GAMMA_S_3_OFFSET	IPU_REG_OFFSET + 0x000100C0			
#define IPU_ISP_CSCA_0_OFFSET	IPU_REG_OFFSET + 0x000100C4			
#define IPU_ISP_CSCA_1_OFFSET	IPU_REG_OFFSET + 0x000100C8			
#define IPU_ISP_CSCA_2_OFFSET	IPU_REG_OFFSET + 0x000100CC			
#define IPU_ISP_CSCA_3_OFFSET	IPU_REG_OFFSET + 0x000100D0			
#define IPU_ISP_CSC_0_OFFSET	IPU_REG_OFFSET + 0x000100D4			
#define IPU_ISP_CSC_1_OFFSET	IPU_REG_OFFSET + 0x000100D8			
#define IPU_ISP_CNS_C_0_OFFSET	IPU_REG_OFFSET + 0x000100DC			
#define IPU_ISP_CNS_C_1_OFFSET	IPU_REG_OFFSET + 0x000100E0			
#define IPU_ISP_CNS_C_2_OFFSET	IPU_REG_OFFSET + 0x000100E4			
#define IPU_ISP_CNS_C_3_OFFSET	IPU_REG_OFFSET + 0x000100E8			
#define IPU_ISP_CNS_C_4_OFFSET	IPU_REG_OFFSET + 0x000100EC			
#define IPU_ISP_CNS_C_5_OFFSET	IPU_REG_OFFSET + 0x000100F0			
#define IPU_ISP_CNS_C_6_OFFSET	IPU_REG_OFFSET + 0x000100F4			
#define IPU_ISP_CNS_C_7_OFFSET	IPU_REG_OFFSET + 0x000100F8			
#define IPU_ISP_CNS_S_0_OFFSET	IPU_REG_OFFSET + 0x000100FC			
#define IPU_ISP_CNS_S_1_OFFSET	IPU_REG_OFFSET + 0x00010100			
#define IPU_ISP_CNS_S_2_OFFSET	IPU_REG_OFFSET + 0x00010104			
#define IPU_ISP_CNS_S_3_OFFSET	IPU_REG_OFFSET + 0x00010108			
#define IPU_ISP_MTF_ROC_C_0_OFFSET	IPU_REG_OFFSET + 0x0001010C			
#define IPU_ISP_MTF_ROC_C_1_OFFSET	IPU_REG_OFFSET + 0x00010110			
#define IPU_ISP_MTF_ROC_C_2_OFFSET	IPU_REG_OFFSET + 0x00010114			
#define IPU_ISP_MTF_ROC_C_3_OFFSET	IPU_REG_OFFSET + 0x00010118			
#define IPU_ISP_MTF_ROC_S_0_OFFSET	IPU_REG_OFFSET + 0x0001011C			
#define IPU_ISP_MTF_ROC_S_1_OFFSET	IPU_REG_OFFSET + 0x00010120			
#define IPU_ISP_HFE_0_OFFSET	IPU_REG_OFFSET + 0x00010124			
#define IPU_ISP_HFE_1_OFFSET	IPU_REG_OFFSET + 0x00010128			
#define IPU_ISP_HFE_2_OFFSET	IPU_REG_OFFSET + 0x0001012C			
#define IPU_ISP_HFE_S_0_OFFSET	IPU_REG_OFFSET + 0x00010130			
#define IPU_ISP_HFE_S_1_OFFSET	IPU_REG_OFFSET + 0x00010134			
#define IPU_ISP_HFE_S_2_OFFSET	IPU_REG_OFFSET + 0x00010138			
#define IPU_ISP_HFE_S_3_OFFSET	IPU_REG_OFFSET + 0x0001013C			
#define IPU_ISP_HFE_C_0_OFFSET	IPU_REG_OFFSET + 0x00010140			
#define IPU_ISP_HFE_C_1_OFFSET	IPU_REG_OFFSET + 0x00010144			
#define IPU_ISP_HFE_C_2_OFFSET	IPU_REG_OFFSET + 0x00010148			
#define IPU_ISP_HFE_C_3_OFFSET	IPU_REG_OFFSET + 0x0001014C			
#define IPU_ISP_STC_0_OFFSET	IPU_REG_OFFSET + 0x00010150			
#define IPU_ISP_STC_1_OFFSET	IPU_REG_OFFSET + 0x00010154			
#define IPU_ISP_FC_0_OFFSET	IPU_REG_OFFSET + 0x00010158			
#define IPU_ISP_FC_1_OFFSET	IPU_REG_OFFSET + 0x0001015C			
#define IPU_ISP_FS_SWSX_OFFSET	IPU_REG_OFFSET + 0x00010160			
#define IPU_ISP_FS_SYSZ_OFFSET	IPU_REG_OFFSET + 0x00010164			
#define IPU_ISP_FS_CWCX_OFFSET	IPU_REG_OFFSET + 0x00010168			
#define IPU_ISP_FS_CYCZ_OFFSET	IPU_REG_OFFSET + 0x0001016C			
#define IPU_ISP_FS_MWMX_OFFSET	IPU_REG_OFFSET + 0x00010170			
#define IPU_ISP_FS_MYMZ_OFFSET	IPU_REG_OFFSET + 0x00010174			
#define IPU_ISP_FS_BPRS_OFFSET	IPU_REG_OFFSET + 0x00010178			
#define IPU_DP_COM_CONF_SYNC_OFFSET	IPU_REG_OFFSET + 0x00018000			
#define IPU_DP_GRAPH_WIND_CTRL_SYNC_OFFSET	IPU_REG_OFFSET + 0x00018004			
#define IPU_DP_FG_POS_SYNC_OFFSET	IPU_REG_OFFSET + 0x00018008			
#define IPU_DP_CUR_POS_SYNC_OFFSET	IPU_REG_OFFSET + 0x0001800C			
#define IPU_DP_CUR_MAP_SYNC_OFFSET	IPU_REG_OFFSET + 0x00018010			
#define IPU_DP_GAMMA_C_SYNC_0_OFFSET	IPU_REG_OFFSET + 0x00018014			
#define IPU_DP_GAMMA_C_SYNC_1_OFFSET	IPU_REG_OFFSET + 0x00018018			
#define IPU_DP_GAMMA_C_SYNC_2_OFFSET	IPU_REG_OFFSET + 0x0001801C			
#define IPU_DP_GAMMA_C_SYNC_3_OFFSET	IPU_REG_OFFSET + 0x00018020			
#define IPU_DP_GAMMA_C_SYNC_4_OFFSET	IPU_REG_OFFSET + 0x00018024			
#define IPU_DP_GAMMA_C_SYNC_5_OFFSET	IPU_REG_OFFSET + 0x00018028			
#define IPU_DP_GAMMA_C_SYNC_6_OFFSET	IPU_REG_OFFSET + 0x0001802C			
#define IPU_DP_GAMMA_C_SYNC_7_OFFSET	IPU_REG_OFFSET + 0x00018030			
#define IPU_DP_GAMMA_S_SYNC_0_OFFSET	IPU_REG_OFFSET + 0x00018034			
#define IPU_DP_GAMMA_S_SYNC_1_OFFSET	IPU_REG_OFFSET + 0x00018038			
#define IPU_DP_GAMMA_S_SYNC_2_OFFSET	IPU_REG_OFFSET + 0x0001803C			
#define IPU_DP_GAMMA_S_SYNC_3_OFFSET	IPU_REG_OFFSET + 0x00018040			
#define IPU_DP_CSCA_SYNC_0_OFFSET	IPU_REG_OFFSET + 0x00018044			
#define IPU_DP_CSCA_SYNC_1_OFFSET	IPU_REG_OFFSET + 0x00018048			
#define IPU_DP_CSCA_SYNC_2_OFFSET	IPU_REG_OFFSET + 0x0001804C			
#define IPU_DP_CSCA_SYNC_3_OFFSET	IPU_REG_OFFSET + 0x00018050			
#define IPU_DP_CSC_SYNC_0_OFFSET	IPU_REG_OFFSET + 0x00018054			
#define IPU_DP_CSC_SYNC_1_OFFSET	IPU_REG_OFFSET + 0x00018058			
#define IPU_DP_COM_CONF_ASYNC_OFFSET	IPU_REG_OFFSET + 0x0001805C			
#define IPU_DP_GRAPH_WIND_CTRL_ASYNC_OFFSET	IPU_REG_OFFSET + 0x00018060			
#define IPU_DP_FG_POS_ASYNC_OFFSET	IPU_REG_OFFSET + 0x00018064			
#define IPU_DP_CUR_POS_ASYNC_OFFSET	IPU_REG_OFFSET + 0x00018068			
#define IPU_DP_CUR_MAP_ASYNC_OFFSET	IPU_REG_OFFSET + 0x0001806C			
#define IPU_DP_GAMMA_C_ASYNC_0_OFFSET	IPU_REG_OFFSET + 0x00018070			
#define IPU_DP_GAMMA_C_ASYNC_1_OFFSET	IPU_REG_OFFSET + 0x00018074			
#define IPU_DP_GAMMA_C_ASYNC_2_OFFSET	IPU_REG_OFFSET + 0x00018078			
#define IPU_DP_GAMMA_C_ASYNC_3_OFFSET	IPU_REG_OFFSET + 0x0001807C			
#define IPU_DP_GAMMA_C_ASYNC_4_OFFSET	IPU_REG_OFFSET + 0x00018080			
#define IPU_DP_GAMMA_C_ASYNC_5_OFFSET	IPU_REG_OFFSET + 0x00018084			
#define IPU_DP_GAMMA_C_ASYNC_6_OFFSET	IPU_REG_OFFSET + 0x00018088			
#define IPU_DP_GAMMA_C_ASYNC_7_OFFSET	IPU_REG_OFFSET + 0x0001808C			
#define IPU_DP_GAMMA_S_ASYNC_0_OFFSET	IPU_REG_OFFSET + 0x00018090			
#define IPU_DP_GAMMA_S_ASYNC_1_OFFSET	IPU_REG_OFFSET + 0x00018094			
#define IPU_DP_GAMMA_S_ASYNC_2_OFFSET	IPU_REG_OFFSET + 0x00018098			
#define IPU_DP_GAMMA_S_ASYNC_3_OFFSET	IPU_REG_OFFSET + 0x0001809C			
#define IPU_DP_CSCA_ASYNC_0_OFFSET	IPU_REG_OFFSET + 0x000180A0			
#define IPU_DP_CSCA_ASYNC_1_OFFSET	IPU_REG_OFFSET + 0x000180A4			
#define IPU_DP_CSCA_ASYNC_2_OFFSET	IPU_REG_OFFSET + 0x000180A8			
#define IPU_DP_CSCA_ASYNC_3_OFFSET	IPU_REG_OFFSET + 0x000180AC			
#define IPU_DP_CSC_ASYNC_0_OFFSET	IPU_REG_OFFSET + 0x000180B0			
#define IPU_DP_CSC_ASYNC_1_OFFSET	IPU_REG_OFFSET + 0x000180B4			
#define IPU_DP_DEBUG_CNT_OFFSET	IPU_REG_OFFSET + 0x000180B8			
#define IPU_DP_DEBUG_STAT_OFFSET	IPU_REG_OFFSET + 0x000180BC			
#define IPU_IC_CONF_OFFSET	IPU_REG_OFFSET + 0x00020000			
#define IPU_IC_PRP_ENC_RSC_OFFSET	IPU_REG_OFFSET + 0x00020004			
#define IPU_IC_PRP_VF_RSC_OFFSET	IPU_REG_OFFSET + 0x00020008			
#define IPU_IC_PP_RSC_OFFSET	IPU_REG_OFFSET + 0x0002000C			
#define IPU_IC_CMBP_1_OFFSET	IPU_REG_OFFSET + 0x00020010			
#define IPU_IC_CMBP_2_OFFSET	IPU_REG_OFFSET + 0x00020014			
#define IPU_IC_IDMAC_1_OFFSET	IPU_REG_OFFSET + 0x00020018			
#define IPU_IC_IDMAC_2_OFFSET	IPU_REG_OFFSET + 0x0002001C			
#define IPU_IC_IDMAC_3_OFFSET	IPU_REG_OFFSET + 0x00020020			
#define IPU_IC_IDMAC_4_OFFSET	IPU_REG_OFFSET + 0x00020024			
#define IPU_CSI0_SENS_CONF_OFFSET	IPU_REG_OFFSET + 0x00030000			
#define IPU_CSI0_SENS_FRM_SIZE_OFFSET	IPU_REG_OFFSET + 0x00030004			
#define IPU_CSI0_ACT_FRM_SIZE_OFFSET	IPU_REG_OFFSET + 0x00030008			
#define IPU_CSI0_OUT_FRM_CTRL_OFFSET	IPU_REG_OFFSET + 0x0003000C			
#define IPU_CSI0_TST_CTRL_OFFSET	IPU_REG_OFFSET + 0x00030010			
#define IPU_CSI0_CCIR_CODE_1_OFFSET	IPU_REG_OFFSET + 0x00030014			
#define IPU_CSI0_CCIR_CODE_2_OFFSET	IPU_REG_OFFSET + 0x00030018			
#define IPU_CSI0_CCIR_CODE_3_OFFSET	IPU_REG_OFFSET + 0x0003001C			
#define IPU_CSI0_DI_OFFSET	IPU_REG_OFFSET + 0x00030020			
#define IPU_CSI0_SKIP_OFFSET	IPU_REG_OFFSET + 0x00030024			
#define IPU_CSI0_CPD_CTRL_OFFSET	IPU_REG_OFFSET + 0x00030028			
#define IPU_CSI0_CPD_RC_0_OFFSET	IPU_REG_OFFSET + 0x0003002C			
#define IPU_CSI0_CPD_RC_1_OFFSET	IPU_REG_OFFSET + 0x00030030			
#define IPU_CSI0_CPD_RC_2_OFFSET	IPU_REG_OFFSET + 0x00030034			
#define IPU_CSI0_CPD_RC_3_OFFSET	IPU_REG_OFFSET + 0x00030038			
#define IPU_CSI0_CPD_RC_4_OFFSET	IPU_REG_OFFSET + 0x0003003C			
#define IPU_CSI0_CPD_RC_5_OFFSET	IPU_REG_OFFSET + 0x00030040			
#define IPU_CSI0_CPD_RC_6_OFFSET	IPU_REG_OFFSET + 0x00030044			
#define IPU_CSI0_CPD_RC_7_OFFSET	IPU_REG_OFFSET + 0x00030048			
#define IPU_CSI0_CPD_RS_0_OFFSET	IPU_REG_OFFSET + 0x0003004C			
#define IPU_CSI0_CPD_RS_1_OFFSET	IPU_REG_OFFSET + 0x00030050			
#define IPU_CSI0_CPD_RS_2_OFFSET	IPU_REG_OFFSET + 0x00030054			
#define IPU_CSI0_CPD_RS_3_OFFSET	IPU_REG_OFFSET + 0x00030058			
#define IPU_CSI0_CPD_GRC_0_OFFSET	IPU_REG_OFFSET + 0x0003005C			
#define IPU_CSI0_CPD_GRC_1_OFFSET	IPU_REG_OFFSET + 0x00030060			
#define IPU_CSI0_CPD_GRC_2_OFFSET	IPU_REG_OFFSET + 0x00030064			
#define IPU_CSI0_CPD_GRC_3_OFFSET	IPU_REG_OFFSET + 0x00030068			
#define IPU_CSI0_CPD_GRC_4_OFFSET	IPU_REG_OFFSET + 0x0003006C			
#define IPU_CSI0_CPD_GRC_5_OFFSET	IPU_REG_OFFSET + 0x00030070			
#define IPU_CSI0_CPD_GRC_6_OFFSET	IPU_REG_OFFSET + 0x00030074			
#define IPU_CSI0_CPD_GRC_7_OFFSET	IPU_REG_OFFSET + 0x00030078			
#define IPU_CSI0_CPD_GRS_0_OFFSET	IPU_REG_OFFSET + 0x0003007C			
#define IPU_CSI0_CPD_GRS_1_OFFSET	IPU_REG_OFFSET + 0x00030080			
#define IPU_CSI0_CPD_GRS_2_OFFSET	IPU_REG_OFFSET + 0x00030084			
#define IPU_CSI0_CPD_GRS_3_OFFSET	IPU_REG_OFFSET + 0x00030088			
#define IPU_CSI0_CPD_GBC_0_OFFSET	IPU_REG_OFFSET + 0x0003008C			
#define IPU_CSI0_CPD_GBC_1_OFFSET	IPU_REG_OFFSET + 0x00030090			
#define IPU_CSI0_CPD_GBC_2_OFFSET	IPU_REG_OFFSET + 0x00030094			
#define IPU_CSI0_CPD_GBC_3_OFFSET	IPU_REG_OFFSET + 0x00030098			
#define IPU_CSI0_CPD_GBC_4_OFFSET	IPU_REG_OFFSET + 0x0003009C			
#define IPU_CSI0_CPD_GBC_5_OFFSET	IPU_REG_OFFSET + 0x000300A0			
#define IPU_CSI0_CPD_GBC_6_OFFSET	IPU_REG_OFFSET + 0x000300A4			
#define IPU_CSI0_CPD_GBC_7_OFFSET	IPU_REG_OFFSET + 0x000300A8			
#define IPU_CSI0_CPD_GBS_0_OFFSET	IPU_REG_OFFSET + 0x000300AC			
#define IPU_CSI0_CPD_GBS_1_OFFSET	IPU_REG_OFFSET + 0x000300B0			
#define IPU_CSI0_CPD_GBS_2_OFFSET	IPU_REG_OFFSET + 0x000300B4			
#define IPU_CSI0_CPD_GBS_3_OFFSET	IPU_REG_OFFSET + 0x000300B8			
#define IPU_CSI0_CPD_BC_0_OFFSET	IPU_REG_OFFSET + 0x000300BC			
#define IPU_CSI0_CPD_BC_1_OFFSET	IPU_REG_OFFSET + 0x000300C0			
#define IPU_CSI0_CPD_BC_2_OFFSET	IPU_REG_OFFSET + 0x000300C4			
#define IPU_CSI0_CPD_BC_3_OFFSET	IPU_REG_OFFSET + 0x000300C8			
#define IPU_CSI0_CPD_BC_4_OFFSET	IPU_REG_OFFSET + 0x000300CC			
#define IPU_CSI0_CPD_BC_5_OFFSET	IPU_REG_OFFSET + 0x000300D0			
#define IPU_CSI0_CPD_BC_6_OFFSET	IPU_REG_OFFSET + 0x000300D4			
#define IPU_CSI0_CPD_BC_7_OFFSET	IPU_REG_OFFSET + 0x000300D8			
#define IPU_CSI0_CPD_BS_0_OFFSET	IPU_REG_OFFSET + 0x000300DC			
#define IPU_CSI0_CPD_BS_1_OFFSET	IPU_REG_OFFSET + 0x000300E0			
#define IPU_CSI0_CPD_BS_2_OFFSET	IPU_REG_OFFSET + 0x000300E4			
#define IPU_CSI0_CPD_BS_3_OFFSET	IPU_REG_OFFSET + 0x000300E8			
#define IPU_CSI0_CPD_OFFSET1_OFFSET	IPU_REG_OFFSET + 0x000300EC			
#define IPU_CSI0_CPD_OFFSET2_OFFSET	IPU_REG_OFFSET + 0x000300F0			
#define IPU_CSI1_SENS_CONF_OFFSET	IPU_REG_OFFSET + 0x00038000			
#define IPU_CSI1_SENS_FRM_SIZE_OFFSET	IPU_REG_OFFSET + 0x00038004			
#define IPU_CSI1_ACT_FRM_SIZE_OFFSET	IPU_REG_OFFSET + 0x00038008			
#define IPU_CSI1_OUT_FRM_CTRL_OFFSET	IPU_REG_OFFSET + 0x0003800C			
#define IPU_CSI1_TST_CTRL_OFFSET	IPU_REG_OFFSET + 0x00038010			
#define IPU_CSI1_CCIR_CODE_1_OFFSET	IPU_REG_OFFSET + 0x00038014			
#define IPU_CSI1_CCIR_CODE_2_OFFSET	IPU_REG_OFFSET + 0x00038018			
#define IPU_CSI1_CCIR_CODE_3_OFFSET	IPU_REG_OFFSET + 0x0003801C			
#define IPU_CSI1_DI_OFFSET	IPU_REG_OFFSET + 0x00038020			
#define IPU_CSI1_SKIP_OFFSET	IPU_REG_OFFSET + 0x00038024			
#define IPU_CSI1_CPD_CTRL_OFFSET	IPU_REG_OFFSET + 0x00038028			
#define IPU_CSI1_CPD_RC_0_OFFSET	IPU_REG_OFFSET + 0x0003802C			
#define IPU_CSI1_CPD_RC_1_OFFSET	IPU_REG_OFFSET + 0x00038030			
#define IPU_CSI1_CPD_RC_2_OFFSET	IPU_REG_OFFSET + 0x00038034			
#define IPU_CSI1_CPD_RC_3_OFFSET	IPU_REG_OFFSET + 0x00038038			
#define IPU_CSI1_CPD_RC_4_OFFSET	IPU_REG_OFFSET + 0x0003803C			
#define IPU_CSI1_CPD_RC_5_OFFSET	IPU_REG_OFFSET + 0x00038040			
#define IPU_CSI1_CPD_RC_6_OFFSET	IPU_REG_OFFSET + 0x00038044			
#define IPU_CSI1_CPD_RC_7_OFFSET	IPU_REG_OFFSET + 0x00038048			
#define IPU_CSI1_CPD_RS_0_OFFSET	IPU_REG_OFFSET + 0x0003804C			
#define IPU_CSI1_CPD_RS_1_OFFSET	IPU_REG_OFFSET + 0x00038050			
#define IPU_CSI1_CPD_RS_2_OFFSET	IPU_REG_OFFSET + 0x00038054			
#define IPU_CSI1_CPD_RS_3_OFFSET	IPU_REG_OFFSET + 0x00038058			
#define IPU_CSI1_CPD_GRC_0_OFFSET	IPU_REG_OFFSET + 0x0003805C			
#define IPU_CSI1_CPD_GRC_1_OFFSET	IPU_REG_OFFSET + 0x00038060			
#define IPU_CSI1_CPD_GRC_2_OFFSET	IPU_REG_OFFSET + 0x00038064			
#define IPU_CSI1_CPD_GRC_3_OFFSET	IPU_REG_OFFSET + 0x00038068			
#define IPU_CSI1_CPD_GRC_4_OFFSET	IPU_REG_OFFSET + 0x0003806C			
#define IPU_CSI1_CPD_GRC_5_OFFSET	IPU_REG_OFFSET + 0x00038070			
#define IPU_CSI1_CPD_GRC_6_OFFSET	IPU_REG_OFFSET + 0x00038074			
#define IPU_CSI1_CPD_GRC_7_OFFSET	IPU_REG_OFFSET + 0x00038078			
#define IPU_CSI1_CPD_GRS_0_OFFSET	IPU_REG_OFFSET + 0x0003807C			
#define IPU_CSI1_CPD_GRS_1_OFFSET	IPU_REG_OFFSET + 0x00038080			
#define IPU_CSI1_CPD_GRS_2_OFFSET	IPU_REG_OFFSET + 0x00038084			
#define IPU_CSI1_CPD_GRS_3_OFFSET	IPU_REG_OFFSET + 0x00038088			
#define IPU_CSI1_CPD_GBC_0_OFFSET	IPU_REG_OFFSET + 0x0003808C			
#define IPU_CSI1_CPD_GBC_1_OFFSET	IPU_REG_OFFSET + 0x00038090			
#define IPU_CSI1_CPD_GBC_2_OFFSET	IPU_REG_OFFSET + 0x00038094			
#define IPU_CSI1_CPD_GBC_3_OFFSET	IPU_REG_OFFSET + 0x00038098			
#define IPU_CSI1_CPD_GBC_4_OFFSET	IPU_REG_OFFSET + 0x0003809C			
#define IPU_CSI1_CPD_GBC_5_OFFSET	IPU_REG_OFFSET + 0x000380A0			
#define IPU_CSI1_CPD_GBC_6_OFFSET	IPU_REG_OFFSET + 0x000380A4			
#define IPU_CSI1_CPD_GBC_7_OFFSET	IPU_REG_OFFSET + 0x000380A8			
#define IPU_CSI1_CPD_GBS_0_OFFSET	IPU_REG_OFFSET + 0x000380AC			
#define IPU_CSI1_CPD_GBS_1_OFFSET	IPU_REG_OFFSET + 0x000380B0			
#define IPU_CSI1_CPD_GBS_2_OFFSET	IPU_REG_OFFSET + 0x000380B4			
#define IPU_CSI1_CPD_GBS_3_OFFSET	IPU_REG_OFFSET + 0x000380B8			
#define IPU_CSI1_CPD_BC_0_OFFSET	IPU_REG_OFFSET + 0x000380BC			
#define IPU_CSI1_CPD_BC_1_OFFSET	IPU_REG_OFFSET + 0x000380C0			
#define IPU_CSI1_CPD_BC_2_OFFSET	IPU_REG_OFFSET + 0x000380C4			
#define IPU_CSI1_CPD_BC_3_OFFSET	IPU_REG_OFFSET + 0x000380C8			
#define IPU_CSI1_CPD_BC_4_OFFSET	IPU_REG_OFFSET + 0x000380CC			
#define IPU_CSI1_CPD_BC_5_OFFSET	IPU_REG_OFFSET + 0x000380D0			
#define IPU_CSI1_CPD_BC_6_OFFSET	IPU_REG_OFFSET + 0x000380D4			
#define IPU_CSI1_CPD_BC_7_OFFSET	IPU_REG_OFFSET + 0x000380D8			
#define IPU_CSI1_CPD_BS_0_OFFSET	IPU_REG_OFFSET + 0x000380DC			
#define IPU_CSI1_CPD_BS_1_OFFSET	IPU_REG_OFFSET + 0x000380E0			
#define IPU_CSI1_CPD_BS_2_OFFSET	IPU_REG_OFFSET + 0x000380E4			
#define IPU_CSI1_CPD_BS_3_OFFSET	IPU_REG_OFFSET + 0x000380E8			
#define IPU_CSI1_CPD_OFFSET1_OFFSET	IPU_REG_OFFSET + 0x000380EC			
#define IPU_CSI1_CPD_OFFSET2_OFFSET	IPU_REG_OFFSET + 0x000380F0			
#define IPU_DI0_GENERAL_OFFSET	IPU_REG_OFFSET + 0x00040000			
#define IPU_DI0_BS_CLKGEN0_OFFSET	IPU_REG_OFFSET + 0x00040004			
#define IPU_DI0_BS_CLKGEN1_OFFSET	IPU_REG_OFFSET + 0x00040008			
#define IPU_DI0_SW_GEN0_1_OFFSET	IPU_REG_OFFSET + 0x0004000C			
#define IPU_DI0_SW_GEN0_2_OFFSET	IPU_REG_OFFSET + 0x00040010			
#define IPU_DI0_SW_GEN0_3_OFFSET	IPU_REG_OFFSET + 0x00040014			
#define IPU_DI0_SW_GEN0_4_OFFSET	IPU_REG_OFFSET + 0x00040018			
#define IPU_DI0_SW_GEN0_5_OFFSET	IPU_REG_OFFSET + 0x0004001C			
#define IPU_DI0_SW_GEN0_6_OFFSET	IPU_REG_OFFSET + 0x00040020			
#define IPU_DI0_SW_GEN0_7_OFFSET	IPU_REG_OFFSET + 0x00040024			
#define IPU_DI0_SW_GEN0_8_OFFSET	IPU_REG_OFFSET + 0x00040028			
#define IPU_DI0_SW_GEN0_9_OFFSET	IPU_REG_OFFSET + 0x0004002C			
#define IPU_DI0_SW_GEN1_1_OFFSET	IPU_REG_OFFSET + 0x00040030			
#define IPU_DI0_SW_GEN1_2_OFFSET	IPU_REG_OFFSET + 0x00040034			
#define IPU_DI0_SW_GEN1_3_OFFSET	IPU_REG_OFFSET + 0x00040038			
#define IPU_DI0_SW_GEN1_4_OFFSET	IPU_REG_OFFSET + 0x0004003C			
#define IPU_DI0_SW_GEN1_5_OFFSET	IPU_REG_OFFSET + 0x00040040			
#define IPU_DI0_SW_GEN1_6_OFFSET	IPU_REG_OFFSET + 0x00040044			
#define IPU_DI0_SW_GEN1_7_OFFSET	IPU_REG_OFFSET + 0x00040048			
#define IPU_DI0_SW_GEN1_8_OFFSET	IPU_REG_OFFSET + 0x0004004C			
#define IPU_DI0_SW_GEN1_9_OFFSET	IPU_REG_OFFSET + 0x00040050			
#define IPU_DI0_SYNC_AS_GEN_OFFSET	IPU_REG_OFFSET + 0x00040054			
#define IPU_DI0_DW_GEN_0_OFFSET	IPU_REG_OFFSET + 0x00040058			
#define IPU_DI0_DW_GEN_1_OFFSET	IPU_REG_OFFSET + 0x0004005C			
#define IPU_DI0_DW_GEN_2_OFFSET	IPU_REG_OFFSET + 0x00040060			
#define IPU_DI0_DW_GEN_3_OFFSET	IPU_REG_OFFSET + 0x00040064			
#define IPU_DI0_DW_GEN_4_OFFSET	IPU_REG_OFFSET + 0x00040068			
#define IPU_DI0_DW_GEN_5_OFFSET	IPU_REG_OFFSET + 0x0004006C			
#define IPU_DI0_DW_GEN_6_OFFSET	IPU_REG_OFFSET + 0x00040070			
#define IPU_DI0_DW_GEN_7_OFFSET	IPU_REG_OFFSET + 0x00040074			
#define IPU_DI0_DW_GEN_8_OFFSET	IPU_REG_OFFSET + 0x00040078			
#define IPU_DI0_DW_GEN_9_OFFSET	IPU_REG_OFFSET + 0x0004007C			
#define IPU_DI0_DW_GEN_10_OFFSET	IPU_REG_OFFSET + 0x00040080			
#define IPU_DI0_DW_GEN_11_OFFSET	IPU_REG_OFFSET + 0x00040084			
#define IPU_DI0_DW_SET0_0_OFFSET	IPU_REG_OFFSET + 0x00040088			
#define IPU_DI0_DW_SET0_1_OFFSET	IPU_REG_OFFSET + 0x0004008C			
#define IPU_DI0_DW_SET0_2_OFFSET	IPU_REG_OFFSET + 0x00040090			
#define IPU_DI0_DW_SET0_3_OFFSET	IPU_REG_OFFSET + 0x00040094			
#define IPU_DI0_DW_SET0_4_OFFSET	IPU_REG_OFFSET + 0x00040098			
#define IPU_DI0_DW_SET0_5_OFFSET	IPU_REG_OFFSET + 0x0004009C			
#define IPU_DI0_DW_SET0_6_OFFSET	IPU_REG_OFFSET + 0x000400A0			
#define IPU_DI0_DW_SET0_7_OFFSET	IPU_REG_OFFSET + 0x000400A4			
#define IPU_DI0_DW_SET0_8_OFFSET	IPU_REG_OFFSET + 0x000400A8			
#define IPU_DI0_DW_SET0_9_OFFSET	IPU_REG_OFFSET + 0x000400AC			
#define IPU_DI0_DW_SET0_10_OFFSET	IPU_REG_OFFSET + 0x000400B0			
#define IPU_DI0_DW_SET0_11_OFFSET	IPU_REG_OFFSET + 0x000400B4			
#define IPU_DI0_DW_SET1_0_OFFSET	IPU_REG_OFFSET + 0x000400B8			
#define IPU_DI0_DW_SET1_1_OFFSET	IPU_REG_OFFSET + 0x000400BC			
#define IPU_DI0_DW_SET1_2_OFFSET	IPU_REG_OFFSET + 0x000400C0			
#define IPU_DI0_DW_SET1_3_OFFSET	IPU_REG_OFFSET + 0x000400C4			
#define IPU_DI0_DW_SET1_4_OFFSET	IPU_REG_OFFSET + 0x000400C8			
#define IPU_DI0_DW_SET1_5_OFFSET	IPU_REG_OFFSET + 0x000400CC			
#define IPU_DI0_DW_SET1_6_OFFSET	IPU_REG_OFFSET + 0x000400D0			
#define IPU_DI0_DW_SET1_7_OFFSET	IPU_REG_OFFSET + 0x000400D4			
#define IPU_DI0_DW_SET1_8_OFFSET	IPU_REG_OFFSET + 0x000400D8			
#define IPU_DI0_DW_SET1_9_OFFSET	IPU_REG_OFFSET + 0x000400DC			
#define IPU_DI0_DW_SET1_10_OFFSET	IPU_REG_OFFSET + 0x000400E0			
#define IPU_DI0_DW_SET1_11_OFFSET	IPU_REG_OFFSET + 0x000400E4			
#define IPU_DI0_DW_SET2_0_OFFSET	IPU_REG_OFFSET + 0x000400E8			
#define IPU_DI0_DW_SET2_1_OFFSET	IPU_REG_OFFSET + 0x000400EC			
#define IPU_DI0_DW_SET2_2_OFFSET	IPU_REG_OFFSET + 0x000400F0			
#define IPU_DI0_DW_SET2_3_OFFSET	IPU_REG_OFFSET + 0x000400F4			
#define IPU_DI0_DW_SET2_4_OFFSET	IPU_REG_OFFSET + 0x000400F8			
#define IPU_DI0_DW_SET2_5_OFFSET	IPU_REG_OFFSET + 0x000400FC			
#define IPU_DI0_DW_SET2_6_OFFSET	IPU_REG_OFFSET + 0x00040100			
#define IPU_DI0_DW_SET2_7_OFFSET	IPU_REG_OFFSET + 0x00040104			
#define IPU_DI0_DW_SET2_8_OFFSET	IPU_REG_OFFSET + 0x00040108			
#define IPU_DI0_DW_SET2_9_OFFSET	IPU_REG_OFFSET + 0x0004010C			
#define IPU_DI0_DW_SET2_10_OFFSET	IPU_REG_OFFSET + 0x00040110			
#define IPU_DI0_DW_SET2_11_OFFSET	IPU_REG_OFFSET + 0x00040114			
#define IPU_DI0_DW_SET3_0_OFFSET	IPU_REG_OFFSET + 0x00040118			
#define IPU_DI0_DW_SET3_1_OFFSET	IPU_REG_OFFSET + 0x0004011C			
#define IPU_DI0_DW_SET3_2_OFFSET	IPU_REG_OFFSET + 0x00040120			
#define IPU_DI0_DW_SET3_3_OFFSET	IPU_REG_OFFSET + 0x00040124			
#define IPU_DI0_DW_SET3_4_OFFSET	IPU_REG_OFFSET + 0x00040128			
#define IPU_DI0_DW_SET3_5_OFFSET	IPU_REG_OFFSET + 0x0004012C			
#define IPU_DI0_DW_SET3_6_OFFSET	IPU_REG_OFFSET + 0x00040130			
#define IPU_DI0_DW_SET3_7_OFFSET	IPU_REG_OFFSET + 0x00040134			
#define IPU_DI0_DW_SET3_8_OFFSET	IPU_REG_OFFSET + 0x00040138			
#define IPU_DI0_DW_SET3_9_OFFSET	IPU_REG_OFFSET + 0x0004013C			
#define IPU_DI0_DW_SET3_10_OFFSET	IPU_REG_OFFSET + 0x00040140			
#define IPU_DI0_DW_SET3_11_OFFSET	IPU_REG_OFFSET + 0x00040144			
#define IPU_DI0_STP_REP_1_OFFSET	IPU_REG_OFFSET + 0x00040148			
#define IPU_DI0_STP_REP_2_OFFSET	IPU_REG_OFFSET + 0x0004014C			
#define IPU_DI0_STP_REP_3_OFFSET	IPU_REG_OFFSET + 0x00040150			
#define IPU_DI0_STP_REP_4_OFFSET	IPU_REG_OFFSET + 0x00040154			
#define IPU_DI0_STP_REP_9_OFFSET	IPU_REG_OFFSET + 0x00040158			
#define IPU_DI0_SER_CONF_OFFSET	IPU_REG_OFFSET + 0x0004015C			
#define IPU_DI0_SSC_OFFSET	IPU_REG_OFFSET + 0x00040160			
#define IPU_DI0_POL_OFFSET	IPU_REG_OFFSET + 0x00040164			
#define IPU_DI0_AW0_OFFSET	IPU_REG_OFFSET + 0x00040168			
#define IPU_DI0_AW1_OFFSET	IPU_REG_OFFSET + 0x0004016C			
#define IPU_DI1_GENERAL_OFFSET	IPU_REG_OFFSET + 0x00048000			
#define IPU_DI1_BS_CLKGEN0_OFFSET	IPU_REG_OFFSET + 0x00048004			
#define IPU_DI1_BS_CLKGEN1_OFFSET	IPU_REG_OFFSET + 0x00048008			
#define IPU_DI1_SW_GEN0_1_OFFSET	IPU_REG_OFFSET + 0x0004800C			
#define IPU_DI1_SW_GEN0_2_OFFSET	IPU_REG_OFFSET + 0x00048010			
#define IPU_DI1_SW_GEN0_3_OFFSET	IPU_REG_OFFSET + 0x00048014			
#define IPU_DI1_SW_GEN0_4_OFFSET	IPU_REG_OFFSET + 0x00048018			
#define IPU_DI1_SW_GEN0_5_OFFSET	IPU_REG_OFFSET + 0x0004801C			
#define IPU_DI1_SW_GEN0_6_OFFSET	IPU_REG_OFFSET + 0x00048020			
#define IPU_DI1_SW_GEN0_7_OFFSET	IPU_REG_OFFSET + 0x00048024			
#define IPU_DI1_SW_GEN0_8_OFFSET	IPU_REG_OFFSET + 0x00048028			
#define IPU_DI1_SW_GEN0_9_OFFSET	IPU_REG_OFFSET + 0x0004802C			
#define IPU_DI1_SW_GEN1_1_OFFSET	IPU_REG_OFFSET + 0x00048030			
#define IPU_DI1_SW_GEN1_2_OFFSET	IPU_REG_OFFSET + 0x00048034			
#define IPU_DI1_SW_GEN1_3_OFFSET	IPU_REG_OFFSET + 0x00048038			
#define IPU_DI1_SW_GEN1_4_OFFSET	IPU_REG_OFFSET + 0x0004803C			
#define IPU_DI1_SW_GEN1_5_OFFSET	IPU_REG_OFFSET + 0x00048040			
#define IPU_DI1_SW_GEN1_6_OFFSET	IPU_REG_OFFSET + 0x00048044			
#define IPU_DI1_SW_GEN1_7_OFFSET	IPU_REG_OFFSET + 0x00048048			
#define IPU_DI1_SW_GEN1_8_OFFSET	IPU_REG_OFFSET + 0x0004804C			
#define IPU_DI1_SW_GEN1_9_OFFSET	IPU_REG_OFFSET + 0x00048050			
#define IPU_DI1_SYNC_AS_GEN_OFFSET	IPU_REG_OFFSET + 0x00048054			
#define IPU_DI1_DW_GEN_0_OFFSET	IPU_REG_OFFSET + 0x00048058			
#define IPU_DI1_DW_GEN_1_OFFSET	IPU_REG_OFFSET + 0x0004805C			
#define IPU_DI1_DW_GEN_2_OFFSET	IPU_REG_OFFSET + 0x00048060			
#define IPU_DI1_DW_GEN_3_OFFSET	IPU_REG_OFFSET + 0x00048064			
#define IPU_DI1_DW_GEN_4_OFFSET	IPU_REG_OFFSET + 0x00048068			
#define IPU_DI1_DW_GEN_5_OFFSET	IPU_REG_OFFSET + 0x0004806C			
#define IPU_DI1_DW_GEN_6_OFFSET	IPU_REG_OFFSET + 0x00048070			
#define IPU_DI1_DW_GEN_7_OFFSET	IPU_REG_OFFSET + 0x00048074			
#define IPU_DI1_DW_GEN_8_OFFSET	IPU_REG_OFFSET + 0x00048078			
#define IPU_DI1_DW_GEN_9_OFFSET	IPU_REG_OFFSET + 0x0004807C			
#define IPU_DI1_DW_GEN_10_OFFSET	IPU_REG_OFFSET + 0x00048080			
#define IPU_DI1_DW_GEN_11_OFFSET	IPU_REG_OFFSET + 0x00048084			
#define IPU_DI1_DW_SET0_0_OFFSET	IPU_REG_OFFSET + 0x00048088			
#define IPU_DI1_DW_SET0_1_OFFSET	IPU_REG_OFFSET + 0x0004808C			
#define IPU_DI1_DW_SET0_2_OFFSET	IPU_REG_OFFSET + 0x00048090			
#define IPU_DI1_DW_SET0_3_OFFSET	IPU_REG_OFFSET + 0x00048094			
#define IPU_DI1_DW_SET0_4_OFFSET	IPU_REG_OFFSET + 0x00048098			
#define IPU_DI1_DW_SET0_5_OFFSET	IPU_REG_OFFSET + 0x0004809C			
#define IPU_DI1_DW_SET0_6_OFFSET	IPU_REG_OFFSET + 0x000480A0			
#define IPU_DI1_DW_SET0_7_OFFSET	IPU_REG_OFFSET + 0x000480A4			
#define IPU_DI1_DW_SET0_8_OFFSET	IPU_REG_OFFSET + 0x000480A8			
#define IPU_DI1_DW_SET0_9_OFFSET	IPU_REG_OFFSET + 0x000480AC			
#define IPU_DI1_DW_SET0_10_OFFSET	IPU_REG_OFFSET + 0x000480B0			
#define IPU_DI1_DW_SET0_11_OFFSET	IPU_REG_OFFSET + 0x000480B4			
#define IPU_DI1_DW_SET1_0_OFFSET	IPU_REG_OFFSET + 0x000480B8			
#define IPU_DI1_DW_SET1_1_OFFSET	IPU_REG_OFFSET + 0x000480BC			
#define IPU_DI1_DW_SET1_2_OFFSET	IPU_REG_OFFSET + 0x000480C0			
#define IPU_DI1_DW_SET1_3_OFFSET	IPU_REG_OFFSET + 0x000480C4			
#define IPU_DI1_DW_SET1_4_OFFSET	IPU_REG_OFFSET + 0x000480C8			
#define IPU_DI1_DW_SET1_5_OFFSET	IPU_REG_OFFSET + 0x000480CC			
#define IPU_DI1_DW_SET1_6_OFFSET	IPU_REG_OFFSET + 0x000480D0			
#define IPU_DI1_DW_SET1_7_OFFSET	IPU_REG_OFFSET + 0x000480D4			
#define IPU_DI1_DW_SET1_8_OFFSET	IPU_REG_OFFSET + 0x000480D8			
#define IPU_DI1_DW_SET1_9_OFFSET	IPU_REG_OFFSET + 0x000480DC			
#define IPU_DI1_DW_SET1_10_OFFSET	IPU_REG_OFFSET + 0x000480E0			
#define IPU_DI1_DW_SET1_11_OFFSET	IPU_REG_OFFSET + 0x000480E4			
#define IPU_DI1_DW_SET2_0_OFFSET	IPU_REG_OFFSET + 0x000480E8			
#define IPU_DI1_DW_SET2_1_OFFSET	IPU_REG_OFFSET + 0x000480EC			
#define IPU_DI1_DW_SET2_2_OFFSET	IPU_REG_OFFSET + 0x000480F0			
#define IPU_DI1_DW_SET2_3_OFFSET	IPU_REG_OFFSET + 0x000480F4			
#define IPU_DI1_DW_SET2_4_OFFSET	IPU_REG_OFFSET + 0x000480F8			
#define IPU_DI1_DW_SET2_5_OFFSET	IPU_REG_OFFSET + 0x000480FC			
#define IPU_DI1_DW_SET2_6_OFFSET	IPU_REG_OFFSET + 0x00048100			
#define IPU_DI1_DW_SET2_7_OFFSET	IPU_REG_OFFSET + 0x00048104			
#define IPU_DI1_DW_SET2_8_OFFSET	IPU_REG_OFFSET + 0x00048108			
#define IPU_DI1_DW_SET2_9_OFFSET	IPU_REG_OFFSET + 0x0004810C			
#define IPU_DI1_DW_SET2_10_OFFSET	IPU_REG_OFFSET + 0x00048110			
#define IPU_DI1_DW_SET2_11_OFFSET	IPU_REG_OFFSET + 0x00048114			
#define IPU_DI1_DW_SET3_0_OFFSET	IPU_REG_OFFSET + 0x00048118			
#define IPU_DI1_DW_SET3_1_OFFSET	IPU_REG_OFFSET + 0x0004811C			
#define IPU_DI1_DW_SET3_2_OFFSET	IPU_REG_OFFSET + 0x00048120			
#define IPU_DI1_DW_SET3_3_OFFSET	IPU_REG_OFFSET + 0x00048124			
#define IPU_DI1_DW_SET3_4_OFFSET	IPU_REG_OFFSET + 0x00048128			
#define IPU_DI1_DW_SET3_5_OFFSET	IPU_REG_OFFSET + 0x0004812C			
#define IPU_DI1_DW_SET3_6_OFFSET	IPU_REG_OFFSET + 0x00048130			
#define IPU_DI1_DW_SET3_7_OFFSET	IPU_REG_OFFSET + 0x00048134			
#define IPU_DI1_DW_SET3_8_OFFSET	IPU_REG_OFFSET + 0x00048138			
#define IPU_DI1_DW_SET3_9_OFFSET	IPU_REG_OFFSET + 0x0004813C			
#define IPU_DI1_DW_SET3_10_OFFSET	IPU_REG_OFFSET + 0x00048140			
#define IPU_DI1_DW_SET3_11_OFFSET	IPU_REG_OFFSET + 0x00048144			
#define IPU_DI1_STP_REP_1_OFFSET	IPU_REG_OFFSET + 0x00048148			
#define IPU_DI1_STP_REP_2_OFFSET	IPU_REG_OFFSET + 0x0004814C			
#define IPU_DI1_STP_REP_3_OFFSET	IPU_REG_OFFSET + 0x00048150			
#define IPU_DI1_STP_REP_4_OFFSET	IPU_REG_OFFSET + 0x00048154			
#define IPU_DI1_STP_REP_9_OFFSET	IPU_REG_OFFSET + 0x00048158			
#define IPU_DI1_SER_CONF_OFFSET	IPU_REG_OFFSET + 0x0004815C			
#define IPU_DI1_SSC_OFFSET	IPU_REG_OFFSET + 0x00048160			
#define IPU_DI1_POL_OFFSET	IPU_REG_OFFSET + 0x00048164			
#define IPU_DI1_AW0_OFFSET	IPU_REG_OFFSET + 0x00048168			
#define IPU_DI1_AW1_OFFSET	IPU_REG_OFFSET + 0x0004816C			
#define IPU_SMFC_MAP_OFFSET	IPU_REG_OFFSET + 0x00050000			
#define IPU_SMFC_WMC_OFFSET	IPU_REG_OFFSET + 0x00050004			
#define IPU_SMFC_BS_OFFSET	IPU_REG_OFFSET + 0x00050008			
#define IPU_DC_READ_CH_CONF_OFFSET	IPU_REG_OFFSET + 0x00058000			
#define IPU_DC_READ_CH_ADDR_OFFSET	IPU_REG_OFFSET + 0x00058004			
#define IPU_DC_RL0_CH_0_OFFSET	IPU_REG_OFFSET + 0x00058008			
#define IPU_DC_RL1_CH_0_OFFSET	IPU_REG_OFFSET + 0x0005800C			
#define IPU_DC_RL2_CH_0_OFFSET	IPU_REG_OFFSET + 0x00058010			
#define IPU_DC_RL3_CH_0_OFFSET	IPU_REG_OFFSET + 0x00058014			
#define IPU_DC_RL4_CH_0_OFFSET	IPU_REG_OFFSET + 0x00058018			
#define IPU_DC_WR_CH_CONF_1_OFFSET	IPU_REG_OFFSET + 0x0005801C			
#define IPU_DC_WR_CH_ADDR_1_OFFSET	IPU_REG_OFFSET + 0x00058020			
#define IPU_DC_RL0_CH_1_OFFSET	IPU_REG_OFFSET + 0x00058024			
#define IPU_DC_RL1_CH_1_OFFSET	IPU_REG_OFFSET + 0x00058028			
#define IPU_DC_RL2_CH_1_OFFSET	IPU_REG_OFFSET + 0x0005802C			
#define IPU_DC_RL3_CH_1_OFFSET	IPU_REG_OFFSET + 0x00058030			
#define IPU_DC_RL4_CH_1_OFFSET	IPU_REG_OFFSET + 0x00058034			
#define IPU_DC_WR_CH_CONF_2_OFFSET	IPU_REG_OFFSET + 0x00058038			
#define IPU_DC_WR_CH_ADDR_2_OFFSET	IPU_REG_OFFSET + 0x0005803C			
#define IPU_DC_RL0_CH_2_OFFSET	IPU_REG_OFFSET + 0x00058040			
#define IPU_DC_RL1_CH_2_OFFSET	IPU_REG_OFFSET + 0x00058044			
#define IPU_DC_RL2_CH_2_OFFSET	IPU_REG_OFFSET + 0x00058048			
#define IPU_DC_RL3_CH_2_OFFSET	IPU_REG_OFFSET + 0x0005804C			
#define IPU_DC_RL4_CH_2_OFFSET	IPU_REG_OFFSET + 0x00058050			
#define IPU_DC_CMD_CH_CONF_3_OFFSET	IPU_REG_OFFSET + 0x00058054			
#define IPU_DC_CMD_CH_CONF_4_OFFSET	IPU_REG_OFFSET + 0x00058058			
#define IPU_DC_WR_CH_CONF_5_OFFSET	IPU_REG_OFFSET + 0x0005805C			
#define IPU_DC_WR_CH_ADDR_5_OFFSET	IPU_REG_OFFSET + 0x00058060			
#define IPU_DC_RL0_CH_5_OFFSET	IPU_REG_OFFSET + 0x00058064			
#define IPU_DC_RL1_CH_5_OFFSET	IPU_REG_OFFSET + 0x00058068			
#define IPU_DC_RL2_CH_5_OFFSET	IPU_REG_OFFSET + 0x0005806C			
#define IPU_DC_RL3_CH_5_OFFSET	IPU_REG_OFFSET + 0x00058070			
#define IPU_DC_RL4_CH_5_OFFSET	IPU_REG_OFFSET + 0x00058074			
#define IPU_DC_WR_CH_CONF_6_OFFSET	IPU_REG_OFFSET + 0x00058078			
#define IPU_DC_WR_CH_ADDR_6_OFFSET	IPU_REG_OFFSET + 0x0005807C			
#define IPU_DC_RL0_CH_6_OFFSET	IPU_REG_OFFSET + 0x00058080			
#define IPU_DC_RL1_CH_6_OFFSET	IPU_REG_OFFSET + 0x00058084			
#define IPU_DC_RL2_CH_6_OFFSET	IPU_REG_OFFSET + 0x00058088			
#define IPU_DC_RL3_CH_6_OFFSET	IPU_REG_OFFSET + 0x0005808C			
#define IPU_DC_RL4_CH_6_OFFSET	IPU_REG_OFFSET + 0x00058090			
#define IPU_DC_WR_CH_CONF1_8_OFFSET	IPU_REG_OFFSET + 0x00058094			
#define IPU_DC_WR_CH_CONF2_8_OFFSET	IPU_REG_OFFSET + 0x00058098			
#define IPU_DC_RL1_CH_8_OFFSET	IPU_REG_OFFSET + 0x0005809C			
#define IPU_DC_RL2_CH_8_OFFSET	IPU_REG_OFFSET + 0x000580A0			
#define IPU_DC_RL3_CH_8_OFFSET	IPU_REG_OFFSET + 0x000580A4			
#define IPU_DC_RL4_CH_8_OFFSET	IPU_REG_OFFSET + 0x000580A8			
#define IPU_DC_RL5_CH_8_OFFSET	IPU_REG_OFFSET + 0x000580AC			
#define IPU_DC_RL6_CH_8_OFFSET	IPU_REG_OFFSET + 0x000580B0			
#define IPU_DC_WR_CH_CONF1_9_OFFSET	IPU_REG_OFFSET + 0x000580B4			
#define IPU_DC_WR_CH_CONF2_9_OFFSET	IPU_REG_OFFSET + 0x000580B8			
#define IPU_DC_RL1_CH_9_OFFSET	IPU_REG_OFFSET + 0x000580BC			
#define IPU_DC_RL2_CH_9_OFFSET	IPU_REG_OFFSET + 0x000580C0			
#define IPU_DC_RL3_CH_9_OFFSET	IPU_REG_OFFSET + 0x000580C4			
#define IPU_DC_RL4_CH_9_OFFSET	IPU_REG_OFFSET + 0x000580C8			
#define IPU_DC_RL5_CH_9_OFFSET	IPU_REG_OFFSET + 0x000580CC			
#define IPU_DC_RL6_CH_9_OFFSET	IPU_REG_OFFSET + 0x000580D0			
#define IPU_DC_GEN_OFFSET	IPU_REG_OFFSET + 0x000580D4			
#define IPU_DC_DISP_CONF1_0_OFFSET	IPU_REG_OFFSET + 0x000580D8			
#define IPU_DC_DISP_CONF1_1_OFFSET	IPU_REG_OFFSET + 0x000580DC			
#define IPU_DC_DISP_CONF1_2_OFFSET	IPU_REG_OFFSET + 0x000580E0			
#define IPU_DC_DISP_CONF1_3_OFFSET	IPU_REG_OFFSET + 0x000580E4			
#define IPU_DC_DISP_CONF2_0_OFFSET	IPU_REG_OFFSET + 0x000580E8			
#define IPU_DC_DISP_CONF2_1_OFFSET	IPU_REG_OFFSET + 0x000580EC			
#define IPU_DC_DISP_CONF2_2_OFFSET	IPU_REG_OFFSET + 0x000580F0			
#define IPU_DC_DISP_CONF2_3_OFFSET	IPU_REG_OFFSET + 0x000580F4			
#define IPU_DC_DI0_CONF_0_OFFSET	IPU_REG_OFFSET + 0x000580F8			
#define IPU_DC_DI0_CONF_1_OFFSET	IPU_REG_OFFSET + 0x000580FC			
#define IPU_DC_DI0_CONF_2_OFFSET	IPU_REG_OFFSET + 0x00058100			
#define IPU_DC_DI1_CONF_0_OFFSET	IPU_REG_OFFSET + 0x00058104			
#define IPU_DC_DI1_CONF_1_OFFSET	IPU_REG_OFFSET + 0x00058108			
#define IPU_DC_DI1_CONF_2_OFFSET	IPU_REG_OFFSET + 0x0005810C			
#define IPU_DC_MAP_CONF_0_OFFSET	IPU_REG_OFFSET + 0x00058110			
#define IPU_DC_MAP_CONF_1_OFFSET	IPU_REG_OFFSET + 0x00058114			
#define IPU_DC_MAP_CONF_2_OFFSET	IPU_REG_OFFSET + 0x00058118			
#define IPU_DC_MAP_CONF_3_OFFSET	IPU_REG_OFFSET + 0x0005811C			
#define IPU_DC_MAP_CONF_4_OFFSET	IPU_REG_OFFSET + 0x00058120			
#define IPU_DC_MAP_CONF_5_OFFSET	IPU_REG_OFFSET + 0x00058124			
#define IPU_DC_MAP_CONF_6_OFFSET	IPU_REG_OFFSET + 0x00058128			
#define IPU_DC_MAP_CONF_7_OFFSET	IPU_REG_OFFSET + 0x0005812C			
#define IPU_DC_MAP_CONF_8_OFFSET	IPU_REG_OFFSET + 0x00058130			
#define IPU_DC_MAP_CONF_9_OFFSET	IPU_REG_OFFSET + 0x00058134			
#define IPU_DC_MAP_CONF_10_OFFSET	IPU_REG_OFFSET + 0x00058138			
#define IPU_DC_MAP_CONF_11_OFFSET	IPU_REG_OFFSET + 0x0005813C			
#define IPU_DC_MAP_CONF_12_OFFSET	IPU_REG_OFFSET + 0x00058140			
#define IPU_DC_MAP_CONF_13_OFFSET	IPU_REG_OFFSET + 0x00058144			
#define IPU_DC_MAP_CONF_14_OFFSET	IPU_REG_OFFSET + 0x00058148			
#define IPU_DC_MAP_CONF_15_OFFSET	IPU_REG_OFFSET + 0x0005814C			
#define IPU_DC_MAP_CONF_16_OFFSET	IPU_REG_OFFSET + 0x00058150			
#define IPU_DC_MAP_CONF_17_OFFSET	IPU_REG_OFFSET + 0x00058154			
#define IPU_DC_MAP_CONF_18_OFFSET	IPU_REG_OFFSET + 0x00058158			
#define IPU_DC_MAP_CONF_19_OFFSET	IPU_REG_OFFSET + 0x0005815C			
#define IPU_DC_MAP_CONF_20_OFFSET	IPU_REG_OFFSET + 0x00058160			
#define IPU_DC_MAP_CONF_21_OFFSET	IPU_REG_OFFSET + 0x00058164			
#define IPU_DC_MAP_CONF_22_OFFSET	IPU_REG_OFFSET + 0x00058168			
#define IPU_DC_MAP_CONF_23_OFFSET	IPU_REG_OFFSET + 0x0005816C			
#define IPU_DC_MAP_CONF_24_OFFSET	IPU_REG_OFFSET + 0x00058170			
#define IPU_DC_MAP_CONF_25_OFFSET	IPU_REG_OFFSET + 0x00058174			
#define IPU_DC_MAP_CONF_26_OFFSET	IPU_REG_OFFSET + 0x00058178			
#define IPU_DC_UGDE0_0_OFFSET	IPU_REG_OFFSET + 0x0005817C			
#define IPU_DC_UGDE0_1_OFFSET	IPU_REG_OFFSET + 0x00058180			
#define IPU_DC_UGDE0_2_OFFSET	IPU_REG_OFFSET + 0x00058184			
#define IPU_DC_UGDE0_3_OFFSET	IPU_REG_OFFSET + 0x00058188			
#define IPU_DC_UGDE1_0_OFFSET	IPU_REG_OFFSET + 0x0005818C			
#define IPU_DC_UGDE1_1_OFFSET	IPU_REG_OFFSET + 0x00058190			
#define IPU_DC_UGDE1_2_OFFSET	IPU_REG_OFFSET + 0x00058194			
#define IPU_DC_UGDE1_3_OFFSET	IPU_REG_OFFSET + 0x00058198			
#define IPU_DC_UGDE2_0_OFFSET	IPU_REG_OFFSET + 0x0005819C			
#define IPU_DC_UGDE2_1_OFFSET	IPU_REG_OFFSET + 0x000581A0			
#define IPU_DC_UGDE2_2_OFFSET	IPU_REG_OFFSET + 0x000581A4			
#define IPU_DC_UGDE2_3_OFFSET	IPU_REG_OFFSET + 0x000581A8			
#define IPU_DC_UGDE3_0_OFFSET	IPU_REG_OFFSET + 0x000581AC			
#define IPU_DC_UGDE3_1_OFFSET	IPU_REG_OFFSET + 0x000581B0			
#define IPU_DC_UGDE3_2_OFFSET	IPU_REG_OFFSET + 0x000581B4			
#define IPU_DC_UGDE3_3_OFFSET	IPU_REG_OFFSET + 0x000581B8			
#define IPU_DC_LLA0_OFFSET	IPU_REG_OFFSET + 0x000581BC			
#define IPU_DC_LLA1_OFFSET	IPU_REG_OFFSET + 0x000581C0			
#define IPU_DC_WR_CH_ADDR_5_ALT_OFFSET	IPU_REG_OFFSET + 0x000581C4			
#define IPU_DMFC_RD_CHAN_OFFSET	IPU_REG_OFFSET + 0x00060000			
#define IPU_DMFC_WR_CHAN_OFFSET	IPU_REG_OFFSET + 0x00060004			
#define IPU_DMFC_WR_CHAN_DEF_OFFSET	IPU_REG_OFFSET + 0x00060008			
#define IPU_DMFC_DP_CHAN_OFFSET	IPU_REG_OFFSET + 0x0006000C			
#define IPU_DMFC_DP_CHAN_DEF_OFFSET	IPU_REG_OFFSET + 0x00060010			
#define IPU_DMFC_GENERAL1_OFFSET	IPU_REG_OFFSET + 0x00060014			
#define IPU_DMFC_GENERAL2_OFFSET	IPU_REG_OFFSET + 0x00060018			
#define IPU_DMFC_IC_CTRL_OFFSET	IPU_REG_OFFSET + 0x0006001C			
#define CPMEM_WORD0_DATA0_INT_OFFSET	IPU_REG_OFFSET + 0x01000000			
#define CPMEM_WORD0_DATA1_INT_OFFSET	IPU_REG_OFFSET + 0x01000004			
#define CPMEM_WORD0_DATA2_INT_OFFSET	IPU_REG_OFFSET + 0x01000008			
#define CPMEM_WORD0_DATA3_INT_OFFSET	IPU_REG_OFFSET + 0x0100000C			
#define CPMEM_WORD0_DATA4_INT_OFFSET	IPU_REG_OFFSET + 0x01000010			
#define CPMEM_WORD0_DATA0_N_INT_OFFSET	IPU_REG_OFFSET + 0x01000000			
#define CPMEM_WORD0_DATA1_N_INT_OFFSET	IPU_REG_OFFSET + 0x01000004			
#define CPMEM_WORD0_DATA2_N_INT_OFFSET	IPU_REG_OFFSET + 0x01000008			
#define CPMEM_WORD0_DATA3_N_INT_OFFSET	IPU_REG_OFFSET + 0x0100000C			
#define CPMEM_WORD0_DATA4_N_INT_OFFSET	IPU_REG_OFFSET + 0x01000010			
#define CPMEM_WORD1_DATA0_INT_OFFSET	IPU_REG_OFFSET + 0x01000020			
#define CPMEM_WORD1_DATA1_INT_OFFSET	IPU_REG_OFFSET + 0x01000024			
#define CPMEM_WORD1_DATA2_INT_OFFSET	IPU_REG_OFFSET + 0x01000028			
#define CPMEM_WORD1_DATA3_INT_OFFSET	IPU_REG_OFFSET + 0x0100002C			
#define CPMEM_WORD1_DATA4_INT_OFFSET	IPU_REG_OFFSET + 0x01000030			
#define CPMEM_WORD1_DATA0_N_INT_OFFSET	IPU_REG_OFFSET + 0x01000020			
#define CPMEM_WORD1_DATA1_N_INT_OFFSET	IPU_REG_OFFSET + 0x01000024			
#define CPMEM_WORD1_DATA2_N_INT_OFFSET	IPU_REG_OFFSET + 0x01000028			
#define CPMEM_WORD1_DATA3_N_INT_OFFSET	IPU_REG_OFFSET + 0x0100002C			
#define CPMEM_WORD1_DATA4_N_INT_OFFSET	IPU_REG_OFFSET + 0x01000030			
#define IPU_IC_TPMEM_ENC_CSC1_WORD0_OFFSET	IPU_REG_OFFSET + 0x01061688			
#define IPU_IC_TPMEM_ENC_CSC1_WORD1_OFFSET	IPU_REG_OFFSET + 0x0106168C			
#define IPU_IC_TPMEM_ENC_CSC1_WORD2_OFFSET	IPU_REG_OFFSET + 0x01061690			
#define IPU_IC_TPMEM_ENC_CSC1_WORD3_OFFSET	IPU_REG_OFFSET + 0x01061694			
#define IPU_IC_TPMEM_ENC_CSC1_WORD4_OFFSET	IPU_REG_OFFSET + 0x01061698			
#define IPU_IC_TPMEM_ENC_CSC1_WORD5_OFFSET	IPU_REG_OFFSET + 0x0106169c			
#define IPU_IC_TPMEM_VIEW_CSC1_WORD0_OFFSET	IPU_REG_OFFSET + 0x01062D28			
#define IPU_IC_TPMEM_VIEW_CSC1_WORD1_OFFSET	IPU_REG_OFFSET + 0x01062D2C			
#define IPU_IC_TPMEM_VIEW_CSC1_WORD2_OFFSET	IPU_REG_OFFSET + 0x01062D30			
#define IPU_IC_TPMEM_VIEW_CSC1_WORD3_OFFSET	IPU_REG_OFFSET + 0x01062D34			
#define IPU_IC_TPMEM_VIEW_CSC1_WORD4_OFFSET	IPU_REG_OFFSET + 0x01062D38			
#define IPU_IC_TPMEM_VIEW_CSC1_WORD5_OFFSET	IPU_REG_OFFSET + 0x01062D3C			
#define IPU_IC_TPMEM_VIEW_CSC2_WORD0_OFFSET	IPU_REG_OFFSET + 0x01062D40			
#define IPU_IC_TPMEM_VIEW_CSC2_WORD1_OFFSET	IPU_REG_OFFSET + 0x01062D44			
#define IPU_IC_TPMEM_VIEW_CSC2_WORD2_OFFSET	IPU_REG_OFFSET + 0x01062D48			
#define IPU_IC_TPMEM_VIEW_CSC2_WORD3_OFFSET	IPU_REG_OFFSET + 0x01062D4C			
#define IPU_IC_TPMEM_VIEW_CSC2_WORD4_OFFSET	IPU_REG_OFFSET + 0x01062D50			
#define IPU_IC_TPMEM_VIEW_CSC2_WORD5_OFFSET	IPU_REG_OFFSET + 0x01062D54			
#define IPU_IC_TPMEM_POST_CSC1_WORD0_OFFSET	IPU_REG_OFFSET + 0x010643E0			
#define IPU_IC_TPMEM_POST_CSC1_WORD1_OFFSET	IPU_REG_OFFSET + 0x010643E4			
#define IPU_IC_TPMEM_POST_CSC1_WORD2_OFFSET	IPU_REG_OFFSET + 0x010643E8			
#define IPU_IC_TPMEM_POST_CSC1_WORD3_OFFSET	IPU_REG_OFFSET + 0x010643EC			
#define IPU_IC_TPMEM_POST_CSC1_WORD4_OFFSET	IPU_REG_OFFSET + 0x010643F0			
#define IPU_IC_TPMEM_POST_CSC1_WORD5_OFFSET	IPU_REG_OFFSET + 0x010643F4			
#define IPU_IC_TPMEM_POST_CSC2_WORD0_OFFSET	IPU_REG_OFFSET + 0x010643F8			
#define IPU_IC_TPMEM_POST_CSC2_WORD1_OFFSET	IPU_REG_OFFSET + 0x010643FC			
#define IPU_IC_TPMEM_POST_CSC2_WORD2_OFFSET	IPU_REG_OFFSET + 0x01064400			
#define IPU_IC_TPMEM_POST_CSC2_WORD3_OFFSET	IPU_REG_OFFSET + 0x01064404			
#define IPU_IC_TPMEM_POST_CSC2_WORD4_OFFSET	IPU_REG_OFFSET + 0x01064408			
#define IPU_IC_TPMEM_POST_CSC2_WORD5_OFFSET	IPU_REG_OFFSET + 0x0106440C			
#define SRM_DP_COM_CONF_SYNC_OFFSET	IPU_REG_OFFSET + 0x01040000			
#define SRM_DP_GRAPH_WIND_CTRL_SYNC_OFFSET	IPU_REG_OFFSET + 0x01040004			
#define SRM_DP_FG_POS_SYNC_OFFSET	IPU_REG_OFFSET + 0x01040008			
#define SRM_DP_CUR_POS_SYNC_OFFSET	IPU_REG_OFFSET + 0x0104000C			
#define SRM_DP_CUR_MAP_SYNC_OFFSET	IPU_REG_OFFSET + 0x01040010			
#define SRM_DP_GAMMA_C_SYNC_0_OFFSET	IPU_REG_OFFSET + 0x01040014			
#define SRM_DP_GAMMA_C_SYNC_1_OFFSET	IPU_REG_OFFSET + 0x01040018			
#define SRM_DP_GAMMA_C_SYNC_2_OFFSET	IPU_REG_OFFSET + 0x0104001C			
#define SRM_DP_GAMMA_C_SYNC_3_OFFSET	IPU_REG_OFFSET + 0x01040020			
#define SRM_DP_GAMMA_C_SYNC_4_OFFSET	IPU_REG_OFFSET + 0x01040024			
#define SRM_DP_GAMMA_C_SYNC_5_OFFSET	IPU_REG_OFFSET + 0x01040028			
#define SRM_DP_GAMMA_C_SYNC_6_OFFSET	IPU_REG_OFFSET + 0x0104002C			
#define SRM_DP_GAMMA_C_SYNC_7_OFFSET	IPU_REG_OFFSET + 0x01040030			
#define SRM_DP_GAMMA_S_SYNC_0_OFFSET	IPU_REG_OFFSET + 0x01040034			
#define SRM_DP_GAMMA_S_SYNC_1_OFFSET	IPU_REG_OFFSET + 0x01040038			
#define SRM_DP_GAMMA_S_SYNC_2_OFFSET	IPU_REG_OFFSET + 0x0104003C			
#define SRM_DP_GAMMA_S_SYNC_3_OFFSET	IPU_REG_OFFSET + 0x01040040			
#define SRM_DP_CSCA_SYNC_0_OFFSET	IPU_REG_OFFSET + 0x01040044			
#define SRM_DP_CSCA_SYNC_1_OFFSET	IPU_REG_OFFSET + 0x01040048			
#define SRM_DP_CSCA_SYNC_2_OFFSET	IPU_REG_OFFSET + 0x0104004C			
#define SRM_DP_CSCA_SYNC_3_OFFSET	IPU_REG_OFFSET + 0x01040050			
#define SRM_DP_CSC_SYNC_0_OFFSET	IPU_REG_OFFSET + 0x01040054			
#define SRM_DP_CSC_SYNC_1_OFFSET	IPU_REG_OFFSET + 0x01040058			
#define SRM_DP_COM_CONF_ASYNC0_OFFSET	IPU_REG_OFFSET + 0x0104005C			
#define SRM_DP_GRAPH_WIND_CTRL_ASYNC0_OFFSET	IPU_REG_OFFSET + 0x01040060			
#define SRM_DP_FG_POS_ASYNC0_OFFSET	IPU_REG_OFFSET + 0x01040064			
#define SRM_DP_CUR_POS_ASYNC0_OFFSET	IPU_REG_OFFSET + 0x01040068			
#define SRM_DP_CUR_MAP_ASYNC0_OFFSET	IPU_REG_OFFSET + 0x0104006C			
#define SRM_DP_GAMMA_C_ASYNC0_0_OFFSET	IPU_REG_OFFSET + 0x01040070			
#define SRM_DP_GAMMA_C_ASYNC0_1_OFFSET	IPU_REG_OFFSET + 0x01040074			
#define SRM_DP_GAMMA_C_ASYNC0_2_OFFSET	IPU_REG_OFFSET + 0x01040078			
#define SRM_DP_GAMMA_C_ASYNC0_3_OFFSET	IPU_REG_OFFSET + 0x0104007C			
#define SRM_DP_GAMMA_C_ASYNC0_4_OFFSET	IPU_REG_OFFSET + 0x01040080			
#define SRM_DP_GAMMA_C_ASYNC0_5_OFFSET	IPU_REG_OFFSET + 0x01040084			
#define SRM_DP_GAMMA_C_ASYNC0_6_OFFSET	IPU_REG_OFFSET + 0x01040088			
#define SRM_DP_GAMMA_C_ASYNC0_7_OFFSET	IPU_REG_OFFSET + 0x0104008C			
#define SRM_DP_GAMMA_S_ASYNC0_0_OFFSET	IPU_REG_OFFSET + 0x01040090			
#define SRM_DP_GAMMA_S_ASYNC0_1_OFFSET	IPU_REG_OFFSET + 0x01040094			
#define SRM_DP_GAMMA_S_ASYNC0_2_OFFSET	IPU_REG_OFFSET + 0x01040098			
#define SRM_DP_GAMMA_S_ASYNC0_3_OFFSET	IPU_REG_OFFSET + 0x0104009C			
#define SRM_DP_CSCA_ASYNC0_0_OFFSET	IPU_REG_OFFSET + 0x010400A0			
#define SRM_DP_CSCA_ASYNC0_1_OFFSET	IPU_REG_OFFSET + 0x010400A4			
#define SRM_DP_CSCA_ASYNC0_2_OFFSET	IPU_REG_OFFSET + 0x010400A8			
#define SRM_DP_CSCA_ASYNC0_3_OFFSET	IPU_REG_OFFSET + 0x010400AC			
#define SRM_DP_CSC_ASYNC0_0_OFFSET	IPU_REG_OFFSET + 0x010400B0			
#define SRM_DP_CSC_ASYNC0_1_OFFSET	IPU_REG_OFFSET + 0x010400B4			
#define SRM_DP_COM_CONF_ASYNC1_OFFSET	IPU_REG_OFFSET + 0x010400B8			
#define SRM_DP_GRAPH_WIND_CTRL_ASYNC1_OFFSET	IPU_REG_OFFSET + 0x010400BC			
#define SRM_DP_FG_POS_ASYNC1_OFFSET	IPU_REG_OFFSET + 0x010400C0			
#define SRM_DP_CUR_POS_ASYNC1_OFFSET	IPU_REG_OFFSET + 0x010400C4			
#define SRM_DP_CUR_MAP_ASYNC1_OFFSET	IPU_REG_OFFSET + 0x010400C8			
#define SRM_DP_GAMMA_C_ASYNC1_0_OFFSET	IPU_REG_OFFSET + 0x010400CC			
#define SRM_DP_GAMMA_C_ASYNC1_1_OFFSET	IPU_REG_OFFSET + 0x010400D0			
#define SRM_DP_GAMMA_C_ASYNC1_2_OFFSET	IPU_REG_OFFSET + 0x010400D4			
#define SRM_DP_GAMMA_C_ASYNC1_3_OFFSET	IPU_REG_OFFSET + 0x010400D8			
#define SRM_DP_GAMMA_C_ASYNC1_4_OFFSET	IPU_REG_OFFSET + 0x010400DC			
#define SRM_DP_GAMMA_C_ASYNC1_5_OFFSET	IPU_REG_OFFSET + 0x010400E0			
#define SRM_DP_GAMMA_C_ASYNC1_6_OFFSET	IPU_REG_OFFSET + 0x010400E4			
#define SRM_DP_GAMMA_C_ASYNC1_7_OFFSET	IPU_REG_OFFSET + 0x010400E8			
#define SRM_DP_GAMMA_S_ASYNC1_0_OFFSET	IPU_REG_OFFSET + 0x010400EC			
#define SRM_DP_GAMMA_S_ASYNC1_1_OFFSET	IPU_REG_OFFSET + 0x010400F0			
#define SRM_DP_GAMMA_S_ASYNC1_2_OFFSET	IPU_REG_OFFSET + 0x010400F4			
#define SRM_DP_GAMMA_S_ASYNC1_3_OFFSET	IPU_REG_OFFSET + 0x010400F8			
#define SRM_DP_CSCA_ASYNC1_0_OFFSET	IPU_REG_OFFSET + 0x010400FC			
#define SRM_DP_CSCA_ASYNC1_1_OFFSET	IPU_REG_OFFSET + 0x01040100			
#define SRM_DP_CSCA_ASYNC1_2_OFFSET	IPU_REG_OFFSET + 0x01040104			
#define SRM_DP_CSCA_ASYNC1_3_OFFSET	IPU_REG_OFFSET + 0x01040108			
#define SRM_DP_CSC_ASYNC1_0_OFFSET	IPU_REG_OFFSET + 0x0104010C			
#define SRM_DP_CSC_ASYNC1_1_OFFSET	IPU_REG_OFFSET + 0x01040110			
#define SRM_ISP_C0_OFFSET	IPU_REG_OFFSET + 0x01040114			
#define SRM_ISP_C1_OFFSET	IPU_REG_OFFSET + 0x01040118			
#define SRM_ISP_FS_OFFSET	IPU_REG_OFFSET + 0x0104011C			
#define SRM_ISP_BI_OFFSET	IPU_REG_OFFSET + 0x01040120			
#define SRM_ISP_OCO_OFFSET	IPU_REG_OFFSET + 0x01040128			
#define SRM_ISP_BPR1_OFFSET	IPU_REG_OFFSET + 0x01040128			
#define SRM_ISP_BPR2_OFFSET	IPU_REG_OFFSET + 0x0104012C			
#define SRM_ISP_BPR3_OFFSET	IPU_REG_OFFSET + 0x01040130			
#define SRM_ISP_CG_0_OFFSET	IPU_REG_OFFSET + 0x01040134			
#define SRM_ISP_CG_1_OFFSET	IPU_REG_OFFSET + 0x01040138			
#define SRM_ISP_ROC_0_OFFSET	IPU_REG_OFFSET + 0x0104013C			
#define SRM_ISP_ROC_1_OFFSET	IPU_REG_OFFSET + 0x01040140			
#define SRM_ISP_ROC_2_OFFSET	IPU_REG_OFFSET + 0x01040144			
#define SRM_ISP_RRO_0_OFFSET	IPU_REG_OFFSET + 0x01040148			
#define SRM_ISP_RRO_1_OFFSET	IPU_REG_OFFSET + 0x0104014C			
#define SRM_ISP_RRO_2_OFFSET	IPU_REG_OFFSET + 0x01040150			
#define SRM_ISP_RRO_3_OFFSET	IPU_REG_OFFSET + 0x01040154			
#define SRM_ISP_RRO_4_OFFSET	IPU_REG_OFFSET + 0x01040158			
#define SRM_ISP_RRO_5_OFFSET	IPU_REG_OFFSET + 0x0104015C			
#define SRM_ISP_RRO_6_OFFSET	IPU_REG_OFFSET + 0x01040160			
#define SRM_ISP_RRO_7_OFFSET	IPU_REG_OFFSET + 0x01040164			
#define SRM_ISP_GRO_0_OFFSET	IPU_REG_OFFSET + 0x01040168			
#define SRM_ISP_GRO_1_OFFSET	IPU_REG_OFFSET + 0x0104016C			
#define SRM_ISP_GRO_2_OFFSET	IPU_REG_OFFSET + 0x01040170			
#define SRM_ISP_GRO_3_OFFSET	IPU_REG_OFFSET + 0x01040174			
#define SRM_ISP_GRO_4_OFFSET	IPU_REG_OFFSET + 0x01040178			
#define SRM_ISP_GRO_5_OFFSET	IPU_REG_OFFSET + 0x0104017C			
#define SRM_ISP_GRO_6_OFFSET	IPU_REG_OFFSET + 0x01040180			
#define SRM_ISP_GRO_7_OFFSET	IPU_REG_OFFSET + 0x01040184			
#define SRM_ISP_BRO_0_OFFSET	IPU_REG_OFFSET + 0x01040188			
#define SRM_ISP_BRO_1_OFFSET	IPU_REG_OFFSET + 0x0104018C			
#define SRM_ISP_BRO_2_OFFSET	IPU_REG_OFFSET + 0x01040190			
#define SRM_ISP_BRO_3_OFFSET	IPU_REG_OFFSET + 0x01040194			
#define SRM_ISP_BRO_4_OFFSET	IPU_REG_OFFSET + 0x01040198			
#define SRM_ISP_BRO_5_OFFSET	IPU_REG_OFFSET + 0x0104019C			
#define SRM_ISP_BRO_6_OFFSET	IPU_REG_OFFSET + 0x010401A0			
#define SRM_ISP_BRO_7_OFFSET	IPU_REG_OFFSET + 0x010401A4			
#define SRM_ISP_GAMMA_C_0_OFFSET	IPU_REG_OFFSET + 0x010401A8			
#define SRM_ISP_GAMMA_C_1_OFFSET	IPU_REG_OFFSET + 0x010401AC			
#define SRM_ISP_GAMMA_C_2_OFFSET	IPU_REG_OFFSET + 0x010401B0			
#define SRM_ISP_GAMMA_C_3_OFFSET	IPU_REG_OFFSET + 0x010401B4			
#define SRM_ISP_GAMMA_C_4_OFFSET	IPU_REG_OFFSET + 0x010401B8			
#define SRM_ISP_GAMMA_C_5_OFFSET	IPU_REG_OFFSET + 0x010401BC			
#define SRM_ISP_GAMMA_C_6_OFFSET	IPU_REG_OFFSET + 0x010401C0			
#define SRM_ISP_GAMMA_C_7_OFFSET	IPU_REG_OFFSET + 0x010401C4			
#define SRM_ISP_GAMMA_S_0_OFFSET	IPU_REG_OFFSET + 0x010401C8			
#define SRM_ISP_GAMMA_S_1_OFFSET	IPU_REG_OFFSET + 0x010401CC			
#define SRM_ISP_GAMMA_S_2_OFFSET	IPU_REG_OFFSET + 0x010401D0			
#define SRM_ISP_GAMMA_S_3_OFFSET	IPU_REG_OFFSET + 0x010401D4			
#define SRM_ISP_CSCA_0_OFFSET	IPU_REG_OFFSET + 0x010401D8			
#define SRM_ISP_CSCA_1_OFFSET	IPU_REG_OFFSET + 0x010401DC			
#define SRM_ISP_CSCA_2_OFFSET	IPU_REG_OFFSET + 0x010401E0			
#define SRM_ISP_CSCA_3_OFFSET	IPU_REG_OFFSET + 0x010401E4			
#define SRM_ISP_CSC_0_OFFSET	IPU_REG_OFFSET + 0x010401E8			
#define SRM_ISP_CSC_1_OFFSET	IPU_REG_OFFSET + 0x010401EC			
#define SRM_ISP_CNS_C_0_OFFSET	IPU_REG_OFFSET + 0x010401F0			
#define SRM_ISP_CNS_C_1_OFFSET	IPU_REG_OFFSET + 0x010401F4			
#define SRM_ISP_CNS_C_2_OFFSET	IPU_REG_OFFSET + 0x010401F8			
#define SRM_ISP_CNS_C_3_OFFSET	IPU_REG_OFFSET + 0x010401FC			
#define SRM_ISP_CNS_C_4_OFFSET	IPU_REG_OFFSET + 0x01040200			
#define SRM_ISP_CNS_C_5_OFFSET	IPU_REG_OFFSET + 0x01040204			
#define SRM_ISP_CNS_C_6_OFFSET	IPU_REG_OFFSET + 0x01040208			
#define SRM_ISP_CNS_C_7_OFFSET	IPU_REG_OFFSET + 0x0104020C			
#define SRM_ISP_CNS_S_0_OFFSET	IPU_REG_OFFSET + 0x01040210			
#define SRM_ISP_CNS_S_1_OFFSET	IPU_REG_OFFSET + 0x01040214			
#define SRM_ISP_CNS_S_2_OFFSET	IPU_REG_OFFSET + 0x01040218			
#define SRM_ISP_CNS_S_3_OFFSET	IPU_REG_OFFSET + 0x0104021C			
#define SRM_ISP_MTF_ROC_C_0_OFFSET	IPU_REG_OFFSET + 0x01040220			
#define SRM_ISP_MTF_ROC_C_1_OFFSET	IPU_REG_OFFSET + 0x01040224			
#define SRM_ISP_MTF_ROC_C_2_OFFSET	IPU_REG_OFFSET + 0x01040228			
#define SRM_ISP_MTF_ROC_C_3_OFFSET	IPU_REG_OFFSET + 0x0104022C			
#define SRM_ISP_MTF_ROC_S_0_OFFSET	IPU_REG_OFFSET + 0x01040230			
#define SRM_ISP_MTF_ROC_S_1_OFFSET	IPU_REG_OFFSET + 0x01040234			
#define SRM_ISP_HFE_0_OFFSET	IPU_REG_OFFSET + 0x01040238			
#define SRM_ISP_HFE_1_OFFSET	IPU_REG_OFFSET + 0x0104023C			
#define SRM_ISP_HFE_2_OFFSET	IPU_REG_OFFSET + 0x01040240			
#define SRM_ISP_HFE_S_0_OFFSET	IPU_REG_OFFSET + 0x01040244			
#define SRM_ISP_HFE_S_1_OFFSET	IPU_REG_OFFSET + 0x01040248			
#define SRM_ISP_HFE_S_2_OFFSET	IPU_REG_OFFSET + 0x0104024C			
#define SRM_ISP_HFE_S_3_OFFSET	IPU_REG_OFFSET + 0x01040250			
#define SRM_ISP_HFE_C_0_OFFSET	IPU_REG_OFFSET + 0x01040254			
#define SRM_ISP_HFE_C_1_OFFSET	IPU_REG_OFFSET + 0x01040258			
#define SRM_ISP_HFE_C_2_OFFSET	IPU_REG_OFFSET + 0x0104025C			
#define SRM_ISP_HFE_C_3_OFFSET	IPU_REG_OFFSET + 0x01040260			
#define SRM_ISP_STC_0_OFFSET	IPU_REG_OFFSET + 0x01040264			
#define SRM_ISP_STC_1_OFFSET	IPU_REG_OFFSET + 0x01040268			
#define SRM_ISP_FC_0_OFFSET	IPU_REG_OFFSET + 0x0104026C			
#define SRM_ISP_FC_1_OFFSET	IPU_REG_OFFSET + 0x01040270			
#define SRM_CSI0_CPD_CTRL_OFFSET	IPU_REG_OFFSET + 0x01040274			
#define SRM_CSI0_CPD_RC_0_OFFSET	IPU_REG_OFFSET + 0x01040278			
#define SRM_CSI0_CPD_RC_1_OFFSET	IPU_REG_OFFSET + 0x0104027C			
#define SRM_CSI0_CPD_RC_2_OFFSET	IPU_REG_OFFSET + 0x01040280			
#define SRM_CSI0_CPD_RC_3_OFFSET	IPU_REG_OFFSET + 0x01040284			
#define SRM_CSI0_CPD_RC_4_OFFSET	IPU_REG_OFFSET + 0x01040288			
#define SRM_CSI0_CPD_RC_5_OFFSET	IPU_REG_OFFSET + 0x0104028C			
#define SRM_CSI0_CPD_RC_6_OFFSET	IPU_REG_OFFSET + 0x01040290			
#define SRM_CSI0_CPD_RC_7_OFFSET	IPU_REG_OFFSET + 0x01040294			
#define SRM_CSI0_CPD_RS_0_OFFSET	IPU_REG_OFFSET + 0x01040298			
#define SRM_CSI0_CPD_RS_1_OFFSET	IPU_REG_OFFSET + 0x0104029C			
#define SRM_CSI0_CPD_RS_2_OFFSET	IPU_REG_OFFSET + 0x010402A0			
#define SRM_CSI0_CPD_RS_3_OFFSET	IPU_REG_OFFSET + 0x010402A4			
#define SRM_CSI0_CPD_GRC_0_OFFSET	IPU_REG_OFFSET + 0x010402A8			
#define SRM_CSI0_CPD_GRC_1_OFFSET	IPU_REG_OFFSET + 0x010402AC			
#define SRM_CSI0_CPD_GRC_2_OFFSET	IPU_REG_OFFSET + 0x010402B0			
#define SRM_CSI0_CPD_GRC_3_OFFSET	IPU_REG_OFFSET + 0x010402B4			
#define SRM_CSI0_CPD_GRC_4_OFFSET	IPU_REG_OFFSET + 0x010402B8			
#define SRM_CSI0_CPD_GRC_5_OFFSET	IPU_REG_OFFSET + 0x010402BC			
#define SRM_CSI0_CPD_GRC_6_OFFSET	IPU_REG_OFFSET + 0x010402C0			
#define SRM_CSI0_CPD_GRC_7_OFFSET	IPU_REG_OFFSET + 0x010402C4			
#define SRM_CSI0_CPD_GRS_0_OFFSET	IPU_REG_OFFSET + 0x010402C8			
#define SRM_CSI0_CPD_GRS_1_OFFSET	IPU_REG_OFFSET + 0x010402CC			
#define SRM_CSI0_CPD_GRS_2_OFFSET	IPU_REG_OFFSET + 0x010402D0			
#define SRM_CSI0_CPD_GRS_3_OFFSET	IPU_REG_OFFSET + 0x010402D4			
#define SRM_CSI0_CPD_GBC_0_OFFSET	IPU_REG_OFFSET + 0x010402D8			
#define SRM_CSI0_CPD_GBC_1_OFFSET	IPU_REG_OFFSET + 0x010402DC			
#define SRM_CSI0_CPD_GBC_2_OFFSET	IPU_REG_OFFSET + 0x010402E0			
#define SRM_CSI0_CPD_GBC_3_OFFSET	IPU_REG_OFFSET + 0x010402E4			
#define SRM_CSI0_CPD_GBC_4_OFFSET	IPU_REG_OFFSET + 0x010402E8			
#define SRM_CSI0_CPD_GBC_5_OFFSET	IPU_REG_OFFSET + 0x010402EC			
#define SRM_CSI0_CPD_GBC_6_OFFSET	IPU_REG_OFFSET + 0x010402F0			
#define SRM_CSI0_CPD_GBC_7_OFFSET	IPU_REG_OFFSET + 0x010402F4			
#define SRM_CSI0_CPD_GBS_0_OFFSET	IPU_REG_OFFSET + 0x010402F8			
#define SRM_CSI0_CPD_GBS_1_OFFSET	IPU_REG_OFFSET + 0x010402FC			
#define SRM_CSI0_CPD_GBS_2_OFFSET	IPU_REG_OFFSET + 0x01040300			
#define SRM_CSI0_CPD_GBS_3_OFFSET	IPU_REG_OFFSET + 0x01040304			
#define SRM_CSI0_CPD_BC_0_OFFSET	IPU_REG_OFFSET + 0x01040308			
#define SRM_CSI0_CPD_BC_1_OFFSET	IPU_REG_OFFSET + 0x0104030C			
#define SRM_CSI0_CPD_BC_2_OFFSET	IPU_REG_OFFSET + 0x01040310			
#define SRM_CSI0_CPD_BC_3_OFFSET	IPU_REG_OFFSET + 0x01040314			
#define SRM_CSI0_CPD_BC_4_OFFSET	IPU_REG_OFFSET + 0x01040318			
#define SRM_CSI0_CPD_BC_5_OFFSET	IPU_REG_OFFSET + 0x0104031C			
#define SRM_CSI0_CPD_BC_6_OFFSET	IPU_REG_OFFSET + 0x01040320			
#define SRM_CSI0_CPD_BC_7_OFFSET	IPU_REG_OFFSET + 0x01040324			
#define SRM_CSI0_CPD_BS_0_OFFSET	IPU_REG_OFFSET + 0x01040328			
#define SRM_CSI0_CPD_BS_1_OFFSET	IPU_REG_OFFSET + 0x0104032C			
#define SRM_CSI0_CPD_BS_2_OFFSET	IPU_REG_OFFSET + 0x01040330			
#define SRM_CSI0_CPD_BS_3_OFFSET	IPU_REG_OFFSET + 0x01040334			
#define SRM_CSI0_CPD_OFFSET1_OFFSET	IPU_REG_OFFSET + 0x01040338			
#define SRM_CSI0_CPD_OFFSET2_OFFSET	IPU_REG_OFFSET + 0x0104033C			
#define SRM_CSI1_CPD_CTRL_OFFSET	IPU_REG_OFFSET + 0x01040340			
#define SRM_CSI1_CPD_RC_0_OFFSET	IPU_REG_OFFSET + 0x01040344			
#define SRM_CSI1_CPD_RC_1_OFFSET	IPU_REG_OFFSET + 0x01040348			
#define SRM_CSI1_CPD_RC_2_OFFSET	IPU_REG_OFFSET + 0x0104034C			
#define SRM_CSI1_CPD_RC_3_OFFSET	IPU_REG_OFFSET + 0x01040350			
#define SRM_CSI1_CPD_RC_4_OFFSET	IPU_REG_OFFSET + 0x01040354			
#define SRM_CSI1_CPD_RC_5_OFFSET	IPU_REG_OFFSET + 0x01040358			
#define SRM_CSI1_CPD_RC_6_OFFSET	IPU_REG_OFFSET + 0x0104035C			
#define SRM_CSI1_CPD_RC_7_OFFSET	IPU_REG_OFFSET + 0x01040360			
#define SRM_CSI1_CPD_RS_0_OFFSET	IPU_REG_OFFSET + 0x01040364			
#define SRM_CSI1_CPD_RS_1_OFFSET	IPU_REG_OFFSET + 0x01040368			
#define SRM_CSI1_CPD_RS_2_OFFSET	IPU_REG_OFFSET + 0x0104036C			
#define SRM_CSI1_CPD_RS_3_OFFSET	IPU_REG_OFFSET + 0x01040370			
#define SRM_CSI1_CPD_GRC_0_OFFSET	IPU_REG_OFFSET + 0x01040374			
#define SRM_CSI1_CPD_GRC_1_OFFSET	IPU_REG_OFFSET + 0x01040378			
#define SRM_CSI1_CPD_GRC_2_OFFSET	IPU_REG_OFFSET + 0x0104037C			
#define SRM_CSI1_CPD_GRC_3_OFFSET	IPU_REG_OFFSET + 0x01040380			
#define SRM_CSI1_CPD_GRC_4_OFFSET	IPU_REG_OFFSET + 0x01040384			
#define SRM_CSI1_CPD_GRC_5_OFFSET	IPU_REG_OFFSET + 0x01040388			
#define SRM_CSI1_CPD_GRC_6_OFFSET	IPU_REG_OFFSET + 0x0104038C			
#define SRM_CSI1_CPD_GRC_7_OFFSET	IPU_REG_OFFSET + 0x01040390			
#define SRM_CSI1_CPD_GRS_0_OFFSET	IPU_REG_OFFSET + 0x01040394			
#define SRM_CSI1_CPD_GRS_1_OFFSET	IPU_REG_OFFSET + 0x01040398			
#define SRM_CSI1_CPD_GRS_2_OFFSET	IPU_REG_OFFSET + 0x0104039C			
#define SRM_CSI1_CPD_GRS_3_OFFSET	IPU_REG_OFFSET + 0x010403A0			
#define SRM_CSI1_CPD_GBC_0_OFFSET	IPU_REG_OFFSET + 0x010403A4			
#define SRM_CSI1_CPD_GBC_1_OFFSET	IPU_REG_OFFSET + 0x010403A8			
#define SRM_CSI1_CPD_GBC_2_OFFSET	IPU_REG_OFFSET + 0x010403AC			
#define SRM_CSI1_CPD_GBC_3_OFFSET	IPU_REG_OFFSET + 0x010403B0			
#define SRM_CSI1_CPD_GBC_4_OFFSET	IPU_REG_OFFSET + 0x010403B4			
#define SRM_CSI1_CPD_GBC_5_OFFSET	IPU_REG_OFFSET + 0x010403B8			
#define SRM_CSI1_CPD_GBC_6_OFFSET	IPU_REG_OFFSET + 0x010403BC			
#define SRM_CSI1_CPD_GBC_7_OFFSET	IPU_REG_OFFSET + 0x010403C0			
#define SRM_CSI1_CPD_GBS_0_OFFSET	IPU_REG_OFFSET + 0x010403C4			
#define SRM_CSI1_CPD_GBS_1_OFFSET	IPU_REG_OFFSET + 0x010403C8			
#define SRM_CSI1_CPD_GBS_2_OFFSET	IPU_REG_OFFSET + 0x010403CC			
#define SRM_CSI1_CPD_GBS_3_OFFSET	IPU_REG_OFFSET + 0x010403D0			
#define SRM_CSI1_CPD_BC_0_OFFSET	IPU_REG_OFFSET + 0x010403D4			
#define SRM_CSI1_CPD_BC_1_OFFSET	IPU_REG_OFFSET + 0x010403D8			
#define SRM_CSI1_CPD_BC_2_OFFSET	IPU_REG_OFFSET + 0x010403DC			
#define SRM_CSI1_CPD_BC_3_OFFSET	IPU_REG_OFFSET + 0x010403E0			
#define SRM_CSI1_CPD_BC_4_OFFSET	IPU_REG_OFFSET + 0x010403E4			
#define SRM_CSI1_CPD_BC_5_OFFSET	IPU_REG_OFFSET + 0x010403E8			
#define SRM_CSI1_CPD_BC_6_OFFSET	IPU_REG_OFFSET + 0x010403EC			
#define SRM_CSI1_CPD_BC_7_OFFSET	IPU_REG_OFFSET + 0x010403F0			
#define SRM_CSI1_CPD_BS_0_OFFSET	IPU_REG_OFFSET + 0x010403F4			
#define SRM_CSI1_CPD_BS_1_OFFSET	IPU_REG_OFFSET + 0x010403F8			
#define SRM_CSI1_CPD_BS_2_OFFSET	IPU_REG_OFFSET + 0x010403FC			
#define SRM_CSI1_CPD_BS_3_OFFSET	IPU_REG_OFFSET + 0x01040400			
#define SRM_CSI1_CPD_OFFSET1_OFFSET	IPU_REG_OFFSET + 0x01040404			
#define SRM_CSI1_CPD_OFFSET2_OFFSET	IPU_REG_OFFSET + 0x01040408			
#define SRM_DI0_GENERAL_OFFSET	IPU_REG_OFFSET + 0x01040444			
#define SRM_DI0_BS_CLKGEN0_OFFSET	IPU_REG_OFFSET + 0x01040448			
#define SRM_DI0_BS_CLKGEN1_OFFSET	IPU_REG_OFFSET + 0x0104044C			
#define SRM_DI0_SW_GEN0_1_OFFSET	IPU_REG_OFFSET + 0x01040450			
#define SRM_DI0_SW_GEN0_2_OFFSET	IPU_REG_OFFSET + 0x01040454			
#define SRM_DI0_SW_GEN0_3_OFFSET	IPU_REG_OFFSET + 0x01040458			
#define SRM_DI0_SW_GEN0_4_OFFSET	IPU_REG_OFFSET + 0x0104045C			
#define SRM_DI0_SW_GEN0_5_OFFSET	IPU_REG_OFFSET + 0x01040460			
#define SRM_DI0_SW_GEN0_6_OFFSET	IPU_REG_OFFSET + 0x01040464			
#define SRM_DI0_SW_GEN0_7_OFFSET	IPU_REG_OFFSET + 0x01040468			
#define SRM_DI0_SW_GEN0_8_OFFSET	IPU_REG_OFFSET + 0x0104046C			
#define SRM_DI0_SW_GEN0_9_OFFSET	IPU_REG_OFFSET + 0x01040470			
#define SRM_DI0_SW_GEN1_1_OFFSET	IPU_REG_OFFSET + 0x01040474			
#define SRM_DI0_SW_GEN1_2_OFFSET	IPU_REG_OFFSET + 0x01040478			
#define SRM_DI0_SW_GEN1_3_OFFSET	IPU_REG_OFFSET + 0x0104047C			
#define SRM_DI0_SW_GEN1_4_OFFSET	IPU_REG_OFFSET + 0x01040480			
#define SRM_DI0_SW_GEN1_5_OFFSET	IPU_REG_OFFSET + 0x01040484			
#define SRM_DI0_SW_GEN1_6_OFFSET	IPU_REG_OFFSET + 0x01040488			
#define SRM_DI0_SW_GEN1_7_OFFSET	IPU_REG_OFFSET + 0x0104048C			
#define SRM_DI0_SW_GEN1_8_OFFSET	IPU_REG_OFFSET + 0x01040490			
#define SRM_DI0_SW_GEN1_9_OFFSET	IPU_REG_OFFSET + 0x01040494			
#define SRM_DI0_SYNC_AS_GEN_OFFSET	IPU_REG_OFFSET + 0x01040498			
#define SRM_DI0_DW_GEN_0_OFFSET	IPU_REG_OFFSET + 0x0104049C			
#define SRM_DI0_DW_GEN_1_OFFSET	IPU_REG_OFFSET + 0x010404A0			
#define SRM_DI0_DW_GEN_2_OFFSET	IPU_REG_OFFSET + 0x010404A4			
#define SRM_DI0_DW_GEN_3_OFFSET	IPU_REG_OFFSET + 0x010404A8			
#define SRM_DI0_DW_GEN_4_OFFSET	IPU_REG_OFFSET + 0x010404AC			
#define SRM_DI0_DW_GEN_5_OFFSET	IPU_REG_OFFSET + 0x010404B0			
#define SRM_DI0_DW_GEN_6_OFFSET	IPU_REG_OFFSET + 0x010404B4			
#define SRM_DI0_DW_GEN_7_OFFSET	IPU_REG_OFFSET + 0x010404B8			
#define SRM_DI0_DW_GEN_8_OFFSET	IPU_REG_OFFSET + 0x010404BC			
#define SRM_DI0_DW_GEN_9_OFFSET	IPU_REG_OFFSET + 0x010404C0			
#define SRM_DI0_DW_GEN_10_OFFSET	IPU_REG_OFFSET + 0x010404C4			
#define SRM_DI0_DW_GEN_11_OFFSET	IPU_REG_OFFSET + 0x010404C8			
#define SRM_DI0_DW_SET0_0_OFFSET	IPU_REG_OFFSET + 0x010404CC			
#define SRM_DI0_DW_SET0_1_OFFSET	IPU_REG_OFFSET + 0x010404D0			
#define SRM_DI0_DW_SET0_2_OFFSET	IPU_REG_OFFSET + 0x010404D4			
#define SRM_DI0_DW_SET0_3_OFFSET	IPU_REG_OFFSET + 0x010404D8			
#define SRM_DI0_DW_SET0_4_OFFSET	IPU_REG_OFFSET + 0x010404DC			
#define SRM_DI0_DW_SET0_5_OFFSET	IPU_REG_OFFSET + 0x010404E0			
#define SRM_DI0_DW_SET0_6_OFFSET	IPU_REG_OFFSET + 0x010404E4			
#define SRM_DI0_DW_SET0_7_OFFSET	IPU_REG_OFFSET + 0x010404E8			
#define SRM_DI0_DW_SET0_8_OFFSET	IPU_REG_OFFSET + 0x010404EC			
#define SRM_DI0_DW_SET0_9_OFFSET	IPU_REG_OFFSET + 0x010404F0			
#define SRM_DI0_DW_SET0_10_OFFSET	IPU_REG_OFFSET + 0x010404F4			
#define SRM_DI0_DW_SET0_11_OFFSET	IPU_REG_OFFSET + 0x010404F8			
#define SRM_DI0_DW_SET1_0_OFFSET	IPU_REG_OFFSET + 0x010404FC			
#define SRM_DI0_DW_SET1_1_OFFSET	IPU_REG_OFFSET + 0x01040500			
#define SRM_DI0_DW_SET1_2_OFFSET	IPU_REG_OFFSET + 0x01040504			
#define SRM_DI0_DW_SET1_3_OFFSET	IPU_REG_OFFSET + 0x01040508			
#define SRM_DI0_DW_SET1_4_OFFSET	IPU_REG_OFFSET + 0x0104050C			
#define SRM_DI0_DW_SET1_5_OFFSET	IPU_REG_OFFSET + 0x01040510			
#define SRM_DI0_DW_SET1_6_OFFSET	IPU_REG_OFFSET + 0x01040514			
#define SRM_DI0_DW_SET1_7_OFFSET	IPU_REG_OFFSET + 0x01040518			
#define SRM_DI0_DW_SET1_8_OFFSET	IPU_REG_OFFSET + 0x0104051C			
#define SRM_DI0_DW_SET1_9_OFFSET	IPU_REG_OFFSET + 0x01040520			
#define SRM_DI0_DW_SET1_10_OFFSET	IPU_REG_OFFSET + 0x01040524			
#define SRM_DI0_DW_SET1_11_OFFSET	IPU_REG_OFFSET + 0x01040528			
#define SRM_DI0_DW_SET2_0_OFFSET	IPU_REG_OFFSET + 0x0104052C			
#define SRM_DI0_DW_SET2_1_OFFSET	IPU_REG_OFFSET + 0x01040530			
#define SRM_DI0_DW_SET2_2_OFFSET	IPU_REG_OFFSET + 0x01040534			
#define SRM_DI0_DW_SET2_3_OFFSET	IPU_REG_OFFSET + 0x01040538			
#define SRM_DI0_DW_SET2_4_OFFSET	IPU_REG_OFFSET + 0x0104053C			
#define SRM_DI0_DW_SET2_5_OFFSET	IPU_REG_OFFSET + 0x01040540			
#define SRM_DI0_DW_SET2_6_OFFSET	IPU_REG_OFFSET + 0x01040544			
#define SRM_DI0_DW_SET2_7_OFFSET	IPU_REG_OFFSET + 0x01040548			
#define SRM_DI0_DW_SET2_8_OFFSET	IPU_REG_OFFSET + 0x0104054C			
#define SRM_DI0_DW_SET2_9_OFFSET	IPU_REG_OFFSET + 0x01040550			
#define SRM_DI0_DW_SET2_10_OFFSET	IPU_REG_OFFSET + 0x01040554			
#define SRM_DI0_DW_SET2_11_OFFSET	IPU_REG_OFFSET + 0x01040558			
#define SRM_DI0_DW_SET3_0_OFFSET	IPU_REG_OFFSET + 0x0104055C			
#define SRM_DI0_DW_SET3_1_OFFSET	IPU_REG_OFFSET + 0x01040560			
#define SRM_DI0_DW_SET3_2_OFFSET	IPU_REG_OFFSET + 0x01040564			
#define SRM_DI0_DW_SET3_3_OFFSET	IPU_REG_OFFSET + 0x01040568			
#define SRM_DI0_DW_SET3_4_OFFSET	IPU_REG_OFFSET + 0x0104056C			
#define SRM_DI0_DW_SET3_5_OFFSET	IPU_REG_OFFSET + 0x01040570			
#define SRM_DI0_DW_SET3_6_OFFSET	IPU_REG_OFFSET + 0x01040574			
#define SRM_DI0_DW_SET3_7_OFFSET	IPU_REG_OFFSET + 0x01040578			
#define SRM_DI0_DW_SET3_8_OFFSET	IPU_REG_OFFSET + 0x0104057C			
#define SRM_DI0_DW_SET3_9_OFFSET	IPU_REG_OFFSET + 0x01040580			
#define SRM_DI0_DW_SET3_10_OFFSET	IPU_REG_OFFSET + 0x01040584			
#define SRM_DI0_DW_SET3_11_OFFSET	IPU_REG_OFFSET + 0x01040588			
#define SRM_DI0_STP_REP_1_OFFSET	IPU_REG_OFFSET + 0x0104058C			
#define SRM_DI0_STP_REP_2_OFFSET	IPU_REG_OFFSET + 0x01040590			
#define SRM_DI0_STP_REP_3_OFFSET	IPU_REG_OFFSET + 0x01040594			
#define SRM_DI0_STP_REP_4_OFFSET	IPU_REG_OFFSET + 0x01040598			
#define SRM_DI0_STP_REP_9_OFFSET	IPU_REG_OFFSET + 0x0104059C			
#define SRM_DI0_SER_CONF_OFFSET	IPU_REG_OFFSET + 0x010405A0			
#define SRM_DI0_SSC_OFFSET	IPU_REG_OFFSET + 0x010405A4			
#define SRM_DI0_POL_OFFSET	IPU_REG_OFFSET + 0x010405A8			
#define SRM_DI0_AW0_OFFSET	IPU_REG_OFFSET + 0x010405AC			
#define SRM_DI0_AW1_OFFSET	IPU_REG_OFFSET + 0x010405B0			
#define SRM_DI1_GENERAL_OFFSET	IPU_REG_OFFSET + 0x010405B4			
#define SRM_DI1_BS_CLKGEN0_OFFSET	IPU_REG_OFFSET + 0x010405B8			
#define SRM_DI1_BS_CLKGEN1_OFFSET	IPU_REG_OFFSET + 0x010405BC			
#define SRM_DI1_SW_GEN0_1_OFFSET	IPU_REG_OFFSET + 0x010405C0			
#define SRM_DI1_SW_GEN0_2_OFFSET	IPU_REG_OFFSET + 0x010405C4			
#define SRM_DI1_SW_GEN0_3_OFFSET	IPU_REG_OFFSET + 0x010405C8			
#define SRM_DI1_SW_GEN0_4_OFFSET	IPU_REG_OFFSET + 0x010405CC			
#define SRM_DI1_SW_GEN0_5_OFFSET	IPU_REG_OFFSET + 0x010405D0			
#define SRM_DI1_SW_GEN0_6_OFFSET	IPU_REG_OFFSET + 0x010405D4			
#define SRM_DI1_SW_GEN0_7_OFFSET	IPU_REG_OFFSET + 0x010405D8			
#define SRM_DI1_SW_GEN0_8_OFFSET	IPU_REG_OFFSET + 0x010405DC			
#define SRM_DI1_SW_GEN0_9_OFFSET	IPU_REG_OFFSET + 0x010405E0			
#define SRM_DI1_SW_GEN1_1_OFFSET	IPU_REG_OFFSET + 0x010405E4			
#define SRM_DI1_SW_GEN1_2_OFFSET	IPU_REG_OFFSET + 0x010405E8			
#define SRM_DI1_SW_GEN1_3_OFFSET	IPU_REG_OFFSET + 0x010405EC			
#define SRM_DI1_SW_GEN1_4_OFFSET	IPU_REG_OFFSET + 0x010405F0			
#define SRM_DI1_SW_GEN1_5_OFFSET	IPU_REG_OFFSET + 0x010405F4			
#define SRM_DI1_SW_GEN1_6_OFFSET	IPU_REG_OFFSET + 0x010405F8			
#define SRM_DI1_SW_GEN1_7_OFFSET	IPU_REG_OFFSET + 0x010405FC			
#define SRM_DI1_SW_GEN1_8_OFFSET	IPU_REG_OFFSET + 0x01040600			
#define SRM_DI1_SW_GEN1_9_OFFSET	IPU_REG_OFFSET + 0x01040604			
#define SRM_DI1_SYNC_AS_GEN_OFFSET	IPU_REG_OFFSET + 0x01040608			
#define SRM_DI1_DW_GEN_0_OFFSET	IPU_REG_OFFSET + 0x0104060C			
#define SRM_DI1_DW_GEN_1_OFFSET	IPU_REG_OFFSET + 0x01040610			
#define SRM_DI1_DW_GEN_2_OFFSET	IPU_REG_OFFSET + 0x01040614			
#define SRM_DI1_DW_GEN_3_OFFSET	IPU_REG_OFFSET + 0x01040618			
#define SRM_DI1_DW_GEN_4_OFFSET	IPU_REG_OFFSET + 0x0104061C			
#define SRM_DI1_DW_GEN_5_OFFSET	IPU_REG_OFFSET + 0x01040620			
#define SRM_DI1_DW_GEN_6_OFFSET	IPU_REG_OFFSET + 0x01040624			
#define SRM_DI1_DW_GEN_7_OFFSET	IPU_REG_OFFSET + 0x01040628			
#define SRM_DI1_DW_GEN_8_OFFSET	IPU_REG_OFFSET + 0x0104062C			
#define SRM_DI1_DW_GEN_9_OFFSET	IPU_REG_OFFSET + 0x01040630			
#define SRM_DI1_DW_GEN_10_OFFSET	IPU_REG_OFFSET + 0x01040634			
#define SRM_DI1_DW_GEN_11_OFFSET	IPU_REG_OFFSET + 0x01040638			
#define SRM_DI1_DW_SET0_0_OFFSET	IPU_REG_OFFSET + 0x0104063C			
#define SRM_DI1_DW_SET0_1_OFFSET	IPU_REG_OFFSET + 0x01040640			
#define SRM_DI1_DW_SET0_2_OFFSET	IPU_REG_OFFSET + 0x01040644			
#define SRM_DI1_DW_SET0_3_OFFSET	IPU_REG_OFFSET + 0x01040648			
#define SRM_DI1_DW_SET0_4_OFFSET	IPU_REG_OFFSET + 0x0104064C			
#define SRM_DI1_DW_SET0_5_OFFSET	IPU_REG_OFFSET + 0x01040650			
#define SRM_DI1_DW_SET0_6_OFFSET	IPU_REG_OFFSET + 0x01040654			
#define SRM_DI1_DW_SET0_7_OFFSET	IPU_REG_OFFSET + 0x01040658			
#define SRM_DI1_DW_SET0_8_OFFSET	IPU_REG_OFFSET + 0x0104065C			
#define SRM_DI1_DW_SET0_9_OFFSET	IPU_REG_OFFSET + 0x01040660			
#define SRM_DI1_DW_SET0_10_OFFSET	IPU_REG_OFFSET + 0x01040664			
#define SRM_DI1_DW_SET0_11_OFFSET	IPU_REG_OFFSET + 0x01040668			
#define SRM_DI1_DW_SET1_0_OFFSET	IPU_REG_OFFSET + 0x0104066C			
#define SRM_DI1_DW_SET1_1_OFFSET	IPU_REG_OFFSET + 0x01040670			
#define SRM_DI1_DW_SET1_2_OFFSET	IPU_REG_OFFSET + 0x01040674			
#define SRM_DI1_DW_SET1_3_OFFSET	IPU_REG_OFFSET + 0x01040678			
#define SRM_DI1_DW_SET1_4_OFFSET	IPU_REG_OFFSET + 0x0104067C			
#define SRM_DI1_DW_SET1_5_OFFSET	IPU_REG_OFFSET + 0x01040680			
#define SRM_DI1_DW_SET1_6_OFFSET	IPU_REG_OFFSET + 0x01040684			
#define SRM_DI1_DW_SET1_7_OFFSET	IPU_REG_OFFSET + 0x01040688			
#define SRM_DI1_DW_SET1_8_OFFSET	IPU_REG_OFFSET + 0x0104068C			
#define SRM_DI1_DW_SET1_9_OFFSET	IPU_REG_OFFSET + 0x01040690			
#define SRM_DI1_DW_SET1_10_OFFSET	IPU_REG_OFFSET + 0x01040694			
#define SRM_DI1_DW_SET1_11_OFFSET	IPU_REG_OFFSET + 0x01040698			
#define SRM_DI1_DW_SET2_0_OFFSET	IPU_REG_OFFSET + 0x0104069C			
#define SRM_DI1_DW_SET2_1_OFFSET	IPU_REG_OFFSET + 0x010406A0			
#define SRM_DI1_DW_SET2_2_OFFSET	IPU_REG_OFFSET + 0x010406A4			
#define SRM_DI1_DW_SET2_3_OFFSET	IPU_REG_OFFSET + 0x010406A8			
#define SRM_DI1_DW_SET2_4_OFFSET	IPU_REG_OFFSET + 0x010406AC			
#define SRM_DI1_DW_SET2_5_OFFSET	IPU_REG_OFFSET + 0x010406B0			
#define SRM_DI1_DW_SET2_6_OFFSET	IPU_REG_OFFSET + 0x010406B4			
#define SRM_DI1_DW_SET2_7_OFFSET	IPU_REG_OFFSET + 0x010406B8			
#define SRM_DI1_DW_SET2_8_OFFSET	IPU_REG_OFFSET + 0x010406BC			
#define SRM_DI1_DW_SET2_9_OFFSET	IPU_REG_OFFSET + 0x010406C0			
#define SRM_DI1_DW_SET2_10_OFFSET	IPU_REG_OFFSET + 0x010406C4			
#define SRM_DI1_DW_SET2_11_OFFSET	IPU_REG_OFFSET + 0x010406C8			
#define SRM_DI1_DW_SET3_0_OFFSET	IPU_REG_OFFSET + 0x010406CC			
#define SRM_DI1_DW_SET3_1_OFFSET	IPU_REG_OFFSET + 0x010406D0			
#define SRM_DI1_DW_SET3_2_OFFSET	IPU_REG_OFFSET + 0x010406D4			
#define SRM_DI1_DW_SET3_3_OFFSET	IPU_REG_OFFSET + 0x010406D8			
#define SRM_DI1_DW_SET3_4_OFFSET	IPU_REG_OFFSET + 0x010406DC			
#define SRM_DI1_DW_SET3_5_OFFSET	IPU_REG_OFFSET + 0x010406E0			
#define SRM_DI1_DW_SET3_6_OFFSET	IPU_REG_OFFSET + 0x010406E4			
#define SRM_DI1_DW_SET3_7_OFFSET	IPU_REG_OFFSET + 0x010406E8			
#define SRM_DI1_DW_SET3_8_OFFSET	IPU_REG_OFFSET + 0x010406EC			
#define SRM_DI1_DW_SET3_9_OFFSET	IPU_REG_OFFSET + 0x010406F0			
#define SRM_DI1_DW_SET3_10_OFFSET	IPU_REG_OFFSET + 0x010406F4			
#define SRM_DI1_DW_SET3_11_OFFSET	IPU_REG_OFFSET + 0x010406F8			
#define SRM_DI1_STP_REP_1_OFFSET	IPU_REG_OFFSET + 0x010406FC			
#define SRM_DI1_STP_REP_2_OFFSET	IPU_REG_OFFSET + 0x01040700			
#define SRM_DI1_STP_REP_3_OFFSET	IPU_REG_OFFSET + 0x01040704			
#define SRM_DI1_STP_REP_4_OFFSET	IPU_REG_OFFSET + 0x01040708			
#define SRM_DI1_STP_REP_9_OFFSET	IPU_REG_OFFSET + 0x0104070C			
#define SRM_DI1_SER_CONF_OFFSET	IPU_REG_OFFSET + 0x01040710			
#define SRM_DI1_SSC_OFFSET	IPU_REG_OFFSET + 0x01040714			
#define SRM_DI1_POL_OFFSET	IPU_REG_OFFSET + 0x01040718			
#define SRM_DI1_AW0_OFFSET	IPU_REG_OFFSET + 0x0104071C			
#define SRM_DI1_AW1_OFFSET	IPU_REG_OFFSET + 0x01040720			
#define SRM_DC_WR_CH_CONF_2_OFFSET	IPU_REG_OFFSET + 0x0104040C			
#define SRM_DC_WR_CH_ADDR_2_OFFSET	IPU_REG_OFFSET + 0x01040410			
#define SRM_DC_RL0_CH_2_OFFSET	IPU_REG_OFFSET + 0x01040414			
#define SRM_DC_RL1_CH_2_OFFSET	IPU_REG_OFFSET + 0x01040418			
#define SRM_DC_RL2_CH_2_OFFSET	IPU_REG_OFFSET + 0x0104041C			
#define SRM_DC_RL3_CH_2_OFFSET	IPU_REG_OFFSET + 0x01040420			
#define SRM_DC_RL4_CH_2_OFFSET	IPU_REG_OFFSET + 0x01040424			
#define SRM_DC_WR_CH_CONF_6_OFFSET	IPU_REG_OFFSET + 0x01040428			
#define SRM_DC_WR_CH_ADDR_6_OFFSET	IPU_REG_OFFSET + 0x0104042C			
#define SRM_DC_RL0_CH_6_OFFSET	IPU_REG_OFFSET + 0x01040430			
#define SRM_DC_RL1_CH_6_OFFSET	IPU_REG_OFFSET + 0x01040434			
#define SRM_DC_RL2_CH_6_OFFSET	IPU_REG_OFFSET + 0x01040438			
#define SRM_DC_RL3_CH_6_OFFSET	IPU_REG_OFFSET + 0x0104043C			
#define SRM_DC_RL4_CH_6_OFFSET	IPU_REG_OFFSET + 0x01040440			
#define IPU_ISP_TBPR_0_OFFSET	IPU_REG_OFFSET + 0x010C0000			
#define IPU_ISP_TBPR_1_OFFSET	IPU_REG_OFFSET + 0x010C0004			
#define IPU_ISP_TBPR_2_OFFSET	IPU_REG_OFFSET + 0x010C0008			
#define IPU_ISP_TBPR_3_OFFSET	IPU_REG_OFFSET + 0x010C000C			
#define IPU_ISP_TBPR_4_OFFSET	IPU_REG_OFFSET + 0x010C0010			
#define IPU_ISP_TBPR_5_OFFSET	IPU_REG_OFFSET + 0x010C0014			
#define IPU_ISP_TBPR_6_OFFSET	IPU_REG_OFFSET + 0x010C0018			
#define IPU_ISP_TBPR_7_OFFSET	IPU_REG_OFFSET + 0x010C001C			
#define IPU_ISP_TBPR_8_OFFSET	IPU_REG_OFFSET + 0x010C0020			
#define IPU_ISP_TBPR_9_OFFSET	IPU_REG_OFFSET + 0x010C0024			
#define IPU_ISP_TBPR_10_OFFSET	IPU_REG_OFFSET + 0x010C0028			
#define IPU_ISP_TBPR_11_OFFSET	IPU_REG_OFFSET + 0x010C002C			
#define IPU_ISP_TBPR_12_OFFSET	IPU_REG_OFFSET + 0x010C0030			
#define IPU_ISP_TBPR_13_OFFSET	IPU_REG_OFFSET + 0x010C0034			
#define IPU_ISP_TBPR_14_OFFSET	IPU_REG_OFFSET + 0x010C0038			
#define IPU_ISP_TBPR_15_OFFSET	IPU_REG_OFFSET + 0x010C003C			
#define IPU_ISP_TBPR_16_OFFSET	IPU_REG_OFFSET + 0x010C0040			
#define IPU_ISP_TBPR_17_OFFSET	IPU_REG_OFFSET + 0x010C0044			
#define IPU_ISP_TBPR_18_OFFSET	IPU_REG_OFFSET + 0x010C0048			
#define IPU_ISP_TBPR_19_OFFSET	IPU_REG_OFFSET + 0x010C004C			
#define IPU_ISP_TBPR_20_OFFSET	IPU_REG_OFFSET + 0x010C0050			
#define IPU_ISP_TBPR_21_OFFSET	IPU_REG_OFFSET + 0x010C0054			
#define IPU_ISP_TBPR_22_OFFSET	IPU_REG_OFFSET + 0x010C0058			
#define IPU_ISP_TBPR_23_OFFSET	IPU_REG_OFFSET + 0x010C005C			
#define IPU_ISP_TBPR_24_OFFSET	IPU_REG_OFFSET + 0x010C0060			
#define IPU_ISP_TBPR_25_OFFSET	IPU_REG_OFFSET + 0x010C0064			
#define IPU_ISP_TBPR_26_OFFSET	IPU_REG_OFFSET + 0x010C0068			
#define IPU_ISP_TBPR_27_OFFSET	IPU_REG_OFFSET + 0x010C006C			
#define IPU_ISP_TBPR_28_OFFSET	IPU_REG_OFFSET + 0x010C0070			
#define IPU_ISP_TBPR_29_OFFSET	IPU_REG_OFFSET + 0x010C0074			
#define IPU_ISP_TBPR_30_OFFSET	IPU_REG_OFFSET + 0x010C0078			
#define IPU_ISP_TBPR_31_OFFSET	IPU_REG_OFFSET + 0x010C007C			
#define IPU_ISP_TBPR_32_OFFSET	IPU_REG_OFFSET + 0x010C0080			
#define IPU_ISP_TBPR_33_OFFSET	IPU_REG_OFFSET + 0x010C0084			
#define IPU_ISP_TBPR_34_OFFSET	IPU_REG_OFFSET + 0x010C0088			
#define IPU_ISP_TBPR_35_OFFSET	IPU_REG_OFFSET + 0x010C008C			
#define IPU_ISP_TBPR_36_OFFSET	IPU_REG_OFFSET + 0x010C0090			
#define IPU_ISP_TBPR_37_OFFSET	IPU_REG_OFFSET + 0x010C0094			
#define IPU_ISP_TBPR_38_OFFSET	IPU_REG_OFFSET + 0x010C0098			
#define IPU_ISP_TBPR_39_OFFSET	IPU_REG_OFFSET + 0x010C009C			
#define IPU_ISP_TBPR_40_OFFSET	IPU_REG_OFFSET + 0x010C00A0			
#define IPU_ISP_TBPR_41_OFFSET	IPU_REG_OFFSET + 0x010C00A4			
#define IPU_ISP_TBPR_42_OFFSET	IPU_REG_OFFSET + 0x010C00A8			
#define IPU_ISP_TBPR_43_OFFSET	IPU_REG_OFFSET + 0x010C00AC			
#define IPU_ISP_TBPR_44_OFFSET	IPU_REG_OFFSET + 0x010C00B0			
#define IPU_ISP_TBPR_45_OFFSET	IPU_REG_OFFSET + 0x010C00B4			
#define IPU_ISP_TBPR_46_OFFSET	IPU_REG_OFFSET + 0x010C00B8			
#define IPU_ISP_TBPR_47_OFFSET	IPU_REG_OFFSET + 0x010C00BC			
#define IPU_ISP_TBPR_48_OFFSET	IPU_REG_OFFSET + 0x010C00C0			
#define IPU_ISP_TBPR_49_OFFSET	IPU_REG_OFFSET + 0x010C00C4			
#define IPU_ISP_TBPR_50_OFFSET	IPU_REG_OFFSET + 0x010C00C8			
#define IPU_ISP_TBPR_51_OFFSET	IPU_REG_OFFSET + 0x010C00CC			
#define IPU_ISP_TBPR_52_OFFSET	IPU_REG_OFFSET + 0x010C00D0			
#define IPU_ISP_TBPR_53_OFFSET	IPU_REG_OFFSET + 0x010C00D4			
#define IPU_ISP_TBPR_54_OFFSET	IPU_REG_OFFSET + 0x010C00D8			
#define IPU_ISP_TBPR_55_OFFSET	IPU_REG_OFFSET + 0x010C00DC			
#define IPU_ISP_TBPR_56_OFFSET	IPU_REG_OFFSET + 0x010C00E0			
#define IPU_ISP_TBPR_57_OFFSET	IPU_REG_OFFSET + 0x010C00E4			
#define IPU_ISP_TBPR_58_OFFSET	IPU_REG_OFFSET + 0x010C00E8			
#define IPU_ISP_TBPR_59_OFFSET	IPU_REG_OFFSET + 0x010C00EC			
#define IPU_ISP_TBPR_60_OFFSET	IPU_REG_OFFSET + 0x010C00F0			
#define IPU_ISP_TBPR_61_OFFSET	IPU_REG_OFFSET + 0x010C00F4			
#define IPU_ISP_TBPR_62_OFFSET	IPU_REG_OFFSET + 0x010C00F8			
#define IPU_ISP_TBPR_63_OFFSET	IPU_REG_OFFSET + 0x010C00FC			
//#########################################				
//# GPU           				
//#########################################				
#define MBX1_3DDATA_BASE_ADDR_OFFSET                   	0x00800000	// Req. by telma - no other defines at the spec		
#define MBX1_2DDATA_BASE_ADDR_OFFSET                   	0x00A00000	// Req. by telma - no other defines at the spec		
#define MBX1_TA_BASE_ADDR_OFFSET                       	0xC00000	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_SW_RESET_OFFSET                    	0x080	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_CLK_RATIO_OFFSET                   	0x090	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_IDLE_COUNT_OFFSET                 	0x100	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_ACTIVITY_COUNT_OFFSET             	0x104	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_GPO_OFFSET                         	0x108	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_INT_STATUS_OFFSET                  	0x12C	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_INT_ENABLE_OFFSET                  	0x130	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_INT_CLEAR_OFFSET                   	0x134	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MEMPAGE_SIZE_OFFSET                	0x140	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_RGNBASE_OFFSET                     	0x608	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_OBJBASE_OFFSET                     	0x60C	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_ZLOADSTORE_OFFSET                  	0x610	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_FPUPERPVAL_OFFSET                  	0x614	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_FPUCULLVAL_OFFSET                  	0x618	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_3DPIXSAMP_OFFSET                  	0x61C	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_CK1_OFFSET                        	0x624	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_CK2_OFFSET                         	0x628	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_CKUV_OFFSET                        	0x62C	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_CKMASK_OFFSET                      	0x630	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_FOGCOLTABLE_OFFSET                 	0x638	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_FOGCOLVERT_OFFSET                  	0x63C	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_VERTFOGSTATUS_OFFSET               	0x640	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_SCALERCTL_OFFSET                   	0x644	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_BLENDCTL_OFFSET                   	0x648	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_ARGBSUM_OFFSET                    	0x64C	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_FBCTL_OFFSET                       	0x650	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_FBXCLIP_OFFSET                    	0x654	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_FBYCLIP_OFFSET                     	0x658	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_FBSTART_OFFSET                     	0x65C	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_FBLINESTRIDE_OFFSET                	0x660	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_LATENCYCOUNT_OFFSET                	0x664	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_ENDIANCTL_OFFSET                   	0x668	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_ZBASEADDR_OFFSET                   	0x674	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_STARTRENDER_OFFSET                	0x680	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_3DFLATSHADEDCS_OFFSET              	0x6D0	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_3D_ZL_BACKGROUND_TAG_OFFSET        	0x6D8	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_3D_ZL_BACKGROUND_DEPTH_OFFSET      	0x6DC	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_1BPP_BACKGROUND_COLOR_OFFSET       	0x6E0	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_1BPP_FOREGROUND_COLOR_OFFSET       	0x6E4	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_USE_1BPP_REGS_CTL_OFFSET          	0x6E8	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_3D_RENDER_ID_OFFSET               	0x6EC	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_3D_TEX_DECIM_OFFSET                	0x6F0	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_START_OFFSET                     	0x800	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_RESTART_OFFSET                   	0x804	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_ABORT_OFFSET                    	0x808	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_RENDER_ID_OFFSET                 	0x810	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_CONTEXT_LOAD_OFFSET              	0x814	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_CONTEXT_STORE_OFFSET             	0x818	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_CONTEXT_RESET_OFFSET             	0x81C	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_CONTEXT_BASE_OFFSET             	0x820	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_EVM_PAGETBL_BASE_OFFSET         	0x824	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_EVM_LIST_START_OFFSET            	0x828	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_EVM_LIST_END_OFFSET              	0x82C	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_EVM_RENDER_TIMEOUT_OFFSET        	0x830	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_EVM_TA_TIMEOUT_OFFSET            	0x834	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_EVM_INIT_OFFSET                 	0x838	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_OBJDATABASE_OFFSET               	0x83C	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_TAILPTRBASE_OFFSET               	0x840	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_REGION_BASE_OFFSET               	0x844	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_GLOBAL_LIST_CTRL_OFFSET         	0x848	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_XCLIP_OFFSET                    	0x84C	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_YCLIP_OFFSET                     	0x850	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_RHWCLAMP_OFFSET                  	0x854	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_RHWCOMP_OFFSET                  	0x858	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_CONFIG_OFFSET                   	0x85C	// Req. by telma - no other defines at the spec		
#define MBX1_TAGLOBREG_EVM_CONTEXT_FLUSH_ADDR_OFFSET    	0x864	// Req. by telma - no other defines at the spec		
#define MBX1_ISP1_SIG_OFFSET                            	0xCC4	// Req. by telma - no other defines at the spec		
#define MBX1_TSP1_SIG_OFFSET                            	0xCCC	// Req. by telma - no other defines at the spec		
#define MBX1_DIAG_EN_OFFSET                            	0xCE0	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_CORE_ID_OFFSET                     	0xF00	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_REVISION_OFFSET                    	0xF10	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MMU_PAGE0_ADDR_OFFSET              	0x1000	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MMU_PAGE1_ADDR_OFFSET             	0x1004	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MMU_PAGE2_ADDR_OFFSET              	0x1008	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MMU_PAGE3_ADDR_OFFSET              	0x100C	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MMU_PAGE4_ADDR_OFFSET              	0x1010	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MMU_PAGE5_ADDR_OFFSET              	0x1014	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MMU_PAGE6_ADDR_OFFSET              	0x1018	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MMU_PAGE7_ADDR_OFFSET              	0x101C	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MMU_ENABLE_OFFSET                  	0x1020	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MMU_IDX_INVAL_OFFSET               	0x1024	// Req. by telma - no other defines at the spec		
#define MBX1_GLOBREG_MMU_PHYSADD_INVAL_OFFSET           	0x1028	// Req. by telma - no other defines at the spec		
#define MBX1_BIST_CONTROL_OFFSET                        	0x4000	// Req. by telma - no other defines at the spec		
#define MBX1_BIST_TARGET_OFFSET                         	0x4010	// Req. by telma - no other defines at the spec		
#define MBX1_BIST_RESULT_0_OFFSET                       	0x4020	// Req. by telma - no other defines at the spec		
#define MBX1_BIST_RESULT_1_OFFSET                       	0x4030	// Req. by telma - no other defines at the spec		
#define MBX1_BIST_FAILADDRESS_OFFSET                    	0x4060	// Req. by telma - no other defines at the spec		
#define MBX1_BIST_FAILDATA_0_OFFSET                     	0x4070	// Req. by telma - no other defines at the spec		
#define MBX1_BIST_FAILDATA_1_OFFSET                     	0x4080	// Req. by telma - no other defines at the spec		
#define MBX1_BIST_FAILDATA_2_OFFSET                     	0x4090	// Req. by telma - no other defines at the spec		
#define MBX1_BIST_FAILDATA_3_OFFSET	0x40A0	// Req. by telma - no other defines at the spec		
//#########################################				
//# L2CC Program control registers    				
//#########################################				
#define L2CC_R0_CACHE_ID_OFFSET	0x0000			
#define L2CC_R0_CACHE_TYPE_OFFSET	0x0004			
#define L2CC_R1_CNTL_OFFSET	0x0100			
#define L2CC_R1_AUX_CNTL_OFFSET	0x0104			
#define L2CC_R2_EVENT_CNT_CNTL_OFFSET	0x0200			
#define L2CC_R2_EVENT_CNT1_CONFIG_OFFSET	0x0204			
#define L2CC_R2_EVENT_CNT0_CONFIG_OFFSET	0x0208			
#define L2CC_R2_EVENT_CNT1_VALUE_OFFSET	0x020C			
#define L2CC_R2_EVENT_CNT0_VALUE_OFFSET	0x0210			
#define L2CC_R2_INT_MASK_OFFSET	0x0214			
#define L2CC_R2_MASKED_INT_STATUS_OFFSET	0x0218			
#define L2CC_R2_RAW_INT_STATUS_OFFSET	0x021C			
#define L2CC_R2_INT_CLEAR_OFFSET	0x0220			
#define L2CC_R7_CACHE_SYNC_OFFSET	0x0730			
#define L2CC_R7_INVAL_BY_PA_OFFSET	0x0770			
#define L2CC_R7_INVAL_BY_WAY_OFFSET	0x077C			
#define L2CC_R7_CLEAN_BY_PA_OFFSET	0x07B0			
#define L2CC_R7_CLEAN_BY_IDXWAY_OFFSET	0x07B8			
#define L2CC_R7_CLEAN_BY_WAY_OFFSET	0x07BC			
#define L2CC_R7_CLEANINVAL_LINE_BY_PA_OFFSET	0x07F0			
#define L2CC_R7_CLEANINVAL_LINE_BY_IDXWAY_OFFSET	0x07FB			
#define L2CC_R7_CLEANINVAL_BY_WAY_OFFSET	0x07FC			
#define L2CC_R9_LOCKDOWN_BY_WAY_D_OFFSET	0x0900			
#define L2CC_R9_LOCKDOWN_BY_WAY_I_OFFSET	0x0904			
#define L2CC_R15_TEST_OPER_OFFSET	0x0F00			
#define L2CC_R15_LINE_DATA_0_OFFSET	0x0F10			
#define L2CC_R15_LINE_DATA_1_OFFSET	0x0F14			
#define L2CC_R15_LINE_DATA_2_OFFSET	0x0F18			
#define L2CC_R15_LINE_DATA_3_OFFSET	0x0F1C			
#define L2CC_R15_LINE_DATA_4_OFFSET	0x0F20			
#define L2CC_R15_LINE_DATA_5_OFFSET	0x0F24			
#define L2CC_R15_LINE_DATA_6_OFFSET	0x0F28			
#define L2CC_R15_LINE_DATA_7_OFFSET	0x0F2C			
#define L2CC_R15_LINE_TAG_OFFSET                        	0x0F30			
#define L2CC_R15_DBG_CNTL_OFFSET	0x0F40			
//#########################################				
//# EVTMON     				
//#########################################
#define EVTMON_EMMC_OFFSET          0x000
#define EVTMON_EMCS_OFFSET          0x004
#define EVTMON_EMCC0_OFFSET         0x008
#define EVTMON_EMCC1_OFFSET         0x00c
#define EVTMON_EMCC2_OFFSET         0x010
#define EVTMON_EMCC3_OFFSET         0x014
#define EVTMON_EMCC4_OFFSET         0x018
#define EVTMON_EMCC5_OFFSET         0x01c
#define EVTMON_EMC0_OFFSET          0x020
#define EVTMON_EMC1_OFFSET          0x024
#define EVTMON_EMC2_OFFSET          0x028
#define EVTMON_EMC3_OFFSET          0x02c
#define EVTMON_EMC4_OFFSET          0x030
#define EVTMON_EMC5_OFFSET          0x034
#define EVTMON_PNNC0_OFFSET         0x040				
#define EVTMON_PNNC1_OFFSET         0x044				
#define EVTMON_PNNC2_OFFSET         0x048				
#define EVTMON_CCNT_OFFSET          0x04c				
#define EVTMON_PMN0_OFFSET          0x050				
#define EVTMON_PMN1_OFFSET          0x054				
#define EVTMON_PMN2_OFFSET          0x058				
#define EVTMON_PMN3_OFFSET          0x05c				
#define EVTMON_PMN4_OFFSET          0x060				
#define EVTMON_PMN5_OFFSET          0x064				
//#########################################				
//# TZIC          				
//#########################################				
#define TZIC_INTCTRL_OFFSET	0x0000	// Control Register		
#define TZIC_INTTYPE_OFFSET	0x0004	// Interrupt Controller Type Reg.		
#define TZIC_IMPID_OFFSET	0x0008	// Distributor Implementer Identification Reg.		
#define TZIC_PRIOMASK_OFFSET	0x000C	// Priority Mask Register R/W 		
#define TZIC_INTSEC0_OFFSET	0x0080	// Interrupt Security Register		
#define TZIC_INTSEC1_OFFSET	0x0084	// Interrupt Security Register		
#define TZIC_INTSEC2_OFFSET	0x0088	// Interrupt Security Register		
#define TZIC_INTSEC3_OFFSET	0x008C	// Interrupt Security Register		
#define TZIC_ENSET0_OFFSET	0x0100	// Enable Set Register		
#define TZIC_ENSET1_OFFSET	0x0104	// Enable Set Register		
#define TZIC_ENSET2_OFFSET	0x0108	// Enable Set Register		
#define TZIC_ENSET3_OFFSET	0x010C	// Enable Set Register		
#define TZIC_ENCLEAR0_OFFSET	0x0180	// Enable Clear Register		
#define TZIC_ENCLEAR1_OFFSET	0x0184	// Enable Clear Register		
#define TZIC_ENCLEAR2_OFFSET	0x0188	// Enable Clear Register		
#define TZIC_ENCLEAR3_OFFSET	0x018C	// Enable Clear Register		
#define TZIC_SRCSET0_OFFSET	0x0200	// Source Set Register		
#define TZIC_SRCSET1_OFFSET	0x0204	// Source Set Register		
#define TZIC_SRCSET2_OFFSET	0x0208	// Source Set Register		
#define TZIC_SRCSET3_OFFSET	0x020C	// Source Set Register		
#define TZIC_SRCCLEAR0_OFFSET	0x0280	// Source Clear Register		
#define TZIC_SRCCLEAR1_OFFSET	0x0284	// Source Clear Register		
#define TZIC_SRCCLEAR2_OFFSET	0x0288	// Source Clear Register		
#define TZIC_SRCCLEAR3_OFFSET	0x028C	// Source Clear Register		
#define TZIC_PRIORITY0_OFFSET	0x0400	// Priority Register		
#define TZIC_PRIORITY1_OFFSET	0x0404	// Priority Register		
#define TZIC_PRIORITY2_OFFSET	0x0408	// Priority Register		
#define TZIC_PRIORITY3_OFFSET	0x040C	// Priority Register		
#define TZIC_PRIORITY4_OFFSET	0x0410	// Priority Register		
#define TZIC_PRIORITY5_OFFSET	0x0414	// Priority Register		
#define TZIC_PRIORITY6_OFFSET	0x0418	// Priority Register		
#define TZIC_PRIORITY7_OFFSET	0x041C	// Priority Register		
#define TZIC_PRIORITY8_OFFSET	0x0420	// Priority Register		
#define TZIC_PRIORITY9_OFFSET	0x0424	// Priority Register		
#define TZIC_PRIORITY10_OFFSET	0x0428	// Priority Register		
#define TZIC_PRIORITY11_OFFSET	0x042C	// Priority Register		
#define TZIC_PRIORITY12_OFFSET	0x0430	// Priority Register		
#define TZIC_PRIORITY13_OFFSET	0x0434	// Priority Register		
#define TZIC_PRIORITY14_OFFSET	0x0438	// Priority Register		
#define TZIC_PRIORITY15_OFFSET	0x043C	// Priority Register		
#define TZIC_PRIORITY16_OFFSET	0x0440	// Priority Register		
#define TZIC_PRIORITY17_OFFSET	0x0444	// Priority Register		
#define TZIC_PRIORITY18_OFFSET	0x0448	// Priority Register		
#define TZIC_PRIORITY19_OFFSET	0x044C	// Priority Register		
#define TZIC_PRIORITY20_OFFSET	0x0450	// Priority Register		
#define TZIC_PRIORITY21_OFFSET	0x0454	// Priority Register		
#define TZIC_PRIORITY22_OFFSET	0x0458	// Priority Register		
#define TZIC_PRIORITY23_OFFSET	0x045C	// Priority Register		
#define TZIC_PRIORITY24_OFFSET	0x0460	// Priority Register		
#define TZIC_PRIORITY25_OFFSET	0x0464	// Priority Register		
#define TZIC_PRIORITY26_OFFSET	0x0468	// Priority Register		
#define TZIC_PRIORITY27_OFFSET	0x046C	// Priority Register		
#define TZIC_PRIORITY28_OFFSET	0x0470	// Priority Register		
#define TZIC_PRIORITY29_OFFSET	0x0474	// Priority Register		
#define TZIC_PRIORITY30_OFFSET	0x0478	// Priority Register		
#define TZIC_PRIORITY31_OFFSET	0x047C	// Priority Register		
#define TZIC_PND0_OFFSET	0x0D00	// Pending Register		
#define TZIC_PND1_OFFSET	0x0D04	// Pending Register		
#define TZIC_PND2_OFFSET	0x0D08	// Pending Register		
#define TZIC_PND3_OFFSET	0x0D0C	// Pending Register		
#define TZIC_HIPND0_OFFSET	0x0D80	// High Priority Pending Register		
#define TZIC_HIPND1_OFFSET	0x0D84	// High Priority Pending Register		
#define TZIC_HIPND2_OFFSET	0x0D88	// High Priority Pending Register		
#define TZIC_HIPND3_OFFSET	0x0D8C	// High Priority Pending Register		
#define TZIC_WAKEUP0_OFFSET	0x0E00	// Wakeup Config Register		
#define TZIC_WAKEUP1_OFFSET	0x0E04	// Wakeup Config Register		
#define TZIC_WAKEUP2_OFFSET	0x0E08	// Wakeup Config Register		
#define TZIC_WAKEUP3_OFFSET	0x0E0C	// Wakeup Config Register		
#define TZIC_SWINT_OFFSET	0x0F00	// Software Interrupt Trigger Register 		
#define TZIC_ID0_OFFSET	0x0Fd0	// Identification Register 0 		
#define TZIC_ID1_OFFSET	0x0fd4	// Identification Register 1		
#define TZIC_ID2_OFFSET	0x0fd8	// Identification Register 2		
#define TZIC_ID3_OFFSET	0x0fdc	// Identification Register 3		
#define TZIC_ID4_OFFSET	0x0fe0	// Identification Register 4		
#define TZIC_ID5_OFFSET	0x0fe4	// Identification Register 5		
#define TZIC_ID6_OFFSET	0x0fe8	// Identification Register 6		
#define TZIC_ID7_OFFSET	0x0fec	// Identification Register 7		
#define TZIC_ID8_OFFSET	0x0ff0	// Identification Register 8		
#define TZIC_ID9_OFFSET	0x0ff4	// Identification Register 9		
#define TZIC_ID10_OFFSET	0x0ff8	// Identification Register 10		
#define TZIC_ID11_OFFSET	0x0ffc	// Identification Register 11		
#define TZIC_PRIO0PND0_OFFSET	0x8000	// Priority 0 Pending Register		
#define TZIC_PRIO8PND1_OFFSET	0x8004	// Priority 8 Pending Register		
#define TZIC_PRIO16PND2_OFFSET	0x8008	// Priority 16 Pending Register		
#define TZIC_PRIO24PND3_OFFSET	0x800C	// Priority 24 Pending Register		
//#########################################				
//# Debug ROM           				
//#########################################				
				
//#########################################				
//# ETB         				
//#########################################				
				
//#########################################				
//# ETM       				
//#########################################				
				
//#########################################				
//# TPIU       				
//#########################################				
				
//#########################################				
//# CTI     				
//#########################################				
#define CTICONTROL_OFFSET	0x000			
#define CTIINTACK_OFFSET	0x010			
#define CTIAPPSET_OFFSET	0x014			
#define CTIAPPCLEAR_OFFSET	0x018			
#define CTIAPPPULSE_OFFSET	0x01C			
#define CTIINEN0_OFFSET	0x020			
#define CTIINEN1_OFFSET	0x024			
#define CTIINEN2_OFFSET	0x028			
#define CTIINEN3_OFFSET	0x02C			
#define CTIINEN4_OFFSET	0x030			
#define CTIINEN5_OFFSET	0x034			
#define CTIINEN6_OFFSET	0x038			
#define CTIINEN7_OFFSET	0x03C			
#define CTIOUTEN0_OFFSET	0x0A0			
#define CTIOUTEN1_OFFSET	0x0A4			
#define CTIOUTEN2_OFFSET	0x0A8			
#define CTIOUTEN3_OFFSET	0x0AC			
#define CTIOUTEN4_OFFSET	0x0B0			
#define CTIOUTEN5_OFFSET	0x0B4			
#define CTIOUTEN6_OFFSET	0x0B8			
#define CTIOUTEN7_OFFSET	0x0BC			
#define CTITRIGINSTATUS_OFFSET	0x130			
#define CTITRIGOUTSTATUS_OFFSET	0x134			
#define CTICHINSTATUS_OFFSET	0x138			
#define CTICHOUTSTATUS_OFFSET	0x13C			
#define CTIGATE_OFFSET	0x140			
#define ASICCTL_OFFSET	0x144			
#define CTS_OFFSET	0xFA0			
#define CTC_OFFSET	0xFA4			
#define ITCR_OFFSET	0xF00			
#define ITCHINACK_OFFSET	0xEDC			
#define ITTRIGINACK_OFFSET	0xEE0			
#define ITCHOUT_OFFSET	0xEE4			
#define ITTRIGOUT_OFFSET	0xEE8			
#define ITCHOUTACK_OFFSET	0xEEC			
#define ITTRIGOUTACK_OFFSET	0xEF0			
#define ITCHIN_OFFSET	0xEF4			
#define ITTRIGIN_OFFSET	0xEF8			
#define CTILOCK_OFFSET	0xFB0			
#define CTILOCKSTATUS_OFFSET	0xFB4			
#define AUTHENSTATUS_OFFSET	0xFB8			
#define DEVICEID_OFFSET	0xFC8			
#define DEVICETYPEID_OFFSET	0xFCC			
#define PERIPHID0_OFFSET	0xFE0			
#define PERIPHID1_OFFSET	0xFE4			
#define PERIPHID2_OFFSET	0xFE8			
#define PERIPHID3_OFFSET	0xFEC			
#define PERIPHID4_OFFSET	0xFD0			
#define COMPONENTID0_OFFSET	0xFF0			
#define COMPONENTID1_OFFSET	0xFF4			
#define COMPONENTID2_OFFSET	0xFF8			
#define COMPONENTID3_OFFSET	0xFFC			
				
//#########################################				
//# Debug Data Funnel    				
//#########################################				
				
//#########################################				
//# CSD0 SDRAM/DDR     				
//#########################################				
				
//#########################################				
//# CSD1 SDRAM/DDR         				
//#########################################				
				
//#########################################				
//# CS0 (Flash) 128MB             				
//#########################################				
				
//#########################################				
//# CS1 (Flash) 64MB               				
//#########################################				
				
//#########################################				
//# CS2 (sram)                 				
//#########################################				
				
//#########################################				
//# CS3 (Spare)              				
//#########################################				
				
//#########################################				
//#  CS4 (Spare)                              				
//#########################################				
				
//#########################################				
//# CS5 (spare)                  				
//#########################################				
				
//#########################################				
//# Nand Flash                       				
//#########################################				
				
//#########################################				
//# ESDCTL registers                    				
//#########################################				
#define ESDCTL_ESDCTL0_OFFSET 	0x00	// Req. by telma		
#define ESDCTL_ESDCFG0_OFFSET 	0x04	// Req. by telma		
#define ESDCTL_ESDCTL1_OFFSET 	0x08	// Req. by telma		
#define ESDCTL_ESDCFG1_OFFSET 	0x0C	// Req. by telma		
#define ESDCTL_ESDMISC_OFFSET 	0x10	// Req. by telma		
#define ESDCTL_ESDSCR_OFFSET	0x14			
#define ESDCTL_DEBUG_ADDR_OFFSET	0x18			
#define ESDCTL_ESDCDLY1_OFFSET	0x20			
#define ESDCTL_ESDCDLY2_OFFSET	0x24			
#define ESDCTL_ESDCDLY3_OFFSET	0x28			
#define ESDCTL_ESDCDLY4_OFFSET	0x2C			
#define ESDCTL_ESDCDLY5_OFFSET	0x30			
#define ESDCTL_ESDCDLGD_OFFSET	0x34			
//#########################################				
//# WEIM registers                    				
//#########################################				
#define WEIM_CS0_CTL_REG	0x000			
#define WEIM_CS0_CTL_REG2	0x004			
#define WEIM_CS0_RD_CTL_REG1	0x008			
#define WEIM_CS0_RD_CTL_REG2	0x00C			
#define WEIM_CS0_WR_CTL_REG	0x010			
#define WEIM_CS0_WR_CTL_REG2	0x014			
#define WEIM_CS1_CTL_REG	0x018			
#define WEIM_CS1_CTL_REG2	0x01C			
#define WEIM_CS1_RD_CTL_REG1	0x020			
#define WEIM_CS1_RD_CTL_REG2	0x024			
#define WEIM_CS1_WR_CTL_REG	0x028			
#define WEIM_CS1_WR_CTL_REG2	0x02C			
#define WEIM_CS2_CTL_REG	0x030			
#define WEIM_CS2_CTL_REG2	0x034			
#define WEIM_CS2_RD_CTL_REG1	0x038			
#define WEIM_CS2_RD_CTL_REG2	0x03c			
#define WEIM_CS2_WR_CTL_REG	0x040			
#define WEIM_CS2_WR_CTL_REG2	0x044			
#define WEIM_CS3_CTL_REG	0x048			
#define WEIM_CS3_CTL_REG2	0x04C			
#define WEIM_CS3_RD_CTL_REG1	0x050			
#define WEIM_CS3_RD_CTL_REG2	0x054			
#define WEIM_CS3_WR_CTL_REG	0x058			
#define WEIM_CS3_WR_CTL_REG2	0x05c			
#define WEIM_CS4_CTL_REG	0x060			
#define WEIM_CS4_CTL_REG2	0x064			
#define WEIM_CS4_RD_CTL_REG1	0x068			
#define WEIM_CS4_RD_CTL_REG2	0x06C			
#define WEIM_CS4_WR_CTL_REG	0x070			
#define WEIM_CS4_WR_CTL_REG2	0x074			
#define WEIM_CS5_CTL_REG	0x078			
#define WEIM_CS5_CTL_REG2	0x07c			
#define WEIM_CS5_RD_CTL_REG1	0x080			
#define WEIM_CS5_RD_CTL_REG2	0x084			
#define WEIM_CS5_WR_CTL_REG	0x088			
#define WEIM_CS5_WR_CTL_REG2	0x08c			
#define WEIM_CONFIG_REG	0x090			
#define WEIM_IP_ACCESS_REG	0x094			
#define WEIM_ERR_ADDR_REG	0x098			
//#########################################				
//# EMI Registers                     				
//#########################################				
				
//#########################################				
//# MARC                  				
//#########################################				
#define MARC_SCR_OFFSET	0x000			
#define MARC_IS_START_OFFSET	0x004			
#define MARC_IS_STOP_OFFSET	0x008			
#define MARC_GAIN_STEP_OFFSET	0x00C			
#define MARC_IS_LOCK_OFFSET	0x010			
#define MARC_WM_STAT_OFFSET	0x020			
#define MARC_INT_STAT_OFFSET	0x028			
#define MARC_DONE_INT_OFFSET	0x02C			
#define MARC_MCU_INT_STAT_OFFSET	0x030			
#define MARC_MCU_DMA_STAT_OFFSET	0x034			
#define MARC_DSP_INT_STAT_OFFSET	0x038			
#define MARC_DSP_DMA_STAT_OFFSET	0x03C			
#define MARC_MCU_DONE_INT_OFFSET	0x040			
#define MARC_DSP_DONE_INT_OFFSET	0x044			
#define MARC_SPE0_PARAM0_OFFSET	0x080			
#define MARC_SPE0_PARAM1_OFFSET	0x084			
#define MARC_SPE0_PARAM2_OFFSET	0x088			
#define MARC_SPE0_PARAM3_OFFSET	0x08C			
#define MARC_SPE0_PARAM4_OFFSET	0x090			
#define MARC_SPE1_PARAM0_OFFSET	0x0A0			
#define MARC_SPE1_PARAM1_OFFSET	0x0A4			
#define MARC_SPE1_PARAM2_OFFSET	0x0A8			
#define MARC_SPE1_PARAM3_OFFSET	0x0AC			
#define MARC_SPE1_PARAM4_OFFSET	0x0B0			
#define MARC_IS0_FIFO_OFFSET	0x100			
#define MARC_IS1_FIFO_OFFSET	0x104			
#define MARC_IS2_FIFO_OFFSET	0x108			
#define MARC_IS3_FIFO_OFFSET	0x10C			
#define MARC_IS4_FIFO_OFFSET	0x110			
#define MARC_IS5_FIFO_OFFSET	0x114			
#define MARC_IS6_FIFO_OFFSET	0x118			
#define MARC_IS7_FIFO_OFFSET	0x11C			
#define MARC_IS8_FIFO_OFFSET	0x120			
#define MARC_IS9_FIFO_OFFSET	0x124			
#define MARC_IS10_FIFO_OFFSET	0x128			
#define MARC_IS11_FIFO_OFFSET	0x12C			
#define MARC_IS12_FIFO_OFFSET	0x130			
#define MARC_IS13_FIFO_OFFSET	0x134			
#define MARC_IS14_FIFO_OFFSET	0x138			
#define MARC_IS15_FIFO_OFFSET	0x13C			
#define MARC_OS0_FIFO_OFFSET	0x180			
#define MARC_OS1_FIFO_OFFSET	0x184			
#define MARC_OS2_FIFO_OFFSET	0x188			
#define MARC_IS0_CR_OFFSET	0x200			
#define MARC_IS0_GAIN_OFFSET	0x204			
#define MARC_IS0_DELAY_OFFSET	0x208			
#define MARC_IS1_CR_OFFSET	0x210			
#define MARC_IS1_GAIN_OFFSET	0x214			
#define MARC_IS1_DELAY_OFFSET	0x218			
#define MARC_IS2_CR_OFFSET	0x220			
#define MARC_IS2_GAIN_OFFSET	0x224			
#define MARC_IS2_DELAY_OFFSET	0x228			
#define MARC_IS3_CR_OFFSET	0x230			
#define MARC_IS3_GAIN_OFFSET	0x234			
#define MARC_IS3_DELAY_OFFSET	0x238			
#define MARC_IS4_CR_OFFSET	0x240			
#define MARC_IS4_GAIN_OFFSET	0x244			
#define MARC_IS4_DELAY_OFFSET	0x248			
#define MARC_IS5_CR_OFFSET	0x250			
#define MARC_IS5_GAIN_OFFSET	0x254			
#define MARC_IS5_DELAY_OFFSET	0x258			
#define MARC_IS6_CR_OFFSET	0x260			
#define MARC_IS6_GAIN_OFFSET	0x264			
#define MARC_IS6_DELAY_OFFSET	0x268			
#define MARC_IS7_CR_OFFSET	0x270			
#define MARC_IS7_GAIN_OFFSET	0x274			
#define MARC_IS7_DELAY_OFFSET	0x278			
#define MARC_IS8_CR_OFFSET	0x280			
#define MARC_IS8_GAIN_OFFSET	0x284			
#define MARC_IS8_DELAY_OFFSET	0x288			
#define MARC_IS9_CR_OFFSET	0x290			
#define MARC_IS9_GAIN_OFFSET	0x294			
#define MARC_IS9_DELAY_OFFSET	0x298			
#define MARC_IS10_CR_OFFSET	0x2A0			
#define MARC_IS10_GAIN_OFFSET	0x2A4			
#define MARC_IS10_DELAY_OFFSET	0x2A8			
#define MARC_IS11_CR_OFFSET	0x2B0			
#define MARC_IS11_GAIN_OFFSET	0x2B4			
#define MARC_IS11_DELAY_OFFSET	0x2B8			
#define MARC_IS12_CR_OFFSET	0x2C0			
#define MARC_IS12_GAIN_OFFSET	0x2C4			
#define MARC_IS12_DELAY_OFFSET	0x2C8			
#define MARC_IS13_CR_OFFSET	0x2D0			
#define MARC_IS13_GAIN_OFFSET	0x2D4			
#define MARC_IS13_DELAY_OFFSET	0x2D8			
#define MARC_IS14_CR_OFFSET	0x2E0			
#define MARC_IS14_GAIN_OFFSET	0x2E4			
#define MARC_IS14_DELAY_OFFSET	0x2E8			
#define MARC_IS15_CR_OFFSET	0x2F0			
#define MARC_IS15_GAIN_OFFSET	0x2F4			
#define MARC_IS15_DELAY_OFFSET	0x2F8			
#define MARC_OS0_CR_OFFSET	0x300			
#define MARC_OS0_GAIN_OFFSET	0x304			
#define MARC_OS1_CR_OFFSET	0x310			
#define MARC_OS1_GAIN_OFFSET	0x314			
#define MARC_OS2_CR_OFFSET	0x320			
#define MARC_CD0_CR_OFFSET	0x340			
#define MARC_CD1_CR_OFFSET	0x350			
//#########################################				
//# LMLC                 				
//#########################################				
#define LMLC_CLKHKEEP_OFFSET	0x00			
#define LMLC_UID_OFFSET	0x04			
#define LMLC_TXCFG_OFFSET	0x08			
#define LMLC_RXCFG_OFFSET	0x0C			
#define LMLC_INTSTAT_OFFSET	0x10			
#define LMLC_INTEN_OFFSET	0x14			
//#########################################				
//#  FEC                         				
//#########################################				
#define FEC_EIR_OFFSET	0x004			
#define FEC_EIMR_OFFSET	0x008			
#define FEC_RDAR_OFFSET	0x010			
#define FEC_TDAR_OFFSET	0x014			
#define FEC_ECR_OFFSET	0x024			
#define FEC_MMFR_OFFSET	0x040			
#define FEC_MSCR_OFFSET	0x044			
#define FEC_MIBC_OFFSET	0x064			
#define FEC_RCR_OFFSET	0x084			
#define FEC_TCR_OFFSET	0x0C4			
#define FEC_PALR_OFFSET	0x0E4			
#define FEC_PAUR_OFFSET	0x0E8			
#define FEC_OPD_OFFSET	0x0EC			
#define FEC_IAUR_OFFSET	0x118			
#define FEC_IALR_OFFSET	0x11C			
#define FEC_GAUR_OFFSET	0x120			
#define FEC_GALR_OFFSET	0x124			
#define FEC_TFWR_OFFSET	0x144			
#define FEC_FRBR_OFFSET	0x14C			
#define FEC_FRSR_OFFSET	0x150			
#define FEC_ERDSR_OFFSET	0x180			
#define FEC_ETDSR_OFFSET	0x184			
#define FEC_EMRBR_OFFSET	0x188			
//#########################################				
//# AIPS 1 off platform global module enable no.1				
//#########################################				
#define AIPS_MPROT0_7_OFFSET	0x00			
#define AIPS_MPROT8_15_OFFSET	0x04			
#define AIPS_PACR0_7_OFFSET	0x20			
#define AIPS_PACR8_15_OFFSET	0x24			
#define AIPS_PACR16_23_OFFSET	0x28			
#define AIPS_PACR24_31_OFFSET	0x2C			
#define AIPS_OPACR0_7_OFFSET	0x40			
#define AIPS_OPACR8_15_OFFSET	0x44			
#define AIPS_OPACR16_23_OFFSET	0x48			
#define AIPS_OPACR24_31_OFFSET	0x4C			
#define AIPS_OPACR32_33_OFFSET	0x50			
//#########################################				
//# AHBMAX                				
//#########################################				
#define AHBMAX_MPR0_OFFSET	0x000			
#define AHBMAX_MPR1_OFFSET	0x100			
#define AHBMAX_MPR2_OFFSET	0x200			
#define AHBMAX_MPR3_OFFSET	0x300			
#define AHBMAX_MPR4_OFFSET	0x400			
#define AHBMAX_SGPCR0_OFFSET	0x010			
#define AHBMAX_SGPCR1_OFFSET	0x110			
#define AHBMAX_SGPCR2_OFFSET	0x210			
#define AHBMAX_SGPCR3_OFFSET	0x310			
#define AHBMAX_SGPCR4_OFFSET	0x410			
#define AHBMAX_MGPCR0_OFFSET	0x800			
#define AHBMAX_MGPCR1_OFFSET	0x900			
#define AHBMAX_MGPCR2_OFFSET	0xa00			
#define AHBMAX_MGPCR3_OFFSET	0xb00			
#define AHBMAX_MGPCR4_OFFSET	0xc00			
#define AHBMAX_MGPCR5_OFFSET	0xd00			
#define AHBMAX_MGPCR6_OFFSET	0xe00			
//#########################################				
//# SRTC               				
//#########################################				
#define SRTC_LPSCMR_OFFSET	0x0			
#define SRTC_LPSCLR_OFFSET	0x4			
#define SRTC_LPSAR_OFFSET	0x8			
#define SRTC_LPSMCR_OFFSET	0xC			
#define SRTC_LPCR_OFFSET	0x10			
#define SRTC_LPSR_OFFSET	0x14			
#define SRTC_LPPDR_OFFSET	0x18			
#define SRTC_LPGR_OFFSET	0x1c			
#define SRTC_HPCMR_OFFSET	0x20			
#define SRTC_HPCLR_OFFSET	0x24			
#define SRTC_HPAMR_OFFSET	0x28			
#define SRTC_HPALR_OFFSET	0x2c			
#define SRTC_HPCR_OFFSET	0x30			
#define SRTC_HPISR_OFFSET	0x34			
#define SRTC_HPIENR_OFFSET	0x38			
//#########################################				
//# IOMUXC               				
//#########################################				
#define IOMUXC_GPR0_OFFSET	0x00			
#define IOMUXC_GPR1_OFFSET	0x04			
//#########################################				
//#  CSU                     				
//#########################################				
#define CSU_CSL0_OFFSET	0x0			
#define CSU_CSL1_OFFSET	0x4			
#define CSU_CSL2_OFFSET	0x8			
#define CSU_CSL3_OFFSET	0xC			
#define CSU_CSL4_OFFSET	0x10			
#define CSU_CSL5_OFFSET	0x14			
#define CSU_CSL6_OFFSET	0x18			
#define CSU_CSL7_OFFSET	0x1C			
#define CSU_CSL8_OFFSET	0x20			
#define CSU_CSL9_OFFSET	0x24			
#define CSU_CSL10_OFFSET	0x28			
#define CSU_CSL11_OFFSET	0x2C			
#define CSU_CSL12_OFFSET	0x30			
#define CSU_CSL13_OFFSET	0x34			
#define CSU_CSL14_OFFSET	0x38			
#define CSU_CSL15_OFFSET	0x3C			
#define CSU_CSL16_OFFSET	0x40			
#define CSU_CSL17_OFFSET	0x44			
#define CSU_CSL18_OFFSET	0x48			
#define CSU_CSL19_OFFSET	0x4C			
#define CSU_CSL20_OFFSET	0x50			
#define CSU_CSL21_OFFSET	0x54			
#define CSU_CSL22_OFFSET	0x58			
#define CSU_CSL23_OFFSET	0x5C			
#define CSU_CSL24_OFFSET	0x60			
#define CSU_CSL25_OFFSET	0x64			
#define CSU_CSL26_OFFSET	0x68			
#define CSU_CSL27_OFFSET	0x6C			
#define CSU_CSL28_OFFSET	0x70			
#define CSU_CSL29_OFFSET	0x74			
#define CSU_CSL30_OFFSET	0x78			
#define CSU_CSL31_OFFSET	0x7C			
#define CSU_CSL32_OFFSET	0x80			
#define CSU_CSL33_OFFSET	0x84			
#define CSU_CSL34_OFFSET	0x88			
#define CSU_CSL35_OFFSET	0x8C			
#define CSU_CSL36_OFFSET	0x90			
#define CSU_CSL37_OFFSET	0x94			
#define CSU_CSL38_OFFSET	0x98			
#define CSU_CSL39_OFFSET	0x9C			
#define CSU_CSL40_OFFSET	0xA0			
#define CSU_CSL41_OFFSET	0xA4			
#define CSU_CSL42_OFFSET	0xA8			
#define CSU_CSL43_OFFSET	0xAC			
#define CSU_CSL44_OFFSET	0xB0			
#define CSU_CSL45_OFFSET	0xB4			
#define CSU_CSL46_OFFSET	0xB8			
#define CSU_CSL47_OFFSET	0xBC			
#define CSU_CSL48_OFFSET	0xC0			
#define CSU_CSL49_OFFSET	0xC4			
#define CSU_CSL50_OFFSET	0xC8			
#define CSU_CSL51_OFFSET	0xCC			
#define CSU_CSL52_OFFSET	0xD0			
#define CSU_CSL53_OFFSET	0xD4			
#define CSU_CSL54_OFFSET	0xD8			
#define CSU_CSL55_OFFSET	0xDC			
#define CSU_CSL56_OFFSET	0xE0			
#define CSU_CSL57_OFFSET	0xE4			
#define CSU_CSL58_OFFSET	0xE8			
#define CSU_CSL59_OFFSET	0xEC			
#define CSU_CSL60_OFFSET	0xF0			
#define CSU_CSL61_OFFSET	0xF4			
#define CSU_CSL62_OFFSET	0xF8			
#define CSU_CSL63_OFFSET	0xFC			
#define CSU_HP0_OFFSET	0x200			
#define CSU_HP1_OFFSET	0x204			
#define CSU_SA0_OFFSET	0x218			
#define CSU_SA1_OFFSET	0x21C			
#define CSU_AMASK0_OFFSET	0x230			
#define CSU_AMASK1_OFFSET	0x234			
#define CSU_AROUT0_OFFSET	0x244			
#define CSU_AROUT1_OFFSET	0x248			
#define CSU_AROUT2_OFFSET	0x24C			
#define CSU_AROUT3_OFFSET	0x250			
#define CSU_AROUT4_OFFSET	0x254			
#define CSU_AROUT5_OFFSET	0x258			
#define CSU_AROUT6_OFFSET	0x25C			
#define CSU_AROUT7_OFFSET	0x260			
#define CSU_AROUT8_OFFSET	0x264			
#define CSU_AROUT9_OFFSET	0x268			
#define CSU_AROUT10_OFFSET	0x26C			
#define CSU_AROUT11_OFFSET	0x270			
#define CSU_AROUT12_OFFSET	0x274			
#define CSU_AROUT13_OFFSET	0x278			
#define CSU_AROUT14_OFFSET	0x27C			
#define CSU_AROUT15_OFFSET	0x280			
#define CSU_AROUT16_OFFSET	0x284			
#define CSU_AROUT17_OFFSET	0x288			
#define CSU_AROUT18_OFFSET	0x28C			
#define CSU_AROUT19_OFFSET	0x290			
#define CSU_AROUT20_OFFSET	0x294			
#define CSU_AROUT21_OFFSET	0x298			
#define CSU_AROUT22_OFFSET	0x29C			
#define CSU_AROUT23_OFFSET	0x2A0			
#define CSU_AROUT24_OFFSET	0x2A4			
#define CSU_AROUT25_OFFSET	0x2A8			
#define CSU_AROUT26_OFFSET	0x2AC			
#define CSU_AROUT27_OFFSET	0x2B0			
#define CSU_AROUT28_OFFSET	0x2B4			
#define CSU_AROUT29_OFFSET	0x2B8			
#define CSU_AROUT30_OFFSET	0x2BC			
#define CSU_AROUT31_OFFSET	0x2C0			
#define CSU_ASOFT0_OFFSET	0x344			
#define CSU_ACOUNTER0_OFFSET	0x348			
#define CSU_ACONTROL0_OFFSET	0x34C			
#define CSU_HPCONTROL0_OFFSET	0x358			
#define CSU_HPCONTROL1_OFFSET	0x35C			
#define CSU_ICR0_OFFSET	0x368			
#define CSU_ICR1_OFFSET	0x36C			
#define CSU_ISR0_OFFSET	0x378			
#define CSU_ISR1_OFFSET	0x37C			
//#########################################				
//# SDMA(pors IPS_HOST)            				
//#########################################				
#define SDMA_MC0PTR_OFFSET         	0x00			
#define SDMA_INTR_OFFSET          	0x04			
#define SDMA_STOP_STAT_OFFSET      	0x08			
#define SDMA_HSTART_OFFSET         	0x0C			
#define SDMA_EVTOVR_OFFSET         	0x10			
#define SDMA_DSPOVR_OFFSET         	0x14			
#define SDMA_HOSTOVR_OFFSET        	0x18			
#define SDMA_EVTPEND_OFFSET        	0x1C			
#define SDMA_DSPENBL_OFFSET       	0x20			
#define SDMA_RESET_OFFSET          	0x24			
#define SDMA_EVTERR_OFFSET         	0x28			
#define SDMA_INTRMASK_OFFSET       	0x2C			
#define SDMA_PSW_OFFSET            	0x30			
#define SDMA_EVTERRDBG_OFFSET     	0x34			
#define SDMA_CONFIG_OFFSET        	0x38			
#define SDMA_LOCK_OFFSET           	0x3C			
#define SDMA_ONCE_ENB_OFFSET       	0x40			
#define SDMA_ONCE_DATA_OFFSET      	0x44			
#define SDMA_ONCE_INSTR_OFFSET     	0x48			
#define SDMA_ONCE_STAT_OFFSET      	0x4C			
#define SDMA_ONCE_CMD_OFFSET       	0x50			
#define SDMA_ILLINSTADDR_OFFSET    	0x58			
#define SDMA_CHN0ADDR_OFFSET       	0x5C			
#define SDMA_EVT_MIRROR_OFFSET     	0x60			
#define SDMA_EVT_MIRROR2_OFFSET    	0x64			
#define SDMA_XTRIG_CONF1_OFFSET    	0x70			
#define SDMA_XTRIG_CONF2_OFFSET    	0x74			
#define SDMA_CHNPRI00_OFFSET      	0x100			
#define SDMA_CHNPRI01_OFFSET       	0x104			
#define SDMA_CHNPRI02_OFFSET      	0x108			
#define SDMA_CHNPRI03_OFFSET      	0x10C			
#define SDMA_CHNPRI04_OFFSET       	0x110			
#define SDMA_CHNPRI05_OFFSET       	0x114			
#define SDMA_CHNPRI06_OFFSET      	0x118			
#define SDMA_CHNPRI07_OFFSET       	0x11C			
#define SDMA_CHNPRI08_OFFSET       	0x120			
#define SDMA_CHNPRI09_OFFSET       	0x124			
#define SDMA_CHNPRI10_OFFSET      	0x128			
#define SDMA_CHNPRI11_OFFSET      	0x12C			
#define SDMA_CHNPRI12_OFFSET       	0x130			
#define SDMA_CHNPRI13_OFFSET       	0x134			
#define SDMA_CHNPRI14_OFFSET       	0x138			
#define SDMA_CHNPRI15_OFFSET       	0x13C			
#define SDMA_CHNPRI16_OFFSET       	0x140			
#define SDMA_CHNPRI17_OFFSET       	0x144			
#define SDMA_CHNPRI18_OFFSET       	0x148			
#define SDMA_CHNPRI19_OFFSET       	0x14C			
#define SDMA_CHNPRI20_OFFSET       	0x150			
#define SDMA_CHNPRI21_OFFSET       	0x154			
#define SDMA_CHNPRI22_OFFSET       	0x158			
#define SDMA_CHNPRI23_OFFSET       	0x15C			
#define SDMA_CHNPRI24_OFFSET       	0x160			
#define SDMA_CHNPRI25_OFFSET       	0x164			
#define SDMA_CHNPRI26_OFFSET       	0x168			
#define SDMA_CHNPRI27_OFFSET       	0x16C			
#define SDMA_CHNPRI28_OFFSET       	0x170			
#define SDMA_CHNPRI29_OFFSET       	0x174			
#define SDMA_CHNPRI30_OFFSET       	0x178			
#define SDMA_CHNPRI31_OFFSET       	0x17C			
#define SDMA_CHNENBL00_OFFSET       	0x200			
#define SDMA_CHNENBL01_OFFSET       	0x204			
#define SDMA_CHNENBL02_OFFSET       	0x208			
#define SDMA_CHNENBL03_OFFSET       	0x20C			
#define SDMA_CHNENBL04_OFFSET       	0x210			
#define SDMA_CHNENBL05_OFFSET       	0x214			
#define SDMA_CHNENBL06_OFFSET       	0x218			
#define SDMA_CHNENBL07_OFFSET       	0x21C			
#define SDMA_CHNENBL08_OFFSET       	0x220			
#define SDMA_CHNENBL09_OFFSET       	0x224			
#define SDMA_CHNENBL10_OFFSET       	0x228			
#define SDMA_CHNENBL11_OFFSET       	0x22C			
#define SDMA_CHNENBL12_OFFSET       	0x230			
#define SDMA_CHNENBL13_OFFSET       	0x234			
#define SDMA_CHNENBL14_OFFSET       	0x238			
#define SDMA_CHNENBL15_OFFSET       	0x23C			
#define SDMA_CHNENBL16_OFFSET       	0x240			
#define SDMA_CHNENBL17_OFFSET       	0x244			
#define SDMA_CHNENBL18_OFFSET       	0x248			
#define SDMA_CHNENBL19_OFFSET       	0x24C			
#define SDMA_CHNENBL20_OFFSET       	0x250			
#define SDMA_CHNENBL21_OFFSET       	0x254			
#define SDMA_CHNENBL22_OFFSET       	0x258			
#define SDMA_CHNENBL23_OFFSET       	0x25C			
#define SDMA_CHNENBL24_OFFSET       	0x260			
#define SDMA_CHNENBL25_OFFSET       	0x264			
#define SDMA_CHNENBL26_OFFSET       	0x268			
#define SDMA_CHNENBL27_OFFSET       	0x26C			
#define SDMA_CHNENBL28_OFFSET       	0x270			
#define SDMA_CHNENBL29_OFFSET       	0x274			
#define SDMA_CHNENBL30_OFFSET       	0x278			
#define SDMA_CHNENBL31_OFFSET       	0x27C			
#define SDMA_CHNENBL32_OFFSET       	0x280			
#define SDMA_CHNENBL33_OFFSET       	0x284			
#define SDMA_CHNENBL34_OFFSET       	0x288			
#define SDMA_CHNENBL35_OFFSET       	0x28C			
#define SDMA_CHNENBL36_OFFSET       	0x290			
#define SDMA_CHNENBL37_OFFSET       	0x294			
#define SDMA_CHNENBL38_OFFSET       	0x298			
#define SDMA_CHNENBL39_OFFSET       	0x29C			
#define SDMA_CHNENBL40_OFFSET       	0x2A0			
#define SDMA_CHNENBL41_OFFSET       	0x2A4			
#define SDMA_CHNENBL42_OFFSET       	0x2A8			
#define SDMA_CHNENBL43_OFFSET       	0x2AC			
#define SDMA_CHNENBL44_OFFSET       	0x2B0			
#define SDMA_CHNENBL45_OFFSET       	0x2B4			
#define SDMA_CHNENBL46_OFFSET       	0x2B8			
#define SDMA_CHNENBL47_OFFSET       	0x2BC			
//#########################################				
//# SAHARA                   				
//#########################################				
#define SAHARA_VERID_OFFSET	0x000			
#define SAHARA_DAR_OFFSET	0x004			
#define SAHARA_CONTROL_OFFSET	0x008			
#define SAHARA_COMMAND_OFFSET	0x00C			
#define SAHARA_STATUS_OFFSET	0x010			
#define SAHARA_ERRSTAT_OFFSET	0x014			
#define SAHARA_FAR_OFFSET	0x018			
#define SAHARA_CDAR_OFFSET	0x01C			
#define SAHARA_IDAR_OFFSET	0x020			
#define SAHARA_BLVL_OFFSET	0x024			
#define SAHARA_OPSTAT_OFFSET	0x028			
#define SAHARA_CONFIG_OFFSET	0x02C			
#define SAHARA_MMSTAT_OFFSET	0x030			
#define SAHARA_DESC_OFFSET	0x080			
#define SAHARA_DESC_A_OFFSET	0x080			
#define SAHARA_DESC_B_OFFSET	0x084			
#define SAHARA_DESC_C_OFFSET	0x088			
#define SAHARA_DESC_D_OFFSET	0x08C			
#define SAHARA_DESC_E_OFFSET	0x090			
#define SAHARA_DESC_F_OFFSET	0x094			
#define SAHARA_LL1_OFFSET	0x0A0			
#define SAHARA_LL1_A_OFFSET	0x0A0			
#define SAHARA_LL1_B_OFFSET	0x0A4			
#define SAHARA_LL1_C_OFFSET	0x0A8			
#define SAHARA_LL2_OFFSET	0x0B0			
#define SAHARA_LL2_A_OFFSET	0x0B0			
#define SAHARA_LL2_B_OFFSET	0x0B4			
#define SAHARA_LL2_C_OFFSET	0x0B8			
#define SAHARA_INTFC_OFFSET	0x0C0			
#define SAHARA_TAR_OFFSET	0x0FC			
#define SAHARA_SMODE_OFFSET	0x100			
#define SAHARA_SKEYSIZE_OFFSET	0x104			
#define SAHARA_SDATASIZE_OFFSET	0x108			
#define SAHARA_SSTAT_OFFSET	0x10C			
#define SAHARA_SERRSTAT_OFFSET	0x110			
#define SAHARA_SEOM_OFFSET	0x114			
#define SAHARA_SVERID_OFFSET	0x118			
#define SAHARA_SCONTEXT_OFFSET	0x140			
#define SAHARA_SKEY_OFFSET	0x180			
#define SAHARA_MMODE_OFFSET	0x200			
#define SAHARA_MKEYSIZE_OFFSET	0x204			
#define SAHARA_MDATASIZE_OFFSET	0x208			
#define SAHARA_MSTAT_OFFSET	0x20c			
#define SAHARA_MERRSTAT_OFFSET	0x210			
#define SAHARA_MEOM_OFFSET	0x214			
#define SAHARA_MVERID_OFFSET	0x218			
#define SAHARA_MDIG_OFFSET	0x240			
#define SAHARA_MKEY_OFFSET	0x280			
#define SAHARA_RMODE_OFFSET	0x300			
#define SAHARA_RDATASIZE_OFFSET	0x308			
#define SAHARA_RSTAT_OFFSET	0x30c			
#define SAHARA_RERRSTAT_OFFSET	0x310			
#define SAHARA_REOM_OFFSET	0x314			
#define SAHARA_RVERID_OFFSET	0x318			
#define SAHARA_RVERIF_OFFSET	0x340			
#define SAHARA_RENTROPY_OFFSET	0x380			
#define SAHARA_IFIFO_OFFSET	0x400			
#define SAHARA_OFIFO_OFFSET	0x500			
#define SAHARA_SBOX_OFFSET	0x600			
#define SAHARA_PMODE_OFFSET	0x800			
#define SAHARA_PKEYSIZE_OFFSET	0x804			
#define SAHARA_PDATASIZE_OFFSET	0x808			
#define SAHARA_PSTAT_OFFSET	0x80c			
#define SAHARA_PERRSTAT_OFFSET	0x810			
#define SAHARA_PEOM_OFFSET	0x814			
#define SAHARA_PVERID_OFFSET	0x818			
#define SAHARA_PCONTROL_OFFSET	0x81c			
#define SAHARA_PABSIZE_OFFSET	0x820			
#define SAHARA_PSTATHIGH_OFFSET	0x824			
#define SAHARA_ARAM_OFFSET	0xc00			
#define SAHARA_A0RAM_OFFSET	0xc00			
#define SAHARA_A1RAM_OFFSET	0xc40			
#define SAHARA_A2RAM_OFFSET	0xc80			
#define SAHARA_A3RAM_OFFSET	0xcc0			
#define SAHARA_BRAM_OFFSET	0xd00			
#define SAHARA_B0RAM_OFFSET	0xd00			
#define SAHARA_B1RAM_OFFSET	0xd40			
#define SAHARA_B2RAM_OFFSET	0xd80			
#define SAHARA_B3RAM_OFFSET	0xdc0			
#define SAHARA_NRAM_OFFSET	0xe00			
#define SAHARA_ERAM_OFFSET	0xf00			
//#########################################				
//# SCC(via IPSYNC)         				
//#########################################				
#define SMN_STATUS_OFFSET	0x100	// TBD - no mnemonic name at spec		
#define SMN_COMMAND_OFFSET	0x104	// TBD - no mnemonic name at spec		
#define SMN_SEQSTART_OFFSET	0x108	// TBD - no mnemonic name at spec		
#define SMN_SEQEND_OFFSET	0x10C	// TBD - no mnemonic name at spec		
#define SMN_SEQCHK_OFFSET	0x110	// TBD - no mnemonic name at spec		
#define SMN_BITCNT_OFFSET	0x114	// TBD - no mnemonic name at spec		
#define SMN_INCSIZE_OFFSET	0x115	// TBD - no mnemonic name at spec		
#define SMN_BBDEC_OFFSET	0x11C	// TBD - no mnemonic name at spec		
#define SMN_COMPSIZE_OFFSET	0x120	// TBD - no mnemonic name at spec		
#define SMN_PTCHK_OFFSET	0x124	// TBD - no mnemonic name at spec		
#define SMN_CTCHK_OFFSET	0x128	// TBD - no mnemonic name at spec		
#define SMN_TIMERIV_OFFSET	0x12C	// TBD - no mnemonic name at spec		
#define SMN_TIMERCTL_OFFSET	0x130	// TBD - no mnemonic name at spec		
#define SMN_DDSTATUS_OFFSET	0x134	// TBD - no mnemonic name at spec		
#define SMN_TIMER_OFFSET	0x138	// TBD - no mnemonic name at spec		
#define SCC_ALL	0xF67			
#define SCC_DNZ	0x80000000			
#define SCC_SCTL_SMID0_OFFSET	0x080			
#define SCC_SCTL_SMID1_OFFSET	0x088			
#define SCC_SCTL_SMID2_OFFSET	0x090			
#define SCC_SCTL_SMID3_OFFSET	0x098			
#define SCC_SCTL_SMID4_OFFSET	0x0A0			
#define SCC_SCTL_SMID5_OFFSET	0x0A8			
#define SCC_SCTL_SMID6_OFFSET	0x0B0			
#define SCC_SCTL_SMID7_OFFSET	0x0B8			
#define SCC_SCTL_SMID8_OFFSET	0x0C0			
#define SCC_PARTITION0_OFFSET	0x0			
#define SCC_PARTITION1_OFFSET	0x2000			
#define SCC_PARTITION2_OFFSET	0x4000			
#define SCC_PARTITION3_OFFSET	0x6000			
#define SCC_PARTITION4_OFFSET	0x8000			
#define SCC_PARTITION5_OFFSET	0xa000			
#define SCC_PARTITION6_OFFSET	0xc000			
#define SCC_PARTITION7_OFFSET	0xe000			
#define SCC_PARTITION8_OFFSET	0x10000			
//#########################################				
//# ROMCP(via IPSYNC)    				
//#########################################				
#define ROMCP_CNTL_OFFSET	0x00F4			
#define ROMCP_ENL_OFFSET	0x00FC			
#define ROMCP_ENH_OFFSET	0x00F8			
#define ROMCP_STAT_OFFSET	0x0208			
#define ROMCP_ADDR0_OFFSET	0x0100			
#define ROMCP_ADDR1_OFFSET	0x0104			
#define ROMCP_ADDR2_OFFSET	0x0108			
#define ROMCP_ADDR3_OFFSET	0x010C			
#define ROMCP_ADDR4_OFFSET	0x0110			
#define ROMCP_ADDR5_OFFSET	0x0114			
#define ROMCP_ADDR6_OFFSET	0x0118			
#define ROMCP_ADDR7_OFFSET	0x011C			
#define ROMCP_ADDR8_OFFSET	0x0120			
#define ROMCP_ADDR9_OFFSET	0x0124			
#define ROMCP_ADDRA_OFFSET	0x0128			
#define ROMCP_ADDRB_OFFSET	0x012C			
#define ROMCP_ADDRC_OFFSET	0x0130			
#define ROMCP_ADDRD_OFFSET	0x0134			
#define ROMCP_ADDRE_OFFSET	0x0138			
#define ROMCP_ADDRF_OFFSET	0x013C			
#define ROMCP_DATA0_OFFSET	0x00F0			
#define ROMCP_DATA1_OFFSET	0x00EC			
#define ROMCP_DATA2_OFFSET	0x00E8			
#define ROMCP_DATA3_OFFSET	0x00E4			
#define ROMCP_DATA4_OFFSET	0x00E0			
#define ROMCP_DATA5_OFFSET	0x00DC			
#define ROMCP_DATA6_OFFSET	0x00D8			
#define ROMCP_DATA7_OFFSET	0x00D4			
//#########################################				
//# RTIC             				
//#########################################				
#define RTIC_STATUS_OFFSET	0x0000			
#define RTIC_CMD_OFFSET	0x0004			
#define RTIC_CTRL_OFFSET	0x0008			
#define RTIC_DMATHROTTLE_OFFSET	0x000C			
#define RTIC_MEMBLOCKA1_OFFSET	0x0010			
#define RTIC_MEMBLOCKA2_OFFSET	0x0018			
#define RTIC_MEMBLOCKB1_OFFSET	0x0030			
#define RTIC_MEMBLOCKB2_OFFSET	0x0038			
#define RTIC_MEMBLOCKC1_OFFSET	0x0050			
#define RTIC_MEMBLOCKC2_OFFSET	0x0058			
#define RTIC_MEMBLOCKD1_OFFSET	0x0070			
#define RTIC_MEMBLOCKD2_OFFSET	0x0078			
#define RTIC_MEMLENA1_OFFSET	0x0014			
#define RTIC_MEMLENA2_OFFSET	0x001C			
#define RTIC_MEMLENB1_OFFSET	0x0034			
#define RTIC_MEMLENB2_OFFSET	0x003C			
#define RTIC_MEMLENC1_OFFSET	0x0054			
#define RTIC_MEMLENC2_OFFSET	0x005C			
#define RTIC_MEMLEND1_OFFSET	0x0074			
#define RTIC_MEMLEND2_OFFSET	0x007C			
#define RTIC_FAULT_OFFSET	0x0090			
#define RTIC_WDOG_OFFSET	0x0094			
#define RTIC_VERID_OFFSET	0x0098			
#define RTIC_HASHA_A_OFFSET	0x00A0			
#define RTIC_HASHA_B_OFFSET	0xA4			
#define RTIC_HASHA_C_OFFSET	0xA8			
#define RTIC_HASHA_D_OFFSET	0xAC			
#define RTIC_HASHA_E_OFFSET	0xB0			
#define RTIC_HASHA_F_OFFSET	0xB4			
#define RTIC_HASHA_G_OFFSET	0xB8			
#define RTIC_HASHA_H_OFFSET	0xBC			
#define RTIC_HASHB_A_OFFSET	0x00C0			
#define RTIC_HASHB_B_OFFSET	0xC4			
#define RTIC_HASHB_C_OFFSET	0xC8			
#define RTIC_HASHB_D_OFFSET	0xCC			
#define RTIC_HASHB_E_OFFSET	0xD0			
#define RTIC_HASHB_F_OFFSET	0xD4			
#define RTIC_HASHB_G_OFFSET	0xD8			
#define RTIC_HASHB_H_OFFSET	0xDC			
#define RTIC_HASHC_A_OFFSET	0x00E0			
#define RTIC_HASHC_B_OFFSET	0xE4			
#define RTIC_HASHC_C_OFFSET	0xE8			
#define RTIC_HASHC_D_OFFSET	0xEC			
#define RTIC_HASHC_E_OFFSET	0xF0			
#define RTIC_HASHC_F_OFFSET	0xF4			
#define RTIC_HASHC_G_OFFSET	0xF8			
#define RTIC_HASHC_H_OFFSET	0xFC			
#define RTIC_HASHD_A_OFFSET	0x0100			
#define RTIC_HASHD_B_OFFSET	0x104			
#define RTIC_HASHD_C_OFFSET	0x108			
#define RTIC_HASHD_D_OFFSET	0x10C			
#define RTIC_HASHD_E_OFFSET	0x110			
#define RTIC_HASHD_F_OFFSET	0x114			
#define RTIC_HASHD_G_OFFSET	0x118			
#define RTIC_HASHD_H_OFFSET	0x11c			
//#########################################				
//#  MU(port IPS_MCU)     				
//#########################################				
#define MU_MTR0_OFFSET	0x00			
#define MU_MTR1_OFFSET	0x04			
#define MU_MTR2_OFFSET	0x08			
#define MU_MTR3_OFFSET	0x0C			
#define MU_MRR0_OFFSET	0x10			
#define MU_MRR1_OFFSET	0x14			
#define MU_MRR2_OFFSET	0x18			
#define MU_MRR3_OFFSET	0x1C			
#define MU_MSR_OFFSET	0x20			
#define MU_MCR_OFFSET	0x24			
//#########################################				
//#  SDDC               				
//#########################################				
#define SDDC_DATA_PORT_OFFSET	0x00			
#define SDDC_IRQ_ID_OFFSET	0x04			
#define SDDC_CIS_FUN0_ADDR_OFFSET	0x08			
#define SDDC_CIS_FUN1_ADDR_OFFSET	0x0C			
#define SDDC_CSA_ADDR_OFFSET	0x10			
#define SDDC_RD_ADDR_OFFSET	0x14			
#define SDDC_WR_ADDR_OFFSET	0x18			
#define SDDC_AHB_TRANS_CNT_OFFSET	0x1C			
#define SDDC_SDDC_TRANS_CNT_OFFSET	0x20			
#define SDDC_CIA_REG_OFFSET	0x24			
#define SDDC_PGM_REG_OFFSET	0x28			
#define SDDC_IRQ_STAT_OFFSET	0x2C			
#define SDDC_IRQ_EN_OFFSET	0x30			
#define SDDC_OCR_REG_OFFSET	0x34			
#define SDDC_CLK_WKUP_OFFSET	0x38			
//#########################################				
//#  VPU                    				
//#########################################				
				
//#########################################				
//# USB           				
//#########################################				
#define USB_UOG_HWGENERAL_OFFSET	0x004			
#define USB_UOG_HWHOST_OFFSET	0x008			
#define USB_UOG_HWTXBUF_OFFSET	0x010			
#define USB_UOG_HWRXBUF_OFFSET	0x014			
#define USB_UOG_GPTIMER0LD_OFFSET	0x080			
#define USB_UOG_GPTIMER0CTRL_OFFSET	0x084			
#define USB_UOG_GPTIMER1LD_OFFSET	0x088			
#define USB_UOG_GPTIMER1CTRL_OFFSET	0x08c			
#define USB_UOG_CAPLENGTH_OFFSET	0x100			
#define USB_UOG_HCIVERSION_OFFSET	0x102			
#define USB_UOG_HCSPARAMS_OFFSET	0x104			
#define USB_UOG_HCCPARAMS_OFFSET	0x108			
#define USB_UOG_DCIVERSION_OFFSET	0x120			
#define USB_UOG_DCCPARAMS_OFFSET	0x124			
#define USB_UOG_USBCMD_OFFSET	0x140			
#define USB_UOG_USBSTS_OFFSET	0x144			
#define USB_UOG_USBINTR_OFFSET	0x148			
#define USB_UOG_FRINDEX_OFFSET	0x14C			
#define USB_UOG_PERIODICLISTBASE_OFFSET	0x154			
#define USB_UOG_ASYNCLISTADDR_OFFSET	0x158			
#define USB_UOG_BURSTSIZE_OFFSET	0x160			
#define USB_UOG_TXFILLTUNING_OFFSET	0x164			
#define USB_UOG_ULPIVIEW_OFFSET	0x170			
#define USB_UOG_CFGFLAG_OFFSET	0x180			
#define USB_UOG_PORTSC1_OFFSET	0x184			
#define USB_UOG_OTGSC_OFFSET	0x1A4			
#define USB_UOG_USBMODE_OFFSET	0x1A8			
#define USB_UOG_ENDPTSETUPSTAT_OFFSET	0x1AC			
#define USB_UOG_ENDPTPRIME_OFFSET	0x1B0			
#define USB_UOG_ENDPTFLUSH_OFFSET	0x1B4			
#define USB_UOG_ENDPTSTAT_OFFSET	0x1B8			
#define USB_UOG_ENDPTCOMPLETE_OFFSET	0x1BC			
#define USB_ENDPTCTRL0_OFFSET	0x1C0			
#define USB_ENDPTCTRL1_OFFSET	0x1C4			
#define USB_ENDPTCTRL2_OFFSET	0x1C8			
#define USB_ENDPTCTRL3_OFFSET	0x1CC			
#define USB_ENDPTCTRL4_OFFSET	0x1D0			
#define USB_ENDPTCTRL5_OFFSET	0x1D4			
#define USB_ENDPTCTRL6_OFFSET	0x1D8			
#define USB_ENDPTCTRL7_OFFSET	0x1DC			
#define USB_UH1_ID_OFFSET	0x200			
#define USB_UH1_HWGENERAL_OFFSET	0x204			
#define USB_UH1_HWHOST_OFFSET	0x208			
#define USB_UH1_HWTXBUF_OFFSET	0x210			
#define USB_UH1_HWRXBUF_OFFSET	0x214			
#define USB_UH1_GPTIMER0LD_OFFSET	0x280			
#define USB_UH1_GPTIMER0CTRL_OFFSET	0x284			
#define USB_UH1_GPTIMER1LD_OFFSET	0x288			
#define USB_UH1_GPTIMER1CTRL_OFFSET	0x28c			
#define USB_UH1_CAPLENGTH_OFFSET	0x300			
#define USB_UH1_HCIVERSION_OFFSET	0x302			
#define USB_UH1_HCSPARAMS_OFFSET	0x304			
#define USB_UH1_HCCPARAMS_OFFSET	0x308			
#define USB_UH1_USBCMD_OFFSET	0x340			
#define USB_UH1_USBSTS_OFFSET	0x344			
#define USB_UH1_USBINTR_OFFSET	0x348			
#define USB_UH1_FRINDEX_OFFSET	0x34C			
#define USB_UH1_PERIODICLISTBASE_OFFSET	0x354			
#define USB_UH1_ASYNCLISTADDR_OFFSET	0x358			
#define USB_UH1_BURSTSIZE_OFFSET	0x360			
#define USB_UH1_TXFILLTUNING_OFFSET	0x364			
#define USB_UH1_ULPIVIEW_OFFSET	0x370			
#define USB_UH1_PORTSC1_OFFSET	0x384			
#define USB_UH1_USBMODE_OFFSET	0x3A8			
#define USB_UH2_ID_OFFSET	0x400			
#define USB_UH2_HWGENERAL_OFFSET	0x404			
#define USB_UH2_HWHOST_OFFSET	0x408			
#define USB_UH2_HWTXBUF_OFFSET	0x410			
#define USB_UH2_HWRXBUF_OFFSET	0x414			
#define USB_UH2_GPTIMER0LD_OFFSET	0x480			
#define USB_UH2_GPTIMER0CTRL_OFFSET	0x484			
#define USB_UH2_GPTIMER1LD_OFFSET	0x488			
#define USB_UH2_GPTIMER1CTRL_OFFSET	0x48c			
#define USB_UH2_CAPLENGTH_OFFSET	0x500			
#define USB_UH2_HCIVERSION_OFFSET	0x502			
#define USB_UH2_HCSPARAMS_OFFSET	0x504			
#define USB_UH2_HCCPARAMS_OFFSET	0x508			
#define USB_UH2_USBCMD_OFFSET	0x540			
#define USB_UH2_USBSTS_OFFSET	0x544			
#define USB_UH2_USBINTR_OFFSET	0x548			
#define USB_UH2_FRINDEX_OFFSET	0x54C			
#define USB_UH2_PERIODICLISTBASE_OFFSET	0x554			
#define USB_UH2_ASYNCLISTADDR_OFFSET	0x558			
#define USB_UH2_BURSTSIZE_OFFSET	0x560			
#define USB_UH2_TXFILLTUNING_OFFSET	0x564			
#define USB_UH2_ULPIVIEW_OFFSET	0x570			
#define USB_UH2_PORTSC1_OFFSET	0x584			
#define USB_UH2_USBMODE_OFFSET	0x5A8			
#define USB_USB_CTRL_OFFSET	0x600			
#define USB_USB_OTG_MIRROR_OFFSET	0x604			
#define USB_PHY_CTRL_0_OFFSET	0x608			
#define USB_PHY_CTRL_1_OFFSET	0x60c			
//#########################################				
//#SJC              				
//#########################################				
#define SJC_GPUSR1_OFFSET	0x00			
#define SJC_GPUSR2_OFFSET	0x01			
#define SJC_GPUSR3_OFFSET	0x02			
#define SJC_GPSSR_OFFSET	0x03			
#define SJC_DCR_OFFSET	0x04			
#define SJC_SSR_OFFSET	0x05			
#define SJC_CPCR_OFFSET	0x06			
#define SJC_GPCCR_OFFSET	0x07			
#define SJC_PLLBR_OFFSET	0x08			
#define SJC_GPUCR1_OFFSET	0x09			
#define SJC_GPUCR2_OFFSET	0x0A			
#define SJC_GPUCR3_OFFSET	0x0B			
#define SJC_GPSCR_OFFSET	0x0C			
#define SJC_TESTREG_OFFSET	0x0D			
#define SJC_SASR_OFFSET	0x0E			
#define SJC_BISTCR1_OFFSET	0x0F			
#define SJC_BISTCR2_OFFSET	0x10			
#define SJC_BISTCR3_OFFSET	0x11			
#define SJC_BISTCR4_OFFSET	0x12			
#define SJC_BISTCR5_OFFSET	0x13			
#define SJC_BISTCR6_OFFSET	0x14			
#define SJC_BISTCR7_OFFSET	0x15			
#define SJC_MBISTPASSR1_OFFSET	0x16			
#define SJC_MBISTPASSR2_OFFSET	0x17			
#define SJC_MBISTDONER1_OFFSET	0x18			
#define SJC_MBISTDONER2_OFFSET	0x19			
#define SJC_MBISTMASKR1_OFFSET	0x1A			
#define SJC_MBISTMASKR2_OFFSET	0x1B			
#define SJC_BISTPASSR_OFFSET	0x1C			
#define SJC_BISTDONER_OFFSET	0x1D			
#define SJC_MONBISTSELR_OFFSET	0x1E			
#define SJC_RWVALCR_OFFSET	0x1F			
//#########################################				
//# CCM                				
//#########################################				
#define CCM_CCR_OFFSET	0x00			
#define CCM_CCDR_OFFSET	0x04			
#define CCM_CR1_RESERVED_OFFSET	0x08			
#define CCM_CCSR_OFFSET	0x0C			
#define CCM_CACRR_OFFSET	0x10			
#define CCM_CBCDR1_OFFSET	0x14			
#define CCM_CBCDR2_OFFSET	0x18			
#define CCM_CBCDR3_OFFSET	0x1C			
#define CCM_CBCDR4_OFFSET	0x20			
#define CCM_CBCDR5_OFFSET	0x24			
#define CCM_CBCDR6_OFFSET	0x28			
#define CCM_CBCDR7_OFFSET	0x2C			
#define CCM_CAMR_OFFSET	0x30			
#define CCM_CSCMR1_OFFSET	0x34			
#define CCM_CSCMR2_OFFSET	0x38			
#define CCM_CSCDR1_OFFSET	0x3C			
#define CCM_CS1CDR_OFFSET	0x40			
#define CCM_CS2CDR_OFFSET	0x44			
#define CCM_CSECDR1_OFFSET	0x48			
#define CCM_CSECDR2_OFFSET	0x4C			
#define CCM_CECDR_OFFSET	0x50			
#define CCM_CDCDR_OFFSET	0x54			
#define CCM_CH1CDR_OFFSET	0x58			
#define CCM_CH2CDR_OFFSET	0x5c			
#define CCM_CSCDR2_OFFSET	0x60			
#define CCM_CR2_RESERVED_OFFSET	0x64			
#define CCM_CDHIPR_OFFSET	0x68			
#define CCM_CDCR_OFFSET	0x6c			
#define CCM_CALPR_OFFSET	0x70			
#define CCM_CLPCR_OFFSET	0x74			
#define CCM_CISR_OFFSET	0x78			
#define CCM_CIMR_OFFSET	0x7c			
#define CCM_CCOSR_OFFSET	0x80			
#define CCM_CGPR_OFFSET	0x84			
#define CCM_CCGR0_OFFSET	0x88			
#define CCM_CCGR1_OFFSET	0x8c			
#define CCM_CCGR2_OFFSET	0x90			
#define CCM_CCGR3_OFFSET	0x94			
#define CCM_CCGR4_OFFSET	0x98			
#define CCM_CCGR5_OFFSET	0x9c			
#define CCM_CMEOR_OFFSET	0xa0			
#define CCM_CSR_OFFSET	0x08			
//#########################################				
//# GPC            				
//#########################################				
#define GPC_CNTR_OFFSET	0x000			
#define GPC_PGR_OFFSET	0x004			
#define GPC_DPTC_LP_OFFSET	0x080			
#define GPC_DPTC_GP_OFFSET	0x100			
#define GPC_DVFS_CORE_OFFSET	0x180			
#define GPC_DVFS_PER_OFFSET	0x1C0			
#define GPC_APU_PG_OFFSET	0x200			
#define GPC_IPU_PG_OFFSET	0x220			
#define GPC_VPU_PG_OFFSET	0x240			
#define GPC_GPU_PG_OFFSET	0x260			
#define GPC_EMI_SRPG_OFFSET	0x280			
#define GPC_ARM_SRPG_OFFSET	0x2A0			
#define GPC_ARM_EMPGC_0_OFFSET	0x2C0			
#define GPC_ARM_EMPGC_1_OFFSET	0x2D0			
//#########################################				
//# SRC                 				
//#########################################				
#define SRC_SCR_OFFSET	0x000			
#define SRC_SBMR_OFFSET	0x004			
#define SRC_SRSR_OFFSET	0x008			
#define SRC_SAIAR_OFFSET	0x00c			
#define SRC_SAIRAR_OFFSET	0x010			
#define SRC_SISR_OFFSET	0x014			
#define SRC_SIMR_OFFSET	0x018			
//#########################################				
//# HSC             High Speed Communications System     				
//#########################################				
#define HSC_MCD_OFFSET	0x00000000	// MCG Control Designation Register 		
#define HSC_MEN_OFFSET	0x00000004	// MCG Enable Register 		
#define HSC_MSA_OFFSET	0x00000008	// MCG Skew Adjust Register 		
#define HSC_MSR_OFFSET	0x0000000C	// MCG Self Refresh Register 		
#define HSC_MUI_OFFSET	0x00000010	// MCG Unit Interval Register 		
#define HSC_MAUI_OFFSET	0x00000014	// MCG Alternate Unit Interval Register 		
//#########################################				
//# SPDIF				
//#########################################				
#define SPDIF_SCR_OFFSET	0x00			
#define SPDIF_SRCD_OFFSET	0x04			
#define SPDIF_SRPC_OFFSET	0x08			
#define SPDIF_SIE_OFFSET	0x0C			
#define SPDIF_SIS_OFFSET	0x10			
#define SPDIF_SIC_OFFSET	0x10			
#define SPDIF_SRL_OFFSET	0x14			
#define SPDIF_SRR_OFFSET	0x18			
#define SPDIF_SRCSH_OFFSET	0x1C			
#define SPDIF_SRCSL_OFFSET	0x20			
#define SPDIF_SRU_OFFSET	0x24			
#define SPDIF_SRQ_OFFSET	0x28			
#define SPDIF_STL_OFFSET	0x2C			
#define SPDIF_STR_OFFSET	0x30			
#define SPDIF_STCSH_OFFSET	0x34			
#define SPDIF_STCSL_OFFSET	0x38			
#define SPDIF_STUH_OFFSET	0x3C			
#define SPDIF_STUL_OFFSET	0x40			
#define SPDIF_SRFM_OFFSET	0x44			
#define SPDIF_STC_OFFSET	0x50			
//#########################################				
//# NFC  				
//#########################################				
#define NFC_NAND_ADD_CMD	0xE00			
#define NFC_ECC_STATUS_RESULT	0xE08			
#define NFC_LAUNCH_NFC	0xE0C						
#define NFC_CONFIGURATION1	0xE04			
#define NFC_UNLOCK_BLK_ADD	0xE14			
#define NFC_CONFIGURATION2	0xE18			
#define NFC_WR_PROTECT_OFFSET	0x3000			
#define NFC_UNLOCK_BLK_ADD0_OFFSET	0x3004			
#define NFC_UNLOCK_BLK_ADD1_OFFSET	0x3008			
#define NFC_UNLOCK_BLK_ADD2_OFFSET	0x300C			
#define NFC_UNLOCK_BLK_ADD3_OFFSET	0x3010			
#define NFC_CONFIGURATION2_OFFSET	0x3014			
#define NFC_IPC_OFFSET	0x3018			
#define NFC_AXI_ERR_ADDR_OFFSET	0x301C			
				
//#########################################				
//# ESDRAMC				
//#########################################				
#define ESDRAMC_ESDCTL0_OFFSET	0x1000			
#define ESDRAMC_ESDCFG0_OFFSET	0x1004			
#define ESDRAMC_ESDCTL1_OFFSET	0x1008			
#define ESDRAMC_ESDCFG1_OFFSET	0x100C			
#define ESDRAMC_ESDMISC_OFFSET	0x1010			
#define ESDRAMC_ESDSCR_OFFSET	0x1014			
#define ESDRAMC_ESDCDLY1_OFFSET	0x1020			
#define ESDRAMC_ESDCDLY2_OFFSET	0x1024			
#define ESDRAMC_ESDCDLY3_OFFSET	0x1028			
#define ESDRAMC_ESDCDLY4_OFFSET	0x102C			
#define ESDRAMC_ESDCDLY5_OFFSET	0x1030			
#define ESDRAMC_ESDCDLYGD_OFFSET	0x1034			
//#########################################				
//# M4IF				
//#########################################				
#define M4IF_PSM0_OFFSET	0x000			
#define M4IF_PSM1_OFFSET	0x004			
#define M4IF_MDSR6_OFFSET   	0x018	// debug status register 6		
#define M4IF_MDSR7_OFFSET   	0x01c	// debug status register 7		
#define M4IF_MDSR8_OFFSET   	0x020	// debug status register 8		
#define M4IF_MDSR0_OFFSET   	0x024	// debug status register 0		
#define M4IF_MDSR1_OFFSET   	0x028	// debug status register 1		
#define M4IF_MDSR2_OFFSET   	0x02c	// debug status register 2		
#define M4IF_MDSR3_OFFSET   	0x030	// debug status register 3		
#define M4IF_MDSR4_OFFSET   	0x034	// debug status register 4		
#define M4IF_MDSR5_OFFSET   	0x038	// debug status register 5		
#define M4IF_IPAR_OFFSET    	0x03c	// IP acknowledge register		
#define M4IF_FBPM0_OFFSET   	0x040	// fast basic priorities reg0		
#define M4IF_FBPM1_OFFSET   	0x044	// fast basic priorities reg1		
#define M4IF_FIDBP_OFFSET   	0x048	// special fast basic priorities 		
#define M4IF_SBAR0_OFFSET   	0x04c	// snooping window register region0		
#define M4IF_SERL0_OFFSET   	0x050	// snooping enable low register region0		
#define M4IF_SERH0_OFFSET   	0x054	// snooping enable 0xig0x register region0		
#define M4IF_SSRL0_OFFSET   	0x058	// snooping status low register region0		
#define M4IF_SSRH0_OFFSET   	0x05c	// snooping status 0xig0x register region0		
#define M4IF_SBAR1_OFFSET   	0x060	// snooping window register region1		
#define M4IF_SERL1_OFFSET   	0x064	// snooping enable low register region1		
#define M4IF_SERH1_OFFSET   	0x068	// snooping enable 0xig0x register region1		
#define M4IF_SSRL1_OFFSET   	0x06c	// snooping status low register region1		
#define M4IF_MCR0_OFFSET    	0x08c	// control reg0		
#define M4IF_MCR1_OFFSET    	0x090	// control reg1		
#define M4IF_MDCR_OFFSET    	0x094	// debug control reg		
#define M4IF_FACR_OFFSET    	0x098	// fast arbitration control register		
#define M4IF_FPWC_OFFSET    	0x09c	// fast priorities weig0xting config register		
#define M4IF_SACR_OFFSET    	0x0a0	// slow arbitration control register		
#define M4IF_PSM2_OFFSET	0x0a4			
#define M4IF_IACR_OFFSET    	0x0a8	// intr arbitration control register		
#define M4IF_PSM3_OFFSET	0x0ac			
#define M4IF_FULA_OFFSET    	0x0b0	// fast unit level register		
#define M4IF_SULA_OFFSET    	0x0b4	// slow unit level register		
#define M4IF_IULA_OFFSET    	0x0b8	// intr unit level register		
#define M4IF_FDPS_OFFSET    	0x0bc	// fast dynamic priority status register		
#define M4IF_FDPC_OFFSET    	0x0c0	// slow dynamic priority status register		
#define M4IF_WMSA0_0_OFFSET 	0x0d4	// watermark AP start address region0 (weim cs0)		
#define M4IF_WMSA0_1_OFFSET 	0x0d8	// watermark AP start address region1 (weim cs1)		
#define M4IF_WMSA0_2_OFFSET 	0x0dc	// watermark AP start address region2 (weim cs2)		
#define M4IF_WMSA0_3_OFFSET 	0x0e0	// watermark AP start address region3 (weim cs3)		
#define M4IF_WMSA0_4_OFFSET 	0x0e4	// watermark AP start address region4 (weim cs4)		
#define M4IF_WMSA0_5_OFFSET 	0x0e8	// watermark AP start address region5 (weim cs5)		
#define M4IF_WMSA0_6_OFFSET 	0x0ec	// watermark AP start address region6 (esdctl csd0)		
#define M4IF_WMSA0_7_OFFSET 	0x0f0	// watermark AP start address region7 (esdctl csd1)		
#define M4IF_WMEA0_0_OFFSET 	0x0f4	// watermark AP end address region0 (weim cs0)		
#define M4IF_WMEA0_1_OFFSET 	0x0f8	// watermark AP end address region1 (weim cs1)		
#define M4IF_WMEA0_2_OFFSET 	0x0fc	// watermark AP end address region2 (weim cs2)		
#define M4IF_WMEA0_3_OFFSET 	0x100	// watermark AP end address region3 (weim cs3)		
#define M4IF_WMEA0_4_OFFSET 	0x104	// watermark AP end address region4 (weim cs4)		
#define M4IF_WMEA0_5_OFFSET 	0x108	// watermark AP end address region5 (weim cs5)		
#define M4IF_WMEA0_6_OFFSET 	0x10c	// watermark AP end address region6 (esdctl csd0)		
#define M4IF_WMEA0_7_OFFSET 	0x110	// watermark AP end address region7 (esdctl csd1)		
#define M4IF_WMIS0_OFFSET   	0x114	// watermark AP interrupt and status register		
#define M4IF_FWMVA0_OFFSET  	0x118	// fast watermark AP violation address		
#define M4IF_SWMVA0_OFFSET  	0x11c	// slow watermark AP violation address		
#define M4IF_WMSA1_0_OFFSET 	0x120	// watermark BP start address region0 (weim cs0)		
#define M4IF_WMSA1_1_OFFSET 	0x124	// watermark BP start address region1 (weim cs1)		
#define M4IF_WMSA1_2_OFFSET 	0x128	// watermark BP start address region2 (weim cs2)		
#define M4IF_WMSA1_3_OFFSET 	0x12c	// watermark BP start address region3 (weim cs3)		
#define M4IF_WMSA1_4_OFFSET 	0x130	// watermark BP start address region4 (weim cs4)		
#define M4IF_WMSA1_5_OFFSET 	0x134	// watermark BP start address region5 (weim cs5)		
#define M4IF_WMSA1_6_OFFSET 	0x138	// watermark BP start address region6 (esdctl csd0)		
#define M4IF_WMSA1_7_OFFSET 	0x13c	// watermark BP start address region7 (esdctl csd1)		
#define M4IF_WMEA1_0_OFFSET 	0x140	// watermark BP end address region0 (weim cs0)		
#define M4IF_WMEA1_1_OFFSET 	0x144	// watermark BP end address region1 (weim cs1)		
#define M4IF_WMEA1_2_OFFSET 	0x148	// watermark BP end address region2 (weim cs2)		
#define M4IF_WMEA1_3_OFFSET 	0x14c	// watermark BP end address region3 (weim cs3)		
#define M4IF_WMEA1_4_OFFSET 	0x150	// watermark BP end address region4 (weim cs4)		
#define M4IF_WMEA1_5_OFFSET 	0x154	// watermark BP end address region5 (weim cs5)		
#define M4IF_WMEA1_6_OFFSET 	0x158	// watermark BP end address region6 (esdctl csd0)		
#define M4IF_WMEA1_7_OFFSET 	0x15c	// watermark BP end address region7 (esdctl csd1)		
#define M4IF_WMIS1_OFFSET   	0x160	// watermark BP interrupt and status register		
#define M4IF_FWMVA1_OFFSET  	0x164	// fast watermark BP violation address		
#define M4IF_SWMVA1_OFFSET  	0x168	// slow watermark BP violation address		
//#endif				

