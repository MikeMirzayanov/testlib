#!/bin/bash
set -eo pipefail

bash ../scripts/compile files/test-println-empty.cpp
bash ../scripts/test-ref test-println-empty/r1 ./test-println-empty
bash ../scripts/test-ref test-println-empty/r2 "$VALGRIND" ./test-println-empty
rm -f test-println-empty test-println-empty.exe
