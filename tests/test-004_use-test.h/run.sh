#!/bin/bash
set -eo pipefail

bash ../scripts/compile test.cpp
bash ../scripts/test-ref r1 "$VALGRIND" ./test
rm -f test test.exe
