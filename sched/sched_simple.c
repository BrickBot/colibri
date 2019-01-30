/* ------------------------------------------------------------------------- */
/* sched_simple.c                                                            */
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

list_linked_sort_head_t sched_simple_ready;

static int sched_simple_compare(list_linked_sort_element_t *element,
				list_linked_sort_element_t *new_element)
{
	sched_thread_t *t = (void *) element;
	sched_thread_t *nt = (void *) new_element;

	if (t->priority > nt->priority) { /* 0 highest, 255 lowest */
		return -1;
	}

	return 0;
}

/* sched_simple_to_ready()
 *
 * will turn the thread "t" from RUNNING or BLOCKED into READY.
 * using this on a already READY thread will screw things up.
 */

void sched_simple_to_ready(sched_thread_t *t)
{
	cpu_flags_t flags = cpu_flags_save_and_cli();

	list_linked_sort_insert((void *)&sched_simple_ready, &t->next_ready);

	sched_state_set(t, SCHED_STATE_READY);

	cpu_flags_restore(flags);

}

extern inline sched_thread_t *sched_simple_get_running(void)
{
	/*
	 * get the first thread from the ready list
	 */

	sched_thread_t *t;

	t = (void *) list_linked_sort_remove((void *)&sched_simple_ready);

	sched_state_set(t, SCHED_STATE_RUNNING);

	return t;
}

void *sched_simple_scheduler(void *stack_current)
{
	sched_thread_running->stack_current = stack_current;

	if (sched_thread_running->state == SCHED_STATE_RUNNING) {
		sched_simple_to_ready(sched_thread_running);
	}
	if (sched_thread_running->state == SCHED_STATE_KILLED) {
		sched_thread_delete(sched_thread_running);
	}
	sched_thread_running = sched_simple_get_running();

	return sched_thread_running->stack_current;
}

int sched_simple_init(void)
{
	return list_linked_sort_init(&sched_simple_ready, 
				     sched_simple_compare);
}

void sched_simple_cleanup(void)
{
}

