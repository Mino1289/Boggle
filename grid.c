#include <grid.h>
#include <game.h>

char** create_grid(int size) {
    char** grid = malloc(sizeof(char*) * size);
    for (int i = 0; i < size; i++) {
        grid[i] = malloc(sizeof(char) * size);
    }
    return grid;
}

char** initialize_grid(int size, char** grid) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            grid[i][j] = '0';
        }
    }
    return grid;
}


void print_grid(int size, char** grid) {
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            printf("+---");
        }
        printf("+\n|");
        for (int j = 0; j < size; j++) {
            printf(" %c |", grid[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < size; i++) {
        printf("+---");
    }
    printf("+\n\n");
}

void free_grid(int size, char** grid) {
    if (grid == NULL) {
        return;
    }
    for (int i = 0; i < size; i++) {
        free(grid[i]);
    }
    free(grid);
}

char** fill_grid_algo(int size, char** grid) {
    int gap_row = 0;
    int gap_col = 0;
    char** matsup = create_grid(3);

    for (int k = 0; k < pow(size-2,2); k++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                matsup[i][j] = grid[gap_col+i][gap_row+j]; //grid[col][row]
                // ici on va gen les lettres
                if (matsup[i][j] == '0') {
                    char c =gen_rand_char();
                    while (contains_char(3, matsup, c)) {
                        c =gen_rand_char();
                    }
                    matsup[i][j] = c;
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
    free_grid(3, matsup);
    return grid;
}