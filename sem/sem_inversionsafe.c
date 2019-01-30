/* ------------------------------------------------------------------------- */
/* sem_inversionsafe.c                                                       */
/*                                                                           */
/* Copyright (c) 2002 Magnus Damm <damm@opensource.se>                       */
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

#include "sem_private.h"
#include "sched.h"
#include "cpu.h"

#define BUG_USAGE(x)

/* insert() and unlock() should be called with interrupts disabled */

static void sem_inversionsafe_insert(sem_t *sem)
{
	sched_thread_t *t = sched_thread_running;
	sched_priority_t p;

	if (!t) {
		BUG_USAGE("trying to wait on sem with no running thread");
	}

	sem->thread_queue.ops->insert(&sem->thread_queue, &t->next_sem);

	p = t->priority;

	/* if thread_first has lower priority than t, 
	 * let it inherit the priority.
	 */

	if (sem->thread_first && (sem->thread_first->priority > p)) {
		sem->old_priority = sem->thread_first->priority;
		sem->thread_first->priority = p;
	}
}

static void sem_inversionsafe_unlock(sem_t *sem)
{
	sched_thread_t *t;
	
	t = (sched_thread_t *) 
		sem->thread_queue.ops->remove(&sem->thread_queue);

	if (!sem->thread_first) {
		sem->thread_first = t;
		sem->old_priority = sem->thread_first->priority;
	}

	sched_to_ready(t);
}

static int sem_inversionsafe_queue_init(sem_t *sem)
{
	return list_linked_init(&sem->thread_queue, SORT, sem_list_compare);
}

const sem_queue_ops_t sem_inversionsafe_queue_ops = {
	sem_inversionsafe_insert,
	sem_inversionsafe_unlock,
	sem_inversionsafe_queue_init,
};



static int sem_inversionsafe_wait(sem_t *sem)
{
	cpu_flags_t flags = cpu_flags_save_and_cli();

	int ret = sem->list->extra_ops->wait(sem);

	if (!sem->thread_first) {
		sem->thread_first = sched_thread_running;
		sem->old_priority = sem->thread_first->priority;
	}

	cpu_flags_restore(flags);

	return ret;
}

static int sem_inversionsafe_trywait(sem_t *sem)
{
	cpu_flags_t flags = cpu_flags_save_and_cli();

	int ret = sem->list->extra_ops->trywait(sem);

	if ((ret == 0) && (!sem->thread_first)) {
		sem->thread_first = sched_thread_running;
		sem->old_priority = sem->thread_first->priority;
	}

	cpu_flags_restore(flags);

	return ret;
}

static int sem_inversionsafe_post(sem_t *sem)
{
	cpu_flags_t flags = cpu_flags_save_and_cli();
	int ret;

	/* restore priority if thread_first */

	if (sched_thread_running == sem->thread_first) {
		if (sched_thread_running) {
			sem->thread_first->priority = sem->old_priority;
			sem->thread_first = NULL;
		}
	}

	ret = sem->list->extra_ops->post(sem);

	cpu_flags_restore(flags);

	return ret;
}

static int sem_inversionsafe_init(sem_t *sem, unsigned int value)
{
	return sem->list->extra_ops->init(sem, value);
}

const sem_ops_t sem_inversionsafe_ops = {

	sem_inversionsafe_post,
	sem_inversionsafe_wait,
	sem_inversionsafe_trywait,
	sem_inversionsafe_init,
};



