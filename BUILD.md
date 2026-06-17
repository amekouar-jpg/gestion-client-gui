# Build Instructions

This document provides detailed instructions for building the GTK3 Contact Manager application on different platforms.

## Quick Start

### Linux (Most Common)
```bash
cd projet
make clean
make
./contacts_app
```

### macOS
```bash
cd projet
make clean
make
./contacts_app
```

### Windows (MSYS2)
```bash
cd projet
build.bat
contacts_app.exe
```

---

## Detailed Setup by Platform

## 1. LINUX (Ubuntu/Debian)

### Step 1: Install Dependencies
```bash
sudo apt-get update
sudo apt-get install -y build-essential
sudo apt-get install -y libgtk-3-dev
sudo apt-get install -y pkg-config
```

### Step 2: Verify Installation
```bash
pkg-config --modversion gtk+-3.0
gcc --version
make --version
```

### Step 3: Build
```bash
cd /path/to/projet
make clean
make
```

### Step 4: Run
```bash
./contacts_app
```

---

## 2. LINUX (Fedora/RHEL/CentOS)

### Step 1: Install Dependencies
```bash
sudo dnf install -y gcc
sudo dnf install -y gtk3-devel
sudo dnf install -y pkg-config
sudo dnf install -y make
```

### Step 2: Verify Installation
```bash
pkg-config --modversion gtk+-3.0
gcc --version
```

### Step 3: Build
```bash
cd /path/to/projet
make clean
make
```

### Step 4: Run
```bash
./contacts_app
```

---

## 3. macOS (Intel/Apple Silicon)

### Step 1: Install Homebrew (if not installed)
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

### Step 2: Install Dependencies
```bash
brew install gtk+3
brew install pkg-config
```

### Step 3: Verify Installation
```bash
pkg-config --modversion gtk+-3.0
gcc --version
```

### Step 4: Build
```bash
cd /path/to/projet
make clean
make
```

### Step 5: Run
```bash
./contacts_app
```

### Note for Apple Silicon Macs
If you encounter architecture issues:
```bash
arch -arm64 brew install gtk+3
```

---

## 4. Windows (MSYS2 / MinGW64)

### Step 1: Install MSYS2
1. Download from: https://www.msys2.org/
2. Run installer (e.g., `msys2-x86_64-20230526.exe`)
3. Follow installation wizard (default location is fine)
4. Launch MSYS2 MINGW64 terminal (NOT "MSYS2 MSYS")

### Step 2: Install Build Tools
In MSYS2 MINGW64 terminal:
```bash
pacman -Syu  # System update
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-gtk3
pacman -S mingw-w64-x86_64-pkg-config
pacman -S mingw-w64-x86_64-make
```

### Step 3: Verify Installation
```bash
pkg-config --modversion gtk+-3.0
gcc --version
make --version
```

### Step 4: Build (Method A - Using Makefile)
```bash
cd /c/path/to/projet  # Note: /c/ for C: drive
make clean
make
```

### Step 5: Build (Method B - Using build.bat)
If Makefile doesn't work:
1. Open CMD (not MSYS2)
2. Navigate to project directory
3. Run: `build.bat`

### Step 6: Run
```bash
./contacts_app
# or
contacts_app.exe
```

### Troubleshooting MSYS2
- If pkg-config not found: `pacman -S pkg-config`
- If gcc not found: Ensure MINGW64 terminal (not MSYS)
- Update all packages: `pacman -Syuu`

---

## 5. Windows (Using CMake)

### Prerequisites
- Install Visual Studio Build Tools or MinGW
- Install CMake: https://cmake.org/download/
- Install GTK3 (via vcpkg or pre-built binaries)

### Build Steps
```bash
cd projet
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

---

## CMake Build (All Platforms)

### Step 1: Install CMake
- Linux: `sudo apt-get install cmake` (or `dnf install cmake`)
- macOS: `brew install cmake`
- Windows: Download from https://cmake.org/download/

### Step 2: Build
```bash
cd /path/to/projet
mkdir -p build
cd build
cmake ..
make
```

### Step 3: Run
```bash
./contacts_app
```

---

## Manual Compilation (If Makefiles Fail)

### Linux/macOS
```bash
gcc -Wall -Wextra \
    $(pkg-config --cflags gtk+-3.0) \
    -o contacts_app projet_gui.c \
    $(pkg-config --libs gtk+-3.0)

./contacts_app
```

### Windows (MSYS2)
```bash
gcc -Wall -Wextra \
    $(pkg-config --cflags gtk+-3.0) \
    -o contacts_app.exe projet_gui.c \
    $(pkg-config --libs gtk+-3.0)

./contacts_app.exe
```

---

## Verification

After successful build, you should see:
```
✓ Compilation complete
✓ Build successful: contacts_app
```

Run the application:
```bash
./contacts_app
```

A window titled "Gestion des Contacts" should appear with:
- Input form on the left
- Contacts table on the right
- Toolbar at the top

---

## Common Build Errors & Solutions

### Error: "gtk/gtk.h: No such file or directory"
**Solution**: Install GTK3 development files
```bash
# Ubuntu/Debian
sudo apt-get install libgtk-3-dev

# Fedora/RHEL
sudo dnf install gtk3-devel

# macOS
brew install gtk+3
```

### Error: "pkg-config: command not found"
**Solution**: Install pkg-config
```bash
# Ubuntu/Debian
sudo apt-get install pkg-config

# Fedora/RHEL
sudo dnf install pkg-config

# macOS
brew install pkg-config
```

### Error: "undefined reference to 'gtk_*' functions"
**Solution**: Ensure GTK3 libraries are linked
```bash
make clean
make
```

Or manually include the GTK3 flags:
```bash
gcc -Wall -Wextra $(pkg-config --cflags gtk+-3.0) \
    -o contacts_app projet_gui.c $(pkg-config --libs gtk+-3.0)
```

### Error: "ld: library not found for -lgtk-3" (macOS)
**Solution**: Reinstall GTK3
```bash
brew uninstall gtk+3
brew install gtk+3
brew link gtk+3 --force
```

### Error on Windows: "The system cannot find the file specified"
**Solution**: Ensure you're in the correct directory containing `projet_gui.c`
```bash
cd /path/to/projet
dir  # List files to verify projet_gui.c exists
build.bat
```

---

## Building for Different Architectures

### 32-bit on 64-bit Linux
```bash
sudo apt-get install libgtk-3-dev:i386
gcc -m32 -o contacts_app projet_gui.c $(pkg-config --libs gtk+-3.0)
```

### Cross-compilation (Advanced)
For embedded systems or different architectures, use:
```bash
arm-linux-gnueabihf-gcc -o contacts_app projet_gui.c \
    $(arm-linux-gnueabihf-pkg-config --libs gtk+-3.0)
```

---

## Performance Optimization Flags

For optimized build:
```bash
CFLAGS="-O2 -march=native" make clean
make
```

Or edit Makefile:
```makefile
CFLAGS = -O2 -march=native -Wall -Wextra
```

---

## Static Linking (Advanced)

To create a standalone executable without external dependencies:
```bash
gcc -static-libgcc $(pkg-config --cflags gtk+-3.0) \
    -o contacts_app projet_gui.c $(pkg-config --libs gtk+-3.0)
```

**Note**: Full static linking with GTK3 is complex and not recommended for beginners.

---

## Troubleshooting Steps

1. **Clean previous builds**: `make clean`
2. **Update packages**: `sudo apt-get update && sudo apt-get upgrade`
3. **Reinstall GTK3**: Remove and reinstall the development files
4. **Check pkg-config**: Run `pkg-config --list-all | grep gtk`
5. **Verify gcc**: Run `gcc -v`
6. **Check symlinks** (macOS): `brew link gtk+3 --force`

---

## Next Steps

After successful build:
1. Run the application: `./contacts_app`
2. Add some test contacts
3. Test all features (add, modify, delete, search)
4. Save to binary and text files
5. Load the files back to verify persistence

For detailed usage instructions, see [README.md](README.md)
