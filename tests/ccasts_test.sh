#!/bin/bash

# to check clang, run as
# CC=clang CXX="clang++ -Wno-deprecated" ./ccasts_test.sh

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

# should be compiled
Step && $CC  -Wall -pedantic -Wextra -c ccasts_test.c || Exit
Step && $CXX -Wall -pedantic -Wextra -c ccasts_test.c || Exit

# should not be compiled
Step && $CC -c ccasts_test.c -DBAD1 && Exit
Step && $CC -c ccasts_test.c -DBAD2 && Exit
Step && $CC -c ccasts_test.c -DBAD3 && Exit
Step && $CC -c ccasts_test.c -DBAD4 && Exit
Step && $CC -c ccasts_test.c -DBAD5 && Exit
Step && $CC -c ccasts_test.c -DBAD6 && Exit
Step && $CC -c ccasts_test.c -DBAD7 && Exit
Step && $CC -c ccasts_test.c -DBAD8 && Exit
Step && $CC -c ccasts_test.c -DBAD9 && Exit
Step && $CC -c ccasts_test.c -DBAD10 && Exit
Step && $CC -c ccasts_test.c -DBAD11 && Exit
Step && $CC -c ccasts_test.c -DBAD12 && Exit
Step && $CC -c ccasts_test.c -DBAD13 && Exit
Step && $CC -c ccasts_test.c -DBAD14 && Exit

# should not be compiled
Step && $CXX -c ccasts_test.c -DBAD1 && Exit
Step && $CXX -c ccasts_test.c -DBAD2 && Exit
Step && $CXX -c ccasts_test.c -DBAD3 && Exit
Step && $CXX -c ccasts_test.c -DBAD4 && Exit
Step && $CXX -c ccasts_test.c -DBAD5 && Exit
Step && $CXX -c ccasts_test.c -DBAD6 && Exit
Step && $CXX -c ccasts_test.c -DBAD7 && Exit
Step && $CXX -c ccasts_test.c -DBAD8 && Exit
Step && $CXX -c ccasts_test.c -DBAD9 && Exit
Step && $CXX -c ccasts_test.c -DBAD10 && Exit
Step && $CXX -c ccasts_test.c -DBAD11 && Exit
Step && $CXX -c ccasts_test.c -DBAD12 && Exit
Step && $CXX -c ccasts_test.c -DBAD13 && Exit
Step && $CXX -c ccasts_test.c -DBAD14 && Exit

echo "=============== all tests OK ==============="
