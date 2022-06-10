                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 4.2.0 #13081 (MINGW64)
                                      4 ;--------------------------------------------------------
                                      5 	.module main
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl __vect
                                     12 	.globl __version
                                     13 	.globl _main
                                     14 	.globl _UART1_RcvB
                                     15 	.globl _reserved_interrupt29
                                     16 	.globl _funcopy
                                     17 	.globl _tryCnt
                                     18 	.globl _buf
                                     19 	.globl _fun_buf
                                     20 	.globl _addr
                                     21 	.globl _verify
                                     22 	.globl _page
                                     23 	.globl _ch
                                     24 ;--------------------------------------------------------
                                     25 ; ram data
                                     26 ;--------------------------------------------------------
                                     27 	.area DATA
      000001                         28 _ch::
      000001                         29 	.ds 1
      000002                         30 _page::
      000002                         31 	.ds 1
      000003                         32 _verify::
      000003                         33 	.ds 1
      000004                         34 _addr::
      000004                         35 	.ds 2
      000006                         36 _fun_buf::
      000006                         37 	.ds 256
      000106                         38 _buf::
      000106                         39 	.ds 128
                                     40 ;--------------------------------------------------------
                                     41 ; ram data
                                     42 ;--------------------------------------------------------
                                     43 	.area INITIALIZED
      000187                         44 _tryCnt::
      000187                         45 	.ds 2
                                     46 ;--------------------------------------------------------
                                     47 ; Stack segment in internal ram
                                     48 ;--------------------------------------------------------
                                     49 	.area	SSEG
      000189                         50 __start__stack:
      000189                         51 	.ds	1
                                     52 
                                     53 ;--------------------------------------------------------
                                     54 ; absolute external ram data
                                     55 ;--------------------------------------------------------
                                     56 	.area DABS (ABS)
                                     57 
                                     58 ; default segment ordering for linker
                                     59 	.area HOME
                                     60 	.area GSINIT
                                     61 	.area GSFINAL
                                     62 	.area CONST
                                     63 	.area INITIALIZER
                                     64 	.area CODE
                                     65 
                                     66 ;--------------------------------------------------------
                                     67 ; interrupt vector
                                     68 ;--------------------------------------------------------
                                     69 	.area HOME
      008000                         70 __interrupt_vect:
      008000 82 00 80 83             71 	int s_GSINIT ; reset
      008004 82 00 00 00             72 	int 0x000000 ; trap
      008008 82 00 00 00             73 	int 0x000000 ; int0
      00800C 82 00 00 00             74 	int 0x000000 ; int1
      008010 82 00 00 00             75 	int 0x000000 ; int2
      008014 82 00 00 00             76 	int 0x000000 ; int3
      008018 82 00 00 00             77 	int 0x000000 ; int4
      00801C 82 00 00 00             78 	int 0x000000 ; int5
      008020 82 00 00 00             79 	int 0x000000 ; int6
      008024 82 00 00 00             80 	int 0x000000 ; int7
      008028 82 00 00 00             81 	int 0x000000 ; int8
      00802C 82 00 00 00             82 	int 0x000000 ; int9
      008030 82 00 00 00             83 	int 0x000000 ; int10
      008034 82 00 00 00             84 	int 0x000000 ; int11
      008038 82 00 00 00             85 	int 0x000000 ; int12
      00803C 82 00 00 00             86 	int 0x000000 ; int13
      008040 82 00 00 00             87 	int 0x000000 ; int14
      008044 82 00 00 00             88 	int 0x000000 ; int15
      008048 82 00 00 00             89 	int 0x000000 ; int16
      00804C 82 00 00 00             90 	int 0x000000 ; int17
      008050 82 00 00 00             91 	int 0x000000 ; int18
      008054 82 00 00 00             92 	int 0x000000 ; int19
      008058 82 00 00 00             93 	int 0x000000 ; int20
      00805C 82 00 00 00             94 	int 0x000000 ; int21
      008060 82 00 00 00             95 	int 0x000000 ; int22
      008064 82 00 00 00             96 	int 0x000000 ; int23
      008068 82 00 00 00             97 	int 0x000000 ; int24
      00806C 82 00 00 00             98 	int 0x000000 ; int25
      008070 82 00 00 00             99 	int 0x000000 ; int26
      008074 82 00 00 00            100 	int 0x000000 ; int27
      008078 82 00 00 00            101 	int 0x000000 ; int28
      00807C 82 00 80 A2            102 	int _reserved_interrupt29 ; int29
                                    103 ;--------------------------------------------------------
                                    104 ; global & static initialisations
                                    105 ;--------------------------------------------------------
                                    106 	.area HOME
                                    107 	.area GSINIT
                                    108 	.area GSFINAL
                                    109 	.area GSINIT
      008083                        110 __sdcc_init_data:
                                    111 ; stm8_genXINIT() start
      008083 AE 01 86         [ 2]  112 	ldw x, #l_DATA
      008086 27 07            [ 1]  113 	jreq	00002$
      008088                        114 00001$:
      008088 72 4F 00 00      [ 1]  115 	clr (s_DATA - 1, x)
      00808C 5A               [ 2]  116 	decw x
      00808D 26 F9            [ 1]  117 	jrne	00001$
      00808F                        118 00002$:
      00808F AE 00 02         [ 2]  119 	ldw	x, #l_INITIALIZER
      008092 27 09            [ 1]  120 	jreq	00004$
      008094                        121 00003$:
      008094 D6 80 9F         [ 1]  122 	ld	a, (s_INITIALIZER - 1, x)
      008097 D7 01 86         [ 1]  123 	ld	(s_INITIALIZED - 1, x), a
      00809A 5A               [ 2]  124 	decw	x
      00809B 26 F7            [ 1]  125 	jrne	00003$
      00809D                        126 00004$:
                                    127 ; stm8_genXINIT() end
                                    128 	.area GSFINAL
      00809D CC 80 80         [ 2]  129 	jp	__sdcc_program_startup
                                    130 ;--------------------------------------------------------
                                    131 ; Home
                                    132 ;--------------------------------------------------------
                                    133 	.area HOME
                                    134 	.area HOME
      008080                        135 __sdcc_program_startup:
      008080 CC 80 AC         [ 2]  136 	jp	_main
                                    137 ;	return from main will return to caller
                                    138 ;--------------------------------------------------------
                                    139 ; code
                                    140 ;--------------------------------------------------------
                                    141 	.area CODE
                                    142 ;	src/main.c: 41: void reserved_interrupt29(void) __interrupt(29){}
                                    143 ;	-----------------------------------------
                                    144 ;	 function reserved_interrupt29
                                    145 ;	-----------------------------------------
      0080A2                        146 _reserved_interrupt29:
      0080A2 80               [11]  147 	iret
                                    148 ;	src/main.c: 66: u8 UART1_RcvB(void)
                                    149 ;	-----------------------------------------
                                    150 ;	 function UART1_RcvB
                                    151 ;	-----------------------------------------
      0080A3                        152 _UART1_RcvB:
                                    153 ;	src/main.c: 68: while(!(USART1->SR & (u8)USART_FLAG_RXNE));
      0080A3                        154 00101$:
      0080A3 72 0B 52 30 FB   [ 2]  155 	btjf	0x5230, #5, 00101$
                                    156 ;	src/main.c: 69: return ((uint8_t)USART1->DR);
      0080A8 C6 52 31         [ 1]  157 	ld	a, 0x5231
                                    158 ;	src/main.c: 70: }
      0080AB 81               [ 4]  159 	ret
                                    160 ;	src/main.c: 77: void main(void)
                                    161 ;	-----------------------------------------
                                    162 ;	 function main
                                    163 ;	-----------------------------------------
      0080AC                        164 _main:
      0080AC 52 02            [ 2]  165 	sub	sp, #2
                                    166 ;	src/main.c: 80: CLK->CKDIVR = (uint8_t)(CLK_SYSCLKDiv_1);
      0080AE 35 00 50 C0      [ 1]  167 	mov	0x50c0+0, #0x00
                                    168 ;	src/main.c: 87: CLK->PCKENR1 = 1 << 5;
      0080B2 35 20 50 C3      [ 1]  169 	mov	0x50c3+0, #0x20
                                    170 ;	src/main.c: 88: USART1->CR1 = 0x00;
      0080B6 35 00 52 34      [ 1]  171 	mov	0x5234+0, #0x00
                                    172 ;	src/main.c: 89: USART1->CR3 = 0x00;
      0080BA 35 00 52 36      [ 1]  173 	mov	0x5236+0, #0x00
                                    174 ;	src/main.c: 91: USART1->BRR2 = 0x0B;
      0080BE 35 0B 52 33      [ 1]  175 	mov	0x5233+0, #0x0b
                                    176 ;	src/main.c: 92: USART1->BRR1 = 0x08;
      0080C2 35 08 52 32      [ 1]  177 	mov	0x5232+0, #0x08
                                    178 ;	src/main.c: 94: USART1->CR2 = 0x0C;
      0080C6 35 0C 52 35      [ 1]  179 	mov	0x5235+0, #0x0c
                                    180 ;	src/main.c: 96: while(i)
      0080CA A6 0A            [ 1]  181 	ld	a, #0x0a
      0080CC 6B 02            [ 1]  182 	ld	(0x02, sp), a
      0080CE                        183 00107$:
      0080CE 0D 02            [ 1]  184 	tnz	(0x02, sp)
      0080D0 27 20            [ 1]  185 	jreq	00109$
                                    186 ;	src/main.c: 98: if(USART1->SR & (u8)USART_FLAG_RXNE)    //wait for head 
      0080D2 C6 52 30         [ 1]  187 	ld	a, 0x5230
      0080D5 A5 20            [ 1]  188 	bcp	a, #0x20
      0080D7 27 0C            [ 1]  189 	jreq	00104$
                                    190 ;	src/main.c: 100: ch = (uint8_t)USART1->DR;
      0080D9 55 52 31 00 01   [ 1]  191 	mov	_ch+0, 0x5231
                                    192 ;	src/main.c: 101: if(ch == BOOT_HEAD) break;
      0080DE C6 00 01         [ 1]  193 	ld	a, _ch+0
      0080E1 A1 A5            [ 1]  194 	cp	a, #0xa5
      0080E3 27 0D            [ 1]  195 	jreq	00109$
      0080E5                        196 00104$:
                                    197 ;	src/main.c: 103: tryCnt--;
      0080E5 CE 01 87         [ 2]  198 	ldw	x, _tryCnt+0
      0080E8 5A               [ 2]  199 	decw	x
                                    200 ;	src/main.c: 104: if(tryCnt == 0) i--;
      0080E9 CF 01 87         [ 2]  201 	ldw	_tryCnt+0, x
      0080EC 26 E0            [ 1]  202 	jrne	00107$
      0080EE 0A 02            [ 1]  203 	dec	(0x02, sp)
      0080F0 20 DC            [ 2]  204 	jra	00107$
      0080F2                        205 00109$:
                                    206 ;	src/main.c: 107: if(i == 0)
      0080F2 0D 02            [ 1]  207 	tnz	(0x02, sp)
      0080F4 26 06            [ 1]  208 	jrne	00124$
                                    209 ;	src/main.c: 109: __asm__("JP 0x8200");
      0080F6 CC 82 00         [ 2]  210 	JP	0x8200
      0080F9 CC 81 B2         [ 2]  211 	jp	00130$
      0080FC                        212 00124$:
                                    213 ;	src/main.c: 113: funcopy(fun_buf);       //复制函数到RAM
      0080FC AE 00 06         [ 2]  214 	ldw	x, #(_fun_buf+0)
      0080FF CD 81 B5         [ 4]  215 	call	_funcopy
                                    216 ;	src/main.c: 114: FLASH->PUKR = 0x56;
      008102 35 56 50 52      [ 1]  217 	mov	0x5052+0, #0x56
                                    218 ;	src/main.c: 115: FLASH->PUKR = 0xAE;
      008106 35 AE 50 52      [ 1]  219 	mov	0x5052+0, #0xae
                                    220 ;	src/main.c: 116: USART1->DR = (0xa0|INIT_PAGE);
      00810A 35 A4 52 31      [ 1]  221 	mov	0x5231+0, #0xa4
                                    222 ;	src/main.c: 117: while(1)
      00810E                        223 00121$:
                                    224 ;	src/main.c: 119: ch = UART1_RcvB();
      00810E CD 80 A3         [ 4]  225 	call	_UART1_RcvB
                                    226 ;	src/main.c: 120: switch(ch)
      008111 C7 00 01         [ 1]  227 	ld	_ch+0, a
      008114 A1 A7            [ 1]  228 	cp	a, #0xa7
      008116 27 17            [ 1]  229 	jreq	00111$
      008118 C6 00 01         [ 1]  230 	ld	a, _ch+0
      00811B A1 A9            [ 1]  231 	cp	a, #0xa9
      00811D 27 03            [ 1]  232 	jreq	00217$
      00811F CC 81 AB         [ 2]  233 	jp	00118$
      008122                        234 00217$:
                                    235 ;	src/main.c: 123: FLASH->IAPSR &= 0xfd;
      008122 C6 50 54         [ 1]  236 	ld	a, 0x5054
      008125 A4 FD            [ 1]  237 	and	a, #0xfd
      008127 C7 50 54         [ 1]  238 	ld	0x5054, a
                                    239 ;	src/main.c: 124: __asm__("JP 0x8200");
      00812A CC 82 00         [ 2]  240 	JP	0x8200
                                    241 ;	src/main.c: 125: break;
      00812D 20 DF            [ 2]  242 	jra	00121$
                                    243 ;	src/main.c: 126: case BOOT_WRITE:
      00812F                        244 00111$:
                                    245 ;	src/main.c: 127: page = UART1_RcvB();
      00812F CD 80 A3         [ 4]  246 	call	_UART1_RcvB
      008132 C7 00 02         [ 1]  247 	ld	_page+0, a
                                    248 ;	src/main.c: 128: addr = (u8*)(FLASH_START + (page << BLOCK_SHIFT));
      008135 5F               [ 1]  249 	clrw	x
      008136 C6 00 02         [ 1]  250 	ld	a, _page+0
      008139 97               [ 1]  251 	ld	xl, a
      00813A 58               [ 2]  252 	sllw	x
      00813B 58               [ 2]  253 	sllw	x
      00813C 58               [ 2]  254 	sllw	x
      00813D 58               [ 2]  255 	sllw	x
      00813E 58               [ 2]  256 	sllw	x
      00813F 58               [ 2]  257 	sllw	x
      008140 58               [ 2]  258 	sllw	x
      008141 1C 80 00         [ 2]  259 	addw	x, #0x8000
      008144 CF 00 04         [ 2]  260 	ldw	_addr+0, x
                                    261 ;	src/main.c: 129: verify = 0;
      008147 72 5F 00 03      [ 1]  262 	clr	_verify+0
                                    263 ;	src/main.c: 130: for(i = 0; i < BLOCK_BYTES; i++)   
      00814B 0F 02            [ 1]  264 	clr	(0x02, sp)
      00814D                        265 00126$:
                                    266 ;	src/main.c: 132: buf[i] = UART1_RcvB();
      00814D 5F               [ 1]  267 	clrw	x
      00814E 7B 02            [ 1]  268 	ld	a, (0x02, sp)
      008150 97               [ 1]  269 	ld	xl, a
      008151 1C 01 06         [ 2]  270 	addw	x, #(_buf+0)
      008154 89               [ 2]  271 	pushw	x
      008155 CD 80 A3         [ 4]  272 	call	_UART1_RcvB
      008158 85               [ 2]  273 	popw	x
      008159 F7               [ 1]  274 	ld	(x), a
                                    275 ;	src/main.c: 133: verify += buf[i];
      00815A CB 00 03         [ 1]  276 	add	a, _verify+0
      00815D C7 00 03         [ 1]  277 	ld	_verify+0, a
                                    278 ;	src/main.c: 130: for(i = 0; i < BLOCK_BYTES; i++)   
      008160 0C 02            [ 1]  279 	inc	(0x02, sp)
      008162 7B 02            [ 1]  280 	ld	a, (0x02, sp)
      008164 A1 80            [ 1]  281 	cp	a, #0x80
      008166 25 E5            [ 1]  282 	jrc	00126$
                                    283 ;	src/main.c: 135: if(verify == UART1_RcvB())  //通信校验成功
      008168 CD 80 A3         [ 4]  284 	call	_UART1_RcvB
      00816B C1 00 03         [ 1]  285 	cp	a, _verify+0
      00816E 26 3B            [ 1]  286 	jrne	00118$
                                    287 ;	src/main.c: 137: (*(flash_write)fun_buf)(buf, addr);   //运行RAM中的写flash函数
      008170 90 AE 00 06      [ 2]  288 	ldw	y, #(_fun_buf+0)
      008174 3B 00 05         [ 1]  289 	push	_addr+1
      008177 3B 00 04         [ 1]  290 	push	_addr+0
      00817A AE 01 06         [ 2]  291 	ldw	x, #(_buf+0)
      00817D 90 FD            [ 4]  292 	call	(y)
                                    293 ;	src/main.c: 139: for(i = 0; i < BLOCK_BYTES; i++)   
      00817F 0F 02            [ 1]  294 	clr	(0x02, sp)
      008181                        295 00128$:
                                    296 ;	src/main.c: 141: verify -= *addr++;
      008181 CE 00 04         [ 2]  297 	ldw	x, _addr+0
      008184 F6               [ 1]  298 	ld	a, (x)
      008185 6B 01            [ 1]  299 	ld	(0x01, sp), a
      008187 CE 00 04         [ 2]  300 	ldw	x, _addr+0
      00818A 5C               [ 1]  301 	incw	x
      00818B CF 00 04         [ 2]  302 	ldw	_addr+0, x
      00818E C6 00 03         [ 1]  303 	ld	a, _verify+0
      008191 10 01            [ 1]  304 	sub	a, (0x01, sp)
      008193 C7 00 03         [ 1]  305 	ld	_verify+0, a
                                    306 ;	src/main.c: 139: for(i = 0; i < BLOCK_BYTES; i++)   
      008196 0C 02            [ 1]  307 	inc	(0x02, sp)
      008198 7B 02            [ 1]  308 	ld	a, (0x02, sp)
      00819A A1 80            [ 1]  309 	cp	a, #0x80
      00819C 25 E3            [ 1]  310 	jrc	00128$
                                    311 ;	src/main.c: 143: if(verify == 0)  //写入校验成功
      00819E 72 5D 00 03      [ 1]  312 	tnz	_verify+0
      0081A2 26 07            [ 1]  313 	jrne	00118$
                                    314 ;	src/main.c: 145: USART1->DR = (BOOT_OK);
      0081A4 35 A0 52 31      [ 1]  315 	mov	0x5231+0, #0xa0
                                    316 ;	src/main.c: 146: break;
      0081A8 CC 81 0E         [ 2]  317 	jp	00121$
                                    318 ;	src/main.c: 150: default: //上面校验失败的情况也会到这里来
      0081AB                        319 00118$:
                                    320 ;	src/main.c: 151: USART1->DR = (BOOT_ERR);
      0081AB 35 A1 52 31      [ 1]  321 	mov	0x5231+0, #0xa1
                                    322 ;	src/main.c: 153: }
      0081AF CC 81 0E         [ 2]  323 	jp	00121$
      0081B2                        324 00130$:
                                    325 ;	src/main.c: 156: }
      0081B2 5B 02            [ 2]  326 	addw	sp, #2
      0081B4 81               [ 4]  327 	ret
                                    328 	.area CODE
                                    329 	.area CONST
                                    330 	.area INITIALIZER
      0080A0                        331 __xinit__tryCnt:
      0080A0 FF FF                  332 	.dw #0xffff
                                    333 	.area CABS (ABS)
      0081FA                        334 	.org 0x81FA
      0081FA                        335 __version:
      0081FA 56 30 2E 30 31         336 	.ascii "V0.01"
      0081FF 00                     337 	.db 0x00
      008200 00                     338 	.db 0x00
      008004                        339 	.org 0x8004
      008004                        340 __vect:
      008004 82 00 82 04            341 	.byte #0x82, #0x00, #0x82, #0x04	; 2181071364
      008008 82 00 80 00            342 	.byte #0x82, #0x00, #0x80, #0x00	; 2181070848
      00800C 82 00 82 0C            343 	.byte #0x82, #0x00, #0x82, #0x0c	; 2181071372
      008010 82 00 82 10            344 	.byte #0x82, #0x00, #0x82, #0x10	; 2181071376
      008014 82 00 82 14            345 	.byte #0x82, #0x00, #0x82, #0x14	; 2181071380
      008018 82 00 82 18            346 	.byte #0x82, #0x00, #0x82, #0x18	; 2181071384
      00801C 82 00 82 1C            347 	.byte #0x82, #0x00, #0x82, #0x1c	; 2181071388
      008020 82 00 82 20            348 	.byte #0x82, #0x00, #0x82, #0x20	; 2181071392
      008024 82 00 82 24            349 	.byte #0x82, #0x00, #0x82, #0x24	; 2181071396
      008028 82 00 82 28            350 	.byte #0x82, #0x00, #0x82, #0x28	; 2181071400
      00802C 82 00 82 2C            351 	.byte #0x82, #0x00, #0x82, #0x2c	; 2181071404
      008030 82 00 82 30            352 	.byte #0x82, #0x00, #0x82, #0x30	; 2181071408
      008034 82 00 82 34            353 	.byte #0x82, #0x00, #0x82, #0x34	; 2181071412
      008038 82 00 82 38            354 	.byte #0x82, #0x00, #0x82, #0x38	; 2181071416
      00803C 82 00 82 3C            355 	.byte #0x82, #0x00, #0x82, #0x3c	; 2181071420
      008040 82 00 82 40            356 	.byte #0x82, #0x00, #0x82, #0x40	; 2181071424
      008044 82 00 82 44            357 	.byte #0x82, #0x00, #0x82, #0x44	; 2181071428
      008048 82 00 82 48            358 	.byte #0x82, #0x00, #0x82, #0x48	; 2181071432
      00804C 82 00 82 4C            359 	.byte #0x82, #0x00, #0x82, #0x4c	; 2181071436
      008050 82 00 82 50            360 	.byte #0x82, #0x00, #0x82, #0x50	; 2181071440
      008054 82 00 82 54            361 	.byte #0x82, #0x00, #0x82, #0x54	; 2181071444
      008058 82 00 82 58            362 	.byte #0x82, #0x00, #0x82, #0x58	; 2181071448
      00805C 82 00 82 5C            363 	.byte #0x82, #0x00, #0x82, #0x5c	; 2181071452
      008060 82 00 82 60            364 	.byte #0x82, #0x00, #0x82, #0x60	; 2181071456
      008064 82 00 82 64            365 	.byte #0x82, #0x00, #0x82, #0x64	; 2181071460
      008068 82 00 82 68            366 	.byte #0x82, #0x00, #0x82, #0x68	; 2181071464
      00806C 82 00 82 6C            367 	.byte #0x82, #0x00, #0x82, #0x6c	; 2181071468
      008070 82 00 82 70            368 	.byte #0x82, #0x00, #0x82, #0x70	; 2181071472
      008074 82 00 82 74            369 	.byte #0x82, #0x00, #0x82, #0x74	; 2181071476
      008078 82 00 82 78            370 	.byte #0x82, #0x00, #0x82, #0x78	; 2181071480
      00807C 82 00 82 7C            371 	.byte #0x82, #0x00, #0x82, #0x7c	; 2181071484
