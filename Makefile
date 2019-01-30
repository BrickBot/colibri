# ------------------------------------------------------------------------- 
# Makefile - root makefile for colibri
#                                                                            
# Copyright (c) 2002 Magnus Damm <damm@opensource.se>             
# All rights reserved.                                                      
#                                                                            
# Redistribution and use in source and binary forms, with or without        
# modification, are permitted provided that the following conditions        
# are met:                                                                  
#                                                                            
# 1. Redistributions of source code must retain the above copyright         
#    notice, this list of conditions and the following disclaimer.          
# 2. Redistributions in binary form must reproduce the above copyright      
#    notice, this list of conditions and the following disclaimer in the    
#    documentation and/or other materials provided with the distribution.   
# 3. Neither the name of the copyright holders nor the name of its          
#    contributors may be used to endorse or promote products derived        
#    from this software without specific prior written permission.          
#                                                                           
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS   
# IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR    
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT OWNER OR CONTRIBUTORS 
# BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR    
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF      
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS  
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN   
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)   
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF    
# THE POSSIBILITY OF SUCH DAMAGE.                                           
# ------------------------------------------------------------------------- 

TOPDIR = $(shell /bin/pwd)

include $(TOPDIR)/Make.conf

BAD_BOARD := $(shell if [ ! -z "$(BOARD)" ]; then if [ -e "board/$(BOARD)" ]; \
	then echo "ok"; else echo "bad"; fi; else echo "bad"; fi;)

.PHONY: config check_config get_config allobjs

DIRS := . board/$(BOARD) arch/$(ARCH) arch/$(ARCH)/$(CPU) \
        sched timer sem string irq data_types

all: check_config get_config srec

check_config:
ifeq ($(BAD_BOARD),bad)
	@echo
	@echo "Colibri is not configured yet."
	@echo
	@echo "You need to specify what board to configure for."
	@echo "Available boards:"
	@echo
	@cd board && for d in *; do echo $$d; done
	@echo
	@echo "usage: make config BOARD=<your selection>"
	@echo
	@exit 1
endif

get_config: board/$(BOARD)/board.conf
	@cp board/$(BOARD)/board.conf $(TOPDIR)/

config: check_config get_config
	@echo "colibri is now configured for $(BOARD)."

objects.list: $(DIRS:=/objects)
	@rm -f $@
	@for o in $^; do ./tools/get_objects $$o ./colibri_config.sh >> $@; done

includes.list: $(DIRS)
	@rm -f $@
	@for o in $^; do echo -ne " -I$$o" >> $@; done

Makefile.tmp: objects.list includes.list
	@echo "#This file is created by the main Makefile." > $@
	@echo "#Do not edit." >> $@
	@echo -ne "TOPDIR = $$" >> $@
	@echo "(shell /bin/pwd)" >> $@
	@echo -ne "include $$" >> $@
	@echo "(TOPDIR)/Make.conf" >> $@

	@echo -ne "CFLAGS := $$" >> $@
	@echo -ne "(CFLAGS)" >> $@
	@cat includes.list >> $@
	@echo "" >> $@

	@echo -ne "ifeq ($$" >> $@
	@echo -ne "(TOPDIR)/deps.rules,$$" >> $@
	@echo -ne "(wildcard $$" >> $@
	@echo "(TOPDIR)/deps.rules))" >> $@
	@echo -ne "include $$" >> $@
	@echo "(TOPDIR)/deps.rules" >> $@
	@echo "endif" >> $@

	@echo -ne "OBJECTS := " >> $@
	@for o in `cat objects.list`; do echo -ne "$$o " >> $@; done
	@echo "" >> $@
	@echo -ne "allobjs: $$" >> $@
	@echo "(OBJECTS)" >> $@
	@echo -ne "deps.rules: $$" >> $@
	@echo "(OBJECTS:.o=.d)" >> $@
	@echo -ne "\t@./tools/gcc/get_deps $$" >> $@
	@echo "(OBJECTS:.o=.d) > deps.rules" >> $@

ldscript.tmp: objects.list
	@cat $^ | ./tools/$(COMPILER)/get_linkerscript $(LDSCRIPT) > $@

deps.rules: colibri_config.sh Makefile.tmp
	@$(MAKE) -f Makefile.tmp deps.rules

allobjs: colibri_config.sh Makefile.tmp deps.rules 
	@$(MAKE) -f Makefile.tmp allobjs

colibri.o: allobjs ldscript.tmp
	$(CC) $(CFLAGS) $(CFLAGS_LD) -o $@ 

%.srec:	%.o
	$(OBJCOPY) -O srec $*.o $*.srec

%.bin:	%.o
	$(OBJCOPY) -O binary $*.o $*.bin

bin:	colibri.bin

srec:	colibri.srec

colibri_config.sh: colibri_config.h
	@$(MAKE) -f Makefile clean
	@$(HOSTCC) -I. -c tools/config/config_check.c -o colibri_config.tmp
	@cat colibri_config.h | sed -f tools/config/remccoms3.sed | tools/config/get_shellconfig | uniq > $@
	@rm -f colibri_config.tmp

clean:	check_config
	@rm -f includes.list objects.list deps.rules
	@rm -f Makefile.tmp ldscript.tmp 
	@rm -f colibri_config.sh colibri_config.tmp
	@rm -f *.srec *.bin *.map `find -name \*.d` `find -name \*.o`

distclean: clean
	@rm -f $(TOPDIR)/board.conf `find . | grep "~"`
