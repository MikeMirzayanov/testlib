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

bash ../scripts/compile files/test-println-regressions.cpp
bash ../scripts/test-ref test-println-regressions/r1 ./test-println-regressions
rm -f test-println-regressions test-println-regressions.exe

bash ../scripts/compile files/test-println-bitfield-policy.cpp
bash ../scripts/test-ref test-println-bitfield-policy/r1 ./test-println-bitfield-policy
rm -f test-println-bitfield-policy test-println-bitfield-policy.exe

bash ../scripts/compile files/test-format-edge-cases.cpp
bash ../scripts/test-ref test-format-edge-cases/r1 ./test-format-edge-cases
rm -f test-format-edge-cases test-format-edge-cases.exe

bash ../scripts/compile files/test-format-std-compat.cpp
bash ../scripts/test-ref test-format-std-compat/r1 ./test-format-std-compat
if [[ "$MACHINE" == "Linux" ]]; then
  bash ../scripts/test-ref test-format-invalid/r1 ./test-format-std-compat invalid
fi
rm -f test-format-std-compat test-format-std-compat.exe

if [[ "$MACHINE" == "Linux" ]]; then
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

if [[ "$MACHINE" == "Linux" ]]; then
  if CPP_OPTS="${CPP_OPTS:+$CPP_OPTS }-DTEST_PRINTLN_DIRECT_BITFIELD_NEGATIVE" bash ../scripts/compile files/test-println-bitfield-policy.cpp --check-only >println-bitfield-negative.log 2>&1; then
    echo "Direct bit-field println unexpectedly compiled"
    rm -f println-bitfield-negative.log
    exit 1
  fi
  if ! grep -q "TEST_PRINTLN_NEGATIVE_NOT_APPLICABLE" println-bitfield-negative.log &&
     ! grep -Eiq "bit[- ]?field|cannot bind.*reference|reference.*cannot bind" println-bitfield-negative.log; then
    cat println-bitfield-negative.log
    rm -f println-bitfield-negative.log
    exit 1
  fi
  rm -f println-bitfield-negative.log
fi

if [[ "$CPP" == "cl.exe" && "$CPP_STANDARD" == "-std:c++latest" ]]; then
  if ! CL="${CL:+$CL }/Zc:__cplusplus" bash ../scripts/compile files/test-format-std-compat.cpp --check-only; then
    echo "MSVC /Zc:__cplusplus compatibility probe failed"
    exit 1
  fi
fi
