#!/bin/bash
set -eo pipefail

bash ../scripts/compile test.cpp
bash ../scripts/test-ref r1 "$VALGRIND" ./test
rm -f test test.exe

bash ../scripts/compile test-auto-ensure-no-unused-opts.cpp
bash ../scripts/test-ref auto-ensure-no-unused-opts/r1 ./test-auto-ensure-no-unused-opts
bash ../scripts/test-ref auto-ensure-no-unused-opts/r2 ./test-auto-ensure-no-unused-opts -test-count 3
bash ../scripts/test-ref auto-ensure-no-unused-opts/r3 ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-val 1
bash ../scripts/test-ref auto-ensure-no-unused-opts/r4 ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-value 10 -max-val 20
bash ../scripts/test-ref auto-ensure-no-unused-opts/r5 ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-length 3 -min-value 10 -max-value 20
bash ../scripts/test-ref auto-ensure-no-unused-opts/r6 ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-n 3 -min-value 10 -max-value 20 -bias-value 3
bash ../scripts/test-ref auto-ensure-no-unused-opts/r7 ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10
bash ../scripts/test-ref auto-ensure-no-unused-opts/r8 ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-value 10
bash ../scripts/test-ref auto-ensure-no-unused-opts/r9 ./test-auto-ensure-no-unused-opts -test-count 5 -sum-n 100 -min-n 3 -min-value 10 -max-value 20 -value-bias -3

rm -f test-auto-ensure-no-unused-opts test-auto-ensure-no-unused-opts.exe
