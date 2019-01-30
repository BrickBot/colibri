/* ------------------------------------------------------------------------- */
/* sem.h                                                                     */
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

#ifndef __SEM_H__
#define __SEM_H__

#include "types.h"
#include "colibri_config.h"
#include "sched.h"

typedef int8_t sem_cnt_t;

#define SEM_CNT_MAX INT8_MAX
#define SEM_CNT_MIN INT8_MIN

#include "list_linked.h"

typedef struct sem {
	sem_cnt_t c;
	struct sem_ops_list *list;
	list_linked_head_t thread_queue;

#ifdef COLIBRI_CONFIG_SEM_INVERSIONSAFE
	struct sched_thread *thread_first;
	sched_priority_t old_priority;
#endif
} sem_t;

typedef struct sem_ops {
	int (*post)(struct sem *);
	int (*wait)(struct sem *);
	int (*trywait)(struct sem *);
	int (*init)(struct sem *, unsigned int value);
	void (*cleanup)(struct sem *);
} sem_ops_t;

typedef struct sem_queue_ops {
	void (*insert)(struct sem *);
	void (*unlock)(struct sem *);
	int (*init)(struct sem *);
	void (*cleanup)(struct sem *);
} sem_queue_ops_t;

typedef struct sem_ops_list {
	const struct sem_ops *ops;
	const struct sem_queue_ops *queue_ops;
#ifdef COLIBRI_CONFIG_SEM_INVERSIONSAFE
	const struct sem_ops *extra_ops;
#endif
} sem_ops_list_t;

#define SEM_TYPE_DEFAULT 0

#if defined(COLIBRI_CONFIG_SEM_COUNTING)

#define SEM_TYPE_COUNTING 1

#if defined(COLIBRI_CONFIG_SEM_INVERSIONSAFE)
#define SEM_TYPE_CI 2
#endif

#if defined(COLIBRI_CONFIG_SEM_PRIORITY)
#define SEM_TYPE_CP 3
#endif

#if defined(COLIBRI_CONFIG_SEM_FIFO)
#define SEM_TYPE_CF 4
#endif

#endif /* COLIBRI_CONFIG_SEM_COUNTING */


#if defined(COLIBRI_CONFIG_SEM_BINARY)

#define SEM_TYPE_BINARY 5

#if defined(COLIBRI_CONFIG_SEM_INVERSIONSAFE)
#define SEM_TYPE_BI 6
#endif

#if defined(COLIBRI_CONFIG_SEM_PRIORITY)
#define SEM_TYPE_BP 7
#endif

#if defined(COLIBRI_CONFIG_SEM_FIFO)
#define SEM_TYPE_BF 8
#endif

#endif /* COLIBRI_CONFIG_SEM_BINARY */

extern inline int sem_wait(sem_t *sem)
{
	if (sem->list->ops->wait(sem)) {
		sched_to_blocked();
	}

	return 0;
}

extern inline int sem_trywait(sem_t *sem)
{
	return sem->list->ops->trywait(sem);
}

extern inline int sem_post(sem_t *sem)
{
	return sem->list->ops->post(sem);
}

extern inline int sem_getvalue(sem_t *sem, int *sval)
{
	*sval = sem->c;
	return 0;
}

int sem_init(sem_t *sem, int type, unsigned int value);
int sem_destroy(sem_t *sem);

#include "sched.h"

#endif /* __SEM_H__ */










