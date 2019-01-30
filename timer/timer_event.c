/* ------------------------------------------------------------------------- */
/* timer_event.c                                                             */
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
#include "cpu.h"

uint32_t timer_current; /* counter of TICKS */
timer_event_t *timer_chain_root;

int timer_event_add(timer_event_t *e, timer_delay_t time)
{
	timer_event_t **tp;
	timer_delay_t curr = 0;
	timer_delay_t prev = 0;
	int done = 0;
	cpu_flags_t flags;

	if (e->time) {
		return -1;
	}

	flags = cpu_flags_save_and_cli();

	for (tp = &timer_chain_root; *tp; tp = &((*tp)->next)) {

		curr += (*tp)->time;

		if (time < curr) {
			e->time = time - prev;
			(*tp)->time -= e->time;

			e->next = *tp;
			*tp = e;
			done = 1;
			break;
		}

		prev = curr;
	}

	if (!done) {
		e->time = time - curr;
		e->next = NULL;
		*tp = e;
	}

	cpu_flags_restore(flags);

	return 0;
}

int timer_event_remove(timer_event_t *e)
{
	timer_event_t **tp;
	cpu_flags_t flags;
	int ret = -1;

	flags = cpu_flags_save_and_cli();

	for (tp = &timer_chain_root; *tp; tp = &((*tp)->next)) {

		if (*tp == e) {
			*tp = e->next;

			if (*tp) {
				(*tp)->time += e->time;
			}
			ret = 0;
			break;
		}
	}

	cpu_flags_restore(flags);

	return ret;
}

extern inline void timer_event_run(timer_delay_t time)
{
	timer_event_t **tp;
	timer_event_t *e;

	for (tp = &timer_chain_root; *tp; ) {
		if ((*tp)->time > time) {
			(*tp)->time -= time;
			break;
		}

		if ((*tp)->time <= time) {
			e = *tp;
			*tp = e->next;
			time -= e->time;

			e->time = 0;
			e->handler(e->arg);
			continue;
		}

		tp = &((*tp)->next);
	}


}

void timer_event(void)
{
	timer_current++;

	timer_event_run(1);

	/* tell the interrupt code that a tick-initiated schedule is pending */

	sched_schedule_pending = SCHED_SCHEDULE_PENDING_TICK;
}

int timer_init(void)
{
	return cpu_timer_init();
}

void timer_cleanup(void)
{
	cpu_timer_cleanup();
}
