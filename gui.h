#ifndef __GUI_H__
#define __GUI_H__

#include <game.h>
#include <grid.h>
#include <save.h>

#ifdef _WIN32
    #define ACCENT_E 130    // é
    #define ACCENT_E2 136   // ê
    #define ACCENT_E1 138   // è
    #define ACCENT_A 133    // à
    #define CEDILLE 135     // ç
#else //linux : pas d'accent
    #define ACCENT_E 101    // e
    #define ACCENT_E1 101   // e
    #define ACCENT_E2 101   // e
    #define ACCENT_A 97     // a
    #define CEDILLE 99      // c
#endif

/**
 * @brief Print the grid
 * 
 * @param size int - size of the grid
 * @param grid char** - pointer to the grid
 */
void print_grid(int size, char** grid);

/**
 * @brief Print a player in a stream
 * 
 * @param stream FILE* - The stream to print to
 * @param player Player - The player to print.
 */
void printPlayer(FILE* stream, Player player);

/**
 * @brief Print the playerlist in a stream.
 * 
 * @param stream FILE* - The stream to print to.
 * @param playerlist Player* - The list of the players.
 * @param size int - The size of the playerlist.
 */
void printPlayerlist(FILE* stream, Player* playerlist, int size);

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
 * @return Word - the word entered by the user.
 */
Word get_string_input(const char* message);

/**
 * @brief Get an input, just a validation to the next step
 * 
 * @param message char* - the message to print to the user 
 */
void validate(const char* message);


/**
 * @brief Prints the logo 'BOGGLE' to stdin.
 * 
 */
void print_logo();

/**
 * @brief Clear stdin.
 * 
 */
void clear();

/**
 * @brief Wait some time.
 * 
 * @param seconds int - the number of seconds to wait
 */
void wait(int seconds);

/**
 * @brief Start a game.
 * 
 * @return Player - The player who played.
 */
Player play();


#endif