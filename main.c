/* ------------------------------------------------------------------------- */
/* main.c                                                                    */
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

#ifdef COLIBRI_CONFIG_BOARD_LEGO_RCX
#include "board_lcd.h"
#else
int sci_init(void);
void put_u32(uint32_t data);
#define lcd_put_u16(n) put_u32(n)
#define lcd_init() sci_init()
#endif

#include "sched.h"
#include "irq.h"
#include "timer.h"

#include "cpu.h"

#include "sem.h"

#include "string.h"

timer_event_t main_event;

#define MAIN_EVENT_TICKS 19

uint16_t cnt;

void main_handler(void *arg)
{
	cnt++;

	timer_event_add(&main_event, MAIN_EVENT_TICKS);
}

uint16_t cnt2;

void *main_thread_dummy(void *arg)
{
	while(1) {
		lcd_put_u16(0xf00b);
		sleep(2);
		lcd_put_u16(cnt);
		sleep(1);
		lcd_put_u16(cnt2);
		sleep(1);
	}

	return 0;
}

SCHED_THREAD_DECLARE_BUF(main_thread_dummy_buf, main_thread_dummy, 0, 10, 512);
#define THREAD_PARAMS    main_thread_dummy_buf, main_thread_dummy, 0, 10, 512

void main(void) 
{
	sched_init();

	irq_init();

	lcd_init();

	lcd_put_u16(0xc0de);

	timer_init();

	/* start the main-thread */

	sched_thread_create(THREAD_PARAMS);

	/* start the event handler */

	timer_event_init(&main_event, main_handler, NULL);
	timer_event_add(&main_event, MAIN_EVENT_TICKS);

	while(1) {
		cnt2++;
		usleep(1000);
	}

	timer_cleanup();
}
