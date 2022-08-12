#!/bin/bash
set -eo pipefail

os="unix"
if [[ "$MACHINE" == "Windows" ]]; then
  os="win"
fi

bash ../scripts/compile src/interactor-a-plus-b.cpp
bash ../scripts/test-ref r-interactor-a-plus-b-1-1 "$VALGRIND" ./interactor-a-plus-b files/"$os"/input.01 output.01 < files/"$os"/participant.01
tr -d '\r' < output.01 > output.01.nix
bash ../scripts/test-ref r-interactor-a-plus-b-1-2 cat output.01.nix
rm -f interactor-a-plus-b interactor-a-plus-b.exe output.01 output.01.nix
