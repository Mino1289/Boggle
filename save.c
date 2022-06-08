#include <save.h>

/**
 * @brief Save the score of the player.
 * 
 * @param player Player - the player to save.
 * @param file_path char* - the path of the file to save the score.
 */
void save_game(Player player, const char* file_path) {
    FILE* file = fopen(file_path, "a");
    printPlayer(file, player);
    fclose(file);
}

/**
 * @brief Read the list of players from a file.
 * 
 * @param file_path char* the path of the file to read.
 * @param size int - the number of players (by address).
 * @param players Player** - list of players (by address and will be reallocate).
 */
void read_games(const char* file_path, int* size, Player** players) {
    if (*players == NULL) {
        *players = malloc(sizeof(Player));
    }
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not open file %s\n", file_path);
        return;
    }
    Player* playerlist = (Player*) malloc(sizeof(Player)); // we will realloc
    if (playerlist == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for playerlist\n");
        return;
    }
    int i = 0;
    /* file :
    * pseudo    score   sizegrid    timeplayed 
    */
    while (fscanf(file, "%s\t%f\t%d\t%d\n", playerlist[i].pseudo, &(playerlist[i].score), &(playerlist[i].sizegrid), &(playerlist[i].timeplayed)) != EOF) {
        i++;
        playerlist = realloc(playerlist, sizeof(Player)*(i+1));
    }
    *size = i;
    *players = playerlist;
}