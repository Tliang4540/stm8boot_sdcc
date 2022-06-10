;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.2.0 #13081 (MINGW64)
;--------------------------------------------------------
	.module main
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl __vect
	.globl __version
	.globl _main
	.globl _UART1_RcvB
	.globl _reserved_interrupt29
	.globl _funcopy
	.globl _tryCnt
	.globl _buf
	.globl _fun_buf
	.globl _addr
	.globl _verify
	.globl _page
	.globl _ch
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
_ch::
	.ds 1
_page::
	.ds 1
_verify::
	.ds 1
_addr::
	.ds 2
_fun_buf::
	.ds 256
_buf::
	.ds 128
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
_tryCnt::
	.ds 2
;--------------------------------------------------------
; Stack segment in internal ram
;--------------------------------------------------------
	.area	SSEG
__start__stack:
	.ds	1

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
; interrupt vector
;--------------------------------------------------------
	.area HOME
__interrupt_vect:
	int s_GSINIT ; reset
	int 0x000000 ; trap
	int 0x000000 ; int0
	int 0x000000 ; int1
	int 0x000000 ; int2
	int 0x000000 ; int3
	int 0x000000 ; int4
	int 0x000000 ; int5
	int 0x000000 ; int6
	int 0x000000 ; int7
	int 0x000000 ; int8
	int 0x000000 ; int9
	int 0x000000 ; int10
	int 0x000000 ; int11
	int 0x000000 ; int12
	int 0x000000 ; int13
	int 0x000000 ; int14
	int 0x000000 ; int15
	int 0x000000 ; int16
	int 0x000000 ; int17
	int 0x000000 ; int18
	int 0x000000 ; int19
	int 0x000000 ; int20
	int 0x000000 ; int21
	int 0x000000 ; int22
	int 0x000000 ; int23
	int 0x000000 ; int24
	int 0x000000 ; int25
	int 0x000000 ; int26
	int 0x000000 ; int27
	int 0x000000 ; int28
	int _reserved_interrupt29 ; int29
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
__sdcc_init_data:
; stm8_genXINIT() start
	ldw x, #l_DATA
	jreq	00002$
00001$:
	clr (s_DATA - 1, x)
	decw x
	jrne	00001$
00002$:
	ldw	x, #l_INITIALIZER
	jreq	00004$
00003$:
	ld	a, (s_INITIALIZER - 1, x)
	ld	(s_INITIALIZED - 1, x), a
	decw	x
	jrne	00003$
00004$:
; stm8_genXINIT() end
	.area GSFINAL
	jp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
__sdcc_program_startup:
	jp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	src/main.c: 41: void reserved_interrupt29(void) __interrupt(29){}
;	-----------------------------------------
;	 function reserved_interrupt29
;	-----------------------------------------
_reserved_interrupt29:
	iret
;	src/main.c: 66: u8 UART1_RcvB(void)
;	-----------------------------------------
;	 function UART1_RcvB
;	-----------------------------------------
_UART1_RcvB:
;	src/main.c: 68: while(!(USART1->SR & (u8)USART_FLAG_RXNE));
00101$:
	btjf	0x5230, #5, 00101$
;	src/main.c: 69: return ((uint8_t)USART1->DR);
	ld	a, 0x5231
;	src/main.c: 70: }
	ret
;	src/main.c: 77: void main(void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
	sub	sp, #2
;	src/main.c: 80: CLK->CKDIVR = (uint8_t)(CLK_SYSCLKDiv_1);
	mov	0x50c0+0, #0x00
;	src/main.c: 87: CLK->PCKENR1 = 1 << 5;
	mov	0x50c3+0, #0x20
;	src/main.c: 88: USART1->CR1 = 0x00;
	mov	0x5234+0, #0x00
;	src/main.c: 89: USART1->CR3 = 0x00;
	mov	0x5236+0, #0x00
;	src/main.c: 91: USART1->BRR2 = 0x0B;
	mov	0x5233+0, #0x0b
;	src/main.c: 92: USART1->BRR1 = 0x08;
	mov	0x5232+0, #0x08
;	src/main.c: 94: USART1->CR2 = 0x0C;
	mov	0x5235+0, #0x0c
;	src/main.c: 96: while(i)
	ld	a, #0x0a
	ld	(0x02, sp), a
00107$:
	tnz	(0x02, sp)
	jreq	00109$
;	src/main.c: 98: if(USART1->SR & (u8)USART_FLAG_RXNE)    //wait for head 
	ld	a, 0x5230
	bcp	a, #0x20
	jreq	00104$
;	src/main.c: 100: ch = (uint8_t)USART1->DR;
	mov	_ch+0, 0x5231
;	src/main.c: 101: if(ch == BOOT_HEAD) break;
	ld	a, _ch+0
	cp	a, #0xa5
	jreq	00109$
00104$:
;	src/main.c: 103: tryCnt--;
	ldw	x, _tryCnt+0
	decw	x
;	src/main.c: 104: if(tryCnt == 0) i--;
	ldw	_tryCnt+0, x
	jrne	00107$
	dec	(0x02, sp)
	jra	00107$
00109$:
;	src/main.c: 107: if(i == 0)
	tnz	(0x02, sp)
	jrne	00124$
;	src/main.c: 109: __asm__("JP 0x8200");
	JP	0x8200
	jp	00130$
00124$:
;	src/main.c: 113: funcopy(fun_buf);       //复制函数到RAM
	ldw	x, #(_fun_buf+0)
	call	_funcopy
;	src/main.c: 114: FLASH->PUKR = 0x56;
	mov	0x5052+0, #0x56
;	src/main.c: 115: FLASH->PUKR = 0xAE;
	mov	0x5052+0, #0xae
;	src/main.c: 116: USART1->DR = (0xa0|INIT_PAGE);
	mov	0x5231+0, #0xa4
;	src/main.c: 117: while(1)
00121$:
;	src/main.c: 119: ch = UART1_RcvB();
	call	_UART1_RcvB
;	src/main.c: 120: switch(ch)
	ld	_ch+0, a
	cp	a, #0xa7
	jreq	00111$
	ld	a, _ch+0
	cp	a, #0xa9
	jreq	00217$
	jp	00118$
00217$:
;	src/main.c: 123: FLASH->IAPSR &= 0xfd;
	ld	a, 0x5054
	and	a, #0xfd
	ld	0x5054, a
;	src/main.c: 124: __asm__("JP 0x8200");
	JP	0x8200
;	src/main.c: 125: break;
	jra	00121$
;	src/main.c: 126: case BOOT_WRITE:
00111$:
;	src/main.c: 127: page = UART1_RcvB();
	call	_UART1_RcvB
	ld	_page+0, a
;	src/main.c: 128: addr = (u8*)(FLASH_START + (page << BLOCK_SHIFT));
	clrw	x
	ld	a, _page+0
	ld	xl, a
	sllw	x
	sllw	x
	sllw	x
	sllw	x
	sllw	x
	sllw	x
	sllw	x
	addw	x, #0x8000
	ldw	_addr+0, x
;	src/main.c: 129: verify = 0;
	clr	_verify+0
;	src/main.c: 130: for(i = 0; i < BLOCK_BYTES; i++)   
	clr	(0x02, sp)
00126$:
;	src/main.c: 132: buf[i] = UART1_RcvB();
	clrw	x
	ld	a, (0x02, sp)
	ld	xl, a
	addw	x, #(_buf+0)
	pushw	x
	call	_UART1_RcvB
	popw	x
	ld	(x), a
;	src/main.c: 133: verify += buf[i];
	add	a, _verify+0
	ld	_verify+0, a
;	src/main.c: 130: for(i = 0; i < BLOCK_BYTES; i++)   
	inc	(0x02, sp)
	ld	a, (0x02, sp)
	cp	a, #0x80
	jrc	00126$
;	src/main.c: 135: if(verify == UART1_RcvB())  //通信校验成功
	call	_UART1_RcvB
	cp	a, _verify+0
	jrne	00118$
;	src/main.c: 137: (*(flash_write)fun_buf)(buf, addr);   //运行RAM中的写flash函数
	ldw	y, #(_fun_buf+0)
	push	_addr+1
	push	_addr+0
	ldw	x, #(_buf+0)
	call	(y)
;	src/main.c: 139: for(i = 0; i < BLOCK_BYTES; i++)   
	clr	(0x02, sp)
00128$:
;	src/main.c: 141: verify -= *addr++;
	ldw	x, _addr+0
	ld	a, (x)
	ld	(0x01, sp), a
	ldw	x, _addr+0
	incw	x
	ldw	_addr+0, x
	ld	a, _verify+0
	sub	a, (0x01, sp)
	ld	_verify+0, a
;	src/main.c: 139: for(i = 0; i < BLOCK_BYTES; i++)   
	inc	(0x02, sp)
	ld	a, (0x02, sp)
	cp	a, #0x80
	jrc	00128$
;	src/main.c: 143: if(verify == 0)  //写入校验成功
	tnz	_verify+0
	jrne	00118$
;	src/main.c: 145: USART1->DR = (BOOT_OK);
	mov	0x5231+0, #0xa0
;	src/main.c: 146: break;
	jp	00121$
;	src/main.c: 150: default: //上面校验失败的情况也会到这里来
00118$:
;	src/main.c: 151: USART1->DR = (BOOT_ERR);
	mov	0x5231+0, #0xa1
;	src/main.c: 153: }
	jp	00121$
00130$:
;	src/main.c: 156: }
	addw	sp, #2
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
__xinit__tryCnt:
	.dw #0xffff
	.area CABS (ABS)
	.org 0x81FA
__version:
	.ascii "V0.01"
	.db 0x00
	.db 0x00
	.org 0x8004
__vect:
	.byte #0x82, #0x00, #0x82, #0x04	; 2181071364
	.byte #0x82, #0x00, #0x80, #0x00	; 2181070848
	.byte #0x82, #0x00, #0x82, #0x0c	; 2181071372
	.byte #0x82, #0x00, #0x82, #0x10	; 2181071376
	.byte #0x82, #0x00, #0x82, #0x14	; 2181071380
	.byte #0x82, #0x00, #0x82, #0x18	; 2181071384
	.byte #0x82, #0x00, #0x82, #0x1c	; 2181071388
	.byte #0x82, #0x00, #0x82, #0x20	; 2181071392
	.byte #0x82, #0x00, #0x82, #0x24	; 2181071396
	.byte #0x82, #0x00, #0x82, #0x28	; 2181071400
	.byte #0x82, #0x00, #0x82, #0x2c	; 2181071404
	.byte #0x82, #0x00, #0x82, #0x30	; 2181071408
	.byte #0x82, #0x00, #0x82, #0x34	; 2181071412
	.byte #0x82, #0x00, #0x82, #0x38	; 2181071416
	.byte #0x82, #0x00, #0x82, #0x3c	; 2181071420
	.byte #0x82, #0x00, #0x82, #0x40	; 2181071424
	.byte #0x82, #0x00, #0x82, #0x44	; 2181071428
	.byte #0x82, #0x00, #0x82, #0x48	; 2181071432
	.byte #0x82, #0x00, #0x82, #0x4c	; 2181071436
	.byte #0x82, #0x00, #0x82, #0x50	; 2181071440
	.byte #0x82, #0x00, #0x82, #0x54	; 2181071444
	.byte #0x82, #0x00, #0x82, #0x58	; 2181071448
	.byte #0x82, #0x00, #0x82, #0x5c	; 2181071452
	.byte #0x82, #0x00, #0x82, #0x60	; 2181071456
	.byte #0x82, #0x00, #0x82, #0x64	; 2181071460
	.byte #0x82, #0x00, #0x82, #0x68	; 2181071464
	.byte #0x82, #0x00, #0x82, #0x6c	; 2181071468
	.byte #0x82, #0x00, #0x82, #0x70	; 2181071472
	.byte #0x82, #0x00, #0x82, #0x74	; 2181071476
	.byte #0x82, #0x00, #0x82, #0x78	; 2181071480
	.byte #0x82, #0x00, #0x82, #0x7c	; 2181071484
