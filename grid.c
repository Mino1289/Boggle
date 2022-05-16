#include <grid.h>
#include <game.h>

char** create_grid(int size) {
    char** grid = malloc(sizeof(char*) * size);
    if (grid == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for grid, size (%d*%d)\n", size, size);
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        grid[i] = malloc(sizeof(char) * size);
        if (grid[i] == NULL) {
            fprintf(stderr, "ERROR: Could not allocate memory for grid rows, size (%d*%d)[%d]\n", size, size, i);
            free_grid(size, grid); // par sécu
            return NULL;
        }
        for (int j = 0; j < size; j++) {
            grid[i][j] = '0';
        }
    }
    return grid;
}


#ifdef _WIN32
void print_grid(FILE* stream, int size, char** grid) {
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            fprintf(stream, "%c%c%c%c", k==0 ? i==0 ? 201 : 204 : i==0 ? 203 : 206, 205, 205, 205);
        }
        fprintf(stream, "%c\n%c", i==0 ? 187 : 185, 186);
        for (int j = 0; j < size; j++) {
            fprintf(stream, " %c %c", grid[i][j], 186);
        }
        fprintf(stream, "\n");
    }
    for (int i = 0; i < size; i++) {
        fprintf(stream,"%c%c%c%c", i==0 ? 200 : 202, 205, 205, 205);
    }
    fprintf(stream, "%c\n\n", 188);
}
#else
void print_grid(FILE* stream, int size, char** grid) {
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            fprintf(stream, "+---");
        }
        fprintf(stream, "+\n|");
        for (int j = 0; j < size; j++) {
            fprintf(stream, " %c |", grid[i][j]);
        }
        fprintf(stream, "\n");
    }
    for (int i = 0; i < size; i++) {
        fprintf(stream,"+---");
    }
    fprintf(stream, "+\n\n");
}
#endif

void free_grid(int size, char** grid) {
    if (grid == NULL) {
        return;
    }
    for (int i = 0; i < size; i++) {
        free(grid[i]);
    }
    free(grid);
}

Boolean contains_char(int size, char** grid, char c) {
    // if we find the char 2 times in the grid, we return true
    int count = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == c) {
                count++;
            }
        }
    }
    if (count >= 2) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @brief MARCHE PO
 * 
 * @return char 
 */
char gen_rand_char() {
    int lettersdist[] = {8.182, 9.094, 12.443, 16.116, 32.842, 33.909, 34.776, 35.514, 43.101, 43.715, 43.789,
    49.251, 52.222, 59.325, 65.127, 67.651, 69.014, 75.714, 83.671, 90.923, 97.241, 99.081, 99.130, 99.557, 99.685, 100};
    float nombre = rand() / 100.0f;
    while (nombre >= 100) {
        nombre = (rand()) / 100.0f;
    }
    int i = 0;
    while (i < 26 && nombre > lettersdist[i]) {
        i++;
    }
    return 'a' + i;
}

char** check_grid(int size, char** grid) {
    Boolean modif = FALSE;
    char** matsup = create_grid(3);
    int gap_col = 0, gap_row = 0;
    do {
        modif = FALSE;
        gap_col = 0;
        gap_row = 0;
        for (int k = 0; k < pow(size-2,2); k++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    matsup[i][j] = grid[gap_col+i][gap_row+j]; 
                }
            }
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    char c = matsup[i][j];
                    while (contains_char(3, matsup, c)) {
                        c = gen_rand_char();
                        matsup[i][j] = c;
                        modif = TRUE;
                    }
                    grid[gap_col+i][gap_row+j] = c;
                }
            }
            if (gap_row+3 >= size) {
                gap_row = 0;
                gap_col += 1;
            } else {
                gap_row += 1;
            }
        }
    } while (modif);
    free_grid(3, matsup);
    return grid;
}

char** fill_grid_algo(int size, char** grid) {
    int gap_row = 0;
    int gap_col = 0;
    char** matsup = create_grid(3);

    for (int k = 0; k < pow(size-2,2); k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                matsup[i][j] = grid[gap_col+i][gap_row+j]; 
                // ici on va gen les lettres
                if (matsup[i][j] == '0') {
                    char c;
                    do {
                        c = gen_rand_char();
                        matsup[i][j] = c;
                    } while (contains_char(3, matsup, c));
                }
                grid[gap_col+i][gap_row+j] = matsup[i][j];
            }
        }
        if (gap_row+3 >= size) {
            gap_row = 0;
            gap_col += 1;
        } else {
            gap_row += 1;
        }
    }   
    grid = check_grid(size, grid);
    free_grid(3, matsup);
    return grid;
}