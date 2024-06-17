#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 3
#define TAILLE (N * N)

typedef struct
{
    int valeur;
    int candidats[TAILLE];
    int nbCandidats;
} tCase1;

const char CROIX[2] = "+";        /**CONSTANTE pour la procédure affiche grille*/
const char COL[2] = "|";          /**CONSTANTE pour la procédure affiche grille*/
const char LIG[10] = "---------"; /**CONSTANTE pour la procédure affiche grille*/
const char ESPACE[10] = " ";      /**CONSTANTE pour la procédure affiche grille*/

typedef tCase1 tGrille[TAILLE][TAILLE];
typedef int tTab[TAILLE][TAILLE];

int chargerGrille(tGrille g)
{
    tTab TabTemp;
    int nbZero, i, j;
    nbZero = 0;
    char nomFichier[30];
    FILE *f;
    printf("Nom du fichier ? ");
    scanf("%s", nomFichier);     /**saisie du nom du fichier*/
    f = fopen(nomFichier, "rb"); /**Permet de lire le fichier*/
    if (f == NULL)
    {
        printf("\n ERREUR sur le fichier %s\n", nomFichier); /**Affiche une erreur si le fichier n'est pas trouver*/
    }
    else
    {
        fread(TabTemp, sizeof(int), TAILLE * TAILLE, f); /**insert les valeurs du fichier dans le tableau*/
    }
    fclose(f); /**Ferme le fichier ouvert*/

    for (j = 0; j < TAILLE; j++)
    {
        for (i = 0; i < TAILLE; i++)
        {
            (g[i][j]).valeur = TabTemp[i][j];

            if ((g[i][j]).valeur == 0)
            {
                nbZero += 1;
            }
        }
    }
    return nbZero;
}

/*void initialiserCandidats(tGrille g)
{
    int i, j, index;
    for (j = 0; j < TAILLE; j++)
    {
        for (i = 0; i < TAILLE; i++)
        {
            for (index = 0; index < TAILLE; index++)
            {
                (g[i][j]).candidats[index] = 0;
            }
        }
    }
    (g[i][j]).nbCandidats = 0;
}*/

void affichegrille(tGrille g)
{
    int ligne, colonne, compteur1, compteur2;
    char c;
    compteur1 = 1;
    printf("\n");                            /**retour à la ligne*/
    printf("   ");                           /** 3 espaces pour placer le nombre de ligne correctement*/
    for (ligne = 0; ligne < TAILLE; ligne++) /**première boucle pour */
    {
        if ((ligne == N) || (ligne == 6) || (ligne == TAILLE)) /** affiche le numéro de colonne*/
        {
            printf(" ");
        }

        printf(" %d ", ligne + 1);
    }
    printf("\n");
    printf("  ");
    for (compteur2 = 0; compteur2 < N; compteur2++) /** deuxième boucle pour pour écrire la première ligne séparatrice*/
    {
        printf(CROIX); /**écrit une croix*/
        printf(LIG);   /**écrit une ligne*/
    }
    printf(CROIX);                           /**termine la ligne séparatrice avec une croix*/
    printf("\n");                            /** on revient à la ligne*/
    for (ligne = 0; ligne < TAILLE; ligne++) /** troisième boucle pour, elle va permettre d'écrire l'intérireur du SUDOKU*/
    {
        for (colonne = 0; colonne < TAILLE; colonne++)
        {
            if ((g[ligne][colonne]).valeur == 0) /** permet de remplacer les 0 par des '.'*/
            {
                c = '.';
            }
            else
            {
                c = '0' + (g[ligne][colonne]).valeur;
            }
            if (colonne == 0)
            {
                printf("%d | %c ", compteur1, c); /**affiche le numéro de ligne*/
                compteur1 = compteur1 + 1;
            }
            else
            {
                printf(" %c ", c);
            }
            if ((colonne == 2) || (colonne == 5) || (colonne == TAILLE)) /** permet de savoir si quand on met un séparateur de colonne*/
            {
                printf(COL);
            }
        }
        printf(COL);                                           /**écrit un séparateur de colonne à la fin de la ligne*/
        if ((ligne == 2) || (ligne == 5) || (ligne == TAILLE)) /**Permet d'écrire la séparation de ligne*/
        {
            printf("\n");
            printf("  ");
            for (compteur2 = 0; compteur2 < N; compteur2++)
            {
                printf(CROIX);
                printf(LIG);
            }
            printf(CROIX);
        }
        printf("\n");
    }
    printf("  ");
    for (compteur2 = 0; compteur2 < N; compteur2++) /**écrit le dernier séparateur de ligne à la toute fin*/
    {
        printf(CROIX);
        printf(LIG);
    }
    printf(CROIX);
    printf("\n");
}

void ajoutercandidat(tGrille g, int val, int lig, int col)
{
    int i;
    i = 0;
    while ((g[lig][col]).candidats[i] != 0 || i < TAILLE)
    {
        i += 1;
    }
    (g[lig][col]).candidats[i] = val;
    (g[lig][col]).nbCandidats += 1;
}

void retirerCandidat(tGrille g, int val, int lig, int col, int *nbCandidatsEleminee)
{
    int index, i;
    for (i = 0; i < TAILLE; i++)
    {
        for (index = 0; index < TAILLE; index++)
        {
            if ((g[i][col]).candidats[index] == val)
            {
                (g[i][col]).candidats[index] = 0;
                (g[i][col]).nbCandidats -= 1;
                *nbCandidatsEleminee += 1;
            }
        }
    }

    for (i = 0; i < TAILLE; i++)
    {
        for (index = 0; index < TAILLE; index++)
        {
            if ((g[lig][i]).candidats[index] == val)
            {
                (g[lig][i]).candidats[index] = 0;
                (g[i][col]).nbCandidats -= 1;
                *nbCandidatsEleminee += 1;
            }
        }
    }

    int LigBloc = ((lig - 1) / N) * N;
    int ColBloc = ((col - 1) / N) * N;
    int x, y;

    for (x = LigBloc; x < LigBloc + N; x++)
    {
        for (y = ColBloc; y < ColBloc + N; y++)
        {
            if ((g[x][y]).candidats[index] == val)
            {
                (g[x][y]).candidats[index] = 0;
                (g[i][col]).nbCandidats -= 1;
                *nbCandidatsEleminee += 1;
            }
        }
    }
}

int estCandidat(tCase1 laCase, int val)
{
    bool res;
    int i;
    res = false;
    for (i = 0; i < TAILLE - 1; i++)
    {
        if (laCase.candidats[i] == val)
        {
            res = true;
        }
    }
    return res;
}

int nbCandidats(tCase1 laCase)
{
    return laCase.nbCandidats;
}

bool absentLigne(int val, int ligID, tGrille g)
{
    int i;
    for (i = 0; i < TAILLE; i++)
    {
        if (val == (g[ligID][i]).valeur)
        {
            return false;
        }
    }
    return true;
}

bool absentColonne(int val, int colID, tGrille g)
{
    int i;
    for (i = 0; i < TAILLE; i++)
    {
        if (val == (g[i][colID]).valeur)
        {
            return false;
        }
    }
    return true;
}

bool absentBloc(int val, int i, int j, tGrille g)
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
            if ((g[x][y]).valeur == val)
            {
                return false;
            }
        }
    }

    return true;
}

void CreaCandidat(tGrille g, int *totalCandidats)
{
    int val, j, i;
    for (j = 0; j < TAILLE; j++)
    {
        for (i = 0; i < TAILLE; i++)
        {
            if ((g[i][j]).valeur == 0)
            {
                for (val = 1; val < TAILLE; val++)
                {
                    if ((absentLigne(val, i, g) && absentColonne(val, j, g) && absentBloc(val, i, j, g)) == true)
                    {
                        ajoutercandidat(g, val, i, j);
                        *totalCandidats += 1;
                    }
                }
            }
        }
    }
}

void trie(tGrille g)
{
    int i, j, index;
    index = 0;
    for (j = 0; j < TAILLE; j++)
    {
        for (i = 0; i < TAILLE; i++)
        {
            if ((g[i][j]).valeur == 0)
            {
                while ((g[i][j]).candidats[index] == 0 || index > TAILLE)
                {
                }
            }
        }
    }
}

void AfficheProgression(int nbCasesVidesInit, int nbCasesVides, int nbCandidatsEleminee, int totalCandidats)
{
    int nbCasesRemplies;
    float pourcentCasesRemplies, pourcentCandidat;
    nbCasesRemplies = nbCasesVidesInit - nbCasesVides;
    pourcentCasesRemplies = (nbCasesVides * nbCasesVidesInit) / 100;
    pourcentCandidat = (nbCandidatsEleminee * totalCandidats) / 100;
    printf("\n");
    printf("            ****** résultat grille sudoku ******\n");
    printf("Nombre de cases remplies: %d    taux de remplissage: %.3f pourcents\n", nbCasesRemplies, pourcentCasesRemplies);
    printf("Nombre de candidats éliminés : %d   taux de d'élimination: %.3f pourcents\n", nbCandidatsEleminee, pourcentCandidat);
}

bool backtracking(tGrille grille, int numeroCase)
{
    int ligne;
    int colonne;
    bool resultat;
    int valeur;
    
    resultat = false;
    if ( numeroCase ==  TAILLE * TAILLE )
    {
        //on a traité toutes les cases, la grille est résolue
        resultat = true;
    }
    else
    {
        // On récupère les "coordonnées" de la case
        ligne = numeroCase / TAILLE;
        colonne = numeroCase % TAILLE;
        if ( grille[ligne][colonne].valeur != 0  )
        {
            // Si la case n’est pas vide, on passe à la suivante
            // (appel récursif)
            resultat = backtracking(grille, numeroCase+1);
        }
        else
        {
            for ( valeur = 1 ; valeur <= TAILLE ; valeur++ )
            {
                if (   absentLigne(valeur,grille,ligne)
                    && absentColonne(valeur,grille,colonne)
                    && absentBloc(valeur,grille,ligne,colonne)
                )
                {
                    // Si la valeur est autorisée on l'inscrit
                    //dans la case...
                    grille[ligne][colonne].valeur = valeur;
                    // ... et on passe à la suivante : appel récursif
                    // pour voir si ce choix est bon par la suite
                    if ( backtracking(grille,numeroCase+1) )
                    {
                        resultat = true;
                    }
                    else
                    {
                        grille[ligne][colonne].valeur = 0;
                    }
                }
            }
        }
    }
    return resultat;
}

int main()
{
    tGrille g;
    bool progression;
    int nbCasesVidesInit, nbCasesVides, i, j, val, nbCandidatsEleminee, NBcand, index, totalCandidats;
    nbCandidatsEleminee = 0;
    totalCandidats = 0;
    nbCasesVidesInit = chargerGrille(g);
    nbCasesVides = nbCasesVidesInit;
    // initialiserCandidats(g);
    progression = true;
    affichegrille(g);
    CreaCandidat(g, &totalCandidats);
    while (nbCasesVidesInit > 0 && progression == true)
    {
        progression = false;

        // singleton nue

        for (i = 0; i < TAILLE; i++)
        {
            for (j = 0; j < TAILLE; j++)
            {
                NBcand = 0;
                for (index = 0; index < TAILLE; index++)
                {
                    if ((g[i][j]).candidats[index] != 0)
                    {
                        NBcand += 1;
                    }
                }

                if (NBcand == 1)
                {
                    (g[i][j]).valeur = (g[i][j]).candidats[0];
                    val = (g[i][j]).valeur;
                    nbCasesVides -= 1;
                    retirerCandidat(g, val, i, j, &nbCandidatsEleminee);
                }
            }
        }
        backtracking( g, 1);
        affichegrille(g);
/*
        // singleton caché

        int lig = i / N;
        int col = j / N;
        int initialx, initialy, x, y;
        initialx = lig * N;
        initialy = col * N;
        for (x = initialx; x < initialx + N; x++)
        {
            for (y = initialy; y < initialy + N; y++)
            {
                if ((g[x][y]).valeur == val)
                {
                
                }
            }
        }

        AfficheProgression(nbCasesVidesInit, nbCasesVides, nbCandidatsEleminee, totalCandidats);
    }*/
}
