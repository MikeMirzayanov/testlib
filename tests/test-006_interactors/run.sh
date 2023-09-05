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
rm -f output.01 output.01.nix

bash ../scripts/compile src/interactive-a-plus-b.cpp
echo "Running 'java -jar files/crossrun/CrossRun.jar ./interactor-a-plus-b files/"$os"/input.01 output.02 -- ./interactive-a-plus-b'"
java -jar files/crossrun/CrossRun.jar ./interactor-a-plus-b files/"$os"/input.01 output.02 -- ./interactive-a-plus-b
tr -d '\r' < output.02 > output.02.nix
bash ../scripts/test-ref r-interactor-a-plus-b-2-1 cat output.02.nix
rm -f output.02 output.02.nix interactive-a-plus-b interactive-a-plus-b.exe interactor-a-plus-b interactor-a-plus-b.exe interactive_runner.out interactive_runner.err
