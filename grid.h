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

/**
 * @brief Create a grid object (square matrix)
 * 
 * @param size int - size of the grid
 * @return char** Pointer to the grid
 */
char** create_grid(int size);

/**
 * @brief Initialize the grid with 0 in it 
 * 
 * @param size int - size of the grid
 * @param grid char** - pointer to the grid
 * @return char** Pointer to the grid
 */
char** initialize_grid(int size, char** grid);

/**
 * @brief Print the grid
 * 
 * @param size int - size of the grid
 * @param grid char** - pointer to the grid
 * @param stream FILE* - stream to print to
 */
void print_grid(int size, char** grid, FILE* stream);

/**
 * @brief Free the grid
 * 
 * @param size int - size of the grid
 * @param grid char** - pointer to the grid
 */
void free_grid(int size, char** grid);

/**
 * @brief Check if the grid contains a char more than once
 * 
 * @param size int - size of the grid
 * @param grid char** - pointer to the grid
 * @param c char - char to check
 * @return Boolean 
 */
Boolean contains_char(int size, char** grid, char c);

/**
 * @brief Generate a random char
 * 
 * @return char the generated char
 */
char gen_rand_char();

/**
 * @brief Check if the grid is correct : if there is no char more than once in each 3*3 sub-grid
 * 
 * @param size int - size of the grid
 * @param grid char** - pointer to the grid
 * @return char** - pointer to the new grid
 */
char** check_grid(int size, char** grid);

/**
 * @brief Fill the grid with random chars following the rules : no char more than once in each 3*3 sub-grid
 * 
 * @param size int - size of the grid
 * @param grid char** - pointer to the grid
 * @return char** pointer to the new grid
 */
char** fill_grid_algo(int size, char** grid);

#endif