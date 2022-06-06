#ifndef __SAVE_H__
#define __SAVE_H__

#include <gui.h>

/**
 * @brief Save the score of the player.
 * 
 * @param player Player - the player to save.
 * @param file_path char* - the path of the file to save the score.
 */
void save_game(Player player, const char* file_path);

/**
 * @brief Read the list of players from a file.
 * 
 * @param file_path char* the path of the file to read.
 * @param size int - the number of players (by address).
 * @param players Player** - list of players (by address and will be reallocate).
 */
void read_games(const char* file_path, int* size, Player** players);

#endif