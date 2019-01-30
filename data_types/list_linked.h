/* ------------------------------------------------------------------------- */
/* list_linked.h                                                             */
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

#ifndef __LIST_LINKED_H__
#define __LIST_LINKED_H__

#include "colibri_config.h"

/* linked_fifo types: */

typedef struct list_linked_fifo_element {
	struct list_linked_fifo_element *next;
} list_linked_fifo_element_t;
	
typedef struct list_linked_fifo_head {
	list_linked_fifo_element_t *first;
	list_linked_fifo_element_t *last;
} list_linked_fifo_head_t;

/* linked_sort types: */

typedef struct list_linked_sort_element {
	struct list_linked_sort_element *next;
} list_linked_sort_element_t;
	
typedef struct list_linked_sort_head {
	list_linked_sort_element_t *first;
	int (*compare)(list_linked_sort_element_t *element,
		       list_linked_sort_element_t *new_element);
} list_linked_sort_head_t;


/* operations ("methods") and unions of both types */

typedef struct {
	union {
		void *first;
		list_linked_fifo_head_t fifo;
		list_linked_sort_head_t sort;
	} list;
	const struct list_linked_ops *ops;
} list_linked_head_t;

typedef union {
	list_linked_fifo_element_t fifo;
	list_linked_sort_element_t sort;
} list_linked_element_t;

typedef struct list_linked_ops {
	void (*insert)(list_linked_head_t *head,
		       list_linked_element_t *element);
	list_linked_element_t *(*remove)(list_linked_head_t *head);
} list_linked_ops_t;

/* exported functions */

void list_linked_fifo_insert(list_linked_head_t *head,
			     list_linked_element_t *element);
list_linked_element_t *list_linked_fifo_remove(list_linked_head_t *head);
int list_linked_fifo_init(list_linked_fifo_head_t *head);

extern const list_linked_ops_t list_linked_fifo;

void list_linked_sort_insert(list_linked_head_t *head,
			     list_linked_element_t *element);
list_linked_element_t *list_linked_sort_remove(list_linked_head_t *head);
int list_linked_sort_init(list_linked_sort_head_t *head, 
			  int (*compare)
			  (list_linked_sort_element_t *element,
			   list_linked_sort_element_t *new_element));

extern const list_linked_ops_t list_linked_sort;

typedef enum { FIFO, SORT } list_linked_type_t;

int list_linked_init(list_linked_head_t *head, 
		     list_linked_type_t type, 
		     void *arg);

#endif /* __LIST_LINKED_H__ */







