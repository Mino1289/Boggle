#ifndef GRID_H
#define GRID_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef enum {
    FALSE = 0,
    TRUE = 1
} Boolean;

char** create_grid(int size);

char** initialize_grid(int size, char** grid);

void print_grid(int size, char** grid);

void free_grid(int size, char** grid);

char** fill_grid_algo(int size, char** grid);

#endif