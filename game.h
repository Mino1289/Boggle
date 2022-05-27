#ifndef GAME_H
#define GAME_H

#include <grid.h>

#define MAX_CHAR_ARRAY_LENGTH 25
#define NB_LETTER 26

#ifdef _WIN32
    #define ACCENT_E 130
    #define ACCENT_A 133
#else //linux : pas d'accent
    #define ACCENT_E 101
    #define ACCENT_A 97
#endif

/**
 * @brief Represent a Player 
 * 
 * @param pseudo char[MAX_CHAR_ARRAY_LENGTH] - The pseudo of the player
 * @param score int - The score of the player
 * @param timeplaued int - The time played by the player
 * @param sizegrid int - The size of the grid of the player
 */
typedef struct Player {
    char pseudo[MAX_CHAR_ARRAY_LENGTH];
    float score;
    int timeplayed;
    int sizegrid;
} Player;

/**
 * @brief Represent a Word
 * 
 * @param length short - The size of the word
 * @param word char* - The word 
 */
typedef struct Word {
    short length;
    char* str;
} Word;

/**
 * @brief Group all the mots by their first letter
 * 
 * @param lettre char - The first letter of the words
 * @param words Word* - A list of words
 * @param nbWords int - The number of words 
 */
typedef struct GrpWords {
    char lettre;
    Word* words;
    int nbWord;
} GrpWords;

/**
 * @brief Represent a coordinate in the grid for a char.
 * 
 * @param c char - The char represented
 * @param row short - The row of the char in the grid
 * @param col short - The col of the char in the grid
 */
typedef struct Coord {
    char c;
    short row;
    short col;
} Coord;

/**
 * @brief Locates all the occurrences of a character in a grid.
 * 
 * @param size int - size of the grid.
 * @param grid char** - grid to search.
 * @param c char - character to search for.
 * @param sizecoords int* - number of occurrences of c in the grid (passing by address).
 * @return int** - Array of coordinates of the occurrences of c in the grid. {{row, col}, {row, col}, ...}.
 */
Coord* locate_char(int size, char** grid, char c, int* sizecoords);

/**
 * @brief 
 * 
 * @param size int - size of the grid
 * @param grid char** - grid to search in
 * @param row int* - position of the current character (by address).
 * @param col int* - position of the current character (by address).
 * @param c char - the current character.
 * @param next char - the next character.
 * @return Boolean Whether the next character is around the current one.
 */
Boolean is_around(int size, char** grid, int* row, int* col, char c, char next);

/**
 * @brief Checks if a word is in the grid.
 * MARCHE PAS
 * @param size int - size of the grid.
 * @param grid char** - pointer to the grid to search.
 * @param word char* - the word to search.
 * @param sizewoze int - size of the word.
 * @return Boolean
 */
Boolean search2D(int size, char** grid, Word word);

/**
 * @brief Read the dictionary file and store it in a list of words, listed by their first letter.
 * 
 * @return GrpWords* - array of list of words grouped by their first letter.
 */
GrpWords* read_dico();

/**
 * @brief Checks if a word is in the dico, using a binary search inside the GrpWords starting with the same letter.
 * 
 * @param word char* - word to search.
 * @param grpwords GrpWords* - array of list of words grouped by their first letter.
 * @return Boolean 
 */
Boolean valid_word(const char* word, GrpWords* grpwords);

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
 * @brief Swap two players in the list of players.
 * 
 * @param playerlist Player* - list of players.
 * @param index1 int - index of the first player.
 * @param index2 int - index of the second player.
 */
void swapPlayer(Player* playerlist, int index1, int index2);

/**
 * @brief Print the playerlist.
 *
 * @param playerlist Player* - the list of players.
 * @param size int - the number of players.
 * @param playercmp func* - Pointer to the compare function.
 */
Player* orderPlayerlist(Player* playerlist, int size, int (*playercmp)(Player p1, Player p2));

/**
 * @brief Compare two Player by their score.
 * 
 * @param p1 Player - the player to compare.
 * @param p2 Player - the player to compare.
 * @return int 0 if their score are the same, -1 if score2 > score1, and 1 if score1 > score2.
 */
int playercmpscore(Player p1, Player p2);

/**
 * @brief Compare two Player by their score and the size of the grid they played on if their score are equal.
 * We suppose same score and smaller grid id better.
 * @param p1 Player - the player to compare.
 * @param p2 Player - the player to compare. 
 * @return int 1 if (score1 > score2) OR (same score AND sizegrid1 < sizegrid2)
 *            -1 if (score2 > score1) OF (same score AND sizegrid1 > sizegrid2)
 *             0 if (same score AND same sizegrid)  
 */
int playercmpscoreANDsize(Player p1, Player p2);

/**
 * @brief Compare two Player by their score and the time they played on if their score are equal.
 * We suppose same score and less time is better.
 * @param p1 Player - the player to compare.
 * @param p2 Player - the player to compare. 
 * @return int 1 if (score1 > score2) OR (same score AND time1 < time2)
 *            -1 if (score2 > score1) OF (same score AND time1 > time2)
 *             0 if (same score AND same time)  
 */
int playercmpscoreANDtime(Player p1, Player p2);

/**
 * @brief Compare two Player by their pseudo (using strcmp)
 * 
 * @param p1 Player - the player to compare.
 * @param p2 Player - the player to compare.
 * @return int -strcmp(p1.pseudo, p2.pseudo)
 */
int playercmppseudo(Player p1, Player p2);

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
 * 
 * @return Word - the word entered by the user.
 */
Word get_string_input(const char* message);

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
 * @brief Free a Word.
 * 
 * @param word Word* - The word to be free (by address) 
 */
void freeWord(Word *word);

/**
 * @brief Free a Group of Words.
 * 
 * @param grpWords GrpWords* - The group of words to be free (by address) 
 */
void freeGrpWords(GrpWords *grpWords);

/**
 * @brief Start a game.
 * 
 * @return Player - The player object of the player who played. 
 */
Player play();

#endif