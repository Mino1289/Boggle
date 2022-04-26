#include <generation.h>

char gen_rand_char() {
    int c = rand() % 26;
    return 'a'+c;
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
                    char c = gen_rand_char();
                    while (contains_char(3, matsup, c)) {
                        c = gen_rand_char();
                    }
                    matsup[i][j] = c;
                }
                grid[gap_col+i][gap_row+j] = matsup[i][j];
            }
        }
        print_grid(3, matsup);


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