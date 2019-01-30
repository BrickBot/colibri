/* ------------------------------------------------------------------------- */
/* sched.h                                                                   */
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

#ifndef __SCHED_H__
#define __SCHED_H__

#include "types.h"
#include "colibri_config.h"

#include "sched_config_check.h"
#include "sched_idle.h"

#define SCHED_STATE_READY 0 
#define SCHED_STATE_RUNNING 1
#define SCHED_STATE_BLOCKED 3
#define SCHED_STATE_KILLED 2

#define SCHED_FLAGS_MALLOC (1 << 0) /* allocated with malloc() */
#define SCHED_FLAGS_SUSPENDED (1 << 1)


#define SCHED_THREADS_MAX COLIBRI_CONFIG_SCHED_THREADS_MAX

#if (SCHED_THREADS_MAX <= INT8_MAX)
typedef int8_t sched_thread_id_t;
#elif (SCHED_THREADS_MAX <= INT16_MAX)
typedef int16_t sched_thread_id_t;
#elif (SCHED_THREADS_MAX <= INT32_MAX)
typedef int32_t sched_thread_id_t;
#else
#error SCHED_THREADS_MAX too high!
#endif

#define SCHED_PRIORITY_MAX COLIBRI_CONFIG_SCHED_PRIORITY_MAX

#if (SCHED_PRIORITY_MAX <= UINT8_MAX)
typedef uint8_t sched_priority_t;
#elif (SCHED_PRIORITY <= UINT16_MAX)
typedef uint16_t sched_priority_t;
#elif (SCHED_PRIORITY_MAX <= UINT32_MAX)
typedef uint32_t sched_priority_t;
#else
#error SCHED_PRIORITY_MAX too high!
#endif

#include "list_linked.h"

typedef struct sched_thread {
	
	list_linked_element_t next_ready;
	list_linked_element_t next_sem;

	uint8_t state;
	uint8_t flags;

	sched_priority_t priority;

	unsigned int stack_size;
	void *stack_buffer;
	void *stack_current;

} sched_thread_t;

extern sched_thread_t *sched_thread_running;

void sched_schedule(void);

extern inline void schedule(void)
{
	sched_schedule();
}

extern inline void sched_state_set(sched_thread_t *t, int state)
{
	t->state = state;
}

extern inline void sched_to_blocked(void)
{
	sched_state_set(sched_thread_running, SCHED_STATE_BLOCKED);

	schedule();
}

/* wierd defines are taken care of in sched_config_check.h */

#ifdef COLIBRI_CONFIG_SCHED_LIST
#ifdef COLIBRI_CONFIG_SCHED_LIST_STATIC
#include "sched_list_static.h"
#endif
#endif

#ifdef COLIBRI_CONFIG_SCHED_ADVANCED
#include "sched_advanced.h"
#endif
#ifdef COLIBRI_CONFIG_SCHED_SIMPLE
#include "sched_simple.h"
#endif

extern inline void sched_to_ready(sched_thread_t *t)
{
#ifdef COLIBRI_CONFIG_SCHED_ADVANCED
	sched_advanced_to_ready(t);
#endif
#ifdef COLIBRI_CONFIG_SCHED_SIMPLE
	sched_simple_to_ready(t);
#endif

/* this could not be enabled yet:
 * how are we supposed to solve this -
 * sem_post() -> sched_to_ready() could happen in a interrupt handler.
 * but when in interrupt handler, schedule() looks at cpu_irq_nest -> ok.
 * Should it be possible to call sem_post() with interrupts off,
 * but not from a interrupt handler....?
 */

#if 0
	if (t->priority < sched_thread_running->priority) {
		schedule();
	}
#endif
}

#define SCHED_THREAD_BUFSIZE(stacksize) (sizeof(sched_thread_t) + stacksize)
#define SCHED_THREAD_DECLARE_BUF(symbolname, function, arg, priority, stacksize) \
uint8_t symbolname[SCHED_THREAD_BUFSIZE(stacksize)]

sched_thread_id_t sched_thread_create(uint8_t *buf, 
				      void *function, void *arg,
				      sched_priority_t priority,
				      unsigned int stack_size);

void sched_exit(void);
void sched_unblock_handler(void *data);

#define SCHED_SCHEDULE_PENDING_NONE 0
#define SCHED_SCHEDULE_PENDING_IRQ  1
#define SCHED_SCHEDULE_PENDING_TICK 2

extern uint8_t sched_schedule_pending;

int sched_init(void);
void sched_cleanup(void);

#endif /* __SCHED_H__ */


