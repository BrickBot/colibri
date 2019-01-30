/* ------------------------------------------------------------------------- */
/* cpu_head.S - low-level code                                               */
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

#include "colibri_config.h"

#ifdef COLIBRI_CONFIG_CPU_H8S2633
#include "cpu_h8s2633.h"
#else
#error cpu_head.S: cpu not supported yet.
#endif

#define SCHED_SCHEDULE_PENDING_NONE 0 /* from sched/sched.h FIXME */
#define SCHED_SCHEDULE_PENDING_IRQ  1
#define SCHED_SCHEDULE_PENDING_TICK 2
	
#define CPU_FLAG_I 0x80	

	.h8300s
			
	.text
	.org 0x0

#define GLOBAL(symbol)  \
	.global symbol; \
symbol:	

#define CONTEXT_SAVE()        \
	mov.l	er6, @-er7;   \
	mov.l	er5, @-er7;   \
	mov.l	er4, @-er7;   \
	mov.l	er3, @-er7;   \
	mov.l	er2, @-er7;   \
	mov.l	er1, @-er7;   \
	mov.l	er0, @-er7		

#define CONTEXT_RESTORE()     \
	mov.l	@er7+, er0;   \
	mov.l	@er7+, er1;   \
	mov.l	@er7+, er2;   \
	mov.l	@er7+, er3;   \
	mov.l	@er7+, er4;   \
	mov.l	@er7+, er5;   \
	mov.l	@er7+, er6
	
GLOBAL(_cpu_head_handler); 

	mov.l	er0, @_cpu_head_scratch0
	mov.l	@er7+, er0			/* get pointer to vector nr */
	mov.w	@er0, r0
	mov.w	r0, @_cpu_head_scratch1
	mov.l	@_cpu_head_scratch0, er0
	
	CONTEXT_SAVE(); 

	mov.b	@_cpu_irq_nest, r1l             /* increment nest-counter */
	inc.b	r1l
	mov.b	r1l, @_cpu_irq_nest

	cmp.b	#0x01, r1l
	bne	_cpu_head_irq_n

	/* if this is the first irq:	
	 * 1) switch to irq stack
	 * 2) handle the scheduler after the handler
	 */
	
	mov.l	er7, @_cpu_head_sp_save         /* save old stack */
	mov.l   #_cpu_head_stack_irq_top, er7   /* change stack */

	mov.l	#_cpu_head_irq_return_1, er2
	bra	_cpu_head_irq_common
	
_cpu_head_irq_n:
	mov.l	#_cpu_head_irq_return_n, er2

_cpu_head_irq_common:		
	mov.l	er2, @-er7			/* push return address */
	sub.l	er0, er0
	mov.w	@_cpu_head_scratch1, r0	        /* get vector nr to r0 */

	/* enabling interrupts here doesn't seem to work.
	 * I guess we have to ack them before we enable all interrupts.
	 * each handler ack their interrupt until we know better...
	 */	
#if 0
	andc	#~CPU_FLAG_I, ccr		/* enable interrupts   */
#endif
	jmp	_irq_handler_dispatch
	
_cpu_head_irq_return_1:	

	mov.l   @_cpu_head_sp_save, er0
	jsr	_cpu_head_schedule_raw

	mov.l   er0, er7
	mov.b	#0x00, r0l
	mov.b   r0l, @_cpu_irq_nest
	bra	_cpu_head_context_restore_raw
			
_cpu_head_irq_return_n:	
	orc	#CPU_FLAG_I, ccr		/* disable interrupts   */

	mov.b	@_cpu_irq_nest, r0l
	dec.b	r0l
	mov.b	r0l, @_cpu_irq_nest
	
_cpu_head_context_restore_raw:	
	
	CONTEXT_RESTORE();
	rte 

GLOBAL(_cpu_head_context_restore); 
	mov.l	er0, er7			/* new stack pointer in er0 */
	bra	_cpu_head_context_restore_raw
	

GLOBAL(_cpu_head_schedule); 

	/* cpu_head_schedule is called with a jsr
	 * - the return address is alredy pushed on the stack.
	 *
	 * fake an interrupt by storing CCR and disable interrupts
	 */
#ifdef COLIBRI_CONFIG_CPU_ICM0
	stc	ccr, r0h			/* get flags (CCR)     */
	mov.b	r0h, @er7			/* modify flags on the stack */
#endif	
	orc	#CPU_FLAG_I, ccr		/* disable interrupts  */
	
	CONTEXT_SAVE(); 
	
	mov.l	er7, er0			/* pass sp             */

	mov.b	#SCHED_SCHEDULE_PENDING_IRQ, r1l
	mov.b	r1l, @_sched_schedule_pending

	mov.l	#_cpu_head_context_restore, er1
	mov.l	er1, @-er7			/* push return address */
		
_cpu_head_schedule_raw:	              /* r0 contains sp of current task */

	orc	#CPU_FLAG_I, ccr		/* disable interrupts   */

	/* task-initiated schedule() and schedule from interrupt
	 * may overlap, this will protect them */
	
	mov.b	@_cpu_head_sched_lock, r1h
	bne	_cpu_head_schedule_conflict

	mov.b	#0x01, r1h
	mov.b	r1h, @_cpu_head_sched_lock

_cpu_head_schedule_loop:	

	/* do we have any context switch pending?
	 *
	 * sched_schedule_pending gives us the info.
	 */
	
	mov.b	@_sched_schedule_pending, r1l
	cmp.b   #SCHED_SCHEDULE_PENDING_NONE, r1l
	beq	_cpu_head_schedule_out
	
	mov.b	#SCHED_SCHEDULE_PENDING_NONE, r1h
	mov.b	r1h, @_sched_schedule_pending

	andc	#~CPU_FLAG_I, ccr		/* enable interrupts   */

#ifdef COLIBRI_CONFIG_SCHED_ADVANCED
	cmp.b	#SCHED_SCHEDULE_PENDING_TICK, r1l
	beq	_cpu_head_schedule_from_tick

	jsr	_sched_advanced_schedule_from_running
	bra	_cpu_head_schedule_after_tick

_cpu_head_schedule_from_tick:		
	jsr	_sched_advanced_schedule_by_tick
	
_cpu_head_schedule_after_tick:	
#endif	

#ifdef COLIBRI_CONFIG_SCHED_SIMPLE
	jsr	_sched_simple_scheduler
#endif

	orc	#CPU_FLAG_I, ccr		/* disable interrupts   */
	bra	_cpu_head_schedule_loop
	
_cpu_head_schedule_out:
	mov.b	#0x00, r1h
	mov.b	r1h, @_cpu_head_sched_lock
	
_cpu_head_schedule_conflict:
	rts
	
/*
 * void *cpu_head_context_create(void *stack_ptr, void *function, void *arg)
 */
	
GLOBAL(_cpu_head_context_create); 

	mov.l	er3, @-er7

	mov.l	#_sched_exit, er3		/* sched_exit() will be */
	mov.l	er3, @-er0	                /* called if the task   */
						/* returns */
	
#ifdef COLIBRI_CONFIG_CPU_ICM0
	mov.l	er1, @-er0			/* entry point and ccr */
#endif
		
	sub.l	er1, er1

	mov.l	er1, @-er0			/* er6 */
	mov.l	er1, @-er0			/* er5 */
	mov.l	er1, @-er0			/* er4 */
	mov.l	er1, @-er0			/* er3 */
	mov.l	er1, @-er0			/* er2 */
	mov.l	er1, @-er0			/* er1 */
	mov.l	er2, @-er0			/* er0 (arg0) */

	mov.l	@er7+, er3
	rts

	.bss
_cpu_head_scratch0:	
	.long 0	
_cpu_head_scratch1:	
	.word 0
_cpu_head_sp_save:
	.long 0
GLOBAL(_cpu_head_stack_irq)
	.space COLIBRI_CONFIG_CPU_IRQ_STACKSIZE
_cpu_head_stack_irq_top:	
_cpu_head_sched_lock:
	.byte 0
