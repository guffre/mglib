@echo off
setlocal enabledelayedexpansion

set SRC_DIR=src\src
set INCLUDE_DIR=src\include
set OUT_DIR=build

REM === Helper: build function ===
:build
set BUILD_TYPE=%1

if /I "%BUILD_TYPE%"=="release" (
    set OPT_FLAGS=/O2
    set DEBUG_FLAGS=
    set STATIC_LIB=mglib.lib
    set DLL_NAME=mglib.dll
) else (
    REM Default to debug
    set BUILD_TYPE=debug
    set OPT_FLAGS=/Od
    set DEBUG_FLAGS=/Zi /DDEBUG
    set STATIC_LIB=mglib_dbg.lib
    set DLL_NAME=mglib_dbg.dll
)

if not exist "%OUT_DIR%" mkdir "%OUT_DIR%"

echo.
echo Usage: "build.bat release" to build release, otherwise builds debug
echo ===== Building %BUILD_TYPE% =====

REM Compile all .c files
set OBJ_LIST=
for %%f in (%SRC_DIR%\*.c) do (
    echo Compiling %%~nxf
    cl /c /nologo %OPT_FLAGS% %DEBUG_FLAGS% /I %INCLUDE_DIR% %%f /Fo"%OUT_DIR%\%%~nf.obj"
    if errorlevel 1 (
        echo Compilation failed.
        exit /b 1
    )
    set OBJ_LIST=!OBJ_LIST! "%OUT_DIR%\%%~nf.obj"
)

REM Build static lib
echo Creating static library %STATIC_LIB%
lib /nologo /OUT:"%OUT_DIR%\%STATIC_LIB%" %OBJ_LIST%
if errorlevel 1 (
    echo Static library creation failed.
    exit /b 1
)

REM Build DLL without import lib
echo Creating DLL %DLL_NAME%
cl /LD /nologo %OPT_FLAGS% %DEBUG_FLAGS% /I %INCLUDE_DIR% /D MGLIB_EXPORTS %OBJ_LIST% /Fe:"%OUT_DIR%\%DLL_NAME%" /link /NOIMPLIB /PDB:"%OUT_DIR%\%DLL_NAME:.dll=.pdb%"
if errorlevel 1 (
    echo DLL creation failed.
    exit /b 1
)

REM Clean up intermediate files
del "%OUT_DIR%\*.obj" >nul 2>&1
del "%OUT_DIR%\*.ilk" >nul 2>&1

echo Build complete.
goto :eof

REM ========================

REM Parse input arg (release/debug)
set BUILD_TYPE_ARG=%1

if /I "%BUILD_TYPE_ARG%"=="release" (
    call :build release
    goto :end
) else if /I "%BUILD_TYPE_ARG%"=="debug" (
    call :build debug
    goto :end
)

REM No argument = default to debug only
call :build debug

:end
endlocal