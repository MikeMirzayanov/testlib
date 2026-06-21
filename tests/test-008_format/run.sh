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

bash ../scripts/compile files/test-format-std-syntax-println.cpp
bash ../scripts/test-ref std-format-syntax-println/braces "$VALGRIND" ./test-format-std-syntax-println braces
bash ../scripts/test-ref std-format-syntax-println/indexed "$VALGRIND" ./test-format-std-syntax-println indexed
bash ../scripts/test-ref std-format-syntax-println/spec "$VALGRIND" ./test-format-std-syntax-println spec
bash ../scripts/test-ref std-format-syntax-println/escaped "$VALGRIND" ./test-format-std-syntax-println escaped
bash ../scripts/test-ref std-format-syntax-println/no-args "$VALGRIND" ./test-format-std-syntax-println no-args
bash ../scripts/test-ref std-format-syntax-println/suppress "$VALGRIND" ./test-format-std-syntax-println suppress
rm -f test-format-std-syntax-println test-format-std-syntax-println.exe

if bash ../scripts/compile files/test-format-has-format-fix.cpp --check-only >/dev/null 2>&1; then
  bash ../scripts/compile files/test-format-std-syntax-format.cpp
  bash ../scripts/test-ref std-format-syntax-format/braces "$VALGRIND" ./test-format-std-syntax-format braces
  bash ../scripts/test-ref std-format-syntax-format/suppress "$VALGRIND" ./test-format-std-syntax-format suppress
  rm -f test-format-std-syntax-format test-format-std-syntax-format.exe
fi
