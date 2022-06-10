                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 4.2.0 #13081 (MINGW64)
                                      4 ;--------------------------------------------------------
                                      5 	.module funcopy
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl __flash_write
                                     12 	.globl _FUN_SIZE
                                     13 	.globl _funcopy
                                     14 ;--------------------------------------------------------
                                     15 ; ram data
                                     16 ;--------------------------------------------------------
                                     17 	.area DATA
      000186                         18 _FUN_SIZE::
      000186                         19 	.ds 1
                                     20 ;--------------------------------------------------------
                                     21 ; ram data
                                     22 ;--------------------------------------------------------
                                     23 	.area INITIALIZED
                                     24 ;--------------------------------------------------------
                                     25 ; absolute external ram data
                                     26 ;--------------------------------------------------------
                                     27 	.area DABS (ABS)
                                     28 
                                     29 ; default segment ordering for linker
                                     30 	.area HOME
                                     31 	.area GSINIT
                                     32 	.area GSFINAL
                                     33 	.area CONST
                                     34 	.area INITIALIZER
                                     35 	.area CODE
                                     36 
                                     37 ;--------------------------------------------------------
                                     38 ; global & static initialisations
                                     39 ;--------------------------------------------------------
                                     40 	.area HOME
                                     41 	.area GSINIT
                                     42 	.area GSFINAL
                                     43 	.area GSINIT
                                     44 ;--------------------------------------------------------
                                     45 ; Home
                                     46 ;--------------------------------------------------------
                                     47 	.area HOME
                                     48 	.area HOME
                                     49 ;--------------------------------------------------------
                                     50 ; code
                                     51 ;--------------------------------------------------------
                                     52 	.area CODE
                                     53 ;	src/funcopy.c: 16: void funcopy(unsigned char *dest)
                                     54 ;	-----------------------------------------
                                     55 ;	 function funcopy
                                     56 ;	-----------------------------------------
      0081B5                         57 _funcopy:
      0081B5 88               [ 1]   58 	push	a
                                     59 ;	src/funcopy.c: 19: const unsigned char *p_src = (const unsigned char*)_flash_write;
      0081B6 90 AE 81 D3      [ 2]   60 	ldw	y, #(__flash_write+0)
                                     61 ;	src/funcopy.c: 9: __asm__("mov _FUN_SIZE, #l_FUN_SEG");
      0081BA 35 1C 01 86      [ 1]   62 	mov	_FUN_SIZE, #l_FUN_SEG
                                     63 ;	src/funcopy.c: 22: for (i = 0; i < FUN_SIZE; i++)
      0081BE 0F 01            [ 1]   64 	clr	(0x01, sp)
      0081C0                         65 00104$:
      0081C0 7B 01            [ 1]   66 	ld	a, (0x01, sp)
      0081C2 C1 01 86         [ 1]   67 	cp	a, _FUN_SIZE+0
      0081C5 24 0A            [ 1]   68 	jrnc	00106$
                                     69 ;	src/funcopy.c: 24: *p_dest++ = *p_src++;
      0081C7 90 F6            [ 1]   70 	ld	a, (y)
      0081C9 90 5C            [ 1]   71 	incw	y
      0081CB F7               [ 1]   72 	ld	(x), a
      0081CC 5C               [ 1]   73 	incw	x
                                     74 ;	src/funcopy.c: 22: for (i = 0; i < FUN_SIZE; i++)
      0081CD 0C 01            [ 1]   75 	inc	(0x01, sp)
      0081CF 20 EF            [ 2]   76 	jra	00104$
      0081D1                         77 00106$:
                                     78 ;	src/funcopy.c: 26: }
      0081D1 84               [ 1]   79 	pop	a
      0081D2 81               [ 4]   80 	ret
                                     81 	.area CODE
                                     82 	.area CONST
                                     83 	.area INITIALIZER
                                     84 	.area CABS (ABS)
