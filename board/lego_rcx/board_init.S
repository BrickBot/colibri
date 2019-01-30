/* ------------------------------------------------------------------------- */
/* board_init.S - RCX-specific init code                                     */
/*                                                                           */
/* based on first.s by Kekoa Proudfoot                                       */
/* cred to him and all the other RCX hackers!                                */
/*                                                                           */
/* Copyright (c) 2001, 2002 Magnus Damm <damm@opensource.se>                 */
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
	
	.text
			
	.global _board_init
_board_init:
	mov.w	#__bss, r0
	mov.w	#__ebss, r1
	jsr	_rom_clear_memory

        mov.w   @0x0:16,r6
        mov.w   r6,@0xfd90:16

        mov.w	#_firmwaredata,r6
        mov.w   r6,@-r7
        mov.w   #_firmwaredata6,r6
        jsr     _rom_init_milliseconds
        adds    #0x2,r7

        jsr	_rom_power_init
	bclr	#7,@0xc4:8  /* disable software standby */

	/* turn off the frt */

	jsr	_cpu_timer_cleanup
	
	/* hook in interrupt vectors */

	mov.w	#_vector_mapping, r3
_interrupts_next:		
	mov.w	@r3+, r0
	beq	_interrupts_done
	
	mov.w	@r3+, r1
	mov.w	r0, @r1
	bra	_interrupts_next

_interrupts_done:		
	
	mov.w	#_root_stack_top, r7
	jsr	 _main
	
_hang:	
	bra	_hang
	

#define VECTOR_MAP(name) .word _cpu_vector_handler_##name, _rom_vector_##name
	
_vector_mapping:
	VECTOR_MAP(reset); 
	VECTOR_MAP(nmi); 

	VECTOR_MAP(irq0); 
	VECTOR_MAP(irq1); 
	VECTOR_MAP(irq2);

	VECTOR_MAP(icia); 
	VECTOR_MAP(icib); 
	VECTOR_MAP(icic); 
	VECTOR_MAP(icid); 
	VECTOR_MAP(ocia); 
	VECTOR_MAP(ocib); 
	VECTOR_MAP(fovi); 

	VECTOR_MAP(cmi0a); 
	VECTOR_MAP(cmi0b); 
	VECTOR_MAP(ovi0); 

	VECTOR_MAP(cmi1a); 
	VECTOR_MAP(cmi1b); 
	VECTOR_MAP(ovi1); 

	VECTOR_MAP(eri); 
	VECTOR_MAP(rxi); 
	VECTOR_MAP(txi); 
	VECTOR_MAP(tei); 

	VECTOR_MAP(adi); 
	VECTOR_MAP(wovf); 

	.word 0

_rcx_magic:		
        .string "Do you byte, when I knock?"
	
	.bss
_firmwaredata6:
	.space 64
_firmwaredata:	
	.space 6

	.align 2
_root_stack:			
	.space 256
_root_stack_top:	

