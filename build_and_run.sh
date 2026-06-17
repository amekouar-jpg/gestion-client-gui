#!/bin/bash
set -e

echo "Building GTK3 Contact Manager..."
cd "$(dirname "$0")"

# Clean old files
rm -f *.o contacts_app.exe

# Compile
echo "Compiling..."
gcc -Wall -Wextra $(pkg-config --cflags gtk+-3.0) -c projet_gui.c -o projet_gui.o

# Link
echo "Linking..."
gcc -o contacts_app.exe projet_gui.o $(pkg-config --libs gtk+-3.0)

echo "✓ Build successful!"
echo "Running application..."

# Run
./contacts_app.exe
