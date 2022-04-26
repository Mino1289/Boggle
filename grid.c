#include <grid.h>

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