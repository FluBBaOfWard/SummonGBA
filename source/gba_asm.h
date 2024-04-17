
#ifndef VIDEO_ARM9_INCLUDE
#define VIDEO_ARM9_INCLUDE


#define BG_PALETTE			0x05000000		/** \brief background palette memory*/
#define BG_PALETTE_SUB		0x05000400		/** \brief background palette memory (sub engine)*/

#define SPRITE_PALETTE		0x05000200		/** \brief sprite palette memory*/
#define SPRITE_PALETTE_SUB	0x05000600		/** \brief sprite palette memory (sub engine)*/

#define BG_GFX				0x06000000		/** \brief background graphics memory*/
#define BG_GFX_SUB			0x06200000		/** \brief background graphics memory (sub engine)*/
#define SPRITE_GFX			0x06400000		/** \brief sprite graphics memory*/
#define SPRITE_GFX_SUB		0x06600000		/** \brief sprite graphics memory (sub engine)*/

#define VRAM_0				0x06000000
#define VRAM				0x06800000


#define VRAM_A				(0x06800000)	/*!< \brief  pointer to vram bank A mapped as LCD*/
#define VRAM_B				(0x06820000)	/*!< \brief pointer to vram bank B mapped as LCD*/
#define VRAM_C				(0x06840000)	/*!< \brief pointer to vram bank C mapped as LCD*/
#define VRAM_D				(0x06860000)	/*!< \brief pointer to vram bank D mapped as LCD*/
#define VRAM_E				(0x06880000)	/*!< \brief pointer to vram bank E mapped as LCD*/
#define VRAM_F				(0x06890000)	/*!< \brief pointer to vram bank F mapped as LCD*/
#define VRAM_G				(0x06894000)	/*!< \brief pointer to vram bank G mapped as LCD*/
#define VRAM_H				(0x06898000)	/*!< \brief pointer to vram bank H mapped as LCD*/
#define VRAM_I				(0x068A0000)	/*!< \brief pointer to vram bank I mapped as LCD*/

#define OAM					(0x07000000)	/*!< \brief pointer to Object Attribute Memory*/
#define OAM_SUB				(0x07000400)	/*!< \brief pointer to Object Attribute Memory (Sub engine)*/

// macro creates a 15 bit color from 3x5 bit components
/** \brief  Macro to convert 5 bit r g b components into a single 15 bit RGB triplet */
#define RGB15(r,g,b)  ((r)|((g)<<5)|((b)<<10))
#define RGB5(r,g,b)  ((r)|((g)<<5)|((b)<<10))
#define RGB8(r,g,b)  (((r)>>3)|(((g)>>3)<<5)|(((b)>>3)<<10))
/** \brief  Macro to convert 5 bit r g b components plus 1 bit alpha into a single 16 bit ARGB triplet */
#define ARGB16(a, r, g, b) ( ((a) << 15) | (r)|((g)<<5)|((b)<<10))

/** \brief  Screen height in pixels */
#define SCREEN_HEIGHT (160)
/** \brief  Screen width in pixels */
#define SCREEN_WIDTH  (240)

//	Vram Control
#define VRAM_CR			(*(vu32*)0x04000240)
#define VRAM_A_CR		(*(vu8*)0x04000240)
#define VRAM_B_CR		(*(vu8*)0x04000241)
#define VRAM_C_CR		(*(vu8*)0x04000242)
#define VRAM_D_CR		(*(vu8*)0x04000243)
#define VRAM_E_CR		(*(vu8*)0x04000244)
#define VRAM_F_CR		(*(vu8*)0x04000245)
#define VRAM_G_CR		(*(vu8*)0x04000246)
#define WRAM_CR			(*(vu8*)0x04000247)
#define VRAM_H_CR		(*(vu8*)0x04000248)
#define VRAM_I_CR		(*(vu8*)0x04000249)

#define VRAM_ENABLE		(1<<7)

#define VRAM_OFFSET(n)	((n)<<3)

/** \brief  Used for accessing vram E as an external palette */
#define VRAM_E_EXT_PALETTE ((_ext_palette *)VRAM_E)

/** \brief  Used for accessing vram F as an external palette */
#define VRAM_F_EXT_PALETTE ((_ext_palette *)VRAM_F)

/** \brief  Used for accessing vram G as an external palette */
#define VRAM_G_EXT_PALETTE ((_ext_palette *)VRAM_G)

/** \brief  Used for accessing vram H as an external palette */
#define VRAM_H_EXT_PALETTE ((_ext_palette *)VRAM_H)


#define ENABLE_3D    (1<<3)
#define DISPLAY_ENABLE_SHIFT 8
#define DISPLAY_BG0_ACTIVE    (1 << 8)
#define DISPLAY_BG1_ACTIVE    (1 << 9)
#define DISPLAY_BG2_ACTIVE    (1 << 10)
#define DISPLAY_BG3_ACTIVE    (1 << 11)
#define DISPLAY_SPR_ACTIVE    (1 << 12)
#define DISPLAY_WIN0_ON       (1 << 13)
#define DISPLAY_WIN1_ON       (1 << 14)
#define DISPLAY_SPR_WIN_ON    (1 << 15)

// main display only

#define DISPLAY_SPR_HBLANK	   (1 << 23)

#define DISPLAY_SPR_1D_LAYOUT	(1 << 4)

#define DISPLAY_SPR_1D				(1 << 4)
#define DISPLAY_SPR_2D				(0 << 4)
#define DISPLAY_SPR_1D_BMP			(4 << 4)
#define DISPLAY_SPR_2D_BMP_128		(0 << 4)
#define DISPLAY_SPR_2D_BMP_256		(2 << 4)


#define DISPLAY_SPR_1D_SIZE_32		(0 << 20)
#define DISPLAY_SPR_1D_SIZE_64		(1 << 20)
#define DISPLAY_SPR_1D_SIZE_128		(2 << 20)
#define DISPLAY_SPR_1D_SIZE_256		(3 << 20)
#define DISPLAY_SPR_1D_BMP_SIZE_128	(0 << 22)
#define DISPLAY_SPR_1D_BMP_SIZE_256	(1 << 22)

//mask to clear all attributes related to sprites from display control
#define DISPLAY_SPRITE_ATTR_MASK  ((7 << 4) | (7 << 20) | (1 << 31))

#define DISPLAY_SPR_EXT_PALETTE		(1 << 31)
#define DISPLAY_BG_EXT_PALETTE		(1 << 30)

#define DISPLAY_SCREEN_OFF     (1 << 7)

// The next two defines only apply to MAIN 2d engine
// In tile modes, this is multiplied by 64KB and added to BG_TILE_BASE
// In all bitmap modes, it is not used.
#define DISPLAY_CHAR_BASE(n) (((n)&7)<<24)

// In tile modes, this is multiplied by 64KB and added to BG_MAP_BASE
// In bitmap modes, this is multiplied by 64KB and added to BG_BMP_BASE
// In large bitmap modes, this is not used
#define DISPLAY_SCREEN_BASE(n) (((n)&7)<<27)


// Display control registers
#define	REG_BASE			0x04000000

#define	REG_DISPCNT			0x00
#define REG_DISPSTAT		0x04
#define REG_VCOUNT			0x06

#define	REG_BG0CNT			0x08
#define	REG_BG1CNT			0x0A
#define	REG_BG2CNT			0x0C
#define	REG_BG3CNT			0x0E


#define	REG_BGOFFSETS		0x10
#define	REG_BG0HOFS			0x10
#define	REG_BG0VOFS			0x12
#define	REG_BG1HOFS			0x14
#define	REG_BG1VOFS			0x16
#define	REG_BG2HOFS			0x18
#define	REG_BG2VOFS			0x1A
#define	REG_BG3HOFS			0x1C
#define	REG_BG3VOFS			0x1E
#define	REG_BG2PA			0x20
#define	REG_BG2PB			0x22
#define	REG_BG2PC			0x24
#define	REG_BG2PD			0x26

#define	REG_BG2X			0x28
#define	REG_BG2Y			0x2C
#define	REG_BG3PA			0x30
#define	REG_BG3PB			0x32
#define	REG_BG3PC			0x34
#define	REG_BG3PD			0x36

#define	REG_BG3X			0x38
#define	REG_BG3Y			0x3C


#define	REG_WIN0H			0x40
#define	REG_WIN1H			0x42
#define	REG_WIN0V			0x44
#define	REG_WIN1V			0x46
#define	REG_WININ			0x48
#define	REG_WINOUT			0x4A

#define REG_MOSAIC			0x4C

#define REG_BLDCNT			0x50
#define REG_BLDALPHA		0x52
#define REG_BLDY			0x54

#define REG_DMA0SAD			0xB0
#define REG_DMA0DAD			0xB4
#define REG_DMA0CNT			0xB8
#define REG_DMA0CNT_L		0xB8
#define REG_DMA0CNT_H		0xBA

#define REG_DMA1SAD			0xBC
#define REG_DMA1DAD			0xC0
#define REG_DMA1CNT			0xC4
#define REG_DMA1CNT_L		0xC4
#define REG_DMA1CNT_H		0xC6

#define REG_DMA2SAD			0xC8
#define REG_DMA2DAD			0xCC
#define REG_DMA2CNT			0xD0
#define REG_DMA2CNT_L		0xD0
#define REG_DMA2CNT_H		0xD2

#define REG_DMA3SAD			0xD4
#define REG_DMA3DAD			0xD8
#define REG_DMA3CNT			0xDC
#define REG_DMA3CNT_L		0xDC
#define REG_DMA3CNT_H		0xDE

#define REG_IE				0x200


#define BLEND_NONE         (0<<6)
#define BLEND_ALPHA        (1<<6)
#define BLEND_FADE_WHITE   (2<<6)
#define BLEND_FADE_BLACK   (3<<6)

#define BLEND_SRC_BG0      (1<<0)
#define BLEND_SRC_BG1      (1<<1)
#define BLEND_SRC_BG2      (1<<2)
#define BLEND_SRC_BG3      (1<<3)
#define BLEND_SRC_SPRITE   (1<<4)
#define BLEND_SRC_BACKDROP (1<<5)

#define BLEND_DST_BG0      (1<<8)
#define BLEND_DST_BG1      (1<<9)
#define BLEND_DST_BG2      (1<<10)
#define BLEND_DST_BG3      (1<<11)
#define BLEND_DST_SPRITE   (1<<12)
#define BLEND_DST_BACKDROP (1<<13)


#endif
