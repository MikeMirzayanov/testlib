#!/bin/bash
set -eo pipefail

os="unix"
if [[ "$MACHINE" == "Windows" ]]; then
  os="win"
fi

# Loop through the versions v1, v2, v3, and v4
for version in v1 v2 v3 v4; do
    bash ../scripts/compile src/"$version".cpp
    
    # Loop through the test numbers 01, 02, 03, and 04
    for test_num in 01 02 03 04 05 06; do
        bash ../scripts/test-ref "$version"-"$test_num" "$VALGRIND" ./"$version" --testOverviewLogFileName stderr < files/"$os"/input."$test_num"
    done
    
    rm -f "$version" "$version".exe
done

# Byte 0xFF is data, not EOF: bytes after a valid test or inside a token must fail validation
bash ../scripts/compile src/v1.cpp
for test_num in 07 08 09; do
    bash ../scripts/test-ref v1-"$test_num" "$VALGRIND" ./v1 --testOverviewLogFileName stderr < files/"$os"/input."$test_num"
done
rm -f v1 v1.exe
