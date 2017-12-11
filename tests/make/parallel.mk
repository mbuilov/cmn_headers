#----------------------------------------------------------------------------------
# clean-build - non-recursive build system based on GNU Make
# Copyright (C) 2015-2017 Michael M. Builov, https://github.com/mbuilov/clean-build
# Licensed under GPL version 2 or any later version, see COPYING
#----------------------------------------------------------------------------------

# original file: $(CLEAN_BUILD_DIR)/stub/parallel.mk
# description:   support for processing sub-makefiles - define PROCESS_SUBMAKES macro

# Note: This file should be copied AS IS to the directory of the project build system

ifeq (,$(filter-out undefined environment,$(origin PROCESS_SUBMAKES_PREPARE)))
include $(dir $(lastword $(MAKEFILE_LIST)))project.mk
include $(MTOP)/core/_parallel.mk
endif

$(PROCESS_SUBMAKES_PREPARE)
