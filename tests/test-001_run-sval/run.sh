#!/bin/bash
set -eo pipefail

os="unix"
if [[ "$MACHINE" == "Windows" ]]; then
  os="win"
fi

bash ../scripts/compile src/sval.cpp
for i in files/"$os"/sval/*; do
  j=$(basename $i)
  echo "test-ref r-sval-$i ""$VALGRIND"" ./sval < files/$os/sval/input.$i"
  bash ../scripts/test-ref r-sval-$j "$VALGRIND" ./sval <$i
done
rm -f sval sval.exe
