#
# Makefile for the linux kernel.
#

# Common support
obj-y := cpu_common.o gpio.o clock.o wdog.o snoop.o io.o

ifneq ($(CONFIG_ARCH_MX21)$(CONFIG_ARCH_MX27),y)
obj-y += time.o spba.o sdma/
endif

ifeq ($(CONFIG_MXC_TZIC),y)
obj-y += tzic.o
else
obj-y += irq.o
endif

obj-$(CONFIG_ARCH_MX21) += dma_mx2.o
obj-$(CONFIG_ARCH_MX27) += dma_mx2.o usb_common.o
obj-$(CONFIG_ARCH_MX3) += dptc.o usb_common.o
obj-$(CONFIG_ARCH_MX35) += usb_common.o serialxc.o
obj-$(CONFIG_ARCH_MXC91321) += dptc.o
obj-$(CONFIG_ARCH_MX37)     += usb_common.o utmixc.o dvfs_per.o dvfs_core.o
obj-$(CONFIG_ARCH_MX51)	+= usb_common.o utmixc.o
obj-$(CONFIG_MXC_DSP_BRINGUP) += dsp_bringup.o

#CPUFREQ support
obj-$(CONFIG_CPU_FREQ_IMX37)    += cpufreq.o

# LEDs support
obj-$(CONFIG_LEDS) += leds.o

# USB support
obj-$(CONFIG_ISP1504_MXC) += isp1504xc.o
obj-$(CONFIG_ISP1301_MXC) += isp1301xc.o
obj-$(CONFIG_MC13783_MXC) += mc13783_xc.o
obj-$(CONFIG_USB_EHCI_FSL_UTMI) += utmixc.o

ifneq ($(CONFIG_USB_EHCI_ARC_H1),)
ifneq ($(CONFIG_ARCH_MX51),y)
obj-y += serialxc.o
else
obj-y += isp1504xc.o
endif
endif
