#----------------------------------------------------------------------------------
# clean-build - non-recursive build system based on GNU Make
# Copyright (C) 2015-2017 Michael M. Builov, https://github.com/mbuilov/clean-build
# Licensed under GPL version 2 or any later version, see COPYING
#----------------------------------------------------------------------------------

# original file: $(CLEAN_BUILD_DIR)/stub/defs.mk
# description:   support for building generic targets - define DEFINE_TARGETS macro

# Note: This file should be copied AS IS to the directory of the project build system

ifeq (,$(filter-out undefined environment,$(origin CB_PREPARE_TARGET_TYPE)))
include $(dir $(lastword $(MAKEFILE_LIST)))project.mk
endif

$(CB_PREPARE_TARGET_TYPE)
