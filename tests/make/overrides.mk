#----------------------------------------------------------------------------------
# clean-build - non-recursive build system based on GNU Make
# Copyright (C) 2015-2017 Michael M. Builov, https://github.com/mbuilov/clean-build
# Licensed under GPL version 2 or any later version, see COPYING
#----------------------------------------------------------------------------------

# original file: $(CLEAN_BUILD_DIR)/stub/overrides.mk
# description:   core clean-build definitions, processing of CONFIG and OVERRIDES variables, definition of MTOP variable

# Note: This file should be copied AS IS to the directory of the project build system
# Note: This file should be included at end of project configuration makefile 'project.mk' - just before autoconfigure includes

# if CONFIG variable is not defined in project configuration makefile, provide default definition
ifeq (,$(filter-out undefined environment,$(origin CONFIG)))

# CONFIG - name of clean-build generated config file (while completing predefined 'conf' goal)
#
# Note: generated $(CONFIG) file will remember values of command-line or overridden variables;
#  by sourcing $(CONFIG) file below, these variables are will be restored,
#  and only new command-line defined values may override restored ones
#
# Note: by completing predefined 'distclean' goal, $(BUILD) directory will be deleted
#  - together with $(CONFIG) file, which is by default is generated under the $(BUILD)
#
# Note: define CONFIG as recursive variable
#  - for the case when BUILD is redefined in included next $(OVERRIDES) makefile
CONFIG = $(BUILD)/conf.mk

endif # !CONFIG

# process a file with overrides of the project defaults (set in project configuration makefile - 'project.mk')
# Note: OVERRIDES variable may be specified in command line - to override this default empty definition
ifeq (,$(filter-out undefined environment,$(origin OVERRIDES)))
OVERRIDES:=
endif

# override definitions (e.g. BUILD, PRODUCT_VER, etc.) in 'project.mk' by definitions in the custom $(OVERRIDES) makefile
ifdef OVERRIDES
ifeq (,$(wildcard $(OVERRIDES)))
$(error file does not exist: $(OVERRIDES))
endif
include $(OVERRIDES)
endif

# source optional clean-build generated config file, if it exist
-include $(CONFIG)

# MTOP - path to clean-build build system
# Note: normally MTOP is defined in command line, but may be taken from the environment
# redefine MTOP as a simple (i.e. non-recursive) variable
ifeq (undefined,$(origin MTOP))
MTOP:=
else
MTOP := $(MTOP)
endif

# path to clean-build must be defined
ifndef MTOP
$(error MTOP - path to clean-build (https://github.com/mbuilov/clean-build) is not defined,\
 example: MTOP=/usr/local/clean-build or MTOP=C:\User\clean-build)
endif

# source clean-build base definitions
ifeq (,$(wildcard $(MTOP)/core/_defs.mk))
$(error clean-build files are not found under MTOP=$(MTOP))
endif
include $(MTOP)/core/_defs.mk
