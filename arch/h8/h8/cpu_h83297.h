/* ------------------------------------------------------------------------- */
/* cpu_h83297.h - h8/3297-specific definitions                               */
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

#ifndef __CPU_H83297_H__
#define __CPU_H83297_H__

#ifndef __ASSEMBLY__

#include "types.h"

/* the free running timer (frt) block of the h8/3297 */

typedef struct {
	uint8_t tier;
	uint8_t tcsr;
	uint8_t frch;
	uint8_t frcl;
	uint8_t ocrxh;
	uint8_t ocrxl;
	uint8_t tcr;
	uint8_t tocr;
	uint8_t icrah;
	uint8_t icral;
	uint8_t icrbh;
	uint8_t icrbl;
	uint8_t icrch;
	uint8_t icrcl;
	uint8_t icrdh;
	uint8_t icrdl;
} cpu_frt_t;

/* the watchdog timer (wdt) block of the h8/3297 */

typedef struct {
	uint8_t tcsr;
	uint8_t tcnt;
	uint8_t reserved0;
	uint8_t reserved1;
} cpu_wdt_t;

/* the i/o ports (iop) block of the h8/3297 */

typedef struct {
	uint8_t p1pcr;
	uint8_t p2pcr;
	uint8_t p3pcr;
	uint8_t reserved0;
	uint8_t p1ddr;
	uint8_t p2ddr;
	uint8_t p1dr;
	uint8_t p2dr;
	uint8_t p3ddr;
	uint8_t p4ddr;
	uint8_t p3dr;
	uint8_t p4dr;
	uint8_t p5ddr;
	uint8_t p6ddr;
	uint8_t p5dr;
	uint8_t p6dr;
	uint8_t reserved1;
	uint8_t reserved2;
	uint8_t p7pin;
	uint8_t reserved3;
} cpu_iop_t;

/* the system control (sycr) block of the h8/3297 */

typedef struct {
	uint8_t reserved0;
	uint8_t reserved1;
	uint8_t wscr;
	uint8_t stcr;
	uint8_t syscr;
	uint8_t mdcr;
	uint8_t iscr;
	uint8_t ier;
} cpu_sycr_t;

/* the 8 bit timer (tmr) block of the h8/3297 */

typedef struct {
	uint8_t tcr;
	uint8_t tcsr;
	uint8_t tcora;
	uint8_t tcorb;
	uint8_t tcnt;
	uint8_t reserved0;
	uint8_t reserved1;
	uint8_t reserved2;
} cpu_tmr_t;

/* the serial communication interface (sci) block of the h8/3297 */

typedef struct {
	uint8_t smr;
	uint8_t brr;
	uint8_t scr;
	uint8_t tdr;
	uint8_t ssr;
	uint8_t rdr;
	uint8_t reserved0;
	uint8_t reserved1;
} cpu_sci_t;

/* the analog-to-digital converter (adc) block of the h8/3297 */

typedef struct {
	uint8_t addrah;
	uint8_t addral;
	uint8_t addrbh;
	uint8_t addrbl;
	uint8_t addrch;
	uint8_t addrcl;
	uint8_t addrdh;
	uint8_t addrdl;
	uint8_t adcsr;
	uint8_t adcr;
	uint8_t reserved0;
	uint8_t reserved1;
	uint8_t reserved2;
	uint8_t reserved3;
	uint8_t reserved4;
	uint8_t reserved5;
} cpu_adc_t;

/* the entire h8/3297 top-level block */

typedef struct {
	uint8_t reserved0[0x10]; /* 0x80 -> 0x8f */
	cpu_frt_t frt;           /* 0x90 -> 0x9f */
	uint8_t reserved1[0x08]; /* 0xa0 -> 0xa7 */
	cpu_wdt_t wdt;           /* 0xa8 -> 0xab */
	cpu_iop_t iop;           /* 0xac -> 0xbf */
	cpu_sycr_t sycr;         /* 0xc0 -> 0xc7 */
	cpu_tmr_t tmr0;          /* 0xc8 -> 0xcf */
	cpu_tmr_t tmr1;          /* 0xd0 -> 0xd7 */
	cpu_sci_t sci;           /* 0xd8 -> 0xdf */
	cpu_adc_t adc;           /* 0xe0 -> 0xef */
	uint8_t reserved2[0x10]; /* 0xf0 -> 0xff */
} cpu_h83297_t;

extern volatile cpu_h83297_t cpu_regs;

#endif /* __ASSEMBLY__ */

#define CPU_IRQ_NR 23

/* these CPU_IRQ_ definitions are indexes in our
 * handler table kept in ram, not vector numbers!
 */

/* #define CPU_IRQ_RESET  - not handled here! */
#define CPU_IRQ_NMI    0
#define CPU_IRQ_IRQ0   1
#define CPU_IRQ_IRQ1   2
#define CPU_IRQ_IRQ2   3

#define CPU_IRQ_ICIA   4
#define CPU_IRQ_ICIB   5
#define CPU_IRQ_ICIC   6
#define CPU_IRQ_ICID   7
#define CPU_IRQ_OCIA   8
#define CPU_IRQ_OCIB   9
#define CPU_IRQ_FOVI  10

#define CPU_IRQ_CMI0A 11
#define CPU_IRQ_CMI0B 12
#define CPU_IRQ_OVI0  13

#define CPU_IRQ_CMI1A 14
#define CPU_IRQ_CMI1B 15
#define CPU_IRQ_OVI1  16

#define CPU_IRQ_ERI   17
#define CPU_IRQ_RXI   18
#define CPU_IRQ_TXI   19
#define CPU_IRQ_TEI   20

#define CPU_IRQ_ADI   21
#define CPU_IRQ_WOVF  22

#endif /* __CPU_H83297_H__ */


