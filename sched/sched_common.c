/* ------------------------------------------------------------------------- */
/* sched_common.c                                                            */
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

#include "sched.h"
#include "sched_private.h"
#include "cpu.h"

#include "string.h"

sched_thread_t *sched_thread_running;

/* sched_exit() could either be called directly by the RUNNING
 * thread or by being the function that gets called after thread returns.. 
 */

void sched_exit(void)
{
	sched_thread_t *t = sched_thread_running;
	
#ifdef COLIBRI_CONFIG_SCHED_LIST
	sched_list_remove(t);
#endif

	sched_state_set(t, SCHED_STATE_KILLED);

	schedule();
}

/* sched_thread_delete() is called by the scheduler.
 * do NOT call it directly.
 */

void sched_thread_delete(sched_thread_t *t)
{

	if (t->flags & SCHED_FLAGS_MALLOC) {
//		free(t);
	}

}

sched_thread_id_t sched_thread_create(uint8_t *buf, 
				      void *function, void *arg,
				      sched_priority_t priority,
				      unsigned int stack_size)
{
	unsigned int n = SCHED_THREAD_BUFSIZE(stack_size);
	sched_thread_id_t p = 0;
	uint8_t flags = 0;
	sched_thread_t *t = (void *) buf;
	
	if (!t) { /* need to malloc() buffer */
		
		flags |= SCHED_FLAGS_MALLOC;
	}
	
	memset(t, 0, n);

	t->flags = flags;
	t->priority = priority;
	t->state = SCHED_STATE_BLOCKED; /* to_ready() will make it ready */

	if (sched_thread_running) {

		if (stack_size < CPU_STACKSIZE_MIN) {
			return -1;
		}

		t->stack_size = stack_size;
		t->stack_buffer = (void *) (t + 1);

		t->stack_current = cpu_context_init(t->stack_buffer, 
						    stack_size,
						    function, arg);
	}

#ifdef COLIBRI_CONFIG_SCHED_LIST
	p = sched_list_add(t);

	if (p < 0) {
		sched_thread_delete(t);
		return p;
	}
#endif

	if (sched_thread_running) {
		sched_to_ready(t);
	}
	else {
		t->state = SCHED_STATE_RUNNING;
		sched_thread_running = t;
	}

	return p;
}

void sched_unblock_handler(void *data)
{
	sched_thread_t *t = data;

//	if (t->state == SCHED_STATE_BLOCKED) {
		sched_to_ready(t);
//	}
}

uint8_t sched_schedule_pending;

void sched_schedule(void)
{
	if (cpu_irq_nest) { /* if we're in an interrupt */
		if (sched_schedule_pending == SCHED_SCHEDULE_PENDING_NONE) {
			sched_schedule_pending = SCHED_SCHEDULE_PENDING_IRQ;
		}
	}
	else {
		cpu_schedule();
	}
}

SCHED_THREAD_DECLARE_BUF(sched_root_buf, NULL, 0, COLIBRI_CONFIG_SCHED_PRIORITY_ROOT, 0);
#define SCHED_THREAD_ROOT_PARAMS         NULL, 0, COLIBRI_CONFIG_SCHED_PRIORITY_ROOT, 0

int sched_init(void)
{
#ifdef COLIBRI_CONFIG_SCHED_LIST
	sched_list_init();
#endif

	/* initialize scheduler-specific data */

#ifdef COLIBRI_CONFIG_SCHED_ADVANCED
	sched_advanced_init();
#endif
#ifdef COLIBRI_CONFIG_SCHED_SIMPLE
	sched_simple_init();
#endif

	/* let the calling thread be the root thread */

	sched_thread_create(sched_root_buf, SCHED_THREAD_ROOT_PARAMS);

	/* start the idle thread */

	sched_idle_init();

	/* let schedule() do the rest... */

	schedule();

	return 0;
}


void sched_cleanup(void)
{

#ifdef COLIBRI_CONFIG_SCHED_SIMPLE
	sched_simple_cleanup();
#endif
#ifdef COLIBRI_CONFIG_SCHED_ADVANCED
	sched_advanced_cleanup();
#endif

#ifdef COLIBRI_CONFIG_SCHED_LIST
	sched_list_cleanup();
#endif

	sched_thread_running = NULL;
}
