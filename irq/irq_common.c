/* ------------------------------------------------------------------------- */
/* irq_common.c                                                              */
/*                                                                           */
/* Copyright (c) 002 Magnus Damm <damm@opensource.se>                        */
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
#include "cpu.h"
#include "irq.h"

#ifdef COLIBRI_CONFIG_IRQ_SHARE
#include "sem.h"

sem_t irq_sem;
#endif

irq_handler_t *irq_handlers[CPU_IRQ_NR];

void irq_handler_dispatch(int irq)
{
	irq_handler_t *htp = irq_handlers[irq];

	while (htp) {
		if (htp->handler(htp->arg) == 0) {
			break;
		}

#ifdef COLIBRI_CONFIG_IRQ_SHARE
		htp = htp->next;
#else
		break;
#endif
	}
}

static int irq_handler_check(irq_handler_t *htp) 
{
	if ((htp->irq < 0) || (htp->irq > CPU_IRQ_NR)) {
		return -1;
	}

	if (!htp->handler) {
		return -1;
	}

	return 0;
}

int irq_handler_register(irq_handler_t *htp)
{
#ifndef COLIBRI_CONFIG_IRQ_SHARE
	cpu_flags_t flags;
#endif
	int ret = -1;

	if (irq_handler_check(htp) < 0) {
		return ret;
	}

#ifdef COLIBRI_CONFIG_IRQ_SHARE
	sem_wait(&irq_sem);

	htp->next = irq_handlers[htp->irq];
	irq_handlers[htp->irq] = htp;
	ret = 0;

	sem_post(&irq_sem);
#else
	flags = cpu_flags_save_and_cli();

	if (irq_handlers[htp->irq] == 0) {
		irq_handlers[htp->irq] = htp;
		ret = 0;
	}

	cpu_flags_restore(flags);
#endif

	return ret;
}


int irq_handler_unregister(irq_handler_t *htp)
{
#ifdef COLIBRI_CONFIG_IRQ_SHARE
	irq_handler_t **htpp;
#else
	cpu_flags_t flags;
#endif
	int ret = -1;

	if (irq_handler_check(htp) < 0) {
		return -1;
	}

#ifdef COLIBRI_CONFIG_IRQ_SHARE
	sem_wait(&irq_sem);

	for (htpp = &irq_handlers[htp->irq]; *htpp; htpp = &(*htpp)->next) {
		if (*htpp == htp) {
			*htpp = htp->next;
			ret = 0;
			break;
		}
	}

	sem_post(&irq_sem);
#else
	flags = cpu_flags_save_and_cli();

	if (irq_handlers[htp->irq] == htp) {
		irq_handlers[htp->irq] = 0;
		ret = 0;
	}

	cpu_flags_restore(flags);
#endif

	return ret;
}


int irq_init(void)
{
#ifdef COLIBRI_CONFIG_IRQ_SHARE
	sem_init(&irq_sem, 0, 1);
#endif
	return 0;
}

void irq_cleanup(void)
{
#ifdef COLIBRI_CONFIG_IRQ_SHARE
	sem_destroy(&irq_sem);
#endif
}
