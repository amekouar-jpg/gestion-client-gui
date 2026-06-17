#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 40
#define FICHIER_BIN "pers.bin"
#define FICHIER_TXT "pers.txt"

/* ===================== DATA STRUCTURES ===================== */
typedef struct {
    char nom[MAX];
    char prenom[MAX];
    int age;
} Personne;

typedef struct {
    GtkWidget *window;
    GtkWidget *treeview;
    GtkListStore *liststore;
    
    /* Form widgets */
    GtkWidget *entry_nom;
    GtkWidget *entry_prenom;
    GtkWidget *entry_age;
    GtkWidget *btn_ajouter;
    GtkWidget *btn_modifier;
    GtkWidget *btn_supprimer;
    
    /* Search */
    GtkWidget *entry_search;
    
    /* Data */
    Personne *data;
    int taille;
    int selected_index;
    
} AppContext;

/* ===================== UTILITY FUNCTIONS ===================== */

void show_info_dialog(GtkWidget *parent, const char *title, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s", message
    );
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void show_error_dialog(GtkWidget *parent, const char *title, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_ERROR,
        GTK_BUTTONS_OK,
        "%s", message
    );
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

gboolean show_confirm_dialog(GtkWidget *parent, const char *title, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_QUESTION,
        GTK_BUTTONS_YES_NO,
        "%s", message
    );
    gtk_window_set_title(GTK_WINDOW(dialog), title);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return result == GTK_RESPONSE_YES;
}

char *get_input_dialog(GtkWidget *parent, const char *title, const char *message) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        title,
        GTK_WINDOW(parent),
        GTK_DIALOG_MODAL,
        "_OK", GTK_RESPONSE_OK,
        "_Cancel", GTK_RESPONSE_CANCEL,
        NULL
    );
    
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *label = gtk_label_new(message);
    GtkWidget *entry = gtk_entry_new();
    
    gtk_box_pack_start(GTK_BOX(content_area), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), entry, FALSE, FALSE, 5);
    gtk_widget_show_all(dialog);
    
    char *result = NULL;
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        result = g_strdup(gtk_entry_get_text(GTK_ENTRY(entry)));
    }
    gtk_widget_destroy(dialog);
    return result;
}

/* ===================== BACKEND FUNCTIONS ===================== */

void save_fbin(Personne *t, int taille) {
    FILE *f = fopen(FICHIER_BIN, "wb");
    if (f == NULL) return;
    
    fwrite(&taille, sizeof(int), 1, f);
    if (taille > 0)
        fwrite(t, sizeof(Personne), taille, f);
    fclose(f);
}

void load_fbin(Personne **t, int *taille) {
    int n;
    FILE *f = fopen(FICHIER_BIN, "rb");
    if (f == NULL) return;
    
    if (fread(&n, sizeof(int), 1, f) != 1) {
        fclose(f);
        return;
    }
    
    free(*t);
    if (n > 0) {
        *t = (Personne *)malloc(n * sizeof(Personne));
        if (*t == NULL) {
            *taille = 0;
            fclose(f);
            return;
        }
        fread(*t, sizeof(Personne), n, f);
    } else {
        *t = NULL;
    }
    *taille = n;
    fclose(f);
}

void save_ftext(Personne *t, int taille) {
    FILE *f = fopen(FICHIER_TXT, "w");
    if (f == NULL) return;
    
    fprintf(f, "%d\n", taille);
    for (int i = 0; i < taille; i++) {
        fprintf(f, "%s %s %d\n", t[i].nom, t[i].prenom, t[i].age);
    }
    fclose(f);
}

void load_ftext(Personne **t, int *taille) {
    int n;
    FILE *f = fopen(FICHIER_TXT, "r");
    if (f == NULL) return;
    
    if (fscanf(f, "%d", &n) != 1) {
        fclose(f);
        return;
    }
    
    free(*t);
    if (n > 0) {
        *t = (Personne *)malloc(n * sizeof(Personne));
        if (*t == NULL) {
            *taille = 0;
            fclose(f);
            return;
        }
        for (int i = 0; i < n; i++) {
            fscanf(f, "%s %s %d", (*t)[i].nom, (*t)[i].prenom, &(*t)[i].age);
        }
    } else {
        *t = NULL;
    }
    *taille = n;
    fclose(f);
}

/* ===================== TREEVIEW FUNCTIONS ===================== */

void refresh_treeview(AppContext *app) {
    gtk_list_store_clear(app->liststore);
    
    for (int i = 0; i < app->taille; i++) {
        GtkTreeIter iter;
        gtk_list_store_append(app->liststore, &iter);
        gtk_list_store_set(
            app->liststore, &iter,
            0, i + 1,
            1, app->data[i].nom,
            2, app->data[i].prenom,
            3, app->data[i].age,
            -1
        );
    }
}

void clear_form(AppContext *app) {
    gtk_entry_set_text(GTK_ENTRY(app->entry_nom), "");
    gtk_entry_set_text(GTK_ENTRY(app->entry_prenom), "");
    gtk_entry_set_text(GTK_ENTRY(app->entry_age), "");
    app->selected_index = -1;
}

/* ===================== ACTION CALLBACKS ===================== */

void on_ajouter_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    
    const char *nom = gtk_entry_get_text(GTK_ENTRY(app->entry_nom));
    const char *prenom = gtk_entry_get_text(GTK_ENTRY(app->entry_prenom));
    const char *age_str = gtk_entry_get_text(GTK_ENTRY(app->entry_age));
    
    if (strlen(nom) == 0 || strlen(prenom) == 0 || strlen(age_str) == 0) {
        show_error_dialog(app->window, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }
    
    int age = atoi(age_str);
    if (age < 0) {
        show_error_dialog(app->window, "Erreur", "L'âge doit être un nombre positif.");
        return;
    }
    
    Personne *tmp = (Personne *)realloc(app->data, (app->taille + 1) * sizeof(Personne));
    if (tmp == NULL) {
        show_error_dialog(app->window, "Erreur", "Erreur d'allocation mémoire.");
        return;
    }
    
    app->data = tmp;
    strncpy(app->data[app->taille].nom, nom, MAX - 1);
    app->data[app->taille].nom[MAX - 1] = '\0';
    strncpy(app->data[app->taille].prenom, prenom, MAX - 1);
    app->data[app->taille].prenom[MAX - 1] = '\0';
    app->data[app->taille].age = age;
    
    app->taille++;
    refresh_treeview(app);
    clear_form(app);
    show_info_dialog(app->window, "Succès", "Personne ajoutée avec succès.");
}

void on_modifier_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    
    if (app->selected_index < 0 || app->selected_index >= app->taille) {
        show_error_dialog(app->window, "Erreur", "Veuillez sélectionner une personne.");
        return;
    }
    
    const char *nom = gtk_entry_get_text(GTK_ENTRY(app->entry_nom));
    const char *prenom = gtk_entry_get_text(GTK_ENTRY(app->entry_prenom));
    const char *age_str = gtk_entry_get_text(GTK_ENTRY(app->entry_age));
    
    if (strlen(nom) == 0 || strlen(prenom) == 0 || strlen(age_str) == 0) {
        show_error_dialog(app->window, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }
    
    int age = atoi(age_str);
    if (age < 0) {
        show_error_dialog(app->window, "Erreur", "L'âge doit être un nombre positif.");
        return;
    }
    
    strncpy(app->data[app->selected_index].nom, nom, MAX - 1);
    app->data[app->selected_index].nom[MAX - 1] = '\0';
    strncpy(app->data[app->selected_index].prenom, prenom, MAX - 1);
    app->data[app->selected_index].prenom[MAX - 1] = '\0';
    app->data[app->selected_index].age = age;
    
    refresh_treeview(app);
    clear_form(app);
    show_info_dialog(app->window, "Succès", "Personne modifiée avec succès.");
}

void on_supprimer_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    
    if (app->selected_index < 0 || app->selected_index >= app->taille) {
        show_error_dialog(app->window, "Erreur", "Veuillez sélectionner une personne.");
        return;
    }
    
    if (!show_confirm_dialog(
            app->window,
            "Confirmation",
            "Êtes-vous sûr de vouloir supprimer cette personne ?")) {
        return;
    }
    
    for (int j = app->selected_index; j < app->taille - 1; j++) {
        app->data[j] = app->data[j + 1];
    }
    
    app->taille--;
    if (app->taille == 0) {
        free(app->data);
        app->data = NULL;
    } else {
        Personne *tmp = (Personne *)realloc(app->data, app->taille * sizeof(Personne));
        if (tmp == NULL) {
            show_error_dialog(app->window, "Erreur", "Erreur mémoire après réallocation.");
            return;
        }
        app->data = tmp;
    }
    
    refresh_treeview(app);
    clear_form(app);
    show_info_dialog(app->window, "Succès", "Personne supprimée avec succès.");
}

void on_rechercher_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    
    const char *search_term = gtk_entry_get_text(GTK_ENTRY(app->entry_search));
    if (strlen(search_term) == 0) {
        show_error_dialog(app->window, "Erreur", "Veuillez entrer un terme de recherche.");
        return;
    }
    
    gtk_list_store_clear(app->liststore);
    int found_count = 0;
    
    /* Convert search term to lowercase for case-insensitive search */
    char search_lower[MAX];
    strncpy(search_lower, search_term, MAX - 1);
    search_lower[MAX - 1] = '\0';
    for (int i = 0; search_lower[i]; i++) {
        search_lower[i] = tolower(search_lower[i]);
    }
    
    for (int i = 0; i < app->taille; i++) {
        /* Convert to lowercase for comparison */
        char nom_lower[MAX], prenom_lower[MAX];
        strncpy(nom_lower, app->data[i].nom, MAX - 1);
        nom_lower[MAX - 1] = '\0';
        strncpy(prenom_lower, app->data[i].prenom, MAX - 1);
        prenom_lower[MAX - 1] = '\0';
        
        for (int j = 0; nom_lower[j]; j++) nom_lower[j] = tolower(nom_lower[j]);
        for (int j = 0; prenom_lower[j]; j++) prenom_lower[j] = tolower(prenom_lower[j]);
        
        /* Partial match search (substring) */
        if (strstr(nom_lower, search_lower) != NULL ||
            strstr(prenom_lower, search_lower) != NULL) {
            
            GtkTreeIter iter;
            gtk_list_store_append(app->liststore, &iter);
            gtk_list_store_set(
                app->liststore, &iter,
                0, i + 1,
                1, app->data[i].nom,
                2, app->data[i].prenom,
                3, app->data[i].age,
                -1
            );
            found_count++;
        }
    }
    
    if (found_count == 0) {
        show_error_dialog(app->window, "Recherche", "Aucune personne trouvée.");
        refresh_treeview(app);  /* Reset to show all */
    } else {
        char msg[100];
        snprintf(msg, sizeof(msg), "%d personne(s) trouvée(s).", found_count);
        show_info_dialog(app->window, "Résultat", msg);
    }
}

void on_reset_search_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    gtk_entry_set_text(GTK_ENTRY(app->entry_search), "");
    refresh_treeview(app);
    clear_form(app);
}
    AppContext *app = (AppContext *)data;
    save_fbin(app->data, app->taille);
    show_info_dialog(app->window, "Succès", "Fichier binaire sauvegardé avec succès.");
}

void on_load_bin_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    load_fbin(&app->data, &app->taille);
    refresh_treeview(app);
    clear_form(app);
    show_info_dialog(app->window, "Succès", "Fichier binaire chargé avec succès.");
}

void on_save_txt_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    save_ftext(app->data, app->taille);
    show_info_dialog(app->window, "Succès", "Fichier texte sauvegardé avec succès.");
}

void on_load_txt_clicked(GtkWidget *widget, gpointer data) {
    AppContext *app = (AppContext *)data;
    load_ftext(&app->data, &app->taille);
    refresh_treeview(app);
    clear_form(app);
    show_info_dialog(app->window, "Succès", "Fichier texte chargé avec succès.");
}

/* ===================== TREEVIEW SELECTION HANDLER ===================== */

void on_treeview_selection_changed(GtkTreeSelection *selection, gpointer data) {
    AppContext *app = (AppContext *)data;
    GtkTreeModel *model;
    GtkTreeIter iter;
    
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gint index;
        gchar *nom, *prenom;
        gint age;
        
        gtk_tree_model_get(model, &iter,
            0, &index,
            1, &nom,
            2, &prenom,
            3, &age,
            -1);
        
        app->selected_index = index - 1;
        gtk_entry_set_text(GTK_ENTRY(app->entry_nom), nom);
        gtk_entry_set_text(GTK_ENTRY(app->entry_prenom), prenom);
        
        char age_str[10];
        snprintf(age_str, sizeof(age_str), "%d", age);
        gtk_entry_set_text(GTK_ENTRY(app->entry_age), age_str);
        
        g_free(nom);
        g_free(prenom);
    } else {
        app->selected_index = -1;
    }
}

/* ===================== UI CREATION FUNCTIONS ===================== */

GtkWidget *create_toolbar(AppContext *app) {
    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(toolbar), 5);
    
    /* Search */
    GtkWidget *label_search = gtk_label_new("Chercher:");
    app->entry_search = gtk_entry_new();
    gtk_widget_set_size_request(app->entry_search, 150, -1);
    
    GtkWidget *btn_search = gtk_button_new_with_label("🔍 Rechercher");
    g_signal_connect(btn_search, "clicked", G_CALLBACK(on_rechercher_clicked), app);
    
    GtkWidget *btn_reset = gtk_button_new_with_label("🔄 Réinitialiser");
    g_signal_connect(btn_reset, "clicked", G_CALLBACK(on_reset_search_clicked), app);
    
    gtk_box_pack_start(GTK_BOX(toolbar), label_search, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), app->entry_search, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_search, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_reset, FALSE, FALSE, 0);
    
    gtk_box_pack_start(GTK_BOX(toolbar), gtk_separator_new(GTK_ORIENTATION_VERTICAL), FALSE, FALSE, 0);
    
    /* File operations */
    GtkWidget *btn_save_bin = gtk_button_new_with_label("💾 Sauv. Bin");
    GtkWidget *btn_load_bin = gtk_button_new_with_label("📂 Charger Bin");
    GtkWidget *btn_save_txt = gtk_button_new_with_label("💾 Sauv. Texte");
    GtkWidget *btn_load_txt = gtk_button_new_with_label("📂 Charger Texte");
    
    g_signal_connect(btn_save_bin, "clicked", G_CALLBACK(on_save_bin_clicked), app);
    g_signal_connect(btn_load_bin, "clicked", G_CALLBACK(on_load_bin_clicked), app);
    g_signal_connect(btn_save_txt, "clicked", G_CALLBACK(on_save_txt_clicked), app);
    g_signal_connect(btn_load_txt, "clicked", G_CALLBACK(on_load_txt_clicked), app);
    
    gtk_box_pack_start(GTK_BOX(toolbar), btn_save_bin, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_load_bin, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_save_txt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(toolbar), btn_load_txt, FALSE, FALSE, 0);
    
    return toolbar;
}

GtkWidget *create_form_panel(AppContext *app) {
    GtkWidget *frame = gtk_frame_new("Formulaire");
    gtk_container_set_border_width(GTK_CONTAINER(frame), 10);
    
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(frame), vbox);
    
    /* Nom */
    GtkWidget *hbox_nom = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *label_nom = gtk_label_new("Nom:");
    gtk_widget_set_size_request(label_nom, 80, -1);
    app->entry_nom = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox_nom), label_nom, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_nom), app->entry_nom, TRUE, TRUE, 0);
    
    /* Prenom */
    GtkWidget *hbox_prenom = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *label_prenom = gtk_label_new("Prénom:");
    gtk_widget_set_size_request(label_prenom, 80, -1);
    app->entry_prenom = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox_prenom), label_prenom, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_prenom), app->entry_prenom, TRUE, TRUE, 0);
    
    /* Age */
    GtkWidget *hbox_age = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *label_age = gtk_label_new("Âge:");
    gtk_widget_set_size_request(label_age, 80, -1);
    app->entry_age = gtk_entry_new();
    gtk_entry_set_input_purpose(GTK_ENTRY(app->entry_age), GTK_INPUT_PURPOSE_NUMBER);
    gtk_box_pack_start(GTK_BOX(hbox_age), label_age, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_age), app->entry_age, TRUE, TRUE, 0);
    
    /* Buttons */
    GtkWidget *hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    
    app->btn_ajouter = gtk_button_new_with_label("✏️  Ajouter");
    app->btn_modifier = gtk_button_new_with_label("✏️  Modifier");
    app->btn_supprimer = gtk_button_new_with_label("🗑️  Supprimer");
    
    g_signal_connect(app->btn_ajouter, "clicked", G_CALLBACK(on_ajouter_clicked), app);
    g_signal_connect(app->btn_modifier, "clicked", G_CALLBACK(on_modifier_clicked), app);
    g_signal_connect(app->btn_supprimer, "clicked", G_CALLBACK(on_supprimer_clicked), app);
    
    gtk_box_pack_start(GTK_BOX(hbox_buttons), app->btn_ajouter, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), app->btn_modifier, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), app->btn_supprimer, TRUE, TRUE, 0);
    
    /* Pack all */
    gtk_box_pack_start(GTK_BOX(vbox), hbox_nom, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_prenom, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_age, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox_buttons, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, FALSE, 0);
    
    return frame;
}

GtkWidget *create_treeview(AppContext *app) {
    app->liststore = gtk_list_store_new(
        4,
        G_TYPE_INT,        /* # */
        G_TYPE_STRING,      /* Nom */
        G_TYPE_STRING,      /* Prenom */
        G_TYPE_INT          /* Age */
    );
    
    app->treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(app->liststore));
    g_object_unref(app->liststore);
    
    /* Columns */
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("#", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(app->treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(app->treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Prénom", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(app->treeview), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Âge", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(app->treeview), column);
    
    /* Selection handler */
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(app->treeview));
    g_signal_connect(selection, "changed", G_CALLBACK(on_treeview_selection_changed), app);
    
    return app->treeview;
}

void create_main_window(AppContext *app, GtkApplication *gapp) {
    app->window = gtk_application_window_new(gapp);
    gtk_window_set_title(GTK_WINDOW(app->window), "Gestion des Contacts");
    gtk_window_set_default_size(GTK_WINDOW(app->window), 1000, 600);
    gtk_container_set_border_width(GTK_CONTAINER(app->window), 5);
    
    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(app->window), main_vbox);
    
    /* Toolbar */
    GtkWidget *toolbar = create_toolbar(app);
    gtk_box_pack_start(GTK_BOX(main_vbox), toolbar, FALSE, FALSE, 0);
    
    /* Separator */
    gtk_box_pack_start(GTK_BOX(main_vbox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, FALSE, 0);
    
    /* Main content paned layout */
    GtkWidget *hpaned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    
    /* Left: Form */
    GtkWidget *form_panel = create_form_panel(app);
    gtk_paned_pack1(GTK_PANED(hpaned), form_panel, FALSE, FALSE);
    
    /* Right: TreeView */
    GtkWidget *frame_table = gtk_frame_new("Contacts");
    gtk_container_set_border_width(GTK_CONTAINER(frame_table), 5);
    
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(
        GTK_SCROLLED_WINDOW(scrolled_window),
        GTK_POLICY_AUTOMATIC,
        GTK_POLICY_AUTOMATIC
    );
    
    GtkWidget *treeview = create_treeview(app);
    gtk_container_add(GTK_CONTAINER(scrolled_window), treeview);
    gtk_container_add(GTK_CONTAINER(frame_table), scrolled_window);
    
    gtk_paned_pack2(GTK_PANED(hpaned), frame_table, TRUE, TRUE);
    gtk_paned_set_position(GTK_PANED(hpaned), 300);
    
    gtk_box_pack_start(GTK_BOX(main_vbox), hpaned, TRUE, TRUE, 0);
    
    /* Status bar */
    GtkWidget *status_bar = gtk_label_new("Prêt");
    gtk_box_pack_start(GTK_BOX(main_vbox), status_bar, FALSE, FALSE, 0);
    
    gtk_widget_show_all(app->window);
}

void on_activate(GtkApplication *app, gpointer user_data) {
    AppContext *app_context = (AppContext *)user_data;
    create_main_window(app_context, app);
}

int main(int argc, char *argv[]) {
    AppContext app;
    app.data = NULL;
    app.taille = 0;
    app.selected_index = -1;
    
    GtkApplication *application = gtk_application_new(
        "com.example.contacts",
        G_APPLICATION_FLAGS_NONE
    );
    g_signal_connect(application, "activate", G_CALLBACK(on_activate), &app);
    
    int status = g_application_run(G_APPLICATION(application), argc, argv);
    
    free(app.data);
    g_object_unref(application);
    
    return status;
}

