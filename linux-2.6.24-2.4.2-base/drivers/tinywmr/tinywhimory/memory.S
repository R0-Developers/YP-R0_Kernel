.globl FSL_NFC_MemoryCopy_218
FSL_NFC_MemoryCopy_218:
	STMFD   r13!,{r4-r11,r14}
	MOV 	r3,r2,LSR #5
ALIGN_COPY_36:
	LDMIA   r0!,{r4-r12}
	SUBS	r3,r3,#0x1
	STMIA   r1!,{r4-r12}
	BNE	ALIGN_COPY_36
	LDRH	r4,[r0]
	STRH	r4,[r1]
	LDMFD	r13!, {r4-r12,pc}

.globl FSL_NFC_MemoryCopy_32Bytes
FSL_NFC_MemoryCopy_32Bytes:
	STMFD   r13!,{r4-r11,r14}
	MOV 	r3,r2,LSR #5
ALIGN_COPY_32:
	LDMIA   r0!,{r4-r11}
	SUBS	r3,r3,#0x1
	STMIA   r1!,{r4-r11}
	BNE	ALIGN_COPY_32
	LDMFD	r13!, {r4-r11,pc}

.globl FSL_NFC_MemorySet_218
FSL_NFC_MemorySet_218:
	STMFD   r13!,{r4-r12,r14}
	AND		r1,r1,#0xFF
	ADD 	r2,r1,r1,LSL #8
	MOV 	r3,#0x6
	ADD 	r1,r2,r2,LSL #16
	MOV		r4,r1
	MOV		r5,r1
	MOV		r6,r1
	MOV		r7,r1
	MOV		r8,r1
	MOV		r9,r1
	MOV		r10,r1
	MOV		r11,r1
	MOV		r12,r1
ALIGN_SET_36:
    STMIA   r0!,{r4-r12}
	SUBS	r3,r3,#0x1
	BNE		ALIGN_SET_36
	STRH	r2,[r0]
	LDMFD	r13!, {r4-r12,pc}

.globl FSL_NFC_MemorySet_32Bytes
FSL_NFC_MemorySet_32Bytes:
	STMFD   r13!,{r4-r11,r14}
	AND		r1,r1,#0xFF
	ADD 	r1,r1,r1,LSL #8
	MOV 	r3,r2,LSR #5
	ADD 	r1,r1,r1,LSL #16
	MOV		r4,r1
	MOV		r5,r1
	MOV		r6,r1
	MOV		r7,r1
	MOV		r8,r1
	MOV		r9,r1
	MOV		r10,r1
	MOV		r11,r1
ALIGN_SET_32:      
    STMIA   r0!,{r4-r11}
	SUBS	r3,r3,#0x1
	BNE		ALIGN_SET_32
	LDMFD	r13!, {r4-r11,pc}
