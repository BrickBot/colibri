/* ------------------------------------------------------------------------- */
/* cpu_timer.c                                                               */
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
#include "types.h"
#include "timer.h"
#include "cpu.h"
#include "irq.h"

#ifndef COLIBRI_CONFIG_BOARD_XIN
#error cpu_timer: COLIBRI_CONFIG_BOARD_XIN not set
#endif

#define TIMER_VALUE ((COLIBRI_CONFIG_BOARD_XIN / 32) \
                     / COLIBRI_CONFIG_TIMER_TICKS)

#if (TIMER_VALUE > 0xffff) || (TIMER_VALUE < 0)
#error TIMER_VALUE out of range! try a higher COLIBRI_CONFIG_TIMER_TICKS!
#endif

irq_handler_t cpu_timer_handler;

int cpu_timer_irq_handler(void *arg)
{
	/* enable interrupts */

//	cpu_flags_sti();

	/* do timer events */

	timer_event();

	/* ack interrupt - clear OCFA bit */

	cpu_regs.frt.tcsr &= ~0x08; /* OCFA */

	return 0;
}

extern inline void cpu_timer_disable(void)
{
	cpu_regs.frt.tier &= ~0x08; /* TIER_ENABLE_OCA */
}

extern inline void cpu_timer_enable(void)
{
	cpu_regs.frt.tier |= 0x08; /* TIER_ENABLE_OCA */
}

int cpu_timer_init(void)
{
	cpu_timer_disable();

	cpu_regs.frt.tcsr = 0x09; /* TCSR_OCA | TCSR_RESET_ON_A */
	cpu_regs.frt.tcr  = 0x02; /* TCR_CLOCK_32 */

	cpu_regs.frt.tocr &= ~0x10; /* TOCR_OCRB */

	cpu_regs.frt.ocrxh = TIMER_VALUE >> 8;
	cpu_regs.frt.ocrxl = TIMER_VALUE & 0xff;
	
	irq_handler_init(&cpu_timer_handler, CPU_IRQ_OCIA,
			 cpu_timer_irq_handler, NULL, 0);
		
	if (irq_handler_register(&cpu_timer_handler) < 0) {
		return -1;
	}

	cpu_timer_enable();

	return 0;
}

void cpu_timer_cleanup(void)
{
	cpu_timer_disable();

	irq_handler_unregister(&cpu_timer_handler);
}
