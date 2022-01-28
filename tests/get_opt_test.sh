#!/bin/bash

step=0

test "x$CC" = "x"  && CC=gcc
test "x$CXX" = "x" && CXX=g++

Step() {
  echo "step: $step"
  step=$((step + 1))
  return 0
}

Exit() {
  echo "failed!"
  exit 1
}

test1='! $ ^ '"'"' " % ^ 1 2 #'
test2=' -gaf --help 3 --file 2 -b3 --level=4 -l 5 -d6 --debug 7 -o 8& -vr --output=9 -fg -f g'"\
  "'--verbose 1 --verbose=4 -g 3 -g1 -9 - --trace --trace q --trace=v -t -tra -trace -h 6 -ogv- -- 4 -b --y 9'

echo "$test"

Step && $CC -Wall -pedantic -Wextra ./get_opt_test.c -o ./get_opt_test || Exit
Step && ./get_opt_test "$test1" $test2 > get_opt.out || Exit
Step && diff ./get_opt.out ./get_opt_test.out || Exit

Step && $CC -DGET_OPT_ARGV_NZ -Wall -pedantic -Wextra ./get_opt_test.c -o ./get_opt_test_nz || Exit
Step && ./get_opt_test_nz "$test1" $test2 > get_opt_nz.out || Exit
Step && diff ./get_opt_nz.out ./get_opt_test.out || Exit

echo "=============== all tests OK ==============="
