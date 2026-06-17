#include<stdio.h>//les fonctions de base ...
#include<stdlib.h>//malloc,realloc ...
#include<string.h>//pour comparer dans recherche ...
#include<conio.h>//poour appuyer apres traitement pour ne pas skip le resultat

#define MAX  40 
#define fichier_bin "pers.bin"
#define fichier_txt "pers.txt"
 typedef struct 
 {
    char nom[MAX];
    char prenom[MAX];
    int age;

 }personne;
 
void save_fbin(personne *t, int taille);
void load_fbin(personne **t, int *taille);
void save_ftext(personne *t, int taille);
void load_ftext(personne **t, int *taille);
int rechercher(personne *t, int taille);
void lister(personne *t,int taille);

void menu(){
    printf("\n------------- MENU PRINCIPAL -------------\n");
    printf("choisir une des options suivantes : \n");
    printf("1.ajouter\n");
    printf("2.modifier\n");
    printf("3.supprimer\n");
    printf("4.lister\n");
    printf("5.rechercher\n");
    printf("6.fichiers binaires\n");
    printf("7.fichiers textes\n");
    printf("8.quitter\n");
    printf("votre choix : ");
}

void menu_bin(personne **t,int *taille){
    int choix;
    printf("\n------------- MENU BINAIRE -------------\n");
    printf("choisir une des options suivantes : \n");
    printf("1.save\n");
    printf("2.load\n");
    printf("votre choix : ");
    if (scanf("%d",&choix) != 1) return;
    switch (choix)
    {
    case 1:
        save_fbin(*t,*taille);
        break;
    case 2:
        load_fbin(t,taille);
        break;    
    default:
        printf("ERROR:veuillez choisir une des options suivantes : (1-2)");
        break;
    }
}

void menu_txt(personne **t,int *taille){
    int choix;
    printf("\n------------- MENU TEXTE -------------\n");
    printf("choisir une des options suivantes : \n");
    printf("1.save\n");
    printf("2.load\n");
    printf("votre choix : ");
    if (scanf("%d",&choix) != 1) return;
    switch (choix)
    {
    case 1:
        save_ftext(*t,*taille);
        break;
    case 2:
        load_ftext(t,taille);
        break;    
    default:
        printf("ERROR:veuillez choisir une des options suivantes : (1-2)");
        break;
    }
}

void ajouter(personne **t,int *taille){
    personne *tmp=(personne*)realloc(*t,(*taille+1) * sizeof(personne));
    if (tmp == NULL) {       
        printf("Erreur memoire.\n");
        return;
    }
    *t=tmp;
    printf("entrez le nom : ");
    scanf("%s", (*t)[*taille].nom);
    printf("entrez le prenom : ");
    scanf("%s", (*t)[*taille].prenom);
    printf("entrez l'age : ");
    scanf("%d",&((*t)[*taille].age));
    if(((*t)[*taille].age)<0){
        printf("ERROR : veuillez choisir un nombre positive");
    }
    (*taille)++;
    printf("Personne ajoutee avec succes\n");
}

void modifier(personne *t,int taille){
    int num;
    if (taille == 0) {
        printf("\n---la liste est vide---\n");
        return;
    }
    lister(t,taille);
    printf("veuillez choisir la personne a modifier (1/%d) : ",taille);
    scanf("%d",&num);
    if ((num<1)||(num>taille))
    {
        printf("\nveuillez choisir un nombre compris entre 1 et %d",taille);
        return;
    }
    printf("\npersonne %d a ete selectionnee avec succes",num);
    printf("\nnouveau nom : ");
    scanf("%s", t[num - 1].nom);
    printf("\nnouveau prenom : ");
    scanf("%s", t[num - 1].prenom);
    printf("\nnouveau age : ");
    scanf("%d",&t[num - 1].age);
    printf("\npersonne %d a ete modifiee avec succes\n",num);
    lister(t,taille);
}

void supprimer(personne **t,int *taille){
    int place;
    if (*taille==0)
    {
        printf("\n------la liste est vide------\najoutez des personnes dans l'option ajouter !!\n");
        return;
    }
    place = rechercher(*t,*taille);
    if (place == -1)
    {
        return;
    }
    // place is 1-based index from rechercher
    int index = place - 1;
    for (int j = index ; j < (*taille) - 1; j++) {
        (*t)[j] = (*t)[j+1];
    }
    (*taille)--;
    if (*taille == 0) {
        free(*t);
        *t = NULL;
    } else {
        personne *tmp = (personne*)realloc(*t, *taille * sizeof(personne));
        if (tmp == NULL) {
            printf("Erreur memoire apres reallocation.\n");
            return;
        }
        *t=tmp;
        printf("personne %d a ete supprimer avec succes .\n",place);
    }
    lister(*t,*taille);
}

void lister(personne *t,int taille){
    if (taille == 0 ){
        printf("\n---la liste est vide---\najoutez des personnes dans l'option ajouter !!\n");
        return;
    }
    printf("%-25s %-25s %s\n","nom","prenom","age");
    printf("=================================================================\n");
    for (int i = 0; i < taille; i++)
    {
        printf("%d . %-21s %-21s %6d\n",i+1,t[i].nom,t[i].prenom,t[i].age);
    }
    printf("\n--- il y a %d personnes dans la liste ---\n",taille);
}

int rechercher(personne *t,int taille){
    char nom[MAX];
    char prenom[MAX];
    int num;
    int choix;
    if (taille == 0) return -1;
    lister(t,taille);
    printf("====== recherchez par ======\n");
    printf("1.nom\n");
    printf("2.prenom\n");
    printf("3.num\n");
    printf("votre choix : ");
    scanf("%d",&choix);
    switch (choix)
    {
    case 1:
        printf("\ninserez le nom : ");
        scanf("%s",nom);
        for (int i = 0; i < taille; i++)
        {
            if (strcmp(nom,t[i].nom)==0)
            {
                printf("%-25s %-25s %s\n","nom","prenom","age");
                printf("=================================================================\n");
                printf("%d . %-21s %-21s %6d\n",i+1,t[i].nom,t[i].prenom,t[i].age);
                return i+1;
            }
        }
        printf("Nom introuvable.\n");
        return -1;
    case 2:
        printf("\ninserez le prenom : ");
        scanf("%s",prenom);
        for (int i = 0; i < taille; i++)
        {
            if (strcmp(prenom,t[i].prenom)==0)
            {
                printf("%-25s %-25s %s\n","nom","prenom","age");
                printf("=================================================================\n");
                printf("%d . %-21s %-21s %6d\n",i+1,t[i].nom,t[i].prenom,t[i].age);
                return i+1;
            }
        }
        printf("Prenom introuvable.\n");
        return -1;
    case 3:
        printf("\ninserez numero : ");
        scanf("%d",&num);
        if (num >= 1 && num <= taille)
        {
            printf("%-25s %-25s %s\n","nom","prenom","age");
            printf("=================================================================\n");
            printf("%d . %-21s %-21s %6d\n",num,t[num-1].nom,t[num-1].prenom,t[num-1].age);
            return num;
        }
        printf("Numero introuvable.\n");
        return -1;
    default:
        printf("ERROR:veuillez choisir une des options suivantes : (1-3)");
        return -1;
    }
}

void save_fbin(personne *t, int taille){
    FILE *f = fopen(fichier_bin, "wb");
    if (f == NULL) {
        printf("ERROR : impossible d'ouvrir le fichier \n");
        return;
    }
    fwrite(&taille, sizeof(int), 1, f);
    if (taille > 0)
        fwrite(t, sizeof(personne), taille, f);
    fclose(f);
    printf("fichier sauvegarde binairement avec succes \n");
}

void load_fbin(personne **t, int *taille){
    int n;
    FILE *f = fopen(fichier_bin, "rb");
    if (f == NULL){
        printf("ERROR : impossible d'ouvrir le fichier \n");
        return;
    }
    if (fread(&n,sizeof(int),1,f) != 1) {
        fclose(f);
        return;
    }
    free(*t);
    if (n > 0) {
        *t=(personne*)malloc(n*sizeof(personne));
        if (*t == NULL) {
            printf("ERRROR : probleme mémoire.\n");
            *taille = 0;
            fclose(f);
            return;
        }
        fread(*t,sizeof(personne),n,f);
    } else {
        *t = NULL;
    }
    *taille=n;
    fclose(f);
    printf("fichier charger binairement avec succes \n");
}

void save_ftext(personne *t, int taille){
    FILE *f= fopen(fichier_txt,"w");
    if (f == NULL) {
        printf("ERROR : impossible d'ouvrir le fichier \n");
        return;
    }
    fprintf(f,"%d\n",taille);
    for (int i = 0; i < taille; i++) {
        fprintf(f, "%s %s %d\n", t[i].nom, t[i].prenom, t[i].age);
    }
    fclose(f);
    printf("fichier texte sauvegarde avec succes \n");
}

void load_ftext(personne **t, int *taille){
    int n;
    FILE *f = fopen(fichier_txt, "r");
    if (f == NULL) {
        printf("ERROR : impossible d'ouvrir le fichier \n");
        return;
    }
    if (fscanf(f, "%d", &n) != 1) {
        fclose(f);
        return;
    }
    free(*t);
    if (n > 0) {
        *t = (personne *)malloc(n * sizeof(personne));
        if (*t == NULL) {
            printf("ERRROR : probleme mémoire.\n");
            *taille = 0;
            fclose(f);
            return;
        }
        for (int i = 0; i < n; i++) {
            fscanf(f, "%s %s %d", (*t)[i].nom, (*t)[i].prenom,&(*t)[i].age);
        }
    } else {
        *t = NULL;
    }
    *taille = n;
    fclose(f);
    printf("fichier texte charge avec succes \n");
}

void libre(personne **t,int *taille){
    free(*t);
    *t = NULL;
    *taille = 0;
    printf("Memoire liberee.\n");
}

int main(){
    int choice;
    personne *t = NULL;
    int taille = 0;
    do
    {
        menu();
        if (scanf("%d",&choice) != 1) {
            while(getchar() != '\n');
            continue;
        }
        switch (choice)
        {
        case 1 :
            ajouter(&t,&taille);
            printf("il y a %d element dans la liste",taille);
            break;
        case 2 :
            modifier(t,taille);
            break;
        case 3 :
            supprimer(&t,&taille);
            break;
        case 4 :
            lister(t,taille);
            break;
        case 5: 
            {
                rechercher(t,taille);
            }
            break;
        case 6 :
            menu_bin(&t,&taille);
            break;
        case 7 :
            menu_txt(&t,&taille);
            break;
        case 8 :
            libre(&t,&taille);
            printf("bye!!");
            break;
        default:
            printf("veuillez choisir une des options suivantes : (1-8)");
            break;
        }
        if (choice != 8) {
            printf("\nAppuyez sur une touche pour continuer...");
            getch();
        }
    } while (choice!=8);
    return 0;
}
