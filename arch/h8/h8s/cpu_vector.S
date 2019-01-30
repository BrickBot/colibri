/* ------------------------------------------------------------------------- */
/* cpu_vector.S - interrupt vectors                                         */
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

#ifdef COLIBRI_CONFIG_CPU_H8S2633
#include "cpu_h8s2633.h"
#else
#error cpu_vector.S: cpu not supported yet.
#endif

	.h8300s
			
	.text
	.org 0x0

#define GLOBAL(symbol)  \
	.global symbol; \
symbol:	

#define VECTOR(nr, handler)                  \
	.org (nr*4);                         \
	_cpu_vector_vector_##handler:          \
	.long _cpu_vector_handler_##handler;

#define HANDLER(index, handler)              \
	GLOBAL(_cpu_vector_handler_##handler); \
	jsr	_cpu_head_handler;           \
	.word CPU_IRQ_##index
	
	/* the vector table */

	VECTOR(0, power_on_reset); 
	VECTOR(1, manual_reset); 

	VECTOR(5, trace); 
	VECTOR(6, direct_transition); 
	VECTOR(7, nmi); 

	VECTOR(8, trap0); 
	VECTOR(9, trap1); 
	VECTOR(10, trap2); 
	VECTOR(11, trap3); 

	VECTOR(16, irq0); 
	VECTOR(17, irq1); 
	VECTOR(18, irq2);
	VECTOR(19, irq3);
	VECTOR(20, irq4); 
	VECTOR(21, irq5); 
	VECTOR(22, irq6);
	VECTOR(23, irq7);

	VECTOR(24, swdtend);
	VECTOR(25, wovi0);
	VECTOR(26, cmi);
	VECTOR(27, pc_break);
	VECTOR(28, adi);
	VECTOR(29, wovi1);

	VECTOR(32, tgi0a);
	VECTOR(33, tgi0b);
	VECTOR(34, tgi0c);
	VECTOR(35, tgi0d);
	VECTOR(36, tgi0v);

	VECTOR(40, tgi1a);
	VECTOR(41, tgi1b);
	VECTOR(42, tgi1v);
	VECTOR(43, tgi1u);

	VECTOR(44, tgi2a);
	VECTOR(45, tgi2b);
	VECTOR(46, tgi2v);
	VECTOR(47, tgi2u);

	VECTOR(48, tgi3a);
	VECTOR(49, tgi3b);
	VECTOR(50, tgi3c);
	VECTOR(51, tgi3d);
	VECTOR(52, tgi3v);
	
	VECTOR(56, tgi4a);
	VECTOR(57, tgi4b);
	VECTOR(58, tgi4v);
	VECTOR(59, tgi4u);

	VECTOR(60, tgi5a);
	VECTOR(61, tgi5b);
	VECTOR(62, tgi5v);
	VECTOR(63, tgi5u);

	VECTOR(64, cmia0);
	VECTOR(65, cmib0);
	VECTOR(66, ovi0);

	VECTOR(68, cmia1);
	VECTOR(69, cmib1);
	VECTOR(70, ovi1);

	VECTOR(72, dend0a);
	VECTOR(73, dend0b);
	VECTOR(74, dend1a);
	VECTOR(75, dend1b);

	VECTOR(80, eri0);
	VECTOR(81, rxi0);
	VECTOR(82, txi0);
	VECTOR(83, tei0);

	VECTOR(84, eri1);
	VECTOR(85, rxi1);
	VECTOR(86, txi1);
	VECTOR(87, tei1);

	VECTOR(88, eri2);
	VECTOR(89, rxi2);
	VECTOR(90, txi2);
	VECTOR(91, tei2);

	VECTOR(92, cmia2);
	VECTOR(93, cmib2);
	VECTOR(94, ovi2);
	
	VECTOR(96, cmia3);
	VECTOR(97, cmib3);
	VECTOR(98, ovi3);
	
	VECTOR(100, iici0);
	VECTOR(101, ddcsw1);
	VECTOR(102, iici1);

	VECTOR(120, eri3);
	VECTOR(121, rxi3);
	VECTOR(122, txi3);
	VECTOR(123, tei3);

	VECTOR(124, eri4);
	VECTOR(125, rxi4);
	VECTOR(126, txi4);
	VECTOR(127, tei4);

GLOBAL(_cpu_vector_handler_power_on_reset)
	jmp	_board_init

	HANDLER(MANUAL_RESET, manual_reset); 

	HANDLER(TRACE, trace); 
	HANDLER(DIRECT_TRANSITION, direct_transition); 
	HANDLER(NMI, nmi); 

	HANDLER(TRAP0, trap0); 
	HANDLER(TRAP1, trap1); 
	HANDLER(TRAP2, trap2); 
	HANDLER(TRAP3, trap3); 

	HANDLER(IRQ0, irq0); 
	HANDLER(IRQ1, irq1); 
	HANDLER(IRQ2, irq2);
	HANDLER(IRQ3, irq3);
	HANDLER(IRQ4, irq4); 
	HANDLER(IRQ5, irq5); 
	HANDLER(IRQ6, irq6);
	HANDLER(IRQ7, irq7);

	HANDLER(SWDTEND, swdtend);
	HANDLER(WOVI0, wovi0);
	HANDLER(CMI, cmi);
	HANDLER(PC_BREAK, pc_break);
	HANDLER(ADI, adi);
	HANDLER(WOVI1, wovi1);

	HANDLER(TGI0A, tgi0a);
	HANDLER(TGI0B, tgi0b);
	HANDLER(TGI0C, tgi0c);
	HANDLER(TGI0D, tgi0d);
	HANDLER(TGI0V, tgi0v);

	HANDLER(TGI1A, tgi1a);
	HANDLER(TGI1B, tgi1b);
	HANDLER(TGI1V, tgi1v);
	HANDLER(TGI1U, tgi1u);

	HANDLER(TGI2A, tgi2a);
	HANDLER(TGI2B, tgi2b);
	HANDLER(TGI2V, tgi2v);
	HANDLER(TGI2U, tgi2u);

	HANDLER(TGI3A, tgi3a);
	HANDLER(TGI3B, tgi3b);
	HANDLER(TGI3C, tgi3c);
	HANDLER(TGI3D, tgi3d);
	HANDLER(TGI3V, tgi3v);
	
	HANDLER(TGI4A, tgi4a);
	HANDLER(TGI4B, tgi4b);
	HANDLER(TGI4V, tgi4v);
	HANDLER(TGI4U, tgi4u);

	HANDLER(TGI5A, tgi5a);
	HANDLER(TGI5B, tgi5b);
	HANDLER(TGI5V, tgi5v);
	HANDLER(TGI5U, tgi5u);
	
	HANDLER(CMIA0, cmia0);
	HANDLER(CMIB0, cmib0);
	HANDLER(OVI0, ovi0);

	HANDLER(CMIA1, cmia1);
	HANDLER(CMIB1, cmib1);
	HANDLER(OVI1, ovi1);

	HANDLER(DEND0A, dend0a);
	HANDLER(DEND0B, dend0b);
	HANDLER(DEND1A, dend1a);
	HANDLER(DEND1B, dend1b);

	HANDLER(ERI0, eri0);
	HANDLER(RXI0, rxi0);
	HANDLER(TXI0, txi0);
	HANDLER(TEI0, tei0);

	HANDLER(ERI1, eri1);
	HANDLER(RXI1, rxi1);
	HANDLER(TXI1, txi1);
	HANDLER(TEI1, tei1);

	HANDLER(ERI2, eri2);
	HANDLER(RXI2, rxi2);
	HANDLER(TXI2, txi2);
	HANDLER(TEI2, tei2);

	HANDLER(CMIA2, cmia2);
	HANDLER(CMIB2, cmib2);
	HANDLER(OVI2, ovi2);

	HANDLER(CMIA3, cmia3);
	HANDLER(CMIB3, cmib3);
	HANDLER(OVI3, ovi3);

	HANDLER(IICI0, iici0);
	HANDLER(DDCSW1, ddcsw1);
	HANDLER(IICI1, iici1);

	HANDLER(ERI3, eri3);
	HANDLER(RXI3, rxi3);
	HANDLER(TXI3, txi3);
	HANDLER(TEI3, tei3);

	HANDLER(ERI4, eri4);
	HANDLER(RXI4, rxi4);
	HANDLER(TXI4, txi4);
	HANDLER(TEI4, tei4);
