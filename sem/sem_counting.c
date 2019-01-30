/* ------------------------------------------------------------------------- */
/* sem_counting.c                                                            */
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
#include "cpu.h"

static int sem_counting_wait(sem_t *sem)
{
	cpu_flags_t flags = cpu_flags_save_and_cli();
	int ret;

	sem->c--;

	if (sem->c >= 0) {
		ret = 0;
	}
	else {
		sem->list->queue_ops->insert(sem);
		ret = 1;
	}

	cpu_flags_restore(flags);

	return ret;
}

static int sem_counting_trywait(sem_t *sem)
{
	cpu_flags_t flags = cpu_flags_save_and_cli();
	int ret;

	if (sem->c > 0) {
		sem->c--;
		ret = 0;
	}
	else {
		ret = -1;
	}

	cpu_flags_restore(flags);

	return ret;
}

static int sem_counting_post(sem_t *sem)
{
	cpu_flags_t flags = cpu_flags_save_and_cli();

	if (sem->c < SEM_CNT_MAX) {
		sem->c++;
	}
	else {
		cpu_flags_restore(flags);
		return -1;
	}

	if (sem->c <= 0) {
		sem->list->queue_ops->unlock(sem);
	}

	cpu_flags_restore(flags);

	return 0;
}

static int sem_counting_init(sem_t *sem, unsigned int value)
{
	if (value >= SEM_CNT_MAX) {
		return -1;
	}

	sem->c = value;

	return 0;
}

const sem_ops_t sem_counting_ops = {

	sem_counting_post,
	sem_counting_wait,
	sem_counting_trywait,
	sem_counting_init,
};

