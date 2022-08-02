#!/bin/bash
set -eo pipefail

os="unix"
if [[ "$MACHINE" == "Windows" ]]; then
  os="win"
fi

bash ../scripts/compile src/sval.cpp
for i in files/"$os"/sval/*; do
  j=$(basename $i)
  echo "test-ref r-sval-$j ""$VALGRIND"" ./sval < files/$os/sval/input.$i"
  bash ../scripts/test-ref r-sval-$j "$VALGRIND" ./sval <$i
done
rm -f sval sval.exe

bash ../scripts/compile src/case-nval.cpp
for i in files/"$os"/case-nval/*; do
  j=$(basename $i)
  echo "test-ref r-case-nval-$j ""$VALGRIND"" ./case-nval --testMarkupFileName stderr < files/$os/case-nval/input.$i"
  bash ../scripts/test-ref r-case-nval-$j "$VALGRIND" ./case-nval --testMarkupFileName stderr <$i
  for t in 1 2; do
    echo "test-ref r-case-nval-$j-$t ""$VALGRIND"" ./case-nval --testMarkupFileName stderr --dumpTestCase "${t}" < files/$os/case-nval/input.$i"
    bash ../scripts/test-ref r-case-nval-$j-$t "$VALGRIND" ./case-nval --testMarkupFileName stderr --dumpTestCase "${t}" <$i
  done
  for t in 3 4; do
    echo "test-ref r-case-nval-$j-$t ""$VALGRIND"" ./case-nval --dumpTestCase "${t}" < files/$os/case-nval/input.$i"
    bash ../scripts/test-ref r-case-nval-$j-$t "$VALGRIND" ./case-nval --dumpTestCase "${t}" <$i
  done
done
rm -f case-nval case-nval.exe
