call t.bat ncmp.cpp many many many
call t.bat wcmp.cpp many many many

for /L %%i in (1 1 5) do (
    for /L %%j in (1 1 5) do (
        call t.bat ncmp.cpp ncmp.%%i ncmp.%%i ncmp.%%j
        call t.bat lcmp.cpp ncmp.%%i ncmp.%%i ncmp.%%j
        call t.bat fcmp.cpp ncmp.%%i ncmp.%%i ncmp.%%j
        call t.bat wcmp.cpp ncmp.%%i ncmp.%%i ncmp.%%j
    )
)

for /L %%i in (1 1 8) do (
    for /L %%j in (1 1 8) do (
        call t.bat wcmp.cpp wcmp.%%i wcmp.%%i wcmp.%%j
        call t.bat lcmp.cpp wcmp.%%i wcmp.%%i wcmp.%%j
        call t.bat fcmp.cpp wcmp.%%i wcmp.%%i wcmp.%%j
    )
)

call t.bat reads.cpp chars chars chars
call t.bat reads2.cpp chars chars chars
call t.bat reads2.cpp reads2.cpp reads2.cpp reads2.cpp
call t.bat reads2.cpp t.h t.h t.h

for /L %%i in (1 1 13) do (
    for /L %%j in (1 1 13) do (
        call t.bat wcmp.cpp input.%%i input.%%i input.%%j
        call t.bat fcmp.cpp input.%%i input.%%i input.%%j
        call t.bat lcmp.cpp input.%%i input.%%i input.%%j
        call t.bat ncmp.cpp input.%%i input.%%i input.%%j
        call t.bat rcmp9.cpp input.%%i input.%%i input.%%j
    )
)
