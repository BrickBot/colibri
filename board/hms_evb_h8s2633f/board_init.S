/* ------------------------------------------------------------------------- */
/* board_init.S - board-specific init code                                   */
/*                                                                           */
/* Copyright (c) 2002 Magnus Damm <damm@opensource.se>                       */
/* All rights reserved.                                                      */
/*                                                                           */
/* Redistribution and use in source and binary forms, with or without        */
/* modification, are permitted provided that the following conditions        */
/* are met:                                                                  */
/*                                                                           */
/* 1. Redistributions of source code must retain the above copyright         */
/*    notice, this list of conditions and the following disclaimer.          */
/* 2. Redistributions in binary form must reproduce the above copyright      */
/*    notice, this list of conditions and the following disclaimer in the    */
/*    documentation and/or other materials provided with the distribution.   */
/* 3. Neither the name of the copyright holders nor the name of its          */
/*    contributors may be used to endorse or promote products derived        */
/*    from this software without specific prior written permission.          */
/*                                                                           */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS   */
/* IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, */
/* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR    */
/* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR    */
/* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF      */
/* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS  */
/* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN   */
/* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)   */
/* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF    */
/* THE POSSIBILITY OF SUCH DAMAGE.                                           */
/* ------------------------------------------------------------------------- */
	
	.h8300s
	.text
			
	.global _board_init
_board_init:

	/* temporarily place stack on top of our 4k page at 0 */

	mov.l	#4096, er7
	
	ldc	#0x80, ccr
	ldc	#0x00, exr

	jsr	pins_init

	/* clear bss */
	
	mov.l   #__bss, er0
	mov.l   #__ebss, er2
	sub.l   er0, er2
	sub.l   er1, er1
	jsr	_string_mem_set

	mov.l	#_root_stack_top, er7
	
	jsr	_main
	
_hang:	
	bra	_hang

#define P1DDR  0xfe30:16
#define P1DR   0x00:8 /* same as 0xff00:16 */
#define P1DATA 0xb0:8 /* same as 0xffb0:16 */

#define P1_PIN_PMODE   0x80
#define P1_PIN_USERLED 0x20

pins_init:	
	mov.b #(P1_PIN_PMODE + P1_PIN_USERLED), r0l   /* pins are outputs */
	mov.b #P1_PIN_USERLED, r1l   
	mov.b r0l, @P1DDR
	mov.b r1l, @P1DR
	rts			

	.globl _pins_led_toggle
_pins_led_toggle:	
	mov.b @P1DATA, r0l
	xor.b #P1_PIN_USERLED, r0l
	mov.b r0l, @P1DR
	rts
	
pins_led_off:	
	mov.b @P1DATA, r0l
	or.b #P1_PIN_USERLED, r0l
	mov.b r0l, @P1DR
	rts
	
	
	.bss
	.align 2
_root_stack:			
	.space 256
_root_stack_top:	





