/* ------------------------------------------------------------------------- */
/* irq.h                                                                     */
/*                                                                           */
/* Copyright (c) 002 Magnus Damm <damm@opensource.se>                        */
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

#ifndef __IRQ_H__
#define __IRQ_H__

#include "types.h"
#include "string.h"

#include "colibri_config.h"

typedef struct irq_handler {
#ifdef COLIBRI_CONFIG_IRQ_SHARE
	struct irq_handler *next;
#endif
	int (*handler)(void *);
	void *arg;
	int irq;
} irq_handler_t;

extern inline void irq_handler_init(irq_handler_t *ht,
				    int irq,
				    int (*handler)(void *), 
				    void *arg,
				    int flags)
{
	memset(ht, 0, sizeof(*ht));

	ht->handler = handler;
	ht->arg = arg;
	ht->irq = irq;

	/* flags not implemented yet */
}

void irq_handler_dispatch(int irq);
int irq_handler_register(irq_handler_t *ht);
int irq_handler_unregister(irq_handler_t *ht);

int irq_init(void);
void irq_cleanup(void);

#endif /* __IRQ_H__ */


