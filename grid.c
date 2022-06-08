#include <grid.h>

/**
 * @brief Create a grid object (square matrix) filled with '0'.
 * 
 * @param size int - size of the grid
 * @return char** Pointer to the grid
 */
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
            free_grid(size, grid); // for security
            return NULL;
        }
        for (int j = 0; j < size; j++) {
            grid[i][j] = '0';
        }
    }
    return grid;
}

/**
 * @brief Free the grid
 * 
 * @param size int - size of the grid
 * @param grid char** - pointer to the grid
 */
void free_grid(int size, char** grid) {
    if (grid == NULL) {
        return;
    }
    for (int i = 0; i < size; i++) {
        free(grid[i]);
        grid[i] = NULL;
    }
    free(grid);
    grid = NULL;
}

/**
 * @brief Check if the grid contains a char more than once
 * 
 * @param size int - size of the grid
 * @param grid char** - pointer to the grid
 * @param c char - char to check
 * @return Boolean 
 */
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
 * @brief Generate a random char
 * 
 * @return char the generated char
 */
char gen_rand_char() {
    int lettersdist[] = {8.182, 9.094, 12.443, 16.116, 32.842, 33.909, 34.776, 35.514, 43.101, 43.715, 43.789,
    49.251, 52.222, 59.325, 65.127, 67.651, 69.014, 75.714, 83.671, 90.923, 97.241, 99.081, 99.130, 99.557, 99.685, 100};
    // we've sum all the % of the letters in the french alphabet
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

/**
 * @brief Check if the grid is correct : if there is no char more than once in each 3*3 sub-grid
 * 
 * @param size int - size of the grid
 * @param grid char** - pointer to the grid
 * @return char** - pointer to the new grid
 */
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

/**
 * @brief Fill the grid with random chars following the rules : no char more than once in each 3*3 sub-grid
 * 
 * @param size int - size of the grid
 * @param grid char** - pointer to the grid
 * @return char** pointer to the new grid
 */
char** fill_grid_algo(int size, char** grid) {
    int gap_row = 0;
    int gap_col = 0;
    char** matsup = create_grid(3);

    for (int k = 0; k < pow(size-2,2); k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                matsup[i][j] = grid[gap_col+i][gap_row+j]; 
                // here we gen the letters
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