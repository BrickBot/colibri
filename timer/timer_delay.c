/* ------------------------------------------------------------------------- */
/* timer_delay.c                                                             */
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
#include "types.h"
#include "timer.h"
#include "sched.h"

uint32_t timer_delay(uint32_t ticks) 
{
	timer_event_t e;
	timer_delay_t t;
	uint32_t org_ticks = ticks;
	uint32_t last_ticks;

	timer_event_init(&e, sched_unblock_handler, 
			 sched_thread_running);

	while (ticks) {
		if (ticks > TIMER_DELAY_MAX) {
			t = TIMER_DELAY_MAX;
		}
		else {
			t = ticks;
		}
	
		last_ticks = timer_current;

		timer_event_add(&e, t);
	
		sched_to_blocked();

		if (timer_event_remove(&e) == 0) {
			ticks -= (timer_current - last_ticks);
			break;
		}

		ticks -= t;
	}

	return org_ticks - ticks;
}







