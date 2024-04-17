#include "gba_asm.h"

	.global gfxInit
	.global simpleIrqHandler
	.global vblIrqHandler
	.global convertToSprites
	.global fadeIrqHandler
	.global makeWave
	.global darkenPalette
	.global bg0XPos
	.global rndSeed
	.global masterFrameCounter
//	.global sineTable
	.global mmFrame			// MaxMod worker function
	.global copyWithDMA
	.global copyWithCPU
	.global copyScreenCPU

	.syntax unified
	.arm

	.section .text
	.align 2
;@----------------------------------------------------------------------------
gfxInit:
;@----------------------------------------------------------------------------
	stmfd sp!,{lr}

	ldr r0,=OAM_BUFFER			;@ No stray sprites please
	mov r1,#0x200+SCREEN_HEIGHT
	mov r2,#0x400
	bl fillWithCPU

	ldmfd sp!,{lr}
	bx lr
;@----------------------------------------------------------------------------


	.section .iwram, "ax", %progbits
	.align 2
;@----------------------------------------------------------------------------
simpleIrqHandler:
	.type simpleIrqHandler STT_FUNC
;@----------------------------------------------------------------------------
	mov r0,#REG_BASE
	strh r0,[r0,#REG_DMA0CNT_H]	;@ DMA0 stop
	strh r0,[r0,#REG_DMA3CNT_H]	;@ DMA3 stop

	add r0,r0,#REG_DMA3SAD

	ldr r1,=OAM_BUFFER			;@ DMA3 src, OAM transfer:
	mov r2,#OAM					;@ DMA3 dst
	mov r3,#0x84000000			;@ noIRQ 32bit incsrc incdst
	orr r3,r3,#128*2			;@ 128 sprites * 2 longwords
	stmia r0,{r1-r3}			;@ DMA3 go

	ldr r1,=masterFrameCounter
	ldr r0,[r1]
	add r0,r0,#1
	str r0,[r1]

	ldr r0,=mmFrame
	bx r0
;@----------------------------------------------------------------------------
vblIrqHandler:
	.type vblIrqHandler STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{r4-r11,lr}

	ldr r8,=bg0XPos
	ldr r8,[r8]
	ldr r4,=scrollBuffer

	mov r0,r8,lsr#16
	stmia r4!,{r0}

	mov r9,#REG_BASE
	strh r9,[r9,#REG_DMA0CNT_H]	;@ DMA0 stop

	ldr r0,=scrollBuffer		;@ setup DMA0 buffer for scrolling:
	add r1,r9,#REG_BG0HOFS		;@ DMA0 always goes here
	ldmia r0!,{r2-r5}			;@ Read
	stmia r1,{r2-r5}			;@ set 1st value manually, HBL is AFTER 1st line
//	ldr r2,=0xA6600004			;@ noIRQ hblank 32bit repeat incsrc inc_reloaddst, 4 longwords
//	add r8,r9,#REG_DMA0SAD
//	stmia r8,{r0-r2}			;@ DMA0 go

	add r0,r9,#REG_DMA3SAD

	ldr r1,=OAM_BUFFER			;@ DMA3 src, OAM transfer:
	mov r2,#OAM					;@ DMA3 dst
	mov r3,#0x84000000			;@ noIRQ 32bit incsrc incdst
	orr r3,r3,#128*2			;@ 128 sprites * 2 longwords
	stmia r0,{r1-r3}			;@ DMA3 go

	ldmfd sp!,{r4-r11,lr}

	ldr r1,=masterFrameCounter
	ldr r0,[r1]
	add r0,r0,#1
	str r0,[r1]

	ldr r0,=mmFrame
	bx r0

;@----------------------------------------------------------------------------
convertToSprites:			;@ r0= **Mob
	.type convertToSprites STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{r4-r10,lr}
	mov r11,r11

	ldr r1,=OAM_BUFFER			;@ Destination

	mov r6,#0x40000000			;@ 16x16 size

	mov r7,#128					;@ Number of sprites
dm5:
	ldr r8,[r0],#4
	ldr r2,[r8],#4				;@ XPos
	ldr r3,[r8],#4				;@ YPos
	ldrb r4,[r8],#1				;@ Tile Index
	ldrb r5,[r8],#1				;@ Active?
	cmp r5,#0
	beq dm10					;@ Skip if sprite not active

	orr r5,r6,r3				;@ YPos
	mov r2,r2,lsl#23
	orr r5,r5,r2,lsr#7			;@ XPos
	str r5,[r1],#4				;@ Store OBJ Atr 0,1. Xpos, ypos, flip, scale/rot, size, shape.

	mov r5,r4,ror#4
	add r5,r5,r4,lsr#4
	mov r5,r5,ror#32-5
	strh r5,[r1],#4				;@ Store OBJ Atr 2. Pattern, prio & palette.
dm9:
	subs r7,r7,#1
	bne dm5
	ldmfd sp!,{r4-r10,lr}
	bx lr
dm10:
	mov r2,#0x200+SCREEN_HEIGHT	;@ Double, y=SCREEN_HEIGHT
	str r2,[r1],#8
	b dm9

;@----------------------------------------------------------------------------
fadeIrqHandler:
	.type fadeIrqHandler STT_FUNC
;@----------------------------------------------------------------------------
	mov r0,#REG_BASE
	strh r0,[r0,#REG_DMA0CNT_H]	;@ DMA0 stop
	strh r0,[r0,#REG_DMA3CNT_H]	;@ DMA3 stop

	ldr r1,=fadeBuffer			;@ setup DMA3 buffer for alpha blend:
	add r2,r0,#REG_BLDALPHA		;@ DMA3 always goes here
	ldrh r3,[r1],#2				;@ Read
	strh r3,[r2]				;@ set 1st value manually, HBL is AFTER 1st line
	ldr r3,=0xA2600001			;@ noIRQ hblank 16bit repeat incsrc inc_reloaddst, 1 word
	add r0,r0,#REG_DMA3SAD
	stmia r0,{r1-r3}			;@ DMA3 go

	ldr r1,=masterFrameCounter
	ldr r0,[r1]
	add r0,r0,#1
	str r0,[r1]

	ldr r0,=mmFrame
	bx r0

;@----------------------------------------------------------------------------
copyScreenCPU:						;@ r0=dst, r1=src, r2=len.
.type copyScreenCPU STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{r4-r11,lr}
cpu8BitLoop:
	ldmia r1!,{r3-r12,lr}		;@ Read 44 bytes
	stmia r0!,{r3-r12,lr}		;@ Write 44 bytes
	subs r2,r2,#44
	bhi cpu8BitLoop

	ldmfd sp!,{r4-r11,lr}
	bx lr

;@----------------------------------------------------------------------------
	.section .text
;@----------------------------------------------------------------------------
darkenPalette:						;@ r0=dst, r1=src, r2=len. palette (BGR555)
	.type darkenPalette STT_FUNC
;@----------------------------------------------------------------------------
	stmfd sp!,{r4-r11,lr}

	mov r7,#1
darkLoop:
	ldrh r3,[r1],#2
	add r6,r7,r3,lsr#10
	and r5,r3,#0x3E0
	add r5,r7,r5,lsr#5
	and r3,r3,#0x1F
	add r4,r7,r3

	mul r10,r6,r6
	add r10,r10,r6,lsl#4
	sub r10,r10,#1
	mov r10,r10,lsr#6
	mul r9,r5,r5
	add r9,r9,r5,lsl#3
	sub r9,r9,#1
	mov r9,r9,lsr#6
	mul r8,r4,r4
	add r8,r8,r4,lsl#3
	sub r8,r8,#1
	mov r8,r8,lsr#6

	orr r3,r8,r10,lsl#10
	orr r3,r3,r9,lsl#5

	strh r3,[r0],#2
	subs r2,r2,#1
	bne darkLoop

	ldmfd sp!,{r4-r11,lr}
	bx lr

;@----------------------------------------------------------------------------
copyWithDMA:						;@ r0=dst, r1=src, r2=len.
	.type copyWithDMA STT_FUNC
;@----------------------------------------------------------------------------
	mov r3,r0
	mov r0,r1
	mov r1,r3
	orr r2,r2,#0x84000000			;@ dma start, 32bit
	mov r3,#REG_BASE
	add r3,r3,#REG_DMA3SAD
	stmia r3,{r0-r2}			;@ DMA3 go
	bx lr

;@----------------------------------------------------------------------------
fadeBuffer:
;@----------------------------------------------------------------------------
	.short 0x1000, 0x1001, 0x1002, 0x1003, 0x1004, 0x1005, 0x1006, 0x1007, 0x1008, 0x1009, 0x100A, 0x100B, 0x100C, 0x100D, 0x100E, 0x100F
	.short 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010
	.short 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010
	.short 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x1010, 0x100F, 0x100E, 0x100D, 0x100C, 0x100B
	.short 0x100A, 0x1009, 0x1008, 0x1007, 0x1006, 0x1005, 0x1004, 0x1003, 0x1002, 0x1001, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000
	.short 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000
	.short 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000
	.short 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000
	.short 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000
	.short 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000
;@----------------------------------------------------------------------------

bg0XPos:	.long 0
	.align 2

	.section .sbss
	.align 2
OAM_BUFFER:
	.space 0x400
scrollBuffer:
	.space SCREEN_HEIGHT*4				;@ Scroll reg buffer 0
masterFrameCounter:
	.long 0

;@----------------------------------------------------------------------------

