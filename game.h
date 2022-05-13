#ifndef GAME_H
#define GAME_H

#include <grid.h>

/**
 * @brief Locates all the occurrences of a character in a grid.
 * 
 * @param size int - size of the grid.
 * @param grid char** - grid to search.
 * @param c char - character to search for.
 * @param sizecoords int* - number of occurrences of c in the grid (passing by address).
 * @return int** - Array of coordinates of the occurrences of c in the grid. {{row, col}, {row, col}, ...}.
 */
int** locate_char(int size, char** grid, char c, int* sizecoords);

/**
 * @brief Checks if a char is around a given position.
 * 
 * @param size int - size of the grid.
 * @param grid char** - pointer to the grid to check.
 * @param row int* - pointer to the row of the position to check.
 * @param col int* - pointer to the column of the position to check.
 * @param next char - char to check.
 * @return Boolean 
 */
Boolean is_around(int size, char** grid, int *row, int *col, char next);

/**
 * @brief Checks if a word is in the grid.
 * 
 * @param size int - size of the grid.
 * @param grid char** - pointer to the grid to search.
 * @param word char* - pointer to the word to search.
 * @return Boolean
 */
Boolean contains_word(int size, char** grid, const char* word);

int score(int size, int* sizewords);

#endif