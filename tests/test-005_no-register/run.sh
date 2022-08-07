#!/bin/bash
set -eo pipefail

bash ../scripts/compile src/g.cpp
bash ../scripts/test-ref r1 "$VALGRIND" ./g
bash ../scripts/test-ref r2 "$VALGRIND" ./g 100
rm -f g g.exe
