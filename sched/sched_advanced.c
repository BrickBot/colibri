/* ------------------------------------------------------------------------- */
/* sched_advanced.c                                                          */
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

#include "sched.h"
#include "sched_private.h"
#include "cpu.h"

#include "list_linked.h"

#ifdef COLIBRI_CONFIG_SCHED_ADVANCED_RR
typedef unsigned char sched_thread_rr_t; /* round-robin counter type */
#endif

typedef struct {
	list_linked_fifo_head_t list;

#ifdef COLIBRI_CONFIG_SCHED_ADVANCED_RR
	sched_thread_rr_t cnt;
	sched_thread_rr_t limit;
#endif

} sched_thread_prio_lookup_t;

sched_thread_prio_lookup_t sched_advanced_ready_lookup[SCHED_PRIORITY_MAX];
sched_priority_t sched_advanced_ready_highest_prio;
sched_thread_prio_lookup_t *sched_advanced_ready_highest_plt;

static void sched_advanced_to_ready_last(sched_thread_prio_lookup_t *plt, 
					 sched_thread_t *t)
{
	cpu_flags_t flags = cpu_flags_save_and_cli();

	/* insert thread at last position in the
	 * ready-queue for the selected priority.
	 */

	list_linked_fifo_insert((void *)&plt->list, &t->next_ready);

	if (t->priority < sched_advanced_ready_highest_prio) {
		sched_advanced_ready_highest_prio = t->priority;
		sched_advanced_ready_highest_plt = plt;
	}

	t->state = SCHED_STATE_READY;

	cpu_flags_restore(flags);
}

static sched_thread_t *sched_advanced_get_ready(sched_thread_prio_lookup_t *plt)
{
	sched_thread_t *t;
	sched_priority_t p = sched_advanced_ready_highest_prio; 
	cpu_flags_t flags = cpu_flags_save_and_cli();

	while (1) {

		t = (void *) list_linked_fifo_remove((void *)&plt->list);

		if (t) {
			break;
		}

		/* set ready_highest to the previous present prio level */

		plt = &sched_advanced_ready_lookup[p];
		p++;
		
		if (p >= SCHED_PRIORITY_MAX) {
			//BUG();
			break;
		}

	}

	cpu_flags_restore(flags);

	return t;
}

void *sched_advanced_schedule_from_running(void *stack_current)
{
	sched_thread_t *t;
	sched_thread_prio_lookup_t *plt;

	t = sched_thread_running;

	/* save current context */

	t->stack_current = stack_current;

	/* take care of the current RUNNING thread */

	plt = &sched_advanced_ready_lookup[t->priority];

	switch(t->state) {
		
	case SCHED_STATE_RUNNING:
		sched_advanced_to_ready_last(plt, t); /* force round robin */
		break;

	case SCHED_STATE_KILLED:
		sched_thread_delete(t);
		break;

	case SCHED_STATE_READY:
	case SCHED_STATE_BLOCKED:
		break;
#if 0
	default:
		BUG();
#endif
			
	}

	/* figure out which thread that should be RUNNING now */

	plt = sched_advanced_ready_highest_plt;

	sched_thread_running = sched_advanced_get_ready(plt);

	sched_thread_running->state = SCHED_STATE_RUNNING;

	/* return new context */

	return sched_thread_running->stack_current;
}

void *sched_advanced_schedule_by_tick(void *stack_current)
{
	sched_thread_prio_lookup_t *plt = sched_advanced_ready_highest_plt;
	uint8_t ret = 1;

	if (((sched_thread_t *) plt->list.first) == sched_thread_running) {

#ifdef COLIBRI_CONFIG_SCHED_ADVANCED_RR
		cpu_flags_t flags;

		flags = cpu_flags_save_and_cli();

		if (!((plt->limit == 0) || (plt->cnt++ < plt->limit))) {
			ret = 0;
			plt->cnt = 0;
		}
		cpu_flags_restore(flags);
#endif
		if (ret) {
			return stack_current;
		}
	}

	return sched_advanced_schedule_from_running(stack_current);
}

void sched_advanced_to_ready(sched_thread_t *t)
{
	sched_advanced_to_ready_last(&sched_advanced_ready_lookup[t->priority],
				     t);
}

int sched_advanced_init(void)
{
	int k;

	for (k = 0; k < SCHED_PRIORITY_MAX; k++) {
		list_linked_fifo_init(&sched_advanced_ready_lookup[k].list);
	}

	sched_advanced_ready_highest_prio = COLIBRI_CONFIG_SCHED_PRIORITY_ROOT;
	sched_advanced_ready_highest_plt = &sched_advanced_ready_lookup[
		COLIBRI_CONFIG_SCHED_PRIORITY_ROOT];

	return 0;
}

void sched_advanced_cleanup(void)
{
}
