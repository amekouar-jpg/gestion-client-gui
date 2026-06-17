@echo off
REM Build script for Windows (MSYS2/MinGW)
REM Usage: build.bat

echo Cleaning old files...
del /Q contacts_app.exe projet_gui.o 2>nul

echo Compiling GTK3 Contact Manager...
gcc -Wall -Wextra -c projet_gui.c -o projet_gui.o -I"C:\msys64\mingw64\include\gtk-3.0" -I"C:\msys64\mingw64\include\glib-2.0" -I"C:\msys64\mingw64\lib\glib-2.0\include" -I"C:\msys64\mingw64\include\cairo" -I"C:\msys64\mingw64\include\pango-1.0" -I"C:\msys64\mingw64\include\atk-1.0" -I"C:\msys64\mingw64\include\gio-unix-2.0"

if errorlevel 1 (
    echo Compilation failed!
    exit /b 1
)

echo Linking...
gcc -o contacts_app.exe projet_gui.o -L"C:\msys64\mingw64\lib" -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lglib-2.0 -gobject-2.0

if errorlevel 1 (
    echo Linking failed!
    exit /b 1
)

echo.
echo Build successful! Run: contacts_app.exe
pause
