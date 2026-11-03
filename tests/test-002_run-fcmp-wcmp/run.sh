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

# Byte 0xFF is data, not EOF, even when char is unsigned (e.g. aarch64 Linux)
if [[ "$CPP" == "cl.exe" ]]; then
  bash ../scripts/compile src/wcmp.cpp
  bash ../scripts/compile src/fcmp.cpp
else
  CPP_OPTS="$CPP_OPTS -funsigned-char" bash ../scripts/compile src/wcmp.cpp
  CPP_OPTS="$CPP_OPTS -funsigned-char" bash ../scripts/compile src/fcmp.cpp
fi
bash ../scripts/test-ref r7 "$VALGRIND" ./wcmp files/input.01 files/output.01.ff1 files/answer.01
bash ../scripts/test-ref r8 "$VALGRIND" ./wcmp files/input.01 files/output.01.ff2 files/answer.01
bash ../scripts/test-ref r9 "$VALGRIND" ./fcmp files/input.01 files/output.01.ff1 files/answer.01
rm -f wcmp wcmp.exe fcmp fcmp.exe
