
#ifndef __COLIBRI_CONFIG_H__
#define __COLIBRI_CONFIG_H__

/* - - - scheduler configuration - - - */

#define COLIBRI_CONFIG_SCHED_SIMPLE /* define to select a simple scheduler */
//#define COLIBRI_CONFIG_SCHED_ADVANCED /* define to select the advanced one */

#define COLIBRI_CONFIG_SCHED_ADVANCED_RR /* define to enable round-robin */

#define COLIBRI_CONFIG_SCHED_PRIORITY_MAX 16 /* number of priorities */
#define COLIBRI_CONFIG_SCHED_PRIORITY_ROOT 10 /* priority of the root thread */

#define COLIBRI_CONFIG_SCHED_LIST /* should we keep a list of all threads */
#define COLIBRI_CONFIG_SCHED_LIST_STATIC /* should the list be static */
#define COLIBRI_CONFIG_SCHED_LIST_STATIC_THREADS_MAX 10 /* max nr of threads */

/* - - - timer configuration - - - */

#define COLIBRI_CONFIG_TIMER_TICKS 8

/* - - - sem configuration - - - */

#define COLIBRI_CONFIG_SEM_COUNTING
#define COLIBRI_CONFIG_SEM_BINARY

#define COLIBRI_CONFIG_SEM_INVERSIONSAFE
#define COLIBRI_CONFIG_SEM_PRIORITY
#define COLIBRI_CONFIG_SEM_FIFO

/* - - - irq configuration - - - */

#define COLIBRI_CONFIG_IRQ_SHARE /* should interrupts be sharable */

/* - - - cpu configuration - - - */

#define COLIBRI_CONFIG_CPU_IRQ_STACKSIZE 64

/* - - - cpu configuration for h8s2633f - - - */

#define COLIBRI_CONFIG_CPU_ICM0 /* define to use interrupt control mode 0 */
//#define COLIBRI_CONFIG_CPU_ICM2 /* define to use interrupt control mode 2 */

#endif /* __COLIBRI_CONFIG_H__ */

