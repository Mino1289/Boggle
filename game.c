#include <game.h>
#include <grid.h>

char gen_rand_char() {
    char lettre =' '; // lettre aléatoire retournée 
    int j = 0; // indice commun aux deux tableaux 
    int alea = rand()%100000+1; // génération d'un nombre entier aléatoire 

    int tab_proba[30] = {8.182, 9.084, 12.433, 16.106, 32.832, 33.899, 34.766, 35.504, 43.091, 43.705, 43.779,
                         49.241, 52.212, 59.315, 65.117, 67.641, 69.004, 75.704, 83.661, 90.913, 97.231, 99.071, 99.120, 99.547,
                         99.675, 100.000};
    // tableau de 26 cases auquel on associe les proba sommées multipliées par un facteur 1000 afin d'avoir des entiers
    char liste_lettres[30] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'}; // tableau de lettre

    while(alea < tab_proba[j]){ // tant que 
        if (tab_proba[j]<alea){ // si le tableau de proba est bien inférieur à la valeur générée aléatoirement
            j++; //incrémentation de l'indice j
        }
    }

    lettre=liste_lettres[j]; // affectation d'une lettre égale à celle présente dans le tableau lettre et d'indice identique à celui du tableau de proba

    return lettre; // retourner la lettre 
}

Boolean contains_char(int size, char** grid, char c) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == c) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

void position_chars(int size, char** grid, char c, char next, int* row, int* col) {
    for (int i = *row; i < size; i++) {
        for (int j = *col; j < size; j++) {
            if (grid[i][j] == c) {
                if (grid[i+1][j] == next) {
                    *row = i+1;
                    *col = j;
                    return;
                } else if (grid[i-1][j] == next) {
                    *row = i-1;
                    *col = j;
                    return;
                } else if (grid[i][j+1] == next) {
                    *row = i;
                    *col = j+1;
                    return;
                } else if (grid[i][j-1] == next) {
                    *row = i;
                    *col = j-1;
                    return;
                } else if (grid[i+1][j+1] == next) {
                    *row = i+1;
                    *col = j+1;
                    return;
                } else if (grid[i-1][j-1] == next) {
                    *row = i-1;
                    *col = j-1;
                    return;
                } else if (grid[i-1][j+1] == next) {
                    *row = i-1;
                    *col = j+1;
                    return;
                } else if (grid[i+1][j-1] == next) {
                    *row = i+1;
                    *col = j-1;
                    return;
                }
            }
        }
    }
    *row = -1;
    *col = -1;
    printf("Error: no position found for %c\n", next);
}

Boolean contains_word(int size, char** grid, char* word) {
    int len = strlen(word) - 1;
    int row=0, col=0;
    for (int i = 0; i < len; i++) {
        position_chars(size, grid, word[i], word[i+1], &row, &col);
        if (row == -1 || col == -1) {
            return FALSE;
        }
        printf("%c is in?\n", word[i]);
    }
    if (row == -1 || col == -1) {
        return FALSE;
    }
    return TRUE;    
}