#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char** create_grid(int size);

char** initialize_grid(char** grid, int size);

void print_grid(int size, char** grid);

void free_grid(int size, char** grid);