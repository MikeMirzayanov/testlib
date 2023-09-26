echo gcc case
clang++ --version
clang++ -O2 -o ab-clang.exe ab.cpp
ab-clang.exe < input.txt > output.clang
echo -
echo %ERRORLEVEL%
echo -
type output.clang
echo -

echo clang case
g++ --version
g++ -O2 -o ab-gcc.exe ab.cpp
ab-gcc.exe < input.txt > output.gcc
echo -
echo %ERRORLEVEL%
echo -
type output.gcc
echo -

echo end
