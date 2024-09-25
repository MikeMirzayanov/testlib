#!/bin/bash
set -eo pipefail

bash ../scripts/compile files/test-format.cpp
bash ../scripts/test-ref test-format/r1 ./test-format
bash ../scripts/test-ref test-format/r2 "$VALGRIND" ./test-format
rm -f test-format test-format.exe

bash ../scripts/compile files/test-format-format1.cpp
bash ../scripts/test-ref test-format-format1/r1 ./test-format-format1
bash ../scripts/test-ref test-format-format1/r2 "$VALGRIND" ./test-format-format1
rm -f test-format-format1 test-format-format1.exe

bash ../scripts/compile files/test-format-format2.cpp
bash ../scripts/test-ref test-format-format2/r1 ./test-format-format2
bash ../scripts/test-ref test-format-format2/r2 "$VALGRIND" ./test-format-format2
rm -f test-format-format2 test-format-format2.exe
