/* ------------------------------------------------------------------------- */
/* board_i2c.c                                                               */
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
#include "types.h"
#include "cpu.h"

#define P6DDR cpu_regs.iop.p6ddr
#define P6DR  cpu_regs.iop.p6dr

#define SDA_BIT (1 << 6)     // bit 6 SDA
#define SCL_BIT (1 << 5)     // bit 5 SCL

#define SDA_OUTPUT() P6DDR |= SDA_BIT     /* bit set:     output */
#define SDA_INPUT()  P6DDR &= ~(SDA_BIT)  /* bit cleared: input  */

#define SDA_HI()     P6DR |= SDA_BIT
#define SDA_LO()     P6DR &= ~(SDA_BIT)

#define SCL_OUTPUT() P6DDR |= SCL_BIT

#define SCL_HI()     P6DR |= SCL_BIT
#define SCL_LO()     P6DR &= ~(SCL_BIT)

static void i2c_delay(void)
{
	uint8_t k;

	for (k = 0; k < 3; k++) {
		__asm__("nop");
	}
}

static void i2c_start(void)
{
	/* generate a start condition */

	/* both SDA and SCL are high when inactive (ie now) */
	
	SDA_LO();  /* pull down SDA while SCL remains high */

	i2c_delay();
}

static void i2c_stop(void)
{
	/* generate a stop condition */

	/* SDA is low and SCL is high here */
	
	i2c_delay();

	SDA_HI();  /* release SDA while SCL remains high */
}


int i2c_write(uint8_t *data, uint8_t nobytes)
{
	uint8_t k, uc;

	i2c_start();

	while (nobytes > 0) {

		uc = *data++;

		for (k = 0; k < 8; k++) {

			SCL_LO();

			i2c_delay();

			if (uc & 0x80) {
				SDA_HI();
			}
			else {
				SDA_LO();
			}
			
			i2c_delay();

			SCL_HI();

			i2c_delay();
			
			uc <<= 1;
		}

		SCL_LO();       /* some kind of ack */
		SDA_INPUT();
		
		i2c_delay();

		SCL_HI();

		i2c_delay();

		SDA_OUTPUT();

		nobytes--;
	}

	SCL_LO();

	i2c_delay();

	SDA_LO();

	SCL_HI();

	i2c_stop();

	return 0;
}

int i2c_init(void)
{
	SDA_OUTPUT();
	SCL_OUTPUT();
	SDA_HI();
	SCL_HI();

	return 0;
}

void i2c_cleanup(void)
{

}
