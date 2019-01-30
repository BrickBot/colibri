/* ------------------------------------------------------------------------- */
/* timer.h                                                                   */
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

#ifndef __TIMER_H__
#define __TIMER_H__

#include "timer_config_check.h"

#include "colibri_config.h"
#include "types.h"

extern uint32_t timer_current; /* counter of TICKS */

typedef uint16_t timer_delay_t;
#define TIMER_DELAY_MAX UINT16_MAX

typedef struct timer_event {
	struct timer_event *next;

	timer_delay_t time;
	
	void (*handler)(void *);
	void *arg;

} timer_event_t;

extern inline void timer_event_init(timer_event_t *c,
				    void (*handler)(void *), void *arg)
{
	c->next = NULL;
	c->time = 0;
	c->handler = handler;
	c->arg = arg;
}


int timer_event_add(timer_event_t *c, timer_delay_t time);
int timer_event_remove(timer_event_t *e);
void timer_event(void);

int timer_init(void);
void timer_cleanup(void);

uint32_t timer_delay(uint32_t ticks);

extern inline unsigned int sleep(unsigned int seconds)
{
	uint32_t ticks = seconds * COLIBRI_CONFIG_TIMER_TICKS;

	return timer_delay(ticks) / COLIBRI_CONFIG_TIMER_TICKS;
}

extern inline void usleep(unsigned long usec)
{
	uint32_t tticks, ticks;
	
	tticks = (1000000 / COLIBRI_CONFIG_TIMER_TICKS);

	ticks = usec / tticks;

	if ((tticks * ticks) != usec) {
		ticks++;
	}
	
	timer_delay(ticks);
}

#endif

