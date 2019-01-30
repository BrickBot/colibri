/* ------------------------------------------------------------------------- */
/* board_lcd.c                                                               */
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

#include "board_i2c.h"
#include "board_lcd.h"

#include "cpu.h"
#include "sem.h"
#include "types.h"

uint8_t lcd_buffer[] = { 0x7c, 0xc8, 0x80, 0xe0, 0x80, 
			 0x00, 
			 0x00, 0x00, 0x00, 0x00, 
			 0x00, 0x00, 0x00, 0x00, 0x00 };

#define lcd_data (lcd_buffer + 6)

/* lcd_segments[] are indexed by LCD_SEGMENT_xxx from lcd.h */

const uint8_t lcd_segments[] = {
  	/* digit 0:    */ 0x34, 0x35, 0x37, 0x71, 0x73, 0x75, 0x77,
  	/* digit 1:    */ 0x30, 0x31, 0x33, 0x81, 0x83, 0x85, 0x87,
	/* digit 2:    */ 0x10, 0x11, 0x13, 0x51, 0x53, 0x65, 0x67, 
	/* digit 3:    */ 0x04, 0x05, 0x07, 0x41, 0x43, 0x55, 0x57,
	/* digit 4:    */ 0x14, 0x15, 0x17, 0x25, 0x27, 0x45, 0x47,

	/* motor A:    */ 0x72, 0x36, 0x32,
	/* motor B:    */ 0x06, 0x12, 0x42,
	/* motor C:    */ 0x16, 0x46, 0x26,

	/* sensor 1:   */ 0x60, 0x61,
	/* sensor 2:   */ 0x54, 0x50,
	/* sensor 3:   */ 0x44, 0x24,

	/* dot 0,1,2   */ 0x86, 0x66, 0x56, 
	/* minus       */ 0x76,
	/* the man:    */ 0x00, 0x01, 0x02, 0x03,
	/* the circle: */ 0x20, 0x21, 0x23, 0x22,
	/* the dots:   */ 0x64, 0x80, 0x84, 0x70,
	/* infrared:   */ 0x62, 0x63, 
	/* battery:    */ 0x40, 
};

const uint8_t lcd_nibble_base[5] = { LCD_SEGMENT_0_BASE,
				     LCD_SEGMENT_1_BASE,
				     LCD_SEGMENT_2_BASE,
				     LCD_SEGMENT_3_BASE,
				     LCD_SEGMENT_4_BASE };

const uint8_t lcd_nibble_to_segments[] = {
	/* bit 6     5     4      3      2      1      0  */
	/* 0: TOP         BOT   TOPR   BOTR   TOPL   BOTL */ 0x5f,
	/* 1:                   TOPR   BOTR               */ 0x0c,
	/* 2: TOP   MID   BOT   TOPR                 BOTL */ 0x79,
	/* 3: TOP   MID   BOT   TOPR   BOTR               */ 0x7c,
	/* 4:       MID         TOPR   BOTR   TOPL        */ 0x2e,
	/* 5: TOP   MID   TOP          BOTR   TOPL        */ 0x76,
	/* 6: TOP   MID   TOP          BOTR   TOPL   BOTL */ 0x77,
	/* 7: TOP               TOPR   BOTR               */ 0x4c,
	/* 8: TOP   MID   BOT   TOPR   BOTR   TOPL   BOTL */ 0x7f,
	/* 9: TOP   MID   BOT   TOPR   BOTR   TOPL        */ 0x7e,
	/* A: TOP   MID         TOPR   BOTR   TOPL   BOTL */ 0x6f,
	/* b:       MID   BOT          BOTR   TOPL   BOTL */ 0x37,
	/* c:       MID   BOT                        BOTL */ 0x31,
	/* d:       MID   BOT   TOPR   BOTR          BOTL */ 0x3d,
	/* E: TOP   MID   BOT                 TOPL   BOTL */ 0x73,
	/* F: TOP   MID                       TOPL   BOTL */ 0x63,

};

static void lcd_segment_set(uint8_t segment)
{
	uint8_t uc = lcd_segments[segment];

	lcd_data[uc >> 4] |= 1 << (uc & 0x0f);
}

static void lcd_segment_clr(uint8_t segment)
{
	uint8_t uc = lcd_segments[segment];

	lcd_data[uc >> 4] &= ~(1 << (uc & 0x0f));
}

static void lcd_put_nibble(uint8_t pos, uint8_t number)
{
	uint8_t k, uc = lcd_nibble_to_segments[number];

	pos = lcd_nibble_base[pos];

	for (k = 0; k < 7; k++) {
		if (uc & 0x40) {
			lcd_segment_set(pos + k);
		}
		else {
			lcd_segment_clr(pos + k);
		}
		uc <<= 1;
	}
}

sem_t lcd_protect;

void lcd_put_u16x(uint16_t value)
{
	lcd_put_nibble(0, (value >> 0x0c) & 0x0f);
	lcd_put_nibble(1, (value >> 0x08) & 0x0f);
	lcd_put_nibble(2, (value >> 0x04) & 0x0f);
	lcd_put_nibble(3, (value >> 0x00) & 0x0f);

	{
		cpu_flags_t flags = cpu_flags_save_and_cli();

		i2c_write(lcd_buffer, sizeof(lcd_buffer));

		cpu_flags_restore(flags);
	}

#if 0
	{ // stupid busy delay

		int k;

		for (k = 0; k < 8000; k++) {
			lcd_put_nibble(0, (value >> 0x0c) & 0x0f);
		}
	}
#endif
}

void lcd_put_u16(uint16_t value)
{
	sem_wait(&lcd_protect);

	lcd_put_u16x(value);

	sem_post(&lcd_protect);
}

void lcd_init(void)
{
	i2c_init();

	sem_init(&lcd_protect, 0, 1);
}
