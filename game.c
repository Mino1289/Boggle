#include <game.h>

char gen_rand_char() {
    int c = rand() % 26;
    return 'a'+c;
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