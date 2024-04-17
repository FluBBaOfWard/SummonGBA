
#ifdef __arm__

	.syntax unified

//	.section .text
	.section .iwram, "ax", %progbits
	.align 2
	.arm

	.global sineTable
	.global randomNumber
	.global copyWithCPU
	.global fillWithCPU
	.global clear16BitScreen
	.global fill16BitScreen
	.global clear4BitScreen
	.global fill4BitScreen


;@----------------------------------------------------------------------------
IO_reset:
;@----------------------------------------------------------------------------
	bx lr
;@----------------------------------------------------------------------------
randomNumber:			;@ r0 = max value
	.type   randomNumber STT_FUNC
;@----------------------------------------------------------------------------
	ldr r2,rndSeed0
	ldr r3,rndSeed1
	tst r3,r3,lsr#1				;@ top bit into Carry
	movs r1,r2,rrx				;@ 33 bit rotate right
	adc r3,r3,r3				;@ carry into lsb of r1
	eor r1,r1,r2,lsl#12			;@ (involved!)
	eor r2,r1,r1,lsr#20			;@ (similarly involved!)

	str r2,rndSeed0
	str r3,rndSeed1
	umull r3,r0,r2,r0
	bx lr
rndSeed0:
	.long 0x39bbaa98
rndSeed1:
	.long 0x00000000
;@----------------------------------------------------------------------------
copyWithCPU:						;@ r0=dst, r1=src, r2=len.
	.type copyWithCPU STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{r4-r10,lr}
cpu32Loop:
	ldmia r1!,{r3-r10}			;@ Read 32 bytes
	stmia r0!,{r3-r10}			;@ Write 32 bytes
	subs r2,r2,#32
	bhi cpu32Loop

	ldmfd sp!,{r4-r10,lr}
	bx lr
;@----------------------------------------------------------------------------
fillWithCPU:						;@ r0=dst, r1=value, r2=len.
	.type fillWithCPU STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{r4-r9,lr}
	mov r3,r1
	mov r4,r1
	mov r5,r1
	stmfd sp!,{r1,r3-r5}
	ldmfd sp!,{r6-r9}
fill32Loop:
	stmia r0!,{r1,r3-r9}			;@ Write 32 bytes
	subs r2,r2,#32
	bhi fill32Loop

	ldmfd sp!,{r4-r9,lr}
	bx lr
;@----------------------------------------------------------------------------
clear16BitScreen:
.type clear16BitScreen STT_FUNC
;@----------------------------------------------------------------------------
	mov r0,#0x0000
;@----------------------------------------------------------------------------
fill16BitScreen:					;@ r0=value.
	.type fill16BitScreen STT_FUNC
;@----------------------------------------------------------------------------
	mov r1,#0x06000000
	mov r2,#160						;@ Screen height
	stmfd sp!,{r4-r11,lr}
	mov r3,r0
	mov r4,r0
	mov r5,r0
	mov r6,r0
	mov r7,r0
	mov r8,r0
	mov r9,r0
	mov r10,r0
	mov r11,r0
	mov r12,r0
	mov lr,r0
fill16BitLoop:
	stmia r1!,{r0,r3-r12,lr}		;@ Write 48 bytes
	stmia r1!,{r0,r3-r12,lr}		;@ Write 48 bytes
	stmia r1!,{r0,r3-r12,lr}		;@ Write 48 bytes
	stmia r1!,{r0,r3-r12,lr}		;@ Write 48 bytes
	stmia r1!,{r0,r3-r12,lr}		;@ Write 48 bytes
	stmia r1!,{r0,r3-r12,lr}		;@ Write 48 bytes
	stmia r1!,{r0,r3-r12,lr}		;@ Write 48 bytes
	stmia r1!,{r0,r3-r12,lr}		;@ Write 48 bytes
	stmia r1!,{r0,r3-r12,lr}		;@ Write 48 bytes
	stmia r1!,{r0,r3-r12,lr}		;@ Write 48 bytes
	subs r2,r2,#1
	bhi fill16BitLoop

	ldmfd sp!,{r4-r11,lr}
	bx lr
;@----------------------------------------------------------------------------
clear4BitScreen:
	.type clear4BitScreen STT_FUNC
;@----------------------------------------------------------------------------
	mov r0,#0x0000
;@----------------------------------------------------------------------------
fill4BitScreen:					;@ r0=value.
	.type fill4BitScreen STT_FUNC
;@----------------------------------------------------------------------------
	mov r1,#0x06000000
	add r1,r1,#0x4000
	mov r2,#160						;@ Screen height
	stmfd sp!,{r4-r11,lr}
	mov r3,r0
	mov r4,r0
	mov r5,r0
	mov r6,r0
	mov r7,r0
	mov r8,r0
	mov r9,r0
	mov r10,r0
	mov r11,r0
fill4BitLoop:
	stmia r1!,{r0,r3-r11}		;@ Write 40 bytes
	stmia r1!,{r0,r3-r11}		;@ Write 40 bytes
	stmia r1!,{r0,r3-r11}		;@ Write 40 bytes
	subs r2,r2,#1
	bhi fill4BitLoop

	ldmfd sp!,{r4-r11,lr}
	bx lr


;@----------------------------------------------------------------------------
sineTable:
;@----------------------------------------------------------------------------
.byte 0x00, 0x03, 0x06, 0x09, 0x0C, 0x0F, 0x12, 0x15, 0x18, 0x1B, 0x1E, 0x22, 0x25, 0x27, 0x2A, 0x2D
.byte 0x30, 0x33, 0x36, 0x39, 0x3C, 0x3E, 0x41, 0x44, 0x46, 0x49, 0x4B, 0x4E, 0x50, 0x53, 0x55, 0x57
.byte 0x5A, 0x5C, 0x5E, 0x60, 0x62, 0x64, 0x66, 0x68, 0x6A, 0x6B, 0x6D, 0x6E, 0x70, 0x71, 0x73, 0x74
.byte 0x75, 0x76, 0x78, 0x79, 0x7A, 0x7A, 0x7B, 0x7C, 0x7D, 0x7D, 0x7E, 0x7E, 0x7E, 0x7F, 0x7F, 0x7F
.byte 0x7F, 0x7F, 0x7F, 0x7F, 0x7E, 0x7E, 0x7E, 0x7D, 0x7D, 0x7C, 0x7B, 0x7A, 0x7A, 0x79, 0x78, 0x76
.byte 0x75, 0x74, 0x73, 0x71, 0x70, 0x6E, 0x6D, 0x6B, 0x6A, 0x68, 0x66, 0x64, 0x62, 0x60, 0x5E, 0x5C
.byte 0x5A, 0x57, 0x55, 0x53, 0x50, 0x4E, 0x4B, 0x49, 0x46, 0x44, 0x41, 0x3E, 0x3C, 0x39, 0x36, 0x33
.byte 0x30, 0x2D, 0x2A, 0x27, 0x25, 0x22, 0x1E, 0x1B, 0x18, 0x15, 0x12, 0x0F, 0x0C, 0x09, 0x06, 0x03
.byte 0x00, 0xFD, 0xFA, 0xF7, 0xF4, 0xF1, 0xEE, 0xEB, 0xE8, 0xE5, 0xE2, 0xDE, 0xDB, 0xD9, 0xD6, 0xD3
.byte 0xD0, 0xCD, 0xCA, 0xC7, 0xC4, 0xC2, 0xBF, 0xBC, 0xBA, 0xB7, 0xB5, 0xB2, 0xB0, 0xAD, 0xAB, 0xA9
.byte 0xA6, 0xA4, 0xA2, 0xA0, 0x9E, 0x9C, 0x9A, 0x98, 0x96, 0x95, 0x93, 0x92, 0x90, 0x8F, 0x8D, 0x8C
.byte 0x8B, 0x8A, 0x88, 0x87, 0x86, 0x86, 0x85, 0x84, 0x83, 0x83, 0x82, 0x82, 0x82, 0x81, 0x81, 0x81
.byte 0x81, 0x81, 0x81, 0x81, 0x82, 0x82, 0x82, 0x83, 0x83, 0x84, 0x85, 0x86, 0x86, 0x87, 0x88, 0x8A
.byte 0x8B, 0x8C, 0x8D, 0x8F, 0x90, 0x92, 0x93, 0x95, 0x96, 0x98, 0x9A, 0x9C, 0x9E, 0xA0, 0xA2, 0xA4
.byte 0xA6, 0xA9, 0xAB, 0xAD, 0xB0, 0xB2, 0xB5, 0xB7, 0xBA, 0xBC, 0xBF, 0xC2, 0xC4, 0xC7, 0xCA, 0xCD
.byte 0xD0, 0xD3, 0xD6, 0xD9, 0xDB, 0xDE, 0xE2, 0xE5, 0xE8, 0xEB, 0xEE, 0xF1, 0xF4, 0xF7, 0xFA, 0xFD
.pool
;@----------------------------------------------------------------------------

#endif // #ifdef __arm__
