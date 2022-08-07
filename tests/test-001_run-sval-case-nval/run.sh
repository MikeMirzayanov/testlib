#!/bin/bash
set -eo pipefail

os="unix"
if [[ "$MACHINE" == "Windows" ]]; then
  os="win"
fi

bash ../scripts/compile src/case-nval.cpp
for i in files/"$os"/case-nval/*; do
  j=$(basename $i)
  bash ../scripts/test-ref r-case-nval-$j "$VALGRIND" ./case-nval --testMarkupFileName stderr <$i
  for t in 1 2; do
    bash ../scripts/test-ref r-case-nval-$j-$t "$VALGRIND" ./case-nval --testMarkupFileName stderr --testCase "${t}" <$i
  done
  bash ../scripts/test-ref r-case-nval-$j-3 "$VALGRIND" ./case-nval --testCase "3" <$i
  bash ../scripts/test-ref r-case-nval-$j-3-stderr "$VALGRIND" ./case-nval --testCase "3" --testCaseFileName stderr <$i
  bash ../scripts/test-ref r-case-nval-$j-4 "$VALGRIND" ./case-nval --testCase "4" --testCaseFileName stderr <$i
done
rm -f case-nval case-nval.exe

bash ../scripts/compile src/case-string-zb.cpp
for i in files/"$os"/case-string-zb/*; do
  j=$(basename $i)
  bash ../scripts/test-ref r-case-string-zb-$j "$VALGRIND" ./case-string-zb --testMarkupFileName stderr <$i
  for t in 1 2; do
    bash ../scripts/test-ref r-case-string-zb-$j-$t "$VALGRIND" ./case-string-zb --testMarkupFileName stderr --testCase "${t}" <$i
  done
  bash ../scripts/test-ref r-case-string-zb-$j-3 "$VALGRIND" ./case-string-zb --testCase "3" <$i
  bash ../scripts/test-ref r-case-string-zb-$j-3-stderr "$VALGRIND" ./case-string-zb --testCase "3" --testCaseFileName stderr <$i
  bash ../scripts/test-ref r-case-string-zb-$j-4 "$VALGRIND" ./case-string-zb --testCase "4" --testCaseFileName stderr <$i
done
rm -f case-string-zb case-string-zb.exe

bash ../scripts/compile src/sval.cpp
for i in files/"$os"/sval/*; do
  j=$(basename $i)
  echo "test-ref r-sval-$j ""$VALGRIND"" ./sval < files/$os/sval/input.$i"
  bash ../scripts/test-ref r-sval-$j "$VALGRIND" ./sval <$i
done
rm -f sval sval.exe
