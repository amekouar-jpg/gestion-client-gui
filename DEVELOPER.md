# Developer's Quick Reference

Fast-track guide for modifying and extending the GTK3 Contact Manager.

## Project Structure

```
projet/
├── projet_gui.c         ← Main application (650 lines)
├── Makefile             ← Linux/macOS build
├── CMakeLists.txt       ← Alternative CMake build
├── build.bat            ← Windows build script
├── README.md            ← User guide
├── BUILD.md             ← Build instructions
├── ARCHITECTURE.md      ← Technical details
└── DEVELOPER.md         ← This file
```

## File Sections (projet_gui.c)

```
Lines 1-30       Include headers & #defines
Lines 32-47      Data structures (Personne, AppContext)
Lines 49-130     Utility functions (dialogs)
Lines 132-200    Backend functions (file I/O)
Lines 202-236    TreeView functions
Lines 238-420    Action callbacks (handlers)
Lines 422-445    TreeView selection handler
Lines 447-600    UI creation functions
Lines 602-640    Main window setup
Lines 642-660    GTK lifecycle & main()
```

## Common Modifications

### 1. Add a New Column to the Table

**Goal**: Add a "Email" column to Personne

**Steps**:

1. **Modify struct** (line ~40):
```c
typedef struct {
    char nom[MAX];
    char prenom[MAX];
    char email[MAX];     // ← Add this
    int age;
} Personne;
```

2. **Update ListStore** (line ~338):
```c
app->liststore = gtk_list_store_new(
    5,                  // ← Change 4 to 5
    G_TYPE_INT,
    G_TYPE_STRING,
    G_TYPE_STRING,
    G_TYPE_STRING,      // ← Add this for email
    G_TYPE_INT
);
```

3. **Add column rendering** (line ~350):
```c
renderer = gtk_cell_renderer_text_new();
column = gtk_tree_view_column_new_with_attributes(
    "Email", renderer, "text", 3, NULL);  // ← Index 3
gtk_tree_view_append_column(GTK_TREE_VIEW(app->treeview), column);
```

4. **Add form input** (line ~276 in create_form_panel):
```c
GtkWidget *hbox_email = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
GtkWidget *label_email = gtk_label_new("Email:");
GtkWidget *entry_email = gtk_entry_new();
gtk_box_pack_start(GTK_BOX(hbox_email), label_email, FALSE, FALSE, 0);
gtk_box_pack_start(GTK_BOX(hbox_email), entry_email, TRUE, TRUE, 0);
gtk_box_pack_start(GTK_BOX(vbox), hbox_email, FALSE, FALSE, 0);
```

5. **Update refresh_treeview** (line ~225):
```c
gtk_list_store_set(
    app->liststore, &iter,
    0, i + 1,
    1, app->data[i].nom,
    2, app->data[i].prenom,
    3, app->data[i].email,  // ← Add this
    4, app->data[i].age,    // ← Update index
    -1
);
```

6. **Update file I/O** (save/load functions):
```c
// Binary: Already works (fwrite/fread entire struct)

// Text: Update formatting
fprintf(f, "%s %s %s %d\n", t[i].nom, t[i].prenom, t[i].email, t[i].age);
fscanf(f, "%s %s %s %d", (*t)[i].nom, (*t)[i].prenom, (*t)[i].email, &(*t)[i].age);
```

### 2. Change MAX String Size

**Current**: `#define MAX 40`

**To increase**:
```c
#define MAX 100  // Increase buffer size
```

**Warning**: This increases memory per record (80 bytes → 200 bytes per Personne)

### 3. Add a New Button Action

**Goal**: Add a "Clear Form" button

**Steps**:

1. **Add button to context** (line ~44):
```c
typedef struct {
    // ... existing fields ...
    GtkWidget *btn_clear;  // ← Add this
} AppContext;
```

2. **Create button in form** (line ~280):
```c
GtkWidget *btn_clear = gtk_button_new_with_label("🧹 Effacer");
g_signal_connect(btn_clear, "clicked", G_CALLBACK(on_clear_clicked), app);
gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_clear, TRUE, TRUE, 0);
```

3. **Add callback** (line ~420):
```c
void on_clear_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    clear_form(app);
    show_info_dialog(app->window, "Info", "Formulaire effacé.");
}
```

### 4. Add Sorting to Table

**Goal**: Click column headers to sort

**Changes**:

Replace `gtk_list_store_new()` with `gtk_tree_store_new()` and add:
```c
GtkTreeSortable *sortable = GTK_TREE_SORTABLE(app->liststore);
gtk_tree_sortable_set_sort_column_id(
    sortable, 1,  // Sort by column 1 (Nom)
    GTK_SORT_ASCENDING
);
```

### 5. Add Dark Mode Toggle

**Steps**:

1. **Add to toolbar** (line ~243):
```c
GtkWidget *btn_dark = gtk_button_new_with_label("🌙 Dark Mode");
g_signal_connect(btn_dark, "clicked", G_CALLBACK(on_dark_mode_clicked), app);
gtk_box_pack_end(GTK_BOX(toolbar), btn_dark, FALSE, FALSE, 0);
```

2. **Add callback**:
```c
void on_dark_mode_clicked(GtkWidget *widget, gpointer data) {
    GtkSettings *settings = gtk_settings_get_default();
    gboolean dark;
    g_object_get(settings, "gtk-application-prefer-dark-theme", &dark, NULL);
    g_object_set(settings, "gtk-application-prefer-dark-theme", !dark, NULL);
}
```

## Adding New Features

### Feature: Export to CSV

```c
void export_csv(Personne *t, int taille) {
    FILE *f = fopen("pers.csv", "w");
    if (f == NULL) return;
    
    fprintf(f, "Nom,Prenom,Age\n");
    for (int i = 0; i < taille; i++) {
        fprintf(f, "%s,%s,%d\n", t[i].nom, t[i].prenom, t[i].age);
    }
    fclose(f);
}

// Add button in toolbar
GtkWidget *btn_csv = gtk_button_new_with_label("📊 Exporter CSV");
g_signal_connect(btn_csv, "clicked", G_CALLBACK(on_export_csv_clicked), app);

void on_export_csv_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    export_csv(app->data, app->taille);
    show_info_dialog(app->window, "Succès", "Fichier CSV exporté.");
}
```

### Feature: Duplicate Contact

```c
void on_duplicate_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    
    if (app->selected_index < 0) {
        show_error_dialog(app->window, "Erreur", "Sélectionnez un contact.");
        return;
    }
    
    // Realloc
    Personne *tmp = (Personne *)realloc(
        app->data,
        (app->taille + 1) * sizeof(Personne)
    );
    if (tmp == NULL) {
        show_error_dialog(app->window, "Erreur", "Mémoire insuffisante.");
        return;
    }
    
    app->data = tmp;
    app->data[app->taille] = app->data[app->selected_index];
    app->taille++;
    
    refresh_treeview(app);
    show_info_dialog(app->window, "Succès", "Contact dupliqué.");
}
```

### Feature: Statistics Window

```c
void on_stats_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    
    int total = app->taille;
    int sum_age = 0;
    int min_age = INT_MAX;
    int max_age = 0;
    
    for (int i = 0; i < total; i++) {
        sum_age += app->data[i].age;
        if (app->data[i].age < min_age) min_age = app->data[i].age;
        if (app->data[i].age > max_age) max_age = app->data[i].age;
    }
    
    char msg[256];
    double avg_age = total > 0 ? (double)sum_age / total : 0;
    snprintf(msg, sizeof(msg),
        "Total: %d contacts\n"
        "Âge moyen: %.1f\n"
        "Âge min: %d\n"
        "Âge max: %d",
        total, avg_age, min_age, max_age);
    
    show_info_dialog(app->window, "Statistiques", msg);
}
```

## Debug Tips

### Print Variable Values

```c
g_print("Debug: index=%d, taille=%d, ptr=%p\n", 
        app->selected_index, app->taille, app->data);
```

### Check GTK Version

```c
g_print("GTK version: %d.%d.%d\n",
        gtk_get_major_version(),
        gtk_get_minor_version(),
        gtk_get_micro_version());
```

### Trace Signal Connections

```c
gulong handler_id = g_signal_connect(
    button, "clicked",
    G_CALLBACK(on_button_clicked), app);

g_signal_handler_disconnect(button, handler_id);  // Remove handler
```

### Memory Leak Detection

```bash
# Linux with Valgrind
valgrind --leak-check=full --show-leak-kinds=all ./contacts_app
```

## Common Bugs & Fixes

### Bug: Segmentation Fault on Search
```c
// WRONG: uninitialized selected_index
if (app->selected_index >= app->taille) { /* crash */ }

// CORRECT: check bounds properly
if (app->selected_index < 0 || app->selected_index >= app->taille) { 
    show_error_dialog(...);
    return;
}
```

### Bug: Memory Leak on Error
```c
// WRONG: allocation not freed on error
Personne *tmp = realloc(data, ...);
if (strcpy_check_fails()) return;  // Memory leaked

// CORRECT: use temporary pointer
Personne *tmp = realloc(data, ...);
if (tmp == NULL) {
    show_error_dialog(...);
    return;  // Original pointer still valid
}
```

### Bug: TreeView Not Updating
```c
// WRONG: forgot to refresh
app->data[i].age = new_age;
// Display doesn't change!

// CORRECT: refresh after modification
app->data[i].age = new_age;
refresh_treeview(app);  // ← Required
```

### Bug: String Buffer Overflow
```c
// WRONG: unbounded copy
strcpy(app->data[i].nom, input);  // Crash if input > MAX

// CORRECT: bounded copy with null-termination
strncpy(app->data[i].nom, input, MAX - 1);
app->data[i].nom[MAX - 1] = '\0';
```

## Performance Optimization

### Minimize Allocations

**Before** (inefficient):
```c
for (int i = 0; i < 1000; i++) {
    app->data = realloc(app->data, (i+1) * sizeof(Personne));  // 1000 allocations!
}
```

**After** (efficient):
```c
app->data = malloc(1000 * sizeof(Personne));  // 1 allocation
for (int i = 0; i < 1000; i++) {
    // Use directly
}
```

### Batch TreeView Updates

**Before** (slow):
```c
for (int i = 0; i < 1000; i++) {
    refresh_treeview(app);  // Redraw 1000 times!
}
```

**After** (fast):
```c
// ... all modifications ...
refresh_treeview(app);  // Single redraw
```

## Testing Checklist

- [ ] Add contact with special characters (é, ç, é)
- [ ] Delete first, middle, last contact
- [ ] Modify with empty fields
- [ ] Search for non-existent contact
- [ ] Save binary and load back
- [ ] Save text and edit manually, load back
- [ ] Add 1000 contacts (memory stress test)
- [ ] Close and reopen application
- [ ] Test with UTF-8 names

## Build & Run Quick Commands

```bash
# Clean and build
make clean && make && ./contacts_app

# Build with debug symbols (for gdb)
CFLAGS="-g -O0" make clean && make

# Run with debugger
gdb ./contacts_app
(gdb) run
(gdb) bt  # backtrace on crash

# Check for memory leaks
valgrind --leak-check=full ./contacts_app
```

## GTK Reference Quick Links

**Key Widgets**:
- `GtkWindow`, `GtkBox`, `GtkButton`, `GtkEntry`
- `GtkLabel`, `GtkFrame`, `GtkSeparator`
- `GtkTreeView`, `GtkListStore`, `GtkCellRenderer`
- `GtkDialog`, `GtkMessageDialog`

**Key Functions**:
- `gtk_widget_show_all()` - Display all widgets
- `gtk_container_add()` - Add child to parent
- `gtk_box_pack_start/end()` - Add to box
- `g_signal_connect()` - Connect signal to callback
- `g_object_unref()` - Release GTK object reference

**GTK3 Documentation**: https://developer.gnome.org/gtk3/stable/

## Next Steps

1. Build and run the application
2. Test all features
3. Pick a modification from the "Common Modifications" section
4. Implement and test
5. Read ARCHITECTURE.md for deeper understanding
6. Check GTK documentation for advanced features

---

**Happy coding!** 🚀
