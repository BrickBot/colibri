/* ------------------------------------------------------------------------- */
/* arch_common.h                                                             */
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

#ifndef __ARCH_COMMON_H__
#define __ARCH_COMMON_H__

#include "cpu_types.h"
#include "types.h"

typedef uint16_t cpu_flags_t;

extern inline cpu_flags_t cpu_flags_save(void)
{
	cpu_flags_t flags;

	__asm__("stc\tccr,%0h" : "=r" (flags));
	
	return flags; 
}

extern inline void cpu_flags_cli(void)
{
	__asm__("orc\t#0x80,ccr"
		:
		:
		: "cc");
}

extern inline void cpu_flags_sti(void)
{
	__asm__("andc\t#0x7f,ccr"
		:
		:
		: "cc");
}

extern inline cpu_flags_t cpu_flags_save_and_cli(void)
{
	cpu_flags_t flags;

	flags = cpu_flags_save();
	cpu_flags_cli();

	return flags; 
}

extern inline void cpu_flags_restore(cpu_flags_t flags)
{
	__asm__("ldc\t%0h,ccr"
		: /* no output registers */ 
		: "r" (flags));

}

extern inline void cpu_idle(void)
{
//	cpu_regs.iop1.p1dr = cpu_regs.iop2.port1 ^ 0x20;
	__asm__("sleep");
}


#endif /* __ARCH_COMMON_H__ */
