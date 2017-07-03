#----------------------------------------------------------------------------------
# Makefile of program options parsing test
# Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
# Licensed under Apache License v2.0, see LICENSE.TXT
#----------------------------------------------------------------------------------

include $(dir $(lastword $(MAKEFILE_LIST)))project.mk
include $(MTOP)/c.mk
include $(MTOP)/exts/ctest.mk

EXE := get_opt_test S
SRC := get_opt_test.c

$(DO_TEST_EXE)

$(DEFINE_TARGETS)
