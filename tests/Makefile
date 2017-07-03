#----------------------------------------------------------------------------------
# cmn_headers tests
# Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
# Licensed under Apache License v2.0, see LICENSE.TXT
#----------------------------------------------------------------------------------

include $(dir $(lastword $(MAKEFILE_LIST)))project.mk
include $(MTOP)/parallel.mk
ifneq (distclean,$(MAKECMDGOALS))

TO_MAKE := get_opt_test.mk

# we need version info under Windows
ifeq (WINXX,$(OS))
TO_MAKE += $(MTOP)/exts/version
endif

# process sub-makefiles and directories specified in TO_MAKE variable
$(call PROCESS_SUBMAKES,$(TO_MAKE))

endif # !distclean
