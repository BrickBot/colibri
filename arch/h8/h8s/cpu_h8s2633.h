/* ------------------------------------------------------------------------- */
/* cpu_h8s2633.h - h8s/2633-specific definitions                             */
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

#ifndef __CPU_H8S2633_H__
#define __CPU_H8S2633_H__

#ifndef __ASSEMBLY__

#include "types.h"

/* the pwm block of the h8s/2633 */

typedef struct {
	uint8_t dadrah_dacr;
	uint8_t dadral;
	uint8_t dadrbh_dacnth;
	uint8_t dadrbl_dacntl;
} cpu_pwm_t;

/* the 8 bit timer blocks of the h8s/2633 */

typedef struct {
	uint8_t tcr;
	uint8_t reserved0;
	uint8_t tcsr;
	uint8_t reserved1;
	uint8_t tcora;
	uint8_t reserved2;
	uint8_t tcorb;
	uint8_t reserved3;
	uint8_t tcnt;
} cpu_tmr_block_t;

typedef struct {
	uint8_t tcr2;
	uint8_t tcr3;
	uint8_t tcsr2;
	uint8_t tcsr3;
	uint8_t tcora2;
	uint8_t tcora3;
	uint8_t tcorb2;
	uint8_t tcorb3;
	uint8_t tcnt2;
	uint8_t tcnt3;
	uint8_t reserved0[0x06];
} cpu_tmr23_t;

typedef struct {
	uint8_t tcr0;
	uint8_t tcr1;
	uint8_t tcsr0;
	uint8_t tcsr1;
	uint8_t tcora0;
	uint8_t tcora1;
	uint8_t tcorb0;
	uint8_t tcorb1;
	uint8_t tcnt0;
	uint8_t tcnt1;
	uint8_t reserved0[0x02];
} cpu_tmr01_t;

/* the serial communication interface (sci) block of the h8/2633 */

typedef struct {
	uint8_t smr;
	uint8_t brr;
	uint8_t scr;
	uint8_t tdr;
	uint8_t ssr;
	uint8_t rdr;
	uint8_t scmr;
	uint8_t reserved0;
} cpu_sci_t;

/* the pc break controller of the h8s/2633 */

typedef struct {
	uint8_t bara0;
	uint8_t bara1;
	uint8_t bara2;
	uint8_t bara3;
	uint8_t barb0;
	uint8_t barb1;
	uint8_t barb2;
	uint8_t barb3;
	uint8_t bcra;
	uint8_t bcrb;
	uint8_t reserved0[0x06];
} cpu_pbc_t;

/* the interrupt controller blocks of the h8s/2633 */

typedef struct {
	uint8_t reserved0[0x02];
	uint8_t iscrh;
	uint8_t iscrl;
	uint8_t ier;
	uint8_t isr;
} cpu_ic0_t;

typedef struct {
	uint8_t ipra;
	uint8_t iprb;
	uint8_t iprc;
	uint8_t iprd;
	uint8_t ipre;
	uint8_t iprf;
	uint8_t iprg;
	uint8_t iprh;
	uint8_t ipri;
	uint8_t iprj;
	uint8_t iprk;
	uint8_t iprl;
	uint8_t reserved0[0x02];
	uint8_t ipro;
	uint8_t reserved1;
} cpu_ic1_t;

/* the data transfer controller block of the h8s/2633 */

typedef struct {
	uint8_t dtcera;
	uint8_t dtcerb;
	uint8_t dtcerc;
	uint8_t dtcerd;
	uint8_t dtcere;
	uint8_t dtcerf;
	uint8_t reserved0[0x02];
	uint8_t dtceri;
	uint8_t dtvecr;
} cpu_dtc_t;

/* the programmable pulse generator block of the h8s/2633 */

typedef struct {
	uint8_t reserved0[0x06];
	uint8_t pcr;
	uint8_t pmr;
	uint8_t nderh;
	uint8_t nderl;
	uint8_t podrh;
	uint8_t podrl;
	uint8_t ndrh;
	uint8_t ndrl;
	uint8_t ndrhx;
	uint8_t ndrlx;
} cpu_ppg_t;

/* the i/o pin blocks of the h8s/2633 */

typedef struct {
	uint8_t p1ddr;
	uint8_t reserved0;
	uint8_t p3ddr;
	uint8_t reserved1[0x03];
	uint8_t p7ddr;
	uint8_t reserved2[0x02];
	uint8_t paddr;
	uint8_t pbddr;
	uint8_t pcddr;
	uint8_t pdddr;
	uint8_t peddr;
	uint8_t pfddr;
	uint8_t pgddr;

	uint8_t papcr;
	uint8_t pbpcr;
	uint8_t pcpcr;
	uint8_t pdpcr;
	uint8_t pepcr;
	uint8_t reserved3;
	uint8_t p3odr;
	uint8_t paodr;
	uint8_t pbodr;
	uint8_t pcodr;
	uint8_t reserved4[0x06];
} cpu_iop0_t;

typedef struct {
	uint8_t p1dr;
	uint8_t reserved0;
	uint8_t p3dr;
	uint8_t reserved1[0x03];
	uint8_t p7dr;
	uint8_t reserved2[0x02];
	uint8_t padr;
	uint8_t pbdr;
	uint8_t pcdr;
	uint8_t pddr;
	uint8_t pedr;
	uint8_t pfdr;
	uint8_t pgdr;
} cpu_iop1_t;

typedef struct {
	uint8_t port1;
	uint8_t reserved0;
	uint8_t port3;
	uint8_t port4;
	uint8_t reserved1[0x02];
	uint8_t port7;
	uint8_t reserved2;
	uint8_t port9;
	uint8_t porta;
	uint8_t portb;
	uint8_t portc;
	uint8_t portd;
	uint8_t porte;
	uint8_t portf;
	uint8_t portg;
} cpu_iop2_t;

/* the timer pulse unit blocks of the h8s/2633 */

typedef struct {
	uint8_t tcr;
	uint8_t tmdr;
	uint8_t tiorh;
	uint8_t tiorl;
	uint8_t tier;
	uint8_t tsr;
	uint16_t tcnt;
	uint16_t tgra;
	uint16_t tgrb;
	uint16_t tgrc; /* only available for tpu 0, 3 - otherwise reserved */
	uint16_t tgrd; /* only available for tpu 0, 3 - otherwise reserved */
} cpu_tpu_t;

typedef struct {
	uint8_t tstr;
	uint8_t tsyr;
	uint8_t reserved0[0x0e];
} cpu_tpu_shared_t;

/* the bus controller block of the h8s/2633 */

typedef struct {
	uint8_t abwcr;
	uint8_t astcr;
	uint8_t wcrh;
	uint8_t wcrl;
	uint8_t bcrh;
	uint8_t bcrl;
	uint8_t mcr;
	uint8_t dramcr;
	uint8_t rtcnt;
	uint8_t rtcor;
} cpu_bsc_t;

/* the flash blocks of the h8s/2633 */

typedef struct {
	uint8_t reserved0;
	uint8_t ramer;
	uint8_t reserved1[0x04];
} cpu_flash0_t;

typedef struct {
	uint8_t flmcr1;
	uint8_t flmcr2;
	uint8_t ebr1;
	uint8_t ebr2;
	uint8_t flpwcr;
	uint8_t reserved1[0x03];
} cpu_flash1_t;

/* the dma controller blocks of the h8s/2633 */

typedef struct {
	uint16_t mar0ah;
	uint16_t mar0al;
	uint16_t ioar0a;
	uint16_t etcr0a;

	uint16_t mar0bh;
	uint16_t mar0bl;
	uint16_t ioar0b;
	uint16_t etcr0b;

	uint16_t mar1ah;
	uint16_t mar1al;
	uint16_t ioar1a;
	uint16_t etcr1a;

	uint16_t mar1bh;
	uint16_t mar1bl;
	uint16_t ioar1b;
	uint16_t etcr1b;
} cpu_dmac0_t;

typedef struct {
	uint8_t dmawer;
	uint8_t dmatcr;
	uint8_t dmacr0a;
	uint8_t dmacr0b;
	uint8_t dmacr1a;
	uint8_t dmacr1b;
	uint8_t dmabcrh;
	uint8_t dmabcrl;
} cpu_dmac1_t;

/* the watchdog timer blocks of the h8s/2633 */

typedef union {
	struct {
		uint8_t tcsr;
		uint8_t tcnt;
	} r;
	struct {
		uint16_t tcsr_tcnt;
	} w;
} cpu_wdt_t;

typedef union {
	struct {
		uint8_t reserved0;
		uint8_t rstcsr;
	} r;
	struct {
		uint16_t rstcsr;
	} w;
} cpu_wdt_shared_t;

/* the a/d converter block of the h8s/2633 */

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
	uint8_t reserved0[0x06];
} cpu_adc_t;

/* the d/a converter block of the h8s/2633 */

typedef struct {
	uint8_t dadr0;
	uint8_t dadr1;
	uint8_t dacr01;
	uint8_t reserved0;
} cpu_dac_t;

/* the entire h8s/2633 top-level block */

typedef struct {
	uint8_t reserved0[0x5ac]; /* 0xf800 -> 0xfdab */
	cpu_dac_t dac23;          /* 0xfdac -> 0xfdaf */
	uint8_t icr;              /* 0xfdb0 -> 0xfdb0 */
	uint8_t reserved2[0x03];  /* 0xfdb1 -> 0xfdb3 */
	uint8_t scrx;             /* 0xfdb4 -> 0xfdb4 */
	uint8_t ddcswr;           /* 0xfdb5 -> 0xfdb5 */
	uint8_t reserved3[0x02];  /* 0xfdb6 -> 0xfdb7 */
	cpu_pwm_t pwm0;           /* 0xfdb8 -> 0xfdbb */
	cpu_pwm_t pwm1;           /* 0xfdbc -> 0xfdbf */
	cpu_tmr23_t tmr23;        /* 0xfdc0 -> 0xfdcf */
	cpu_sci_t sci3;           /* 0xfdd0 -> 0xfdd7 */
	cpu_sci_t sci4;           /* 0xfdd8 -> 0xfddf */
	uint8_t reserved4[0x04];  /* 0xfde0 -> 0xfde3 */
	uint8_t sbycr;            /* 0xfde4 -> 0xfde4 */
	uint8_t syscr;            /* 0xfde5 -> 0xfde5 */
	uint8_t sckcr;            /* 0xfde6 -> 0xfde6 */
	uint8_t mdcr;             /* 0xfde7 -> 0xfde7 */
	uint8_t mstpcra;          /* 0xfde8 -> 0xfde8 */
	uint8_t mstpcrb;          /* 0xfde9 -> 0xfde9 */
	uint8_t mstpcrc;          /* 0xfdea -> 0xfdea */
	uint8_t pfcr;             /* 0xfdeb -> 0xfdeb */
	uint8_t lpwrcr;           /* 0xfdec -> 0xfdec */
	uint8_t reserved5[0x13];  /* 0xfded -> 0xfdff */
	cpu_pbc_t pbc;            /* 0xfe00 -> 0xfe0f */
	cpu_ic0_t ic0;            /* 0xfe10 -> 0xfe15 */
	cpu_dtc_t dtc;            /* 0xfe16 -> 0xfe1f */
	cpu_ppg_t ppg;            /* 0xfe20 -> 0xfe2f */
	cpu_iop0_t iop0;          /* 0xfe30 -> 0xfe4f */
	uint8_t reserved6[0x30];  /* 0xfe50 -> 0xfe7f */
	cpu_tpu_t tpu3;           /* 0xfe80 -> 0xfe8f */
	cpu_tpu_t tpu4;           /* 0xfe90 -> 0xfe9f */
	cpu_tpu_t tpu5;           /* 0xfea0 -> 0xfeaf */
	cpu_tpu_shared_t tpu;     /* 0xfeb0 -> 0xfebf */
	cpu_ic1_t ic1;            /* 0xfec0 -> 0xfecf */
	cpu_bsc_t bsc;            /* 0xfed0 -> 0xfed9 */
	cpu_flash0_t flash0;      /* 0xfeda -> 0xfedf */
	cpu_dmac0_t dmac0;        /* 0xfee0 -> 0xfeff */
	cpu_iop1_t iop1;          /* 0xff00 -> 0xff0f */
	cpu_tpu_t tpu0;           /* 0xff10 -> 0xff1f */
	cpu_tpu_t tpu1;           /* 0xff20 -> 0xff2f */
	cpu_tpu_t tpu2;           /* 0xff30 -> 0xff3f */
	uint8_t reserved7[0x20];  /* 0xff40 -> 0xff5f */
	cpu_dmac1_t dmac1;        /* 0xff60 -> 0xff67 */
	cpu_tmr01_t tmr01;        /* 0xff68 -> 0xff73 */
	cpu_wdt_t wdt0;           /* 0xff74 -> 0xff75 */
	cpu_wdt_shared_t wdt;     /* 0xff76 -> 0xff77 */
	cpu_sci_t sci0;           /* 0xff78 -> 0xff7f */
	cpu_sci_t sci1;           /* 0xff80 -> 0xff87 */
	cpu_sci_t sci2;           /* 0xff88 -> 0xff8f */
	cpu_adc_t adc;            /* 0xff90 -> 0xff9f */
	uint8_t reserved8[0x2];   /* 0xffa0 -> 0xffa1 */
	cpu_wdt_t wdt1;           /* 0xffa2 -> 0xffa3 */
	cpu_dac_t dac01;          /* 0xffa4 -> 0xffa7 */
	cpu_flash1_t flash1;      /* 0xffa8 -> 0xffaf */
	cpu_iop2_t iop2;          /* 0xffb0 -> 0xffbf */
	uint8_t reserved9[0x30];  /* 0xffc0 -> 0xffff */
} cpu_h8s2633_t;

extern volatile cpu_h8s2633_t cpu_regs;

#endif /* __ASSEMBLY__ */

#define CPU_IRQ_NR 87

/* these CPU_IRQ_ definitions are indexes in our
 * handler table kept in ram, not vector numbers!
 */

/* #define CPU_IRQ_POWER_ON_RESET - not handled here */
#define CPU_IRQ_MANUAL_RESET      0

#define CPU_IRQ_TRACE             1
#define CPU_IRQ_DIRECT_TRANSITION 2
#define CPU_IRQ_NMI               3

#define CPU_IRQ_TRAP0             4
#define CPU_IRQ_TRAP1             5
#define CPU_IRQ_TRAP2             6 
#define CPU_IRQ_TRAP3             7 

#define CPU_IRQ_IRQ0              8 
#define CPU_IRQ_IRQ1              9 
#define CPU_IRQ_IRQ2              10
#define CPU_IRQ_IRQ3              11
#define CPU_IRQ_IRQ4              12
#define CPU_IRQ_IRQ5              13
#define CPU_IRQ_IRQ6              14
#define CPU_IRQ_IRQ7              15

#define CPU_IRQ_SWDTEND           16
#define CPU_IRQ_WOVI0             17
#define CPU_IRQ_CMI               18
#define CPU_IRQ_PC_BREAK          19
#define CPU_IRQ_ADI               20
#define CPU_IRQ_WOVI1             21

#define CPU_IRQ_TGI0A             22
#define CPU_IRQ_TGI0B             23
#define CPU_IRQ_TGI0C             24
#define CPU_IRQ_TGI0D             25
#define CPU_IRQ_TGI0V             26

#define CPU_IRQ_TGI1A             27
#define CPU_IRQ_TGI1B             28
#define CPU_IRQ_TGI1V             29
#define CPU_IRQ_TGI1U             30

#define CPU_IRQ_TGI2A             31
#define CPU_IRQ_TGI2B             32
#define CPU_IRQ_TGI2V             33
#define CPU_IRQ_TGI2U             34

#define CPU_IRQ_TGI3A             35
#define CPU_IRQ_TGI3B             36
#define CPU_IRQ_TGI3C             37
#define CPU_IRQ_TGI3D             38
#define CPU_IRQ_TGI3V             39
	
#define CPU_IRQ_TGI4A             40
#define CPU_IRQ_TGI4B             41
#define CPU_IRQ_TGI4V             42
#define CPU_IRQ_TGI4U             43

#define CPU_IRQ_TGI5A             44
#define CPU_IRQ_TGI5B             45
#define CPU_IRQ_TGI5V             46
#define CPU_IRQ_TGI5U             47

#define CPU_IRQ_CMIA0             48
#define CPU_IRQ_CMIB0             49
#define CPU_IRQ_OVI0              50

#define CPU_IRQ_CMIA1             51
#define CPU_IRQ_CMIB1             52
#define CPU_IRQ_OVI1              53

#define CPU_IRQ_DEND0A            54
#define CPU_IRQ_DEND0B            55
#define CPU_IRQ_DEND1A            56
#define CPU_IRQ_DEND1B            57

#define CPU_IRQ_ERI0              58
#define CPU_IRQ_RXI0              59
#define CPU_IRQ_TXI0              60
#define CPU_IRQ_TEI0              61

#define CPU_IRQ_ERI1              62
#define CPU_IRQ_RXI1              63
#define CPU_IRQ_TXI1              64
#define CPU_IRQ_TEI1              65

#define CPU_IRQ_ERI2              66
#define CPU_IRQ_RXI2              67
#define CPU_IRQ_TXI2              68
#define CPU_IRQ_TEI2              69

#define CPU_IRQ_CMIA2             70
#define CPU_IRQ_CMIB2             71
#define CPU_IRQ_OVI2              72
	
#define CPU_IRQ_CMIA3             73
#define CPU_IRQ_CMIB3             74
#define CPU_IRQ_OVI3              75
	
#define CPU_IRQ_IICI0             76 
#define CPU_IRQ_DDCSW1            77 
#define CPU_IRQ_IICI1             78 

#define CPU_IRQ_ERI3              79 
#define CPU_IRQ_RXI3              80 
#define CPU_IRQ_TXI3              81 
#define CPU_IRQ_TEI3              82 

#define CPU_IRQ_ERI4              83 
#define CPU_IRQ_RXI4              84 
#define CPU_IRQ_TXI4              85 
#define CPU_IRQ_TEI4              86 

#endif /* __CPU_H8S2633_H__ */





