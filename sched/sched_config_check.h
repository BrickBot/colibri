/* ------------------------------------------------------------------------- */
/* sched_config_check.h                                                      */
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

#if (!(defined(COLIBRI_CONFIG_SCHED_ADVANCED) || \
     defined(COLIBRI_CONFIG_SCHED_SIMPLE)) ||  \
     (defined(COLIBRI_CONFIG_SCHED_ADVANCED) && \
     defined(COLIBRI_CONFIG_SCHED_SIMPLE)))
#error Scheduler selection error.
#error Only one of the following must be defined:
#error COLIBRI_CONFIG_SCHED_ADVANCED
#error COLIBRI_CONFIG_SCHED_SIMPLE
#endif

#ifndef COLIBRI_CONFIG_SCHED_PRIORITY_MAX
#error Number of priority levels not defined.
#error Give COLIBRI_CONFIG_SCHED_PRIORITY_MAX a value.
#endif

#ifndef COLIBRI_CONFIG_SCHED_PRIORITY_ROOT
#error The root thread is not assigned any priority level.
#error Give COLIBRI_CONFIG_SCHED_PRIORITY_ROOT a value.
#endif

#if (COLIBRI_CONFIG_SCHED_PRIORITY_ROOT >= COLIBRI_CONFIG_SCHED_PRIORITY_MAX) \
    || (COLIBRI_CONFIG_SCHED_PRIORITY < 0)
#error The root thread is assigned a non-valid priority level.
#error Give COLIBRI_CONFIG_SCHED_PRIORITY_ROOT a decent value.
#endif

#ifdef COLIBRI_CONFIG_SCHED_LIST
#ifndef COLIBRI_CONFIG_SCHED_LIST_STATIC
#error Thread list type undefined.
#error Define COLIBRI_CONFIG_SCHED_LIST_STATIC.

#else

#ifndef COLIBRI_CONFIG_SCHED_LIST_STATIC_THREADS_MAX
#error Maximum number of threads not defined.
#error Give COLIBRI_CONFIG_SCHED_LIST_STATIC_THREADS_MAX a value.
#endif

#endif
#endif
