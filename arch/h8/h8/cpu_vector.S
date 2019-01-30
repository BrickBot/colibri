/* ------------------------------------------------------------------------- */
/* cpu_vector.S - interrupt vectors                                          */
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

#include "colibri_config.h"

#ifdef COLIBRI_CONFIG_CPU_H83297
#include "cpu_h83297.h"
#else
#error cpu_vector.S: cpu not supported yet.
#endif

	.text
	.org 0x0

#define GLOBAL(symbol)  \
	.global symbol; \
symbol:	

#ifdef COLIBRI_CONFIG_BOARD_LEGO_RCX
#define VECTOR(nr, handler) /* no vectors needed */
#else	
#define VECTOR(nr, handler)                  \
	.org (nr*2);                         \
	_cpu_vector_##handler:          \
	.word _cpu_vector_handler_##handler;
#endif

#define HANDLER(index, handler)              \
	GLOBAL(_cpu_vector_handler_##handler); \
	jsr	_cpu_head_handler;           \
	.word CPU_IRQ_##index
	
	/* the vector table */

	VECTOR(0, reset); 

	VECTOR(3, nmi); 

	VECTOR(4, irq0); 
	VECTOR(5, irq1); 
	VECTOR(6, irq2);
	 
	VECTOR(12, icia); 
	VECTOR(13, icib); 
	VECTOR(14, icic); 
	VECTOR(15, icid); 
	VECTOR(16, ocia); 
	VECTOR(17, ocib); 
	VECTOR(18, fovi); 

	VECTOR(19, cmi0a); 
	VECTOR(20, cmi0b); 
	VECTOR(21, ovi0); 

	VECTOR(22, cmi1a); 
	VECTOR(23, cmi1b); 
	VECTOR(24, ovi1); 

	VECTOR(27, eri); 
	VECTOR(28, rxi); 
	VECTOR(29, txi); 
	VECTOR(30, tei); 

	VECTOR(35, adi); 
	VECTOR(36, wovf); 

GLOBAL(_cpu_vector_handler_reset)
	jmp	_board_init

	HANDLER(NMI, nmi);
	
	HANDLER(IRQ0, irq0);
	HANDLER(IRQ1, irq1); 
	HANDLER(IRQ2, irq2);
	
	HANDLER(ICIA, icia); 
	HANDLER(ICIB, icib); 
	HANDLER(ICIC, icic); 
	HANDLER(ICID, icid);
	
	HANDLER(OCIA, ocia); 
	HANDLER(OCIB, ocib);
	HANDLER(FOVI, fovi);
	
	HANDLER(CMI0A, cmi0a); 
	HANDLER(CMI0B, cmi0b); 
	HANDLER(OVI0, ovi0);
	
	HANDLER(CMI1A, cmi1a); 
	HANDLER(CMI1B, cmi1b); 
	HANDLER(OVI1, ovi1);
	
	HANDLER(ERI, eri); 
	HANDLER(RXI, rxi); 
	HANDLER(TXI, txi); 
	HANDLER(TEI, tei);
	
	HANDLER(ADI, adi); 
	HANDLER(WOVF, wovf); 
