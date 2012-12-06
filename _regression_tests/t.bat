set FILE=%~n1
set DIR=%RANDOM%

md %DIR%
pushd %DIR%

md old
md new

copy ..\files\%1 old\%1
copy ..\files\%1 new\%1

if not "%2"=="" copy ..\files\%2 %2
if not "%3"=="" copy ..\files\%3 %3
if not "%4"=="" copy ..\files\%4 %4

copy ..\old\testlib.h old\testlib.h
if not exist ..\old-bin md ..\old-bin
if exist ..\old-bin\%FILE%.exe (
    echo copying
    copy ..\old-bin\%FILE%.exe old\%FILE%.exe
) else (
    echo compiling
    pushd old
    g++ -O2 -Wall -o %FILE% %1
    copy %FILE%.exe ..\..\old-bin\%FILE%.exe
    popd
)
if not exist old\%FILE%.exe pause

old\%FILE%.exe %2 %3 %4 1>old.output 2>old.error
set OLD_ERRORLEVEL=%ERRORLEVEL%

copy ..\new\testlib.h new\testlib.h
if not exist ..\new-bin md ..\new-bin
if exist ..\new-bin\%FILE%.exe (
    echo copying
    copy ..\new-bin\%FILE%.exe new\%FILE%.exe
) else (
    echo compiling
    pushd new
    g++ -O2 -Wall -o %FILE% %1
    copy %FILE%.exe ..\..\new-bin\%FILE%.exe
    popd
)
if not exist new\%FILE%.exe pause

new\%FILE%.exe %2 %3 %4 1>new.output 2>new.error
set NEW_ERRORLEVEL=%ERRORLEVEL%

fc new.error old.error
if errorlevel 1 pause
fc new.output old.output
if errorlevel 1 pause
if not "%OLD_ERRORLEVEL%"=="%NEW_ERRORLEVEL%" pause

popd
rd /S /Q %DIR%