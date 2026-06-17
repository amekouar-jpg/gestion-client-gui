$ErrorActionPreference = "Stop"

# MSYS2 paths
$msys2 = "C:\msys64"
$mingw64Bin = "$msys2\mingw64\bin"
$gcc = "$mingw64Bin\gcc.exe"
$pkgConfig = "$mingw64Bin\pkg-config.exe"

# Project directory
$projDir = Split-Path -Parent $MyInvocation.MyCommand.Path

Write-Host "Building GTK3 Contact Manager..."
cd $projDir

# Clean
Write-Host "Cleaning..."
Remove-Item -Force -ErrorAction SilentlyContinue *.o, *.exe

# Get GTK3 flags
Write-Host "Getting GTK3 configuration..."
$cflags = & $pkgConfig --cflags gtk+-3.0
$libs = & $pkgConfig --libs gtk+-3.0

Write-Host "CFLAGS: $cflags"
Write-Host "LIBS: $libs"

# Compile
Write-Host "Compiling..."
$compileCmd = @("$gcc", "-Wall", "-Wextra") + $cflags.Split() + @("-c", "projet_gui.c", "-o", "projet_gui.o")
& @compileCmd

if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed!"
    exit 1
}

# Link
Write-Host "Linking..."
$linkCmd = @("$gcc", "-o", "contacts_app.exe", "projet_gui.o") + $libs.Split()
& @linkCmd

if ($LASTEXITCODE -ne 0) {
    Write-Host "Linking failed!"
    exit 1
}

Write-Host "Build successful!"
Write-Host "Launching application..."

# Run
& .\contacts_app.exe
