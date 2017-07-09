#----------------------------------------------------------------------------------
# Makefile of program options and parameters parsing test
# Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
# Licensed under Apache License v2.0, see LICENSE.TXT
#----------------------------------------------------------------------------------

include $(dir $(lastword $(MAKEFILE_LIST)))project.mk
include $(MTOP)/c.mk
include $(MTOP)/exts/ctest.mk
include $(MTOP)/exts/cmp.mk

EXE := get_opt_test S
SRC := get_opt_test.c

$(call DO_TEST_EXE,1 -gaf --help 3 --file 2 -b3 --level=4 -l 5 -d6 --debug 7 -o 8 -vr --output=9\
  -fg -f g --verbose 1 --verbose=4 -g 3 -g1 -9 - --trace --trace q --trace=v -t -tra -trace -h 6 -ogv- -- 4 -b --y 9)

$(call DO_CMP_OUT,$(addsuffix .out,$(call ALL_TRG,EXE)),get_opt_test.out)

$(DEFINE_TARGETS)
