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

$(call DO_TEST_EXE,1 --help 3 --file 2 -b3 --level=4 -c 5 -d6 --debug 7 -e 8 -er\
  --output=9 -fg -f g --verbose 1 --verbose=4 -g 3 -g1 -9 - --trace --trace q --trace=v -h 6 -- 4 -b --y 9)

$(DEFINE_TARGETS)
