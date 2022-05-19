#ifndef GAME_H
#define GAME_H

#include <grid.h>

#define MAX_CHAR_ARRAY_LENGTH 25


typedef struct Player {
    char pseudo[MAX_CHAR_ARRAY_LENGTH];
    float score;
    int timeplayed;
    int sizegrid;
} Player;

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
 * @brief Checks if a word is in the grid.
 * MARCHE PAS
 * @param size int - size of the grid.
 * @param grid char** - pointer to the grid to search.
 * @param word char* - the word to search.
 * @return Boolean
 */
Boolean search2D(int size, char** grid, const char* word);

/**
 * @brief Checks if a word is in the dico.
 * 
 * @param word char* - word to search.
 * @return Boolean 
 */
Boolean valid_word(const char* word);

/**
 * @brief Calculate the score of the player.
 * 
 * @param size int - size of the grid.
 * @param sizewords int* - array of length of the words the player found.
 * @return float the score of the player.
 */
float score(int size, int* sizewords);

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
 * @param Player** - list of players (by address).
 */
void read_games(const char* file_path, int* size, Player** players);

/**
 * @brief Print the playerlist.
 *
 * @param stream FILE* - the stream to print to.
 * @param playerlist Player* - the list of players.
 * @param size int - the number of players.
 */
void print_playerlist(FILE* stream, Player* playerlist, int size);

/**
 * @brief Get an integer from the user
 * 
 * @param message char* - the message to print to the user.
 * @param min int - the minimum value of the integer.
 * @param max int - the maximum value of the integer.
 * @return int - the integer entered by the user.
 */
int get_integer_input(const char* message, int min, int max);

/**
 * @brief Get a string from the user
 * 
 * @param message char* - the message to print to the user.
 * @param size int* - the size of the string (by address).
 * @param input char** - the string entered by the user (by address).
 */
void get_string_input(const char* message, int* size, char** input);

/**
 * @brief Start a game.
 * 
 * @return Player - The player object of the player who played. 
 */
Player play();

#endif