/* ------------------------------------------------------------------------- */
/* board_lcd.h                                                               */
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

#ifndef __BOARD_LCD_H__
#define __BOARD_LCD_H__

#include "types.h"

/* LCD_SEGMENT_xxx are used as index to lcd_segments[]... */

/* the 7-segment display parts */

#define LCD_SEGMENT_0_BASE  (7 * 0)
#define LCD_SEGMENT_0_TOP   (LCD_SEGMENT_0_BASE + 0)
#define LCD_SEGMENT_0_MID   (LCD_SEGMENT_0_BASE + 1)
#define LCD_SEGMENT_0_BOT   (LCD_SEGMENT_0_BASE + 2)
#define LCD_SEGMENT_0_TOPR  (LCD_SEGMENT_0_BASE + 3)
#define LCD_SEGMENT_0_BOTR  (LCD_SEGMENT_0_BASE + 4)
#define LCD_SEGMENT_0_TOPL  (LCD_SEGMENT_0_BASE + 5)
#define LCD_SEGMENT_0_BOTL  (LCD_SEGMENT_0_BASE + 6)

#define LCD_SEGMENT_1_BASE  (7 * 1)
#define LCD_SEGMENT_1_TOP   (LCD_SEGMENT_1_BASE + 0)
#define LCD_SEGMENT_1_MID   (LCD_SEGMENT_1_BASE + 1)
#define LCD_SEGMENT_1_BOT   (LCD_SEGMENT_1_BASE + 2)
#define LCD_SEGMENT_1_TOPR  (LCD_SEGMENT_1_BASE + 3)
#define LCD_SEGMENT_1_BOTR  (LCD_SEGMENT_1_BASE + 4)
#define LCD_SEGMENT_1_TOPL  (LCD_SEGMENT_1_BASE + 5)
#define LCD_SEGMENT_1_BOTL  (LCD_SEGMENT_1_BASE + 6)

#define LCD_SEGMENT_2_BASE  (7 * 2)
#define LCD_SEGMENT_2_TOP   (LCD_SEGMENT_2_BASE + 0)
#define LCD_SEGMENT_2_MID   (LCD_SEGMENT_2_BASE + 1)
#define LCD_SEGMENT_2_BOT   (LCD_SEGMENT_2_BASE + 2)
#define LCD_SEGMENT_2_TOPR  (LCD_SEGMENT_2_BASE + 3)
#define LCD_SEGMENT_2_BOTR  (LCD_SEGMENT_2_BASE + 4)
#define LCD_SEGMENT_2_TOPL  (LCD_SEGMENT_2_BASE + 5)
#define LCD_SEGMENT_2_BOTL  (LCD_SEGMENT_2_BASE + 6)

#define LCD_SEGMENT_3_BASE  (7 * 3)
#define LCD_SEGMENT_3_TOP   (LCD_SEGMENT_3_BASE + 0)
#define LCD_SEGMENT_3_MID   (LCD_SEGMENT_3_BASE + 1)
#define LCD_SEGMENT_3_BOT   (LCD_SEGMENT_3_BASE + 2)
#define LCD_SEGMENT_3_TOPR  (LCD_SEGMENT_3_BASE + 3)
#define LCD_SEGMENT_3_BOTR  (LCD_SEGMENT_3_BASE + 4)
#define LCD_SEGMENT_3_TOPL  (LCD_SEGMENT_3_BASE + 5)
#define LCD_SEGMENT_3_BOTL  (LCD_SEGMENT_3_BASE + 6)

#define LCD_SEGMENT_4_BASE  (7 * 4)
#define LCD_SEGMENT_4_TOP   (LCD_SEGMENT_4_BASE + 0)
#define LCD_SEGMENT_4_MID   (LCD_SEGMENT_4_BASE + 1)
#define LCD_SEGMENT_4_BOT   (LCD_SEGMENT_4_BASE + 2)
#define LCD_SEGMENT_4_TOPR  (LCD_SEGMENT_4_BASE + 3)
#define LCD_SEGMENT_4_BOTR  (LCD_SEGMENT_4_BASE + 4)
#define LCD_SEGMENT_4_TOPL  (LCD_SEGMENT_4_BASE + 5)
#define LCD_SEGMENT_4_BOTL  (LCD_SEGMENT_4_BASE + 6)

/* motor output segments */

#define LCD_SEGMENT_A_BASE    ((7 * 5) + (3 * 0))
#define LCD_SEGMENT_A_SELECT  (LCD_SEGMENT_A_BASE + 0)
#define LCD_SEGMENT_A_LEFT    (LCD_SEGMENT_A_BASE + 1)
#define LCD_SEGMENT_A_RIGHT   (LCD_SEGMENT_A_BASE + 2)

#define LCD_SEGMENT_B_BASE    ((7 * 5) + (3 * 1))
#define LCD_SEGMENT_B_SELECT  (LCD_SEGMENT_B_BASE + 0)
#define LCD_SEGMENT_B_LEFT    (LCD_SEGMENT_B_BASE + 1)
#define LCD_SEGMENT_B_RIGHT   (LCD_SEGMENT_B_BASE + 2)

#define LCD_SEGMENT_C_BASE    ((7 * 5) + (3 * 2))
#define LCD_SEGMENT_C_SELECT  (LCD_SEGMENT_C_BASE + 0)
#define LCD_SEGMENT_C_LEFT    (LCD_SEGMENT_C_BASE + 1)
#define LCD_SEGMENT_C_RIGHT   (LCD_SEGMENT_C_BASE + 2)

/* sensor input segments */

#define LCD_SEGMENT_S1_BASE   ((7 * 5) + (3 * 3) + (2 * 0))
#define LCD_SEGMENT_S1_SELECT (LCD_SEGMENT_S1_BASE + 0)
#define LCD_SEGMENT_S1_ACTIVE (LCD_SEGMENT_S1_BASE + 1)

#define LCD_SEGMENT_S2_BASE   ((7 * 5) + (3 * 3) + (2 * 1))
#define LCD_SEGMENT_S2_SELECT (LCD_SEGMENT_S1_BASE + 0)
#define LCD_SEGMENT_S2_ACTIVE (LCD_SEGMENT_S1_BASE + 1)

#define LCD_SEGMENT_S3_BASE   ((7 * 5) + (3 * 3) + (2 * 2))
#define LCD_SEGMENT_S3_SELECT (LCD_SEGMENT_S1_BASE + 0)
#define LCD_SEGMENT_S3_ACTIVE (LCD_SEGMENT_S1_BASE + 1)

/* misc dinkey-toy segments */

#define LCD_SEGMENT_MISC_BASE ((7 * 5) + (3 * 3) + (2 * 3))

#define LCD_SEGMENT_0_DOT     (LCD_SEGMENT_MISC_BASE + 0)  
#define LCD_SEGMENT_1_DOT     (LCD_SEGMENT_MISC_BASE + 1)  
#define LCD_SEGMENT_2_DOT     (LCD_SEGMENT_MISC_BASE + 2)  

#define LCD_SEGMENT_MINUS     (LCD_SEGMENT_MISC_BASE + 3)  

#define LCD_SEGMENT_MAN_ARMS  (LCD_SEGMENT_MISC_BASE + 4)  
#define LCD_SEGMENT_MAN_BODY  (LCD_SEGMENT_MISC_BASE + 5)  
#define LCD_SEGMENT_MAN_LEG   (LCD_SEGMENT_MISC_BASE + 6)  
#define LCD_SEGMENT_MAN_LEGS  (LCD_SEGMENT_MISC_BASE + 7)

#define LCD_SEGMENT_CIRCLE_0  (LCD_SEGMENT_MISC_BASE + 8)
#define LCD_SEGMENT_CIRCLE_1  (LCD_SEGMENT_MISC_BASE + 9)
#define LCD_SEGMENT_CIRCLE_2  (LCD_SEGMENT_MISC_BASE + 10)
#define LCD_SEGMENT_CIRCLE_3  (LCD_SEGMENT_MISC_BASE + 11)

#define LCD_SEGMENT_DOT_0     (LCD_SEGMENT_MISC_BASE + 12)
#define LCD_SEGMENT_DOT_1     (LCD_SEGMENT_MISC_BASE + 13)
#define LCD_SEGMENT_DOT_2     (LCD_SEGMENT_MISC_BASE + 14)
#define LCD_SEGMENT_DOT_3     (LCD_SEGMENT_MISC_BASE + 15)

#define LCD_SEGMENT_IR_LOW    (LCD_SEGMENT_MISC_BASE + 16)
#define LCD_SEGMENT_IR_HIGH   (LCD_SEGMENT_MISC_BASE + 17)

#define LCD_SEGMENT_BATTERY   (LCD_SEGMENT_MISC_BASE + 18)

void lcd_put_u16(uint16_t value);
void lcd_init(void);

#endif /* __BOARD_LCD_H__ */
