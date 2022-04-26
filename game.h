#ifndef GAME_H
#define GAME_H

#include <grid.h>

char gen_rand_char();

Boolean contains_char(int size, char** grid, char c);

void position_chars(int size, char** grid, char c, char next, int* row, int* col);

Boolean contains_word(int size, char** grid, char* word);

#endif