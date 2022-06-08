#include <game.h>

/**
 * @brief Locates all the occurrences of a character in a grid.
 * 
 * @param size int - size of the grid.
 * @param grid char** - grid to search.
 * @param c char - character to search for.
 * @param sizecoords int* - number of occurrences of c in the grid (passing by address).
 * @return int** - Array of coordinates of the occurrences of c in the grid. {{c, row, col}, {c, row, col}, ...}.
 */
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

/**
 * @brief 
 * 
 * @param size int - size of the grid
 * @param grid char** - grid to search in
 * @param row int* - position of the current character (by address).
 * @param col int* - position of the current character (by address).
 * @param c char - the current character.
 * @param next char - the next character.
 * @return Boolean Whether the next character is around the current one.
 */
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

/**
 * @brief Checks if a word is in the grid.
 * @param size int - size of the grid.
 * @param grid char** - pointer to the grid to search.
 * @param word char* - the word to search.
 * @param sizewoze int - size of the word.
 * @return Boolean
 */
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

/**
 * @brief Read the dictionary file and store it in a list of words, listed by their first letter.
 * 
 * @return GrpWords* - array of list of words grouped by their first letter.
 */
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

/**
 * @brief Checks if a word is in the dico, using a binary search inside the GrpWords starting with the same letter.
 * 
 * @param word Word - The word to search.
 * @param grpwords GrpWords* - array of list of words grouped by their first letter.
 * @return Boolean 
 */
Boolean valid_word(const Word word, GrpWords* grpwords) {
    char frstletter = word.str[0];
    int indexletter = frstletter - 'a';

    Boolean found = FALSE;
    int bas = 0;
    int haut = grpwords[indexletter].nbWord - 1;
    int milieu;
    while (bas <= haut && !found) {
        milieu = (bas + haut) / 2;
        int cmp = strcmp(grpwords[indexletter].words[milieu].str, word.str);
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

/**
 * @brief Calculate the score of the player.
 * 
 * @param size int - size of the grid.
 * @param words Word* - array of the words the player found.
 * @return float the score of the player.
 */
float score(int size, Word* words) {
    float score = 0;
    for (int i = 0; i < size; i++) {
        score += pow(words[i].length, 4.0/3.0); // maybe use a series of polynomials
    }
    return score;
}

/**
 * @brief Swap two players in the list of players.
 * 
 * @param playerlist Player* - list of players.
 * @param index1 int - index of the first player.
 * @param index2 int - index of the second player.
 */
void swapPlayer(Player* playerlist, int index1, int index2) {
    Player tmp = playerlist[index1];
    playerlist[index1] = playerlist[index2];
    playerlist[index2] = tmp;
}

/**
 * @brief Order the playerlist.
 *
 * @param playerlist Player* - the list of players.
 * @param size int - the number of players.
 * @param playercmp func* - Pointer to the compare function.
 */
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

/**
 * @brief Compare two Player by their score.
 * 
 * @param p1 Player - the player to compare.
 * @param p2 Player - the player to compare.
 * @return int 0 if their score are the same, -1 if score2 > score1, and 1 if score1 > score2.
 */
int playercmpscore(Player p1, Player p2) {
    if (p1.score > p2.score) {
        return 1;
    } else if (p1.score < p2.score) {
        return -1;
    }
    return 0; // equal
}

/**
 * @brief Compare two Player by their score and the size of the grid they played on if their score are equal.
 * We suppose same score and smaller grid id better.
 * @param p1 Player - the player to compare.
 * @param p2 Player - the player to compare. 
 * @return int  1 if (size1 > size2) OR (size1 == size2 AND score1 > score2), 
 *             -1 if (size1 < size2) OR (size1 == size2 AND score1 < score2), 
 *              0 if their score and size are the same.
 */
int playercmpsizeANDscore(Player p1, Player p2) {
    if (p1.sizegrid > p2.sizegrid) {
        return 1;
    } else if (p1.sizegrid < p2.sizegrid) {
        return -1;
    } else {
        return playercmpscore(p1, p2);
    }
}

/**
 * @brief Compare two Player by their score and the time they played on if their score are equal.
 * We suppose same score and less time is better.
 * @param p1 Player - the player to compare.
 * @param p2 Player - the player to compare. 
 * @return int 1 if (time1 > time2) OR (time1 == time2 AND score1 > score2),
 *            -1 if (time1 < time2) OR (time1 == time2 AND score1 < score2),
 *            0 if their score and time are the same.
 */
int playercmptimeANDscore(Player p1, Player p2) {
    if (p1.timeplayed < p2.timeplayed) {
        return -1;
    } else if (p1.timeplayed > p2.timeplayed) {
        return 1;
    } else {
        return playercmpscore(p1, p2);
    }
}

/**
 * @brief Compare two Player by the size of the grid they played on and the time they played if their score are equal and by score.
 * 
 * @param p1 Player - the player to compare.
 * @param p2 Player - the player to compare.
 * @return int 1 if (size1 > size2) OR (size1 == size2 AND time1 > time2) OR (size1 == size2 AND time1 == time2 AND score1 > score2),
 *            -1 if (size1 < size2) OR (size1 == size2 AND time1 < time2) OR (size1 == size2 AND time1 == time2 AND score1 < score2),
 *             0 if their score, size and time are the same.
 */
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

/**
 * @brief Compare two Player by their pseudo (using strcmp)
 * 
 * @param p1 Player - the player to compare.
 * @param p2 Player - the player to compare.
 * @return int -strcmp(p1.pseudo, p2.pseudo)
 */
int playercmppseudo(Player p1, Player p2) {
    return -strcmp(p1.pseudo, p2.pseudo);
}

/**
 * @brief Checks if a player is in the list of players. Using a binary search.
 * 
 * @param playerlist Player* - The list of players.
 * @param size int - The number of players.
 * @param player Player - The player to search.
 * @return int - The index of the player in the list of players (-1 if not in the list.) 
 */
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

/**
 * @brief Free a Word.
 * 
 * @param word Word* - The word to be free (by address) 
 */
void freeWord(Word *word) {
    free(word->str);
    word->str = NULL;
    word->length = 0;
}

/**
 * @brief Free a Group of Words.
 * 
 * @param grpWords GrpWords* - The group of words to be free (by address) 
 */
void freeGrpWords(GrpWords *grpwords) {
    for (int j = 0; j < grpwords->nbWord-1; j++) {
        freeWord(&(grpwords->words[j]));
    }
    free(grpwords->words);
    grpwords->lettre = '\0';
    grpwords->words = NULL;
    grpwords->nbWord = 0;
}

