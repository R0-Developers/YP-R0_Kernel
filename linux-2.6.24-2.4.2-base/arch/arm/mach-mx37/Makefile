#
# Makefile for the linux kernel.
#

# Object file lists.

obj-y   := system.o iomux.o cpu.o mm.o clock.o devices.o serial.o dma.o

ifneq ($(CONFIG_MACH_MX37_BIGBANG),y)
   obj-$(CONFIG_MACH_MX37_3DS) += mx37_3stack.o mx37_3stack_gpio.o lpmodes.o dptc.o
   obj-$(CONFIG_SPI_MXC) += mx37_3stack_cpld.o
else
   obj-$(CONFIG_MACH_MX37_BIGBANG) += mx37_bigbang.o mx37_bigbang_gpio.o lpmodes.o bus_freq.o
endif
obj-$(CONFIG_REGULATOR_WM8350) += mx37_3stack_pmic_wm8350.o

#imx37-cpufreq-objs := cpufreq.o
#obj-$(CONFIG_CPU_FREQ_IMX37)    += imx37-cpufreq.o

# power management
obj-$(CONFIG_PM)        += pm.o

ifneq ($(strip $(CONFIG_USB_GADGET_ARC) $(CONFIG_USB_EHCI_ARC_OTG)),)
    obj-y   += usb_dr.o
endif
