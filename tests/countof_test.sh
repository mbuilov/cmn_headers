#!/bin/bash

# to check clang, run as
# CC=clang CXX="clang++ -Wno-deprecated" ./countof_test.sh

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
Step && $CC  -Wall -pedantic -Wextra -c countof_test.c || Exit
Step && $CXX -Wall -pedantic -Wextra -c countof_test.c || Exit

# should not be compiled
Step && $CC -c countof_test.c -DBAD1 && Exit
Step && $CC -c countof_test.c -DBAD2 && Exit
Step && $CC -c countof_test.c -DBAD3 && Exit
Step && $CC -c countof_test.c -DBAD4 && Exit
Step && $CC -c countof_test.c -DBAD5 && Exit

# should not be compiled
Step && $CXX -c countof_test.c -DBAD1 && Exit
Step && $CXX -c countof_test.c -DBAD2 && Exit
Step && $CXX -c countof_test.c -DBAD3 && Exit
Step && $CXX -c countof_test.c -DBAD4 && Exit
Step && $CXX -c countof_test.c -DBAD5 && Exit

echo "=============== all tests OK ==============="
