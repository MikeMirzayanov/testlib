#!/bin/bash
set -eo pipefail

bash ../scripts/compile src/wcmp.cpp
bash ../scripts/test-ref r1 "$VALGRIND" ./wcmp files/input.01 files/output.01 files/answer.01
bash ../scripts/test-ref r2 "$VALGRIND" ./wcmp files/input.01 files/output.01 files/output.01
bash ../scripts/test-ref r3 "$VALGRIND" ./wcmp files/input.01 files/output.01 files/answer2.01

# BOM shouldn't change refs
bash ../scripts/test-ref r1 "$VALGRIND" ./wcmp files/input.01 files/output.01.bom files/answer.01
bash ../scripts/test-ref r2 "$VALGRIND" ./wcmp files/input.01 files/output.01.bom files/output.01
bash ../scripts/test-ref r3 "$VALGRIND" ./wcmp files/input.01 files/output.01.bom files/answer2.01
rm -f wcmp wcmp.exe

bash ../scripts/compile src/fcmp.cpp
bash ../scripts/test-ref r4 "$VALGRIND" ./fcmp files/input.01 files/output.01 files/answer.01
bash ../scripts/test-ref r5 "$VALGRIND" ./fcmp files/input.01 files/output.01 files/output.01
bash ../scripts/test-ref r6 "$VALGRIND" ./fcmp files/input.01 files/output.01 files/answer2.01
rm -f fcmp fcmp.exe
