#
# Makefile for the PMIC core drivers.
#
obj-$(CONFIG_MXC_PMIC_MC13783) += pmic_mc13783_mod.o
pmic_mc13783_mod-objs := pmic_external.o pmic_event.o pmic_core_spi.o mc13783.o

obj-$(CONFIG_MXC_PMIC_MC9SDZ60) += pmic_mc9sdz60_mod.o
pmic_mc9sdz60_mod-objs := pmic_external.o pmic_event.o mcu_pmic_core.o max8660.o mc9sdz60.o

obj-$(CONFIG_MXC_PMIC_MC13892) += pmic_mc13892_mod.o
pmic_mc13892_mod-objs := pmic_external.o pmic_event.o pmic_core_i2c.o mc13892.o

obj-$(CONFIG_MXC_PMIC_CHARDEV)	+= pmic-dev.o

