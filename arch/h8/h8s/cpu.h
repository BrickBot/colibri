/* ------------------------------------------------------------------------- */
/* cpu.h                                                                     */
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

#ifndef __CPU_H__
#define __CPU_H__

#include "colibri_config.h"

#include "cpu_types.h"
#include "types.h"

#ifdef COLIBRI_CONFIG_CPU_H8S2633
#include "cpu_h8s2633.h"
#else
#error cpu.h: selected cpu not supported!
#endif

#include "arch_common.h"

typedef uint8_t cpu_irq_nest_t;

extern cpu_irq_nest_t cpu_irq_nest;

void cpu_head_schedule(void);

extern inline void cpu_schedule(void)
{
	cpu_head_schedule();
}

void *cpu_head_context_create(void *stack_ptr, void *function, void *arg);

extern inline void *cpu_context_create(void *stack_ptr, 
				       void *function, 
				       void *arg)
{
	return cpu_head_context_create(stack_ptr, function, arg);

}

void *cpu_context_init(void *stack_buffer, 
		       uint16_t stack_size,
		       void *function,
		       void *arg);

int cpu_timer_init(void);
void cpu_timer_cleanup(void);

#ifdef COLIBRI_CONFIG_CPU_ICM0
#define CPU_STACKSIZE_MIN (9*4) /* er0->er6, (flags + pc), ret-addr */
#else
#error ICM - Interrupt Control Mode not supported	
#error define COLIBRI_CONFIG_CPU_ICM0
#endif

#endif /* __CPU_H__ */


