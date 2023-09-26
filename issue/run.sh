#!/bin/bash

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

echo "gcc case"
clang++ --version
clang++ -O2 -o ab-clang$EXE_EXT ab.cpp
./ab-clang$EXE_EXT < input.txt > output.clang
echo -
echo $?
echo -
cat output.clang
echo -

echo "clang case"
g++ --version
g++ --std=c++11 -Wpedantic -Werror -O2 -o ab-gcc$EXE_EXT ab.cpp
./ab-gcc$EXE_EXT < input.txt > output.gcc
echo -
echo $?
echo -
cat output.gcc
echo -

echo "case-nval gcc case"
clang++ --version
clang++ -O2 -o case-nval-clang$EXE_EXT case-nval.cpp
./case-nval-clang$EXE_EXT --testMarkupFileName a < input.01 1>out 2>err
echo -
echo $?
echo -
cat out
echo -
cat err
echo -
cat a
echo -

echo "case-nval clang case"
g++ --version
g++ --std=c++11 -Wpedantic -Werror -O2 -o case-nval-gcc$EXE_EXT case-nval.cpp
./case-nval-gcc$EXE_EXT --testMarkupFileName a < input.01 1>out 2>err
echo -
echo $?
echo -
cat out
echo -
cat err
echo -
cat a
echo -

echo "end"
