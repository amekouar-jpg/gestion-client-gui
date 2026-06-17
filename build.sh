#!/bin/bash
cd /c/Users/B4Z1Z/Desktop/work\ school/C/projet/graphic
echo "Current directory: $(pwd)"
echo "Files:"
ls -la projet_gui.c
echo "GTK3 flags:"
pkg-config --cflags gtk+-3.0
echo "Compiling..."
gcc -Wall -Wextra $(pkg-config --cflags gtk+-3.0) -c projet_gui.c -o projet_gui.o 2>&1
if [ -f projet_gui.o ]; then
  echo "Object file created!"
  echo "Linking..."
  gcc -o contacts_app.exe projet_gui.o $(pkg-config --libs gtk+-3.0) 2>&1
  if [ -f contacts_app.exe ]; then
    echo "Build successful! Running application..."
    ./contacts_app.exe
  else
    echo "Linking failed!"
    exit 1
  fi
else
  echo "Compilation failed!"
  exit 1
fi
