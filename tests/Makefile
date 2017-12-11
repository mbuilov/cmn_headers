#----------------------------------------------------------------------------------
# cmn_headers tests
# Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
# Licensed under Apache License v2.0, see LICENSE.TXT
#----------------------------------------------------------------------------------

# add support for processing sub-makefiles
include $(dir $(lastword $(MAKEFILE_LIST)))make/parallel.mk

# clean-build provides default rule for the 'distclean' goal, do not handle it here
ifneq (distclean,$(MAKECMDGOALS))

# list of sub-makefiles
TO_MAKE := get_opt_test.mk

# we need version info under Windows
ifneq (,$(filter WIN%,$(OS)))
TO_MAKE += $(MTOP)/extensions/version
endif

# process sub-makefiles and directories specified in TO_MAKE variable
$(call PROCESS_SUBMAKES,$(TO_MAKE))

endif # !distclean
