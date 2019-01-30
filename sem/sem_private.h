/* ------------------------------------------------------------------------- */
/* sem_private.h                                                             */
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

#ifndef __SEM_PRIVATE_H__
#define __SEM_PRIVATE_H__

#include "types.h"
#include "colibri_config.h"
#include "sem.h"

#ifdef COLIBRI_CONFIG_SEM_COUNTING
extern const sem_ops_t sem_counting_ops;
#endif

#ifdef COLIBRI_CONFIG_SEM_BINARY
extern const sem_ops_t sem_binary_ops;
#endif

#ifdef COLIBRI_CONFIG_SEM_FIFO
extern const sem_queue_ops_t sem_fifo_ops;
#endif

#ifdef COLIBRI_CONFIG_SEM_PRIORITY
extern const sem_queue_ops_t sem_priority_ops;
#endif

#ifdef COLIBRI_CONFIG_SEM_INVERSIONSAFE
extern const sem_ops_t sem_inversionsafe_ops;
extern const sem_queue_ops_t sem_inversionsafe_queue_ops;
#endif

void sem_common_insert(sem_t *sem);
void sem_common_unlock(sem_t *sem);
int sem_list_compare(list_linked_sort_element_t *element,
		     list_linked_sort_element_t *new_element);

#endif /* __SEM_PRIVATE_H__ */

