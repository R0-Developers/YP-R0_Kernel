/*
 * linux/kernel/KernelParam.c
 *
 * Interface to get parameters from MBoot
 *
 * Started by Sim Sang Man, Copyright (C) 2009
 *
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/string.h>
#include <linux/KernelParam.h>

KernelParam gKernelParam;

int KernelParamInit(void)
{
	memcpy(&gKernelParam, phys_to_virt(KP_START), sizeof(KernelParam));

	return 0;
}

KernelParam *KernelParamGet(void)
{
	return &gKernelParam;
}

EXPORT_SYMBOL(KernelParamInit);
EXPORT_SYMBOL(KernelParamGet);
