# Contacts Manager - GTK3 GUI Application

A complete desktop GUI application for managing a contact database, built with **GTK 3** and **C**. This application provides a modern, user-friendly interface to add, modify, search, and manage contacts with binary and text file persistence.

## Features

### Core Functionality
✅ **Add Contacts**: Create new contact entries with name, surname, and age  
✅ **Modify Contacts**: Update existing contact information directly from the GUI  
✅ **Delete Contacts**: Remove contacts with confirmation dialogs  
✅ **Search Contacts**: Search by name or surname with instant results  
✅ **List All**: Display all contacts in a sortable table view  

### Data Persistence
✅ **Binary File Format**: Save/load contacts in efficient binary format  
✅ **Text File Format**: Save/load contacts in human-readable text format  
✅ **Dynamic Memory**: Automatic memory allocation/deallocation using realloc()  
✅ **No Memory Leaks**: Careful resource management and cleanup  

### User Interface
- **Modern GTK3 Design**: Clean, professional interface
- **Split Layout**: Form panel on left, contacts table on right
- **Toolbar**: Quick access to search and file operations
- **TreeView Table**: Sortable columns (ID, Name, Surname, Age)
- **Dialog Boxes**: User-friendly alerts, errors, and confirmations
- **Responsive Widgets**: Form inputs clear automatically after operations

## Installation

### Prerequisites

#### On Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install libgtk-3-dev
sudo apt-get install pkg-config
```

#### On Linux (Fedora/RHEL)
```bash
sudo dnf install gcc
sudo dnf install gtk3-devel
sudo dnf install pkg-config
```

#### On macOS
```bash
brew install gtk+3
brew install pkg-config
```

#### On Windows (MinGW)
- Install MSYS2: https://www.msys2.org/
- In MSYS2 terminal:
```bash
pacman -S mingw-w64-x86_64-gtk3
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-make
pacman -S mingw-w64-x86_64-pkg-config
```

### Compilation

#### Using Makefile (Recommended)
```bash
cd project_directory
make clean
make
./contacts_app
```

#### Using CMake
```bash
cd project_directory
mkdir build
cd build
cmake ..
make
./contacts_app
```

#### Manual Compilation
```bash
gcc -Wall -Wextra $(pkg-config --cflags gtk+-3.0) \
    -o contacts_app projet_gui.c \
    $(pkg-config --libs gtk+-3.0)
```

## File Structure

```
projet/
├── projet_gui.c            # Main GUI application (complete)
├── Makefile                # Build configuration
├── CMakeLists.txt          # CMake configuration
├── README.md               # This file
├── pers.bin                # Binary data file (created at runtime)
└── pers.txt                # Text data file (created at runtime)
```

## User Guide

### Main Window Layout

```
┌─────────────────────────────────────────────────────────────┐
│ [Search] [🔍 Search] │ [Save Bin] [Load Bin] [Save Text] [Load Text] │
├──────────────────────┬──────────────────────────────────────┤
│  Formulaire          │      Contacts Table                  │
│  ┌────────────────┐  │  ┌──────────────────────────────┐   │
│  │ Nom: [______]  │  │  │ #│ Nom   │ Prénom  │ Âge    │   │
│  │ Prénom: [____] │  │  ├──┼───────┼─────────┼────────┤   │
│  │ Âge: [______]  │  │  │1 │ Dupont│ Jean    │ 35     │   │
│  │                │  │  │2 │ Martin│ Sophie  │ 28     │   │
│  │ [Add] [Modify] │  │  │  │       │         │        │   │
│  │     [Delete]   │  │  └──┴───────┴─────────┴────────┘   │
│  └────────────────┘  │                                      │
└──────────────────────┴──────────────────────────────────────┘
│ Ready
└─────────────────────────────────────────────────────────────┘
```

### Operations

#### Adding a Contact
1. Fill in the **Nom** (surname), **Prénom** (first name), and **Âge** (age) fields
2. Click **✏️ Ajouter** button
3. Confirmation dialog will appear
4. Contact appears in the table
5. Form clears automatically

#### Modifying a Contact
1. **Click on a row** in the contacts table to select it
2. Form fields populate automatically
3. Edit the values as needed
4. Click **✏️ Modifier** button
5. Changes are saved and reflected in the table

#### Deleting a Contact
1. **Click on a row** in the contacts table to select it
2. Click **🗑️ Supprimer** button
3. Confirm deletion in the dialog
4. Contact is removed from the list

#### Searching
1. Enter search term in the **Search** field (by name or surname)
2. Click **🔍 Rechercher** button
3. If found, contact details appear in the form
4. If not found, an error message displays

#### Saving Data

**Binary Format** (efficient, compact):
- Click **💾 Sauv. Bin** to save as `pers.bin`
- File is suitable for backup and quick loading

**Text Format** (human-readable):
- Click **💾 Sauv. Texte** to save as `pers.txt`
- File can be edited manually or imported elsewhere

#### Loading Data

**From Binary**:
- Click **📂 Charger Bin** to load from `pers.bin`
- Replaces current data with saved contacts

**From Text**:
- Click **📂 Charger Texte** to load from `pers.txt`
- Replaces current data with saved contacts

## Code Architecture

### Key Components

#### Data Structure
```c
typedef struct {
    char nom[MAX];      // Surname (40 chars max)
    char prenom[MAX];   // First name (40 chars max)
    int age;            // Age
} Personne;
```

#### Application Context
```c
typedef struct {
    /* UI Widgets */
    GtkWidget *window, *treeview, *liststore;
    GtkWidget *entry_nom, *entry_prenom, *entry_age;
    GtkWidget *btn_ajouter, *btn_modifier, *btn_supprimer;
    GtkWidget *entry_search;
    
    /* Data */
    Personne *data;     // Dynamic array
    int taille;         // Current count
    int selected_index; // Selected row (-1 if none)
} AppContext;
```

### Memory Management

**Safe Reallocation Pattern**:
```c
Personne *tmp = (Personne *)realloc(app->data, new_size);
if (tmp == NULL) {
    show_error_dialog(app->window, "Error", "Memory error");
    return;
}
app->data = tmp;
```

**Cleanup**:
- Allocated memory freed on application exit
- Reallocation handled for add/delete operations
- NULL checks on all memory operations

### UI Functions

| Function | Purpose |
|----------|---------|
| `create_main_window()` | Main window setup |
| `create_toolbar()` | Search and file operation buttons |
| `create_form_panel()` | Input form with buttons |
| `create_treeview()` | Contacts table display |
| `refresh_treeview()` | Update table from data |
| `show_*_dialog()` | User feedback (info/error/confirm) |

### Signal Handlers

| Handler | Triggered By |
|---------|--------------|
| `on_ajouter_clicked()` | Add button |
| `on_modifier_clicked()` | Modify button |
| `on_supprimer_clicked()` | Delete button |
| `on_rechercher_clicked()` | Search button |
| `on_save_bin_clicked()` | Save binary button |
| `on_load_bin_clicked()` | Load binary button |
| `on_save_txt_clicked()` | Save text button |
| `on_load_txt_clicked()` | Load text button |
| `on_treeview_selection_changed()` | Table row selection |

## Data File Formats

### Binary Format (pers.bin)
```
[4 bytes: int count]
[n × sizeof(Personne): contact data]
```

### Text Format (pers.txt)
```
count
nom1 prenom1 age1
nom2 prenom2 age2
...
```

## Performance Characteristics

- **Add Operation**: O(1) amortized (realloc)
- **Delete Operation**: O(n) linear search + shift
- **Search Operation**: O(n) linear search
- **Display**: O(n) tree view rendering
- **File I/O**: O(n) sequential read/write

## Validation

- **Negative Age Check**: Prevents invalid age values
- **Empty Field Validation**: Requires all fields before add/modify
- **Selection Validation**: Modify/delete require selected row
- **File Operation Errors**: Handles missing or corrupted files
- **Memory Safety**: All allocations checked with NULL tests

## Troubleshooting

### Compilation Errors

**"gtk/gtk.h: No such file or directory"**
- Install GTK3 development files (see Prerequisites)

**"pkg-config: command not found"**
- Install pkg-config package for your OS

**"undefined reference to gtk_*"**
- Ensure `$(pkg-config --libs gtk+-3.0)` is in linker flags

### Runtime Issues

**Application crashes on startup**
- Ensure GTK3 is properly installed
- Try: `pkg-config --modversion gtk+-3.0`

**File not found errors**
- Files are created in the working directory
- Ensure write permissions in the directory

**Memory errors**
- Check available system memory
- Close other applications if needed

## Features Comparison

### Original Console Version vs GUI Version

| Feature | Console | GUI |
|---------|---------|-----|
| User Input | scanf() | GTK Entries |
| Output | printf() | GtkTreeView |
| Dialogs | getch() | GTK Dialogs |
| File Ops | Menu driven | Toolbar buttons |
| Data Display | Text list | Sortable table |
| Error Handling | Console text | Message dialogs |
| Memory | Dynamic | Same pattern |
| Code Structure | Single file | Modular callbacks |

## License

This project is provided as-is for educational purposes.

## Author

Generated as a complete GUI wrapper for C contact management system.

---

**Ready to use!** Compile with `make` and run with `./contacts_app`
