#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define N 4
#define TAILLE (N * N)

const char CROIX[2] = "+";
const char COL[2] = "|";
const char LIG[15] = "--------------";
const char ESPACE[15] = " ";

typedef struct {
    int ligne;
    int colonne;
    int nbCandidats;
} infoCellule;

typedef int tGrille[TAILLE][TAILLE];

void chargerGrille(tGrille g)
{
    char nomFichier[30];
    FILE *f;
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);
    f = fopen(nomFichier, "rb");
    if (f == NULL)
    {
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    }
    else
    {
        fread(g, sizeof(int), TAILLE * TAILLE, f);
    }
    fclose(f);
}

void affichegrille(tGrille g)
{
    int ligne, colonne, compteur1, compteur2;
    printf("\n");
    printf("    ");

    for (ligne = 0; ligne < TAILLE; ligne++)
    {
        if ((ligne == N) || (ligne == N * 2) || (ligne == N * 3) || (ligne == TAILLE))
        {
            printf("   ");
        }
        if (ligne < 10)
        {
            printf("  %d", ligne + 1);
        }
        else
        {

            printf(" %d", ligne + 1);
        }
    }
    printf("\n");
    printf("   ");
    // 1er ligne avec croix
    for (compteur1 = 0; compteur1 < N; compteur1++)
    {
        printf(CROIX);
        printf(LIG);
    }
    printf(CROIX);
    printf("\n");
    //
    compteur1 = 1;
    for (ligne = 0; ligne < TAILLE; ligne++)
    {
        if (compteur1 > 9)
        {
            printf("%d | ", compteur1);
            compteur1 = compteur1 + 1;
        }
        if (compteur1 < 10)
        {
            printf("%d  | ", compteur1);
            compteur1 = compteur1 + 1;
        }
        for (colonne = 0; colonne < TAILLE; colonne++)
        {
            if (g[ligne][colonne] != 0 && g[ligne][colonne] > 9)
            {
                printf("%d ", g[ligne][colonne]);
            }
            else if (g[ligne][colonne] != 0 && g[ligne][colonne] < 10)
            {
                printf(" %d ", g[ligne][colonne]);
            }
            else
            {
                printf(" . ");
            }

            if ((colonne == N - 1) || (colonne == (N * 2 - 1)) || (colonne == (N * 3 - 1)) || (colonne == TAILLE - 1))
            {
                printf(" | ");
            }
        }
        printf("\n");
        if ((ligne == N - 1) || (ligne == (N * 2 - 1)) || (ligne == (N * 3 - 1)) || (ligne == TAILLE - 1))
        {
            printf("   ");
            for (compteur2 = 0; compteur2 < N; compteur2++)
            {
                printf(CROIX);
                printf(LIG);
            }
            printf(CROIX);
            printf("\n");
        }
    }
}




bool absentSurLigne(int val, tGrille g, int ligID)
{
    int i;
    for (i = 0; i < TAILLE; i++)
    {
        if (val == g[ligID][i])
        {
            return false;
        }
    }
    return true;
}

bool absentSurColonne(int val, tGrille g, int colID)
{
    int i;
    for (i = 0; i < TAILLE; i++)
    {
        if (val == g[i][colID])
        {
            return false;
        }
    }
    return true;
}

bool absentSurBloc(int val, tGrille g, int i, int j)
{
    int ligneBloc = i / N;
    int colloneBloc = j / N;
    int initialx, initialy, x, y;
    initialx = ligneBloc * N;
    initialy = colloneBloc * N;
    for (x = initialx; x < initialx + N; x++)
    {
        for (y = initialy; y < initialy + N; y++)
        {
            if (g[x][y] == val)
            {
                return false;
            }
        }
    }

    return true;
}


void triRapide(infoCellule arr[], int debut, int fin)
{
    if (debut < fin)
    {
        int pivot = arr[fin].nbCandidats;
        int i = (debut - 1);

        for (int j = debut; j <= fin - 1; j++)
        {
            if (arr[j].nbCandidats < pivot)
            {
                i++;
                infoCellule temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        infoCellule temp = arr[i + 1];
        arr[i + 1] = arr[fin];
        arr[fin] = temp;

        int pivotIndex = i + 1;

        triRapide(arr, debut, pivotIndex - 1);
        triRapide(arr, pivotIndex + 1, fin);
    }
}

void avoir_candidats(tGrille g, infoCellule candidats[])
{
    int cmpt = 0;
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            if (g[i][j] == 0)
            {
                candidats[cmpt].ligne = i;
                candidats[cmpt].colonne = j;
                candidats[cmpt].nbCandidats = 0;
                for (int val = 1; val <= TAILLE; ++val)
                {
                    if (absentSurLigne(val, g, i) &&
                        absentSurColonne(val, g, j) &&
                        absentSurBloc(val, g, i, j))
                    {
                        candidats[cmpt].nbCandidats++;
                    }
                }
                cmpt++;
            }
        }
    }
}


void singletonsNues(tGrille g, infoCellule candidats[]) {
    int val;
    for (int i = 0; i < TAILLE; ++i) {
        int compteurLigne[TAILLE] = {0};
        int compteurColonne[TAILLE] = {0};
        int compteurBloc[TAILLE] = {0};

        for (int j = 0; j < TAILLE; ++j) {
            if (g[i][j] == 0) {
                for ( val = 1; val <= TAILLE; val++) {
                    if (absentSurLigne(val, g, i) && absentSurColonne(val, g, j) && absentSurBloc(val, g, i, j)) {
                        compteurLigne[val - 1]++;
                        compteurColonne[val - 1]++;
                        compteurBloc[val - 1]++;
                    }
                }
            }

            int blocLigne = (i / N) * N;
            int blocColonne = (j / N) * N;
            if (g[i][j] == 0 && absentSurLigne(val, g, i) && absentSurColonne(val, g, j) && absentSurBloc(val, g, i, j)) {
                compteurBloc[(i - blocLigne) * N + (j - blocColonne)]++;
            }
        }

        for (int val = 1; val <= TAILLE; ++val) {
            if (compteurLigne[val - 1] == 1) {
                for (int j = 0; j < TAILLE; ++j) {
                    if (g[i][j] == 0 && absentSurLigne(val, g, i) && absentSurColonne(val, g, j) && absentSurBloc(val, g, i, j)) {
                        candidats[i * TAILLE + j].nbCandidats = 1;
                    }
                }
            }
            if (compteurColonne[val - 1] == 1) {
                for (int k = 0; k < TAILLE; ++k) {
                    if (g[k][i] == 0 && absentSurLigne(val, g, k) && absentSurColonne(val, g, i) && absentSurBloc(val, g, k, i)) {
                        candidats[k * TAILLE + i].nbCandidats = 1;
                    }
                }
            }
            if (compteurBloc[val - 1] == 1) {
                int blocLigne = (i / N) * N;
                int blocColonne = (i % N) * N;
                for (int k = blocLigne; k < blocLigne + N; ++k) {
                    for (int l = blocColonne; l < blocColonne + N; ++l) {
                        if (g[k][l] == 0 && absentSurLigne(val, g, k) && absentSurColonne(val, g, l) && absentSurBloc(val, g, k, l)) {
                            candidats[k * TAILLE + l].nbCandidats = 1;
                        }
                    }
                }
            }
        }
    }
}

// Fonction pour rechercher les singletons cachés dans une ligne
void singletonsCachesLigne(tGrille g, infoCellule candidats[]) {
    for (int i = 0; i < TAILLE; ++i) {
        int compteur[TAILLE] = {0};
        for (int j = 0; j < TAILLE; ++j)
            if (g[i][j] == 0)
                for (int val = 1; val <= TAILLE; ++val)
                    if (absentSurLigne(val, g, i) && absentSurColonne(val, g, j) && absentSurBloc(val, g, i, j))
                        compteur[val - 1]++;
        for (int val = 1; val <= TAILLE; ++val)
            if (compteur[val - 1] == 1)
                for (int j = 0; j < TAILLE; ++j)
                    if (g[i][j] == 0 && absentSurLigne(val, g, i) && absentSurColonne(val, g, j) && absentSurBloc(val, g, i, j))
                        candidats[i * TAILLE + j].nbCandidats = 1;
    }
}

// Fonction pour rechercher les singletons cachés dans une colonne
void singletonsCachesColonne(tGrille g, infoCellule candidats[]) {
    for (int j = 0; j < TAILLE; ++j) {
        int compteur[TAILLE] = {0};
        for (int i = 0; i < TAILLE; ++i)
            if (g[i][j] == 0)
                for (int val = 1; val <= TAILLE; ++val)
                    if (absentSurLigne(val, g, i) && absentSurColonne(val, g, j) && absentSurBloc(val, g, i, j))
                        compteur[val - 1]++;
        for (int val = 1; val <= TAILLE; ++val)
            if (compteur[val - 1] == 1)
                for (int i = 0; i < TAILLE; ++i)
                    if (g[i][j] == 0 && absentSurLigne(val, g, i) && absentSurColonne(val, g, j) && absentSurBloc(val, g, i, j))
                        candidats[i * TAILLE + j].nbCandidats = 1;
    }
}

// Fonction pour rechercher les singletons cachés dans un bloc
void singletonsCachesBloc(tGrille g, infoCellule candidats[]) {
    for (int k = 0; k < TAILLE; k += N)
        for (int l = 0; l < TAILLE; l += N)
            for (int val = 1; val <= TAILLE; ++val) {
                int compteur = 0, ligne, colonne;
                for (int i = 0; i < N; ++i)
                    for (int j = 0; j < N; ++j) {
                        ligne = k + i; colonne = l + j;
                        if (g[ligne][colonne] == 0 && absentSurLigne(val, g, ligne) && absentSurColonne(val, g, colonne) && absentSurBloc(val, g, ligne, colonne))
                            compteur++;
                    }
                if (compteur == 1)
                    for (int i = 0; i < N; ++i)
                        for (int j = 0; j < N; ++j) {
                            ligne = k + i; colonne = l + j;
                            if (g[ligne][colonne] == 0 && absentSurLigne(val, g, ligne) && absentSurColonne(val, g, colonne) && absentSurBloc(val, g, ligne, colonne))
                                candidats[ligne * TAILLE + colonne].nbCandidats = 1;
                        }
            }
}

// Fonction pour rechercher les singletons cachés dans toute la grille
void chercherSingletonsCaches(tGrille g, infoCellule candidats[]) {
    singletonsCachesLigne(g, candidats);
    singletonsCachesColonne(g, candidats);
    singletonsCachesBloc(g, candidats);
}

void pairesCachees(tGrille g, infoCellule candidats[]) {
    for (int i = 0; i < TAILLE; ++i) {
        int compteurLigne[TAILLE] = {0};
        int compteurColonne[TAILLE] = {0};
        int compteurBloc[TAILLE] = {0};

        for (int j = 0; j < TAILLE; ++j) {
            if (g[i][j] == 0) {
                for (int val = 1; val <= TAILLE; ++val) {
                    if (absentSurLigne(val, g, i) && absentSurColonne(val, g, j) && absentSurBloc(val, g, i, j)) {
                        compteurLigne[val - 1]++;
                        compteurColonne[val - 1]++;
                        compteurBloc[(i / N) * N + (j / N)]++;
                    }
                }
            }
        }

        for (int val = 1; val <= TAILLE; ++val) {
            if (compteurLigne[val - 1] == 2) {
                for (int j1 = 0; j1 < TAILLE - 1; ++j1) {
                    if (g[i][j1] == 0 && absentSurLigne(val, g, i) && absentSurColonne(val, g, j1) && absentSurBloc(val, g, i, j1)) {
                        for (int j2 = j1 + 1; j2 < TAILLE; ++j2) {
                            if (g[i][j2] == 0 && absentSurLigne(val, g, i) && absentSurColonne(val, g, j2) && absentSurBloc(val, g, i, j2)) {
                                for (int k = 0; k < TAILLE; ++k) {
                                    if (k != j1 && k != j2 && g[i][k] == 0) {
                                        candidats[i * TAILLE + k].nbCandidats &= ~(1 << (val - 1));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (compteurColonne[val - 1] == 2) {
                for (int i1 = 0; i1 < TAILLE - 1; ++i1) {
                    if (g[i1][i] == 0 && absentSurLigne(val, g, i1) && absentSurColonne(val, g, i) && absentSurBloc(val, g, i1, i)) {
                        for (int i2 = i1 + 1; i2 < TAILLE; ++i2) {
                            if (g[i2][i] == 0 && absentSurLigne(val, g, i2) && absentSurColonne(val, g, i) && absentSurBloc(val, g, i2, i)) {
                                for (int k = 0; k < TAILLE; ++k) {
                                    if (k != i1 && k != i2 && g[k][i] == 0) {
                                        candidats[k * TAILLE + i].nbCandidats &= ~(1 << (val - 1));
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (compteurBloc[val - 1] == 2) {
                int blocLigne = (i / N) * N;
                int blocColonne = (i % N) * N;
                for (int k1 = blocLigne; k1 < blocLigne + N - 1; ++k1) {
                    for (int l1 = blocColonne; l1 < blocColonne + N - 1; ++l1) {
                        if (g[k1][l1] == 0 && absentSurLigne(val, g, k1) && absentSurColonne(val, g, l1) && absentSurBloc(val, g, k1, l1)) {
                            for (int k2 = k1; k2 < blocLigne + N; ++k2) {
                                for (int l2 = l1 + 1; l2 < blocColonne + N; ++l2) {
                                    if (g[k2][l2] == 0 && absentSurLigne(val, g, k2) && absentSurColonne(val, g, l2) && absentSurBloc(val, g, k2, l2)) {
                                        for (int m = blocLigne; m < blocLigne + N; ++m) {
                                            for (int n = blocColonne; n < blocColonne + N; ++n) {
                                                if ((m != k1 || n != l1) && (m != k2 || n != l2) && g[m][n] == 0) {
                                                    candidats[m * TAILLE + n].nbCandidats &= ~(1 << (val - 1));
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


bool backtracking(tGrille grille, infoCellule candidats[], int numeroCase)
{
    int ligne, colonne;
    bool resultat = false;

    if (numeroCase == TAILLE * TAILLE)
    {
        // Toutes les cases sont traitées, la grille est résolue
        return true;
    }
    else
    {
        // Calcul des coordonnées de la case
        ligne = numeroCase / TAILLE;
        colonne = numeroCase % TAILLE;

        if (grille[ligne][colonne] != 0)
        {
            // La case n'est pas vide, passer à la suivante (appel récursif)
            resultat = backtracking(grille, candidats, numeroCase + 1);
        }
        else
        {
            // La case est vide, essayer toutes les valeurs possibles
            for (int valeur = 1; valeur <= TAILLE; ++valeur)
            {
                if (absentSurLigne(valeur, grille, ligne) &&
                    absentSurColonne(valeur, grille, colonne) &&
                    absentSurBloc(valeur, grille, ligne, colonne))
                {

                    // La valeur est autorisée, l'assigner à la case
                    grille[ligne][colonne] = valeur;

                    // Appel récursif pour vérifier si ce choix est bon par la suite
                    if (backtracking(grille,candidats , numeroCase + 1))
                    {
                        resultat = true;
                    }
                    else
                    {
                        // Annuler l'assignation si le choix n'est pas bon
                        grille[ligne][colonne] = 0;
                    }
                }
            }
        }
        return resultat;
    }
}


int main()
{
    tGrille g;
    int numeroCase = 0;

    infoCellule candidats[TAILLE * TAILLE];

    chargerGrille(g);
    printf("Grille initiale : ");
    affichegrille(g);
    
    
    clock_t debut = clock(); 
    /*singletonsNues( g, candidats);
    chercherSingletonsCaches( g, candidats);
    pairesCachees( g,  candidats);*/
    avoir_candidats(g, candidats); // Calculer les candidats pour chaque case
    triRapide(candidats, 0, TAILLE * TAILLE - 1); // Trier les candidats par nombre de candidats
    backtracking(g, candidats, numeroCase);
    clock_t fin = clock();
    printf("Grille finale : ");
    affichegrille(g);
    double tempsCPU = (fin - debut) * 1.0 / CLOCKS_PER_SEC;
    printf("Temps CPU = %.6f secondes\n", tempsCPU);
    return 0;
}

