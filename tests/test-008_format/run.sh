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

bash ../scripts/compile files/test-format-std-compat.cpp
bash ../scripts/test-ref test-format-std-compat/r1 ./test-format-std-compat
if [[ "$MACHINE" != "Windows" ]]; then
  bash ../scripts/test-ref test-format-invalid/r1 ./test-format-std-compat invalid
fi
rm -f test-format-std-compat test-format-std-compat.exe

if [[ "$MACHINE" != "Windows" ]]; then
  if bash ../scripts/compile files/test-format-random-hidden.cpp --check-only >random-hidden.log 2>&1; then
    echo "Compilation unexpectedly exposed ::random()"
    rm -f random-hidden.log
    exit 1
  fi
  if ! grep -Eiq "random.*(not.*declared|undeclared|no member)|no member.*random" random-hidden.log; then
    cat random-hidden.log
    rm -f random-hidden.log
    exit 1
  fi
  rm -f random-hidden.log
fi
