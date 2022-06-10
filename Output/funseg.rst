                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 4.2.0 #13081 (MINGW64)
                                      4 ;--------------------------------------------------------
                                      5 	.module funseg
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl __flash_write
                                     12 ;--------------------------------------------------------
                                     13 ; ram data
                                     14 ;--------------------------------------------------------
                                     15 	.area DATA
                                     16 ;--------------------------------------------------------
                                     17 ; ram data
                                     18 ;--------------------------------------------------------
                                     19 	.area INITIALIZED
                                     20 ;--------------------------------------------------------
                                     21 ; absolute external ram data
                                     22 ;--------------------------------------------------------
                                     23 	.area DABS (ABS)
                                     24 
                                     25 ; default segment ordering for linker
                                     26 	.area HOME
                                     27 	.area GSINIT
                                     28 	.area GSFINAL
                                     29 	.area CONST
                                     30 	.area INITIALIZER
                                     31 	.area CODE
                                     32 
                                     33 ;--------------------------------------------------------
                                     34 ; global & static initialisations
                                     35 ;--------------------------------------------------------
                                     36 	.area HOME
                                     37 	.area GSINIT
                                     38 	.area GSFINAL
                                     39 	.area GSINIT
                                     40 ;--------------------------------------------------------
                                     41 ; Home
                                     42 ;--------------------------------------------------------
                                     43 	.area HOME
                                     44 	.area HOME
                                     45 ;--------------------------------------------------------
                                     46 ; code
                                     47 ;--------------------------------------------------------
                                     48 	.area FUN_SEG
                                     49 ;	src/funseg.c: 6: void _flash_write(uint8_t *buf, uint8_t *addr)
                                     50 ;	-----------------------------------------
                                     51 ;	 function _flash_write
                                     52 ;	-----------------------------------------
      0081D3                         53 __flash_write:
      0081D3 88               [ 1]   54 	push	a
                                     55 ;	src/funseg.c: 9: FLASH->CR2 |= 0x01;
      0081D4 72 10 50 51      [ 1]   56 	bset	0x5051, #0
                                     57 ;	src/funseg.c: 10: for(i = 0;i < 128;i++)
      0081D8 16 04            [ 2]   58 	ldw	y, (0x04, sp)
      0081DA 0F 01            [ 1]   59 	clr	(0x01, sp)
      0081DC                         60 00102$:
                                     61 ;	src/funseg.c: 12: *addr++ = *buf++;
      0081DC F6               [ 1]   62 	ld	a, (x)
      0081DD 5C               [ 1]   63 	incw	x
      0081DE 90 F7            [ 1]   64 	ld	(y), a
      0081E0 90 5C            [ 1]   65 	incw	y
                                     66 ;	src/funseg.c: 10: for(i = 0;i < 128;i++)
      0081E2 0C 01            [ 1]   67 	inc	(0x01, sp)
      0081E4 7B 01            [ 1]   68 	ld	a, (0x01, sp)
      0081E6 A1 80            [ 1]   69 	cp	a, #0x80
      0081E8 25 F2            [ 1]   70 	jrc	00102$
                                     71 ;	src/funseg.c: 14: }
      0081EA 1E 02            [ 2]   72 	ldw	x, (2, sp)
      0081EC 5B 05            [ 2]   73 	addw	sp, #5
      0081EE FC               [ 2]   74 	jp	(x)
                                     75 	.area FUN_SEG
                                     76 	.area CONST
                                     77 	.area INITIALIZER
                                     78 	.area CABS (ABS)
