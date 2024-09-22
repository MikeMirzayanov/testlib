#!/bin/bash
set -eo pipefail

bash ../scripts/compile files/test-format.cpp
bash ../scripts/test-ref test-format/r1 ./test-format
bash ../scripts/test-ref test-format/r2 "$VALGRIND" ./test-format
rm -f test-format test-format.exe
