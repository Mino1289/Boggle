#include <game.h>


int** locate_char(int size, char** grid, char c, int* sizecoords) {
    int** coords = malloc(sizeof(int*));
    int k = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == c) {
                coords[k] = malloc(sizeof(int)*2);
                coords[k][0] = i;
                coords[k][1] = j;
                k++;

                coords = realloc(coords, sizeof(int*)*(k));
            }
        }
    }
    *sizecoords = k;
    return coords;
}

Boolean is_around(int size, char** grid, int *row, int *col, char next) {
    if (*row-1 >= 0 && grid[*row-1][*col] == next) {
        (*row)--;
        return TRUE;
    } else if (*row+1 < size && grid[*row+1][*col] == next) {
        (*row)++;
        return TRUE;
    } else if (*col-1 >= 0 && grid[*row][*col-1] == next) {
        (*col)--;
        return TRUE;
    } else if (*col+1 < size && grid[*row][*col+1] == next) {
        (*col)++;
        return TRUE;
    } else if (*row-1 >= 0 && *col-1 >= 0 && grid[*row-1][*col-1] == next) {
        (*row)--;
        (*col)--;
        return TRUE;
    } else if (*row-1 >= 0 && *col+1 < size && grid[*row-1][*col+1] == next) {
        (*row)--;
        (*col)++;
        return TRUE;
    } else if (*row+1 < size && *col-1 >= 0 && grid[*row+1][*col-1] == next) {
        (*row)++;
        (*col)--;
        return TRUE;
    } else if (*row+1 < size && *col+1 < size && grid[*row+1][*col+1] == next) {
        (*row)++;
        (*col)++;
        return TRUE;
    } else {
        (*row) = -1;
        (*col) = -1;
        return FALSE;
    }
}

Boolean contains_word(int size, char** grid, const char* word) {
    int wordlen = strlen(word); // size of the word -1
    int sizecoords; // number of coordinates
    int** coords = locate_char(size, grid, word[0], &sizecoords);
    for (int k = 0; k < sizecoords; k++) {
        int row = coords[k][0];
        int col = coords[k][1];
        int p = 0; // position in the word
        while (p<wordlen-1 && (row != -1 || col != -1) && is_around(size, grid, &row, &col, word[p+1])) {
            p++;
        }
        if (p == wordlen-1) {
            return TRUE;
        }
    }

    
    for (int i = 0; i < sizecoords; i++) {
        free(coords[i]);
    }
    free(coords);
    return FALSE;
}

float score(int size, int* sizewords) {
    float score = 0;
    for (int i = 0; i < size; i++) {
        score += pow(sizewords[i], 4.0/3.0);
    }
    return score;
}

void save_game(Player player, const char* file_path) {
    FILE* file = fopen(file_path, "a");
    fprintf(file, "%s\t%.2f", player.pseudo, player.score);
    fclose(file);
}

Player* read_games(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    Player* playerlist = (Player*) malloc(sizeof(Player)); // on va réalloc
    int i = 0;
    while (sscanf(file, "%s\t%d", &(playerlist[i].pseudo), &(playerlist[i].score)) != EOF) {
        i++;
        playerlist = realloc(sizeof(Player)*i);
    }
    return playerlist;
}