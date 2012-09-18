#ifndef _KERNEL_PARAM_H
#define _KERNEL_PARAM_H

#define KP_START	0x43FD0000
#define KP_END		0x44000000
#define KP_SIZE		(KP_END - KP_START)

typedef struct {
	unsigned int 	LedLevel;
	// NAND Device Driver
	unsigned char	stDeviceInfo[92];
	unsigned char	stConfig[20];
	unsigned char	stLayout[32];

	// VFL
	unsigned char	stBufferCxt[40];
	unsigned char	BufferList[32 * 4];
	unsigned char	data[(4096 + 200) * 4];
	unsigned char	stVFLMeta[2048 * 16];
	unsigned char	stAsyncOp[32 * 16];
	unsigned char	VFLSpare[200];
	unsigned char	stROAreaMeta[112];

	// FTL
	unsigned char	stLOGCxt[36 * 12];
	unsigned char	aPOffsetL2P[2048 * 2 * 12];
	unsigned char	aSecBitmap[2048 * 2 * 12];
	unsigned char	stFTLMeta[2048];
	unsigned char	tempSpare[200];

	unsigned char	ucPBAMode;

	unsigned char szUsbId[36];

} KernelParam;

extern int KernelParamInit(void);

extern KernelParam *KernelParamGet(void);

void KernelParamSetup(void);

#endif /* _KERNEL_PARAM_H */
