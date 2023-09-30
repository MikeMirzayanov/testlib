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
