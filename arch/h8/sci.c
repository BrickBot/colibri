
#include "colibri_config.h"
#include "types.h"
#include "cpu.h"

#include "irq.h"

#if 0
#define COLIBRI_CONFIG_SCI_SCI1
#define COLIBRI_CONFIG_SCI_SCI2
#define COLIBRI_CONFIG_SCI_SCI3
#define COLIBRI_CONFIG_SCI_SCI4     
#endif

/* h8/3297 support */

#ifdef COLIBRI_CONFIG_CPU_H83297
#define SCI_USE_PTR
typedef volatile cpu_sci_t sci_hwregs_t;
#define SCI_SCI0 SCI_REGS(&cpu_regs.sci) SCI_PWR(NULL, 0)
#endif

/* h8s/2633 support */

#ifdef COLIBRI_CONFIG_CPU_H8S2633
#define SCI_USE_PTR
#define SCI_USE_PWR
#define SCI_USE_IRQ
typedef volatile cpu_sci_t sci_hwregs_t;
#define SCI_SCI0                         \
        SCI_REGS(&cpu_regs.sci0)         \
        SCI_PWR(&cpu_regs.mstpcrb, 0x80) \
        SCI_IRQ(CPU_IRQ_ERI0, CPU_IRQ_RXI0, CPU_IRQ_TXI0, CPU_IRQ_TEI0)

#define SCI_SCI1 SCI_REGS(&cpu_regs.sci1) SCI_PWR(&cpu_regs.mstpcrb, 0x40)
#define SCI_SCI2 SCI_REGS(&cpu_regs.sci2) SCI_PWR(&cpu_regs.mstpcrb, 0x20)
#define SCI_SCI3 SCI_REGS(&cpu_regs.sci3) SCI_PWR(&cpu_regs.mstpcrc, 0x80)
#define SCI_SCI4 SCI_REGS(&cpu_regs.sci4) SCI_PWR(&cpu_regs.mstpcrc, 0x40)
#endif

/* By defining SCI_USE_PTR, we point directly info each struct
 * that describes the register layout of the sci. 
 * This only works if all sci:s have the same register layout. 
 * If SCI_USE_PTR is undef:ed, we keep lists of all registers, 
 * this leads to generic code but uses more memory.
 */

#ifdef SCI_USE_PTR

#define SCI_REGS(name) (sci_hwregs_t *)(name),

#define SMR(regs) ((regs)->smr)
#define BRR(regs) ((regs)->brr)
#define SCR(regs) ((regs)->scr)
#define TDR(regs) ((regs)->tdr)
#define SSR(regs) ((regs)->ssr)
#define RDR(regs) ((regs)->rdr)
#define SCMR(regs) ((regs)->scmr)

#else

typedef struct sci_hwregs {
	volatile uint8_t *smr;
	volatile uint8_t *brr;
	volatile uint8_t *scr;
	volatile uint8_t *tdr;
	volatile uint8_t *ssr;
	volatile uint8_t *rdr;
	volatile uint8_t *scmr;
} sci_hwregs_t;

#define SCI_REGS(name) \
(&(const sci_hwregs_t)  \
{                       \
  &((name)->smr),       \
  &((name)->brr),       \
  &((name)->scr),       \
  &((name)->tdr),       \
  &((name)->ssr),       \
  &((name)->rdr),       \
  &((name)->scmr),      \
} ),

#define SMR(regs) *((regs)->smr)
#define BRR(regs) *((regs)->brr)
#define SCR(regs) *((regs)->scr)
#define TDR(regs) *((regs)->tdr)
#define SSR(regs) *((regs)->ssr)
#define RDR(regs) *((regs)->rdr)
#define SCMR(regs) *((regs)->scmr)

#endif

/* if SCI_USE_PWR is defined, we control powerup / powerdown of each
 * sci controller. Otherwise nothing like that is done.
 */

#ifdef SCI_USE_PWR
#define SCI_PWR(reg, mask) { reg, mask },

typedef struct sci_hwpwr {
	volatile uint8_t *reg;
	uint8_t mask;
} sci_hwpwr_t;
#else
#define SCI_PWR(reg, mask)
#endif

#ifdef SCI_USE_IRQ
#define SCI_IRQ(eri, rxi, txi, tei) { eri, rxi, txi, tei },

typedef struct sci_irq {
	uint8_t eri;
	uint8_t rxi;
	uint8_t txi;
	uint8_t tei;
} sci_irq_t;
#else
#define SCI_IRQ(eri, rxi, txi, tei)
#endif

typedef struct sci_hwinfo {
	sci_hwregs_t *regs;
#ifdef SCI_USE_PWR
	sci_hwpwr_t pwr;
#endif
#ifdef SCI_USE_IRQ
	sci_irq_t irq;
#endif
} sci_hwinfo_t;

typedef struct sci_portinfo {
	sci_hwinfo_t hw;
} sci_portinfo_t;

sci_portinfo_t sci_ports[] =
{
#ifdef COLIBRI_CONFIG_SCI_SCI0
#if defined(SCI_SCI0)
	{ { SCI_SCI0 }, },
#else
#error sci: SCI0 not available for the selected cpu.
#endif
#endif

#ifdef COLIBRI_CONFIG_SCI_SCI1
#if defined(SCI_SCI1)
	{ { SCI_SCI1 }, },
#else
#error sci: SCI1 not available for the selected cpu.
#endif
#endif

#ifdef COLIBRI_CONFIG_SCI_SCI2
#if defined(SCI_SCI2)
	{ { SCI_SCI2 }, }, 
#else
#error sci: SCI2 not available for the selected cpu.
#endif
#endif

#ifdef COLIBRI_CONFIG_SCI_SCI3
#if defined(SCI_SCI3)
	{ { SCI_SCI3 }, },
#else
#error sci: SCI3 not available for the selected cpu.
#endif
#endif

#ifdef COLIBRI_CONFIG_SCI_SCI4
#if defined(SCI_SCI4)
	{ { SCI_SCI4 }, },
#else
#error sci: SCI4 not available for the selected cpu.
#endif
#endif

#ifdef COLIBRI_CONFIG_SCI_SCI5
#if defined(SCI_SCI5)
	{ { SCI_SCI5 }, },
#else
#error sci: SCI5 not available for the selected cpu.
#endif
#endif

#ifdef COLIBRI_CONFIG_SCI_SCI6
#if defined(SCI_SCI6)
	{ { SCI_SCI6 }, },
#else
#error sci: SCI6 not available for the selected cpu.
#endif
#endif

#ifdef COLIBRI_CONFIG_SCI_SCI7
#if defined(SCI_SCI7)
	{ { SCI_SCI7 }, },
#else
#error sci: SCI7 not available for the selected cpu.
#endif
#endif
};

#define SCI_PORTS_NO (sizeof(sci_ports) / sizeof(sci_portinfo_t))

#define BOARD_BRR_9600 58

#ifdef SCI_USE_IRQ
irq_handler_t sci_handler; //fixme
#endif

int sci_txi_handler(void *arg);

int sci_port_init(sci_portinfo_t *port)
{
	sci_hwregs_t *regs = port->hw.regs;

#ifdef SCI_USE_PWR
	*port->hw.pwr.reg &= ~(port->hw.pwr.mask);
#endif	

	/* disable the serial port */

	SCR(regs) = 0;

	/* setup the serial controller as uart n81 and clock/1 */

	SMR(regs) = 0;
	
	/* setup the clock generator to the desired bps */
	
	BRR(regs) = BOARD_BRR_9600;

#if 0 //def SCI_USE_IRQ
	irq_handler_init(&sci_handler, port->hw.irq.tei,
			 sci_txi_handler, port, 0);

	if (irq_handler_register(&sci_handler) < 0) {
		return -1;
	}
#endif
	/* enable transmit and receive, no interrupts (yet) */

	SCR(regs) = 0x30;

	return 0;
}

int sci_init(void)
{
	int k;

	for (k = 0; k < SCI_PORTS_NO; k++) {
		sci_port_init(&sci_ports[k]);
	}

	return 0;
}

int sci_write(sci_portinfo_t *port, unsigned char *buf, int nobytes)
{
	sci_hwregs_t *regs = port->hw.regs;
	int k;



	for (k = 0; k < nobytes; k++) {
		while (!(SSR(regs) & 0x80)) {
			/* wait until TDRE is 1 */
		}

		TDR(regs) = buf[k];
		SSR(regs) &= ~0x80; /* clear TDRE */
#if 1
//		pins_led_toggle();

		{
			char c;
			unsigned int m;
			for (m = 0; m < 0xffff; m++) {
				memset(&c, 0, 1);
			}
		}
#endif
	}

	/* wait until all bits are sent */

	while (!(SSR(regs) & 0x04)) {
		/* wait until TEND is 1 */
	}

	return k;
}

int strlen(char *str)
{
	char *org = str;

	while(*str != '\0') {
		str++;
	}

	return str - org;
}

#ifdef COLIBRI_CONFIG_BOARD_HMS_EVB_H8S2633F

void puts(char *str)
{
	cpu_flags_t flags = cpu_flags_save_and_cli();

	sci_write(&sci_ports[0], str, strlen(str));

	cpu_flags_restore(flags);
}

char hexchars[] = { '0', '1', '2', '3', '4', '5', '6', '7',
		    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

void put_u32(uint32_t data)
{
	char str[3];
	int k;
	unsigned char c;

	str[2] = '\0';
	
	for (k = 0; k < 4; k++) {
		c = (data >> (((4 - k) - 1) * 8)) & 0xff;
		
		str[0] = hexchars[c >> 4];
		str[1] = hexchars[c & 0x0f];
		puts(str);
	}
	puts("\n\r");
}

#endif
