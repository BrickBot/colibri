/* ------------------------------------------------------------------------- */
/* sched_list_static.c                                                       */
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
#include "sem.h"

#define THREADS_MAX COLIBRI_CONFIG_SCHED_LIST_STATIC_THREADS_MAX

sem_t sched_list_sem;

sched_thread_t *sched_list_threads[THREADS_MAX];

sched_thread_id_t sched_list_add(sched_thread_t *t)
{
	sched_thread_id_t p;

	sem_wait(&sched_list_sem);

	for (p = 0; p < (THREADS_MAX - 1); p++) {
		if (!sched_list_threads[p]) {
			sched_list_threads[p] = t;
			break;
		}
	}

	sem_post(&sched_list_sem);

	if (p < (THREADS_MAX - 1)) {
		return p;
	}

	return -1;
}

void sched_list_remove(sched_thread_t *t)
{
	sched_thread_id_t p;

	sem_wait(&sched_list_sem);

	for (p = 0; p < (THREADS_MAX - 1); p++) {
		if (sched_list_threads[p] == t) {
			sched_list_threads[p] = NULL;
			break;
		}
	}

	sem_post(&sched_list_sem);
}

sched_thread_t *sched_list_get(sched_thread_id_t p)
{
	sched_thread_t *t;

	sem_wait(&sched_list_sem);

	t = sched_list_threads[p];

	sem_post(&sched_list_sem);

	return t;
}

int sched_list_init(void)
{
	sem_init(&sched_list_sem, 0, 1);
	return 0;
}

void sched_list_cleanup(void)
{
	sem_destroy(&sched_list_sem);
}
