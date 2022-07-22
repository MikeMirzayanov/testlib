#!/bin/bash
set -eo pipefail

bash ../scripts/compile src/gen.cpp
bash ../scripts/test-ref r1 $VALGRIND ./gen 10 100
bash ../scripts/test-ref r2 $VALGRIND ./gen 100 1000
rm -f gen gen.exe
