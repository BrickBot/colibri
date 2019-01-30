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

#define CLKS_PER_TICK (COLIBRI_CONFIG_BOARD_XIN / COLIBRI_CONFIG_TIMER_TICKS)

#undef DIV
#undef CKS

#if !defined(DIV) && \
    ((CLKS_PER_TICK / 2) < 0xff) && \
    ((CLKS_PER_TICK / 2) > 0x01)
#define CKS 0
#define DIV 2
#endif

#if !defined(DIV) && \
    ((CLKS_PER_TICK / 64) < 0xff) && \
    ((CLKS_PER_TICK / 64) > 0x01)
#define CKS 1
#define DIV 64
#endif


#if !defined(DIV) && \
    ((CLKS_PER_TICK / 128) < 0xff) && \
    ((CLKS_PER_TICK / 128) > 0x01)
#define CKS 2
#define DIV 128
#endif

#if !defined(DIV) && \
    ((CLKS_PER_TICK / 512) < 0xff) && \
    ((CLKS_PER_TICK / 512) > 0x01)
#define CKS 3
#define DIV 512
#endif

#if !defined(DIV) && \
    ((CLKS_PER_TICK / 2048) < 0xff) && \
    ((CLKS_PER_TICK / 2048) > 0x01)
#define CKS 4
#define DIV 2048
#endif

#if !defined(DIV) && \
    ((CLKS_PER_TICK / 8192) < 0xff) && \
    ((CLKS_PER_TICK / 8192) > 0x01)
#define CKS 5
#define DIV 8192
#endif

#if !defined(DIV) && \
    ((CLKS_PER_TICK / 32768) < 0xff) && \
    ((CLKS_PER_TICK / 32768) > 0x01)
#define CKS 6
#define DIV 32768
#endif

#if !defined(DIV) && \
    ((CLKS_PER_TICK / 131072) < 0xff) && \
    ((CLKS_PER_TICK / 131072) > 0x01)
#define CKS 7
#define DIV 131072
#endif

#ifndef DIV
#error cpu_timer: COLIBRI_CONFIG_TIMER_TICKS value not supported!
#endif

#define TCNT_VALUE (0xff - (CLKS_PER_TICK / DIV)) /* FIXME: CHECKME */

#define WDT &cpu_regs.wdt0
#define WDT_IRQ CPU_IRQ_WOVI0

irq_handler_t cpu_timer_handler;

int cpu_timer_irq_handler(void *arg)
{
	volatile cpu_wdt_t *wdt = WDT;

	/* ack interrupt - clear OVF bit */

	wdt->w.tcsr_tcnt = 0xa500 | (wdt->r.tcsr & 0x7f);

	/* count up from TCNT_VALUE */

	wdt->w.tcsr_tcnt = 0x5a00 | TCNT_VALUE;

	/* enable interrupts */

//	cpu_flags_sti();

	/* do timer events */

	timer_event();

	return 0;
}

extern inline void cpu_timer_disable(void)
{
	volatile cpu_wdt_t *wdt = WDT;

	wdt->w.tcsr_tcnt = 0xa500;
}

extern inline void cpu_timer_enable(void)
{
	volatile cpu_wdt_t *wdt = WDT;

	wdt->w.tcsr_tcnt = 0xa520 | wdt->r.tcsr;
}

int cpu_timer_init(void)
{
	volatile cpu_wdt_t *wdt = WDT;
	uint16_t tmp;

	cpu_timer_disable();

	/* setup as interval timer */

	tmp = wdt->r.tcsr & 0x18;
	tmp |= CKS | 0xa500;
	wdt->w.tcsr_tcnt =  tmp;

	/* count up from TCNT_VALUE */

	tmp = 0x5a00 | TCNT_VALUE;
	wdt->w.tcsr_tcnt =  tmp;

	irq_handler_init(&cpu_timer_handler, WDT_IRQ,
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





