#!/bin/bash
set -eo pipefail

# readLine(): a CR not followed by LF is skipped, a CR right before EOF adds no byte to the line

bash ../scripts/compile src/lines.cpp
bash ../scripts/test-ref r1 "$VALGRIND" ./lines files/input.01 files/output.01 files/answer.01
bash ../scripts/test-ref r2 "$VALGRIND" ./lines files/input.01 files/output.02 files/answer.02
bash ../scripts/test-ref r3 "$VALGRIND" ./lines files/input.01 files/output.01 files/answer.03
rm -f lines lines.exe
