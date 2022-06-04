#include <game.h>

Coord* locate_char(int size, char** grid, char c, int *sizecoords) {
    Coord* coords = malloc(sizeof(Coord*));
    if (coords == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for coords.\n");
        return NULL;
    }
    int k = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == c) {
                coords[k] = (Coord) {c, i, j};
                k++;

                coords = realloc(coords, sizeof(Coord)*(k+1));
                if (coords == NULL) {
                    fprintf(stderr, "ERROR: Could not reallocate memory for coords[%d].\n",k);
                    return NULL;
                }
            }
        }
    }
    *sizecoords = k;
    return coords;
}

Boolean is_around(int size, char** grid, int* row, int* col, char c, char next) {
    if (grid[*row][*col] == c) {
        if (*row > 0 && grid[*row-1][*col] == next) {
            *row -= 1;
            return TRUE;
        } else if (*row < size-1 && grid[*row+1][*col] == next) {
            *row += 1;
            return TRUE;
        } else if (*col > 0 && grid[*row][*col-1] == next) {
            *col -= 1;
            return TRUE;
        } else if (*col < size-1 && grid[*row][*col+1] == next) {
            *col += 1;
            return TRUE;
        } else if (*row > 0 && *col > 0 && grid[*row-1][*col-1] == next) {
            *row -= 1;
            *col -= 1;
            return TRUE;
        } else if (*row > 0 && *col < size-1 && grid[*row-1][*col+1] == next) {
            *row -= 1;
            *col += 1;
            return TRUE;
        } else if (*row < size-1 && *col > 0 && grid[*row+1][*col-1] == next) {
            *row += 1;
            *col -= 1;
            return TRUE;
        } else if (*row < size-1 && *col < size-1 && grid[*row+1][*col+1] == next) {
            *row += 1;
            *col += 1;
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        fprintf(stderr, "ERROR: The character at (%d, %d) is not %c.\n", *row, *col, c);
        return FALSE;
    }
}

Boolean search2D(int size, char** grid, Word word) {

    int sizeword = word.length;
    int sizecoords;
    Coord* coords = locate_char(size, grid, word.str[0], &sizecoords);
    Coord* verif = (Coord*) malloc(sizeof(Coord*)*sizeword);
    int k = 0;
    Boolean found = FALSE, duplicate;
    while (k < sizecoords && !found) {
        duplicate = FALSE;
        int row = coords[k].row, col = coords[k].col, i = 1;

        verif[0] = (Coord) {word.str[i], row, col};
        while (i < sizeword-1 && is_around(size, grid, &row, &col, word.str[i-1], word.str[i])) {
            verif[i] = (Coord) {word.str[i-1], row, col};
            i++;
        }

        // we check there is no two times the same coord
        // we perform an algorithm similar to check if there is doublons in a list

        if (i == sizeword-1) {
            int j = 0, l;
            while (j < sizeword && !duplicate) {
                l = j+1;
                while (l < sizeword && (verif[j].row != verif[l].row || verif[j].col != verif[l].col)) {
                    l++;
                }
                if (l < sizeword) {
                    duplicate = TRUE;
                }
                j++;
            }
            found = !duplicate;
        }
        k++;
    }
    free(coords);
    free(verif);
    
    return found;
}

GrpWords* read_dico() {
    const char* file_path = "dico.txt";
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not open file %s\n", file_path);
        return NULL;
    }
    GrpWords* grpwords = malloc(sizeof(GrpWords)*NB_LETTER); // we know there are 26 groups (26 letters)
    if (grpwords == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for grpwords.\n");
        return NULL;
    }
    for (int i = 0; i < NB_LETTER; i++) {
        grpwords[i].nbWord = 1;
        grpwords[i].words = (Word*) malloc(sizeof(Word));
        if (grpwords[i].words == NULL) {
            fprintf(stderr, "ERROR: Could not allocate memory for grpwords[%d].word.\n", i);
            return NULL;
        }
        grpwords[i].lettre = 'a' + i;
    }
    int i = 0, k = 0;
    
    while (!feof(file)) {
        char buffer [MAX_CHAR_ARRAY_LENGTH];
        int len;
        fscanf(file, "%s\t%d", buffer, &len);
        if (i != buffer[0] - 'a') { // letter change => we put k = 0
            k = 0;  
            i = buffer[0] - 'a'; 
        }

        grpwords[i].words[k].length = len; // length
        grpwords[i].words[k].str = malloc(sizeof(char)*(grpwords[i].words[k].length+1)); //with the '\0'
        if (grpwords[i].words[k].str == NULL) {
            fprintf(stderr, "ERROR: Could not allocate memory for grpwords[%d].words[%d].str.\n", i, k);
            return NULL;
        }
        strncpy(grpwords[i].words[k].str, buffer, grpwords[i].words[k].length+1); 
        
        grpwords[i].nbWord++;
        grpwords[i].words = realloc(grpwords[i].words, sizeof(Word)*(grpwords[i].nbWord));
        if (grpwords[i].words == NULL) {
            fprintf(stderr, "ERROR: Could not reallocate memory for grpwords[%d].\n", i);
            return NULL;
        }
        k++; 
    }
    fclose(file);
    return grpwords;
}

Boolean valid_word(const char* word, GrpWords* grpwords) {
    char frstletter = word[0];
    int indexletter = frstletter - 'a';

    Boolean found = FALSE;
    int bas = 0;
    int haut = grpwords[indexletter].nbWord - 1;
    int milieu;
    while (bas <= haut && !found) {
        milieu = (bas + haut) / 2;
        int cmp = strcmp(grpwords[indexletter].words[milieu].str, word);
        if (cmp == 0) {
            found = TRUE;
        } else if (cmp < 0) {
            bas = milieu + 1;
        } else {
            haut = milieu - 1;
        }
    }
    return found;
}


float score(int size, Word* words) {
    float score = 0;
    for (int i = 0; i < size; i++) {
        score += pow(words[i].length, 4.0/3.0); // maybe use a series of polynomials
    }
    return score;
}

void swapPlayer(Player* playerlist, int index1, int index2) {
    Player tmp = playerlist[index1];
    playerlist[index1] = playerlist[index2];
    playerlist[index2] = tmp;
}

Player* orderPlayerlist(Player* playerlist, int size, int (*playercmp)(Player p1, Player p2)) {
    // we use a bubble sort
    Boolean weswap;
    int loop = 0;
    do {
        weswap = FALSE;
        for (int i = 0; i < size-1-loop; i++) {
            if (playercmp(playerlist[i], playerlist[i+1]) < 0) {
                swapPlayer(playerlist, i, i+1);
                weswap = TRUE;
            }
        }
        loop++;
    } while (weswap);
    return playerlist;
}

int playercmpscore(Player p1, Player p2) {
    if (p1.score > p2.score) {
        return 1;
    } else if (p1.score < p2.score) {
        return -1;
    }
    return 0; // equal
}

int playercmpsizeANDscore(Player p1, Player p2) {
    if (p1.sizegrid > p2.sizegrid) {
        return 1;
    } else if (p1.sizegrid < p2.sizegrid) {
        return -1;
    } else {
        return playercmpscore(p1, p2);
    }
}

int playercmptimeANDscore(Player p1, Player p2) {
    if (p1.timeplayed < p2.timeplayed) {
        return -1;
    } else if (p1.timeplayed > p2.timeplayed) {
        return 1;
    } else {
        return playercmpscore(p1, p2);
    }
}

int playercmpsizeANDtimeANDscore(Player p1, Player p2) {
    if (p1.sizegrid > p2.sizegrid) {
        return 1;
    } else if (p1.sizegrid < p2.sizegrid) {
        return -1;
    } else {
        if (p1.timeplayed < p2.timeplayed) {
            return 1;
        } else if (p1.timeplayed > p2.timeplayed) {
            return -1;
        } else {
            return playercmpscore(p1, p2);
        }
    }
}

int playercmppseudo(Player p1, Player p2) {
    return -strcmp(p1.pseudo, p2.pseudo);
}

int isPseudoInList(Player* playerlist, int size, Word pseudo) {
    Boolean found = FALSE;
    int bas = 0;
    int haut = size - 1;
    int milieu;
    while (bas <= haut && !found) {
        milieu = (bas + haut) / 2;
        int cmp = strcmp(playerlist[milieu].pseudo, pseudo.str);
        if (cmp == 0) {
            found = TRUE;
        } else if (cmp < 0) {
            bas = milieu + 1;
        } else {
            haut = milieu - 1;
        }
    }
    if (found)
        return milieu;
    return -1;
}


void freeWord(Word *word) {
    free(word->str);
    *word = (Word) {0, NULL}; // legal ?
}

void freeGrpWords(GrpWords *grpwords) {
    for (int j = 0; j < grpwords->nbWord-1; j++) {
        freeWord(&(grpwords->words[j]));
    }
    free(grpwords->words);
    *grpwords = (GrpWords) {'\0', NULL, 0};
}

