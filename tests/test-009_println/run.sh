#!/bin/bash
set -eo pipefail

bash ../scripts/compile src/test-println.cpp
bash ../scripts/test-ref test-println/r1 ./test-println
bash ../scripts/test-ref test-println/r2 "$VALGRIND" ./test-println
rm -f test-println test-println.exe