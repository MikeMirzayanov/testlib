#!/bin/bash
set -eo pipefail

# Determine platform
PLATFORM="unknown"
unamestr=$(uname)
if [[ "$unamestr" == "Linux" ]]; then
   PLATFORM="linux"
elif [[ "$unamestr" == "MINGW"* || "$unamestr" == "CYGWIN"* || "$unamestr" == "MSYS"* ]]; then
   PLATFORM="windows"
fi

echo "$PLATFORM"

EXE_EXT=""
if [[ "$PLATFORM" == "windows" ]]; then
    EXE_EXT=".exe"
fi

echo "clang case"
clang++ --version
rm -f ab-clang$EXE_EXT
clang++ -O2 -o ab-clang$EXE_EXT ab.cpp
cp -f input.txt i
./invoke.sh ./ab-clang
rm -f ab-clang$EXE_EXT

echo "gcc case"
g++ --version
rm -f ab-gcc$EXE_EXT
#eval g++ --std=c++11 -Wpedantic -Werror -O2 -o ab-gcc$EXE_EXT ab.cpp
eval g++ -O0 -o ab-gcc$EXE_EXT ab.cpp
cp -f input.txt i
./invoke.sh ./ab-gcc
rm -f ab-gcc$EXE_EXT

echo "case-nval clang case"
clang++ --version
rm -f case-nval-clang$EXE_EXT
clang++ -O2 -o case-nval-clang$EXE_EXT case-nval.cpp
cp -f input.01 i
./invoke.sh ./case-nval-clang --testMarkupFileName a
cat a
echo -
rm -f case-nval-clang$EXE_EXT

echo "case-nval gcc case"
g++ --version
rm -f case-nval-gcc$EXE_EXT
#eval g++ --std=c++11 -Wpedantic -Werror -O2 -o case-nval-gcc$EXE_EXT case-nval.cpp
eval g++ --std=c++11 -O1 -o case-nval-gcc$EXE_EXT case-nval.cpp
cp -f input.01 i
./invoke.sh ./case-nval-gcc --testMarkupFileName a
cat a
echo -
rm -f case-nval-gcc$EXE_EXT

echo "end"
