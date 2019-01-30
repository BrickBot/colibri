/* ------------------------------------------------------------------------- */
/* sem_common.c                                                              */
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
#include "string.h"

#if defined(COLIBRI_CONFIG_SEM_FIFO) || defined(COLIBRI_CONFIG_SEM_PRIORITY)

#define BUG_USAGE(x)

/* insert() and unlock() should be called with interrupts disabled */

void sem_common_insert(sem_t *sem)
{
	sched_thread_t *t = sched_thread_running;

	if (!t) {
		BUG_USAGE("trying to wait on sem with no running thread");
	}

	sem->thread_queue.ops->insert(&sem->thread_queue, &t->next_sem);
}

void sem_common_unlock(sem_t *sem)
{
	sched_thread_t *t;
	
	t = (void *) sem->thread_queue.ops->remove(&sem->thread_queue);

	sched_to_ready(t);
}

#endif

#if defined(COLIBRI_CONFIG_SEM_PRIORITY) \
 || defined(COLIBRI_CONFIG_SEM_INVERSIONSAFE) 

int sem_list_compare(list_linked_sort_element_t *element,
		     list_linked_sort_element_t *new_element)
{
	sched_thread_t *t = (void *) element;
	sched_thread_t *nt = (void *) new_element;

	if (t->priority > nt->priority) { /* 0 highest, 255 lowest */
		return -1;
	}

	return 0;
}

#endif

#if defined(COLIBRI_CONFIG_SEM_COUNTING) && defined(COLIBRI_CONFIG_SEM_FIFO)

const sem_ops_list_t sem_type_cf = {
	&sem_counting_ops,
	&sem_fifo_ops,
};

#endif

#if defined(COLIBRI_CONFIG_SEM_COUNTING) &&defined(COLIBRI_CONFIG_SEM_PRIORITY)

const sem_ops_list_t sem_type_cp = {
	&sem_counting_ops,
	&sem_priority_ops,
};

#endif

#if defined(COLIBRI_CONFIG_SEM_COUNTING) \
   && defined(COLIBRI_CONFIG_SEM_INVERSIONSAFE)

const sem_ops_list_t sem_type_ci = {
	&sem_inversionsafe_ops,
	&sem_inversionsafe_queue_ops,
	&sem_counting_ops,
};

#endif

#if defined(COLIBRI_CONFIG_SEM_BINARY) && defined(COLIBRI_CONFIG_SEM_FIFO)

const sem_ops_list_t sem_type_bf = {
	&sem_binary_ops,
	&sem_fifo_ops,
};

#endif

#if defined(COLIBRI_CONFIG_SEM_BINARY) &&defined(COLIBRI_CONFIG_SEM_PRIORITY)

const sem_ops_list_t sem_type_bp = {
	&sem_binary_ops,
	&sem_priority_ops,
};

#endif

#if defined(COLIBRI_CONFIG_SEM_BINARY) \
   && defined(COLIBRI_CONFIG_SEM_INVERSIONSAFE)

const sem_ops_list_t sem_type_bi = {
	&sem_inversionsafe_ops,
	&sem_inversionsafe_queue_ops,
	&sem_binary_ops,
};

#endif
int sem_init(sem_t *sem, int type, unsigned int value)
{
	memset(sem, 0, sizeof(*sem));

	switch(type) {

	case SEM_TYPE_DEFAULT:

#ifdef SEM_TYPE_COUNTING
	case SEM_TYPE_COUNTING:
#endif

#ifdef SEM_TYPE_CI
	case SEM_TYPE_CI: 
		sem->list = (sem_ops_list_t *) &sem_type_ci;
		break;
#endif

#ifdef SEM_TYPE_CP
	case SEM_TYPE_CP: 
		sem->list = (sem_ops_list_t *) &sem_type_cp;
		break;
#endif

#ifdef SEM_TYPE_CF
	case SEM_TYPE_CF: 
		sem->list = (sem_ops_list_t *) &sem_type_cf;
		break;
#endif
		break;
	
#ifdef SEM_TYPE_BINARY
	case SEM_TYPE_BINARY:
#endif	

#ifdef SEM_TYPE_BI
	case SEM_TYPE_BI: 
		sem->list = (sem_ops_list_t *) &sem_type_bi;
		break;
#endif

#ifdef SEM_TYPE_BP
	case SEM_TYPE_BP: 
		sem->list = (sem_ops_list_t *) &sem_type_bp;
		break;
#endif

#ifdef SEM_TYPE_BF
	case SEM_TYPE_BF: 
		sem->list = (sem_ops_list_t *) &sem_type_bf;
		break;
#endif


	}		

	if (!sem->list) {
		return -1;
	}

	sem->list->queue_ops->init(sem);

	return sem->list->ops->init(sem, value);
}

int sem_destroy(sem_t *sem)
{
	if (sem->list->ops->cleanup) {
		sem->list->ops->cleanup(sem);
	}
	if (sem->list->queue_ops->cleanup) {
		sem->list->queue_ops->cleanup(sem);
	}

	sem->list = NULL;
	return 0;
}
