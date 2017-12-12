#----------------------------------------------------------------------------------
# Makefile of program options and parameters parsing test
# Copyright (C) 2012-2017 Michael M. Builov, https://github.com/mbuilov/cmn_headers
# Licensed under Apache License v2.0, see LICENSE.TXT
#----------------------------------------------------------------------------------

# add rules for building C/C++ sources
include $(dir $(lastword $(MAKEFILE_LIST)))make/c.mk

# define rules for testing built executables - for the 'check' goal
include $(MTOP)/extensions/ctest.mk

# add rule for comparing outputs of tested executables with given file, for the 'check' goal
include $(MTOP)/extensions/cmp.mk

# build S-variant of 'get_opt_test' executable - one with statically linked C runtime
EXE := get_opt_test S
SRC := get_opt_test.c

# generate rules for testing built executable and creating output file
# save the name of generated output file (the same as $(addsuffix .out,$(call ALL_TARGETS,EXE)))
OUT := $(call DO_TEST_EXE_RET,$(call SHELL_ESCAPE,! $$ ^ ' " % ^ 1 2 $(comment)) -gaf\
  --help 3 --file 2 -b3 --level=4 -l 5 -d6 --debug 7 -o 8 -vr --output=9 -fg -f g\
  --verbose 1 --verbose=4 -g 3 -g1 -9 - --trace --trace q --trace=v -t -tra -trace -h 6 -ogv- -- 4 -b --y 9)

# generate a rule for comparing $(OUT) and get_opt_test.out
$(call DO_CMP_TEXT,$(OUT),get_opt_test.out)

$(DEFINE_TARGETS)
