#ifndef GENERATION_H
#define GENERATION_H

#include <grid.h>

char gen_rand_char();

char** fill_grid_algo(int size, char** grid);

Boolean contains_char(int size, char** grid, char c);

#endif