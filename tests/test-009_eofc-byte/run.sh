#!/bin/bash
set -eo pipefail

# Byte 0xFF is data, not EOF, whatever the signedness of char

bash ../scripts/compile src/eofc.cpp
bash ../scripts/test-ref r1 "$VALGRIND" ./eofc files/input.01 files/output.01 files/answer.01
rm -f eofc eofc.exe

# cl.exe has no -funsigned-char and scripts/compile does not pass CPP_OPTS to it
if [[ "$CPP" != "cl.exe" ]]; then
  CPP_OPTS="$CPP_OPTS -funsigned-char" bash ../scripts/compile src/eofc.cpp
  bash ../scripts/test-ref r1 "$VALGRIND" ./eofc files/input.01 files/output.01 files/answer.01
  rm -f eofc eofc.exe
fi
