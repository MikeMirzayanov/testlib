#!/bin/bash
set -eo pipefail

bash ../scripts/compile files/test-auto-ensure-no-unused-opts.cpp
bash ../scripts/test-ref auto-ensure-no-unused-opts/r1 "$VALGRIND" ./test-auto-ensure-no-unused-opts
bash ../scripts/test-ref auto-ensure-no-unused-opts/r2 "$VALGRIND" ./test-auto-ensure-no-unused-opts -test-count 3
bash ../scripts/test-ref auto-ensure-no-unused-opts/r3 "$VALGRIND" ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-val 1
bash ../scripts/test-ref auto-ensure-no-unused-opts/r4 "$VALGRIND" ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-value 10 -max-val 20
bash ../scripts/test-ref auto-ensure-no-unused-opts/r5 "$VALGRIND" ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-length 3 -min-value 10 -max-value 20
bash ../scripts/test-ref auto-ensure-no-unused-opts/r6 "$VALGRIND" ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-n 3 -min-value 10 -max-value 20 -bias-value 3
bash ../scripts/test-ref auto-ensure-no-unused-opts/r7 "$VALGRIND" ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10
bash ../scripts/test-ref auto-ensure-no-unused-opts/r8 "$VALGRIND" ./test-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-value 10
bash ../scripts/test-ref auto-ensure-no-unused-opts/r9 "$VALGRIND" ./test-auto-ensure-no-unused-opts -test-count 5 -sum-n 100 -min-n 3 -min-value 10 -max-value 20 -value-bias -3
rm -f test-auto-ensure-no-unused-opts test-auto-ensure-no-unused-opts.exe

bash ../scripts/compile files/test-suppress-auto-ensure-no-unused-opts.cpp
bash ../scripts/test-ref suppress-auto-ensure-no-unused-opts/r1 "$VALGRIND" ./test-suppress-auto-ensure-no-unused-opts
bash ../scripts/test-ref suppress-auto-ensure-no-unused-opts/r2 "$VALGRIND" ./test-suppress-auto-ensure-no-unused-opts -test-count 3
bash ../scripts/test-ref suppress-auto-ensure-no-unused-opts/r3 "$VALGRIND" ./test-suppress-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-val 1
bash ../scripts/test-ref suppress-auto-ensure-no-unused-opts/r4 "$VALGRIND" ./test-suppress-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-value 10 -max-val 20
bash ../scripts/test-ref suppress-auto-ensure-no-unused-opts/r5 "$VALGRIND" ./test-suppress-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-length 3 -min-value 10 -max-value 20
bash ../scripts/test-ref suppress-auto-ensure-no-unused-opts/r6 "$VALGRIND" ./test-suppress-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-n 3 -min-value 10 -max-value 20 -bias-value 3
bash ../scripts/test-ref suppress-auto-ensure-no-unused-opts/r7 "$VALGRIND" ./test-suppress-auto-ensure-no-unused-opts -test-count 3 -sum-n 10
bash ../scripts/test-ref suppress-auto-ensure-no-unused-opts/r8 "$VALGRIND" ./test-suppress-auto-ensure-no-unused-opts -test-count 3 -sum-n 10 -min-value 10
bash ../scripts/test-ref suppress-auto-ensure-no-unused-opts/r9 "$VALGRIND" ./test-suppress-auto-ensure-no-unused-opts -test-count 5 -sum-n 100 -min-n 3 -min-value 10 -max-value 20 -value-bias -3
rm -f test-suppress-auto-ensure-no-unused-opts test-suppress-auto-ensure-no-unused-opts.exe
