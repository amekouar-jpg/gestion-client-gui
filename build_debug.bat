@echo off
cd /d "C:\Users\B4Z1Z\Desktop\work school\C\projet\graphic"
echo Attempting to build GTK3 Contact Manager > build_log.txt
echo. >> build_log.txt

REM Try using pkg-config from MSYS2
set "PKG_CONFIG=C:\msys64\ucrt64\bin\pkg-config.exe"
set "GCC=C:\msys64\ucrt64\bin\gcc.exe"

echo Checking for GCC... >> build_log.txt
if exist "%GCC%" (
    echo GCC found at %GCC% >> build_log.txt
) else (
    echo GCC NOT found! >> build_log.txt
    type build_log.txt
    exit /b 1
)

echo Cleaning old files... >> build_log.txt
del /Q *.o *.exe 2>nul

echo Getting GTK3 flags... >> build_log.txt
"%PKG_CONFIG%" --cflags gtk+-3.0 > cflags.txt 2>&1
set /p CFLAGS=<cflags.txt
echo CFLAGS: %CFLAGS% >> build_log.txt

"%PKG_CONFIG%" --libs gtk+-3.0 > libs.txt 2>&1
set /p LIBS=<libs.txt
echo LIBS: %LIBS% >> build_log.txt

echo Compiling... >> build_log.txt
"%GCC%" -Wall -Wextra %CFLAGS% -c projet_gui.c -o projet_gui.o 2>> build_log.txt
if errorlevel 1 (
    echo Compilation failed! >> build_log.txt
    type build_log.txt
    exit /b 1
)

echo Linking... >> build_log.txt
"%GCC%" -o contacts_app.exe projet_gui.o %LIBS% 2>> build_log.txt
if errorlevel 1 (
    echo Linking failed! >> build_log.txt
    type build_log.txt
    exit /b 1
)

echo Build successful! >> build_log.txt
echo Running application... >> build_log.txt
start "" contacts_app.exe
type build_log.txt
