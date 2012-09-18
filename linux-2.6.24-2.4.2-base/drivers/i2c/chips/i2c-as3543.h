/*
 *****************************************************************************
 * Copyright @ 2008 by austriamicrosystems AG                                *
 * All rights are reserved.                                                  *
 *                                                                           *
 * Reproduction in whole or in part is prohibited without the written consent*
 * of the copyright owner. Austriamicrosystems reserves the right to make    *
 * changes without notice at any time. The software is provided as is and    *
 * Austriamicrosystems makes no warranty, expressed, implied or statutory,   *
 * including but not limited to any implied warranty of merchantability or   *
 * fitness for any particular purpose, or that the use will not infringe any *
 * third party patent, copyright or trademark. Austriamicrosystems should    *
 * not be liable for any loss or damage arising from its use.                *
 *****************************************************************************
 */

/*
 *      PROJECT:   AS353x
 *      $RCSfile: $
 *      $Revision: 3052 $
 *      LANGUAGE:  ANSI C
 */

/*! \file
 *
 *  \author C. Eisendle
 *  \author M. Barjaktarevic (Substitute)
 *
 *  \brief  AS3543 specific header file
 *
 *  AS3543 specific header file
 */

#ifndef AFE_AS3543_H_
#define AFE_AS3543_H_


#define    AFE_I2C_SLAVE_ADDR               (0x46)

/* defines for the used registers   */
#define    AFE_OUT_R        (0x02)
#define    AFE_OUT_L        (0x03)
#define    AFE_MIC_R        (0x06)
#define    AFE_MIC_L        (0x07)
#define    AFE_LINE_IN_R    (0x0A)
#define    AFE_LINE_IN_L    (0x0B)
#define    AFE_DAC_R        (0x0E)
#define    AFE_DAC_L        (0x0F)
#define    AFE_ADC_R        (0x10)
#define    AFE_ADC_L        (0x11)
#define    AFE_DAC_IF       (0x12)
#define    AFE_AUDIOSET1    (0x14)
#define    AFE_AUDIOSET2    (0x15)
#define    AFE_AUDIOSET3    (0x16)

// PMU Register
#define    AFE_MUX_PMU_REG1 (0x17)
#define    AFE_CVDD         (AFE_MUX_PMU_REG1)
#define    AFE_MUX_PMU_REG2 (0x18)
#define    AFE_LDO          (AFE_MUX_PMU_REG2)
#define    AFE_MUX_PMU_REG3 (0x19)
#define    AFE_MUX_PMU_REG4 (0x1A)
#define    AFE_MUX_PMU_REG5 (0x1B)
#define    AFE_PMU_ENABLE   (0x1C)

#define	   AFE_PLL			(0x1A)

// System Register
#define    AFE_SYSTEM       (0x20)
#define    AFE_SUPERVISOR   (0x21)
#define    AFE_RAM_WAKE_UP  (0x22)
#define    AFE_IRQ_ENRD0    (0x23)
#define    AFE_IRQ_ENRD1    (0x24)
#define    AFE_IRQ_ENRD2    (0x25)
#define    AFE_IRQ_ENRD3    (0x26)
#define    AFE_IRQ_ENRD4    (0x27)
#define    AFE_RTC_CNTR     (0x28)
#define    AFE_RTC_TIME     (0x29)
#define    AFE_RTC_0        (0x2A)
#define    AFE_RTC_1        (0x2B)
#define    AFE_RTC_2        (0x2C)
#define    AFE_RTC_3        (0x2D)
#define    AFE_ADC_0        (0x2E)
#define    AFE_ADC_1        (0x2F)

// UID Register
#define    AFE_UID_0        (0x38)
#define    AFE_UID_1        (0x39)
#define    AFE_UID_2        (0x3A)
#define    AFE_UID_3        (0x3B)
#define    AFE_UID_4        (0x3C)
#define    AFE_UID_5        (0x3D)
#define    AFE_UID_6        (0x3E)
#define    AFE_UID_7        (0x3F)

#define    AFE_CHG_VBUS1    AFE_MUX_PMU_REG3
#define    AFE_PMU_MASTER   AFE_PMU_ENABLE

/* sub addresses for mux'ed registers */
#define    AFE_SUB_CVDD1    (0x1)
#define    AFE_SUB_CVDD2    (0x2)
#define    AFE_SUB_DC_CTRL  (0x7)
#define    AFE_SUB_PVDD1    (0x1)   /*LDO3                                    */
#define    AFE_SUB_LDO3     (0x1)
#define    AFE_SUB_PVDD2    (0x2)   /*LDO4                                    */
#define    AFE_SUB_LDO4     (0x2)
#define    AFE_SUB_AVDD27   (0x6)   /*LDO2                                    */
#define    AFE_SUB_LDO2     (0x6)
#define    AFE_SUB_AVDD17   (0x7)   /*LDO1                                    */
#define    AFE_SUB_LDO1     (0x7)
#define    AFE_SUB_CHG1     (0x1)
#define    AFE_SUB_CHG2     (0x2)
#define    AFE_SUB_OUT1     (0x1)
#define    AFE_SUB_OUT2     (0x2)
#define    AFE_SUB_OUT3     (0x3)
#define    AFE_SUB_DCDC15   (0x1)
#define    AFE_SUB_ISINK1   (0x2)
#define    AFE_SUB_ISINK2   (0x3)
#define    AFE_SUB_HIB      (0x6)
#define    AFE_SUB_PLL      (0x7)

#define    AFE_SUB_MASK     (0x7) /*mask for muxed registers                  */


/* CONFIGURATION SECTION                                                      */

/* masks of level, edge and status change bits, interrupt bits are set to 1   */
#define AFE_IRQ_ENRD0_MASK  (0x0)
#define AFE_IRQ_ENRD1_MASK  (0x0)
#define AFE_IRQ_ENRD2_MASK  (0x97) /*!< bat temp., chg eoc, chg state changed
                                        usb state changed, rvdd low, bvdd low*/
#define AFE_IRQ_ENRD3_MASK  (0xbb) /*!< jtemp high, hp overcurr., i2s status,
                                        i2s changed, mic conn., hp conn     */
#define AFE_IRQ_ENRD4_MASK  (0x07) /*!< adc eoc, rtc update, remote det.    */

#define AFE_IRQ_FIRST   AFE_IRQ_ENRD0
#define AFE_IRQ_LAST    AFE_IRQ_ENRD4

/* the first and the last irq status register (needed by afe base driver)   */
#define AFE_IRQ_STATUS_FIRST    AFE_IRQ_FIRST
#define AFE_IRQ_STATUS_LAST     AFE_IRQ_LAST
/* the first and the last irq mask register (needed by afe base driver)     */
#define AFE_IRQ_MASK_FIRST    AFE_IRQ_FIRST
#define AFE_IRQ_MASK_LAST     AFE_IRQ_LAST

/*number of interrupt registers                                              */
#define AFE_INT_REGISTERS_NO            (0x5)

/*define boundary of last shadowed register address                          */
#define     AFE_SHADOWED_BOUNDARY   (AFE_AUDIOSET3 + 1)

/* END OF CONFIGURATION SECTION                                              */

#define AS3543_REG_MAX		(0x40)
#define AS3543_SUBREG_MAX	(0x07)


#define IS_SUBREG(x)	((x >= AFE_MUX_PMU_REG1) && (x <= AFE_MUX_PMU_REG5))?1:0


#define ADDED_API_FOR_PMUDRIVER

extern int as3543_reg_write(u8 reg, u8 val);
extern u8 as3543_reg_read(u8 reg);
extern u8 as3543_reg_read_from_device(u8 reg);
extern int as3543_clear_bits(u8 reg, u8 mask);
extern int as3543_set_bits(u8 reg, u8 mask);

extern int as3543_subreg_write(u8 reg, u8 subreg, u8 val);
extern u8 as3543_subreg_read(u8 reg, u8 subreg);
extern u8 as3543_subreg_read_from_device(u8 reg, u8 subreg);

#ifdef ADDED_API_FOR_PMUDRIVER
extern u8 	as3543_reg[AS3543_REG_MAX];
extern u8 	as3543_subreg[][AS3543_SUBREG_MAX];
extern u8 	as3543_read_reg(u8 reg);
extern int 	as3543_write_reg(u8 reg, u8 val);
extern int 	as3543_modify_reg(u8 reg, u8 mask, u8 value);
extern u8 	as3543_read_subreg(u8 reg, u8 subreg);
extern int 	as3543_write_subreg(u8 reg, u8 subreg, u8 val);
extern int 	as3543_modify_subreg(u8 reg, u8 subreg, u8 mask, u8 value);

extern u8 	as3543_reg_readbyte_from_device(u8 reg);
extern u8 	as3543_subreg_readbyte_from_device(u8 reg, u8 subreg);
extern int 	as3543_subreg_writebyte(u8 reg, u8 subreg, u8 val);
#endif

#endif /* AFE_AS3543_H_ */

