;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.2.0 #13081 (MINGW64)
;--------------------------------------------------------
	.module funseg
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl __flash_write
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area DABS (ABS)

; default segment ordering for linker
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area CONST
	.area INITIALIZER
	.area CODE

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area FUN_SEG
;	src/funseg.c: 6: void _flash_write(uint8_t *buf, uint8_t *addr)
;	-----------------------------------------
;	 function _flash_write
;	-----------------------------------------
__flash_write:
	push	a
;	src/funseg.c: 9: FLASH->CR2 |= 0x01;
	bset	0x5051, #0
;	src/funseg.c: 10: for(i = 0;i < 128;i++)
	ldw	y, (0x04, sp)
	clr	(0x01, sp)
00102$:
;	src/funseg.c: 12: *addr++ = *buf++;
	ld	a, (x)
	incw	x
	ld	(y), a
	incw	y
;	src/funseg.c: 10: for(i = 0;i < 128;i++)
	inc	(0x01, sp)
	ld	a, (0x01, sp)
	cp	a, #0x80
	jrc	00102$
;	src/funseg.c: 14: }
	ldw	x, (2, sp)
	addw	sp, #5
	jp	(x)
	.area FUN_SEG
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)
