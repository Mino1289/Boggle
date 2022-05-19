#include <game.h>
#include <grid.h>

int** locate_char(int size, char** grid, char c, int* sizecoords) {
    int** coords = malloc(sizeof(int*));
    if (coords == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for coords.\n");
        return NULL;
    }
    int k = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == c) {
                coords[k] = malloc(sizeof(int)*2);
                if (coords[k] == NULL) {
                    fprintf(stderr, "ERROR: Could not allocate memory for coords[%d].\n", k);
                    return NULL;
                }
                coords[k][0] = i;
                coords[k][1] = j;
                k++;

                coords = realloc(coords, sizeof(int*)*(k+1));
                if (coords == NULL) {
                    fprintf(stderr, "ERROR: Could not reallocate memory for coords[%d].\n",k);
                    return NULL;
                }
            }
        }
    }
    *sizecoords = k;
    return coords;
}

Boolean is_around(int size, char** grid, int* row, int* col, char c, char next) {
    if (grid[*row][*col] == c) {
        if (*row > 0 && grid[*row-1][*col] == next) {
            *row -= 1;
            return TRUE;
        } else if (*row < size-1 && grid[*row+1][*col] == next) {
            *row += 1;
            return TRUE;
        } else if (*col > 0 && grid[*row][*col-1] == next) {
            *col -= 1;
            return TRUE;
        } else if (*col < size-1 && grid[*row][*col+1] == next) {
            *col += 1;
            return TRUE;
        } else if (*row > 0 && *col > 0 && grid[*row-1][*col-1] == next) {
            *row -= 1;
            *col -= 1;
            return TRUE;
        } else if (*row > 0 && *col < size-1 && grid[*row-1][*col+1] == next) {
            *row -= 1;
            *col += 1;
            return TRUE;
        } else if (*row < size-1 && *col > 0 && grid[*row+1][*col-1] == next) {
            *row += 1;
            *col -= 1;
            return TRUE;
        } else if (*row < size-1 && *col < size-1 && grid[*row+1][*col+1] == next) {
            *row += 1;
            *col += 1;
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        fprintf(stderr, "ERROR: The character at (%d, %d) is not %c.\n", *row, *col, c);
        return FALSE;
    }
}

Boolean search2D(int size, char** grid, const char* word) {

    int sizeword = strlen(word);
    int sizecoords;
    int** coords = locate_char(size, grid, word[0], &sizecoords);
    int k = 0;
    Boolean found = FALSE;
    while (k < sizecoords && !found) {
        int row = coords[k][0];
        int col = coords[k][1];
        int i = 1;
        while (i < sizeword && is_around(size, grid, &row, &col, word[i-1], word[i])) {
            i++;
        }
        if (i == sizeword) {
            found = TRUE;
        }
        k++;
    }
    
    return found;
}

Boolean valid_word(const char* word) {
    const char* file_path = "dico.txt";
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not open file %s\n", file_path);
        return FALSE;
    }
    char line[50];
    Boolean found = FALSE;
    while (fgets(line, sizeof(line), file) != NULL && !found) {
        if (strncmp(line, word, strlen(word)-1) == 0) {
            found = TRUE;
        }
    }
    fclose(file);
    return found;
}


float score(int size, int* sizewords) {
    float score = 0;
    for (int i = 0; i < size; i++) {
        score += pow(sizewords[i], 4.0/3.0);
    }
    return score;
}

void save_game(Player player, const char* file_path) {
    FILE* file = fopen(file_path, "a");
    fprintf(file, "%s\t%.2f\t%d\t%d\n", player.pseudo, player.score, player.sizegrid, player.timeplayed);
    fclose(file);
}

void read_games(const char* file_path, int* size, Player** players) {
    if (*players == NULL) {
        *players = malloc(sizeof(Player));
    }
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not open file %s\n", file_path);
        return;
    }
    Player* playerlist = (Player*) malloc(sizeof(Player)); // on va réalloc
    if (playerlist == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for playerlist\n");
        return;
    }
    int i = 0;
    /* FILE :
    * pseudo    score   sizegrid    timeplayed 
    */
    while (fscanf(file, "%s\t%f\t%d\t%d\n", playerlist[i].pseudo, &(playerlist[i].score), &(playerlist[i].sizegrid), &(playerlist[i].timeplayed)) != EOF) {
        i++;
        playerlist = realloc(playerlist, sizeof(Player)*(i+1));
    }
    *size = i;
    *players = playerlist;
}

void swapPlayer(Player* playerlist, int index1, int index2) {
    Player tmp = playerlist[index1];
    playerlist[index1] = playerlist[index2];
    playerlist[index2] = tmp;
}

void orderAndPrint_playerlist(FILE* stream, Player* playerlist, int size) {
    //on bubble sort puis on print tout
    Boolean weswap;
    int loop = 0;
    do {
        weswap = FALSE;
        for (int i = 0; i < size-1-loop; i++) {
            if (playerlist[i].score < playerlist[i+1].score) {
                swapPlayer(playerlist, i, i+1);
                weswap = TRUE;
            }
        }
        loop++;
    } while (weswap);

    for (int i = 0; i < size; i++) {
        fprintf(stream, "%s\t%.2f\t%d\t%d\n", playerlist[i].pseudo, playerlist[i].score, playerlist[i].sizegrid, playerlist[i].timeplayed);
    }
}

int get_integer_input(const char* message, int min, int max) {
    char inputstring[MAX_CHAR_ARRAY_LENGTH];
    int tmp, input=0;
    do {
        printf("%s", message);
        scanf("%s", inputstring);
        tmp = atoi(inputstring);
        if (tmp >= min && tmp <= max) {
            input = tmp;
        } else {
            fprintf(stderr, "ERROR: %d is not a valid size. Asking again\n", input);
        }
    } while (input == 0);

    return input;
}

void get_string_input(const char* message, int* size, char** input) {
    printf("%s\n", message);
    char temp[MAX_CHAR_ARRAY_LENGTH];
    scanf("%s", temp);
    int k = strlen(temp)+1, i;
    (*input) = malloc(sizeof(char)*k);

    for (i = 0; i < k; i++) {
        (*input)[i] = temp[i];
    }
    (*input)[k] = '\0';
    *size = k;
}

void clear() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void wait(int seconds) {
    double dtime = time(0);
    while (difftime(time(0), dtime) < seconds) {
        //wait
    }
}

Player play() {
    Player player;
    int yes=1;
    do {
        char* name = NULL;
        int sizename;
        get_string_input("Choisissez un pseudo : ", &sizename, &name);
        
        int n, i = 0;
        Player* gamelist = (Player*) malloc(sizeof(Player));
        read_games("scores.txt", &n, &gamelist);
        while (i <= n && strcmp(gamelist[i].pseudo, name) != 0) {
            i++;
        }

        if (i <= n) {
            printf("Le pseudo %s a déjà été utilisé.\n", name);
            printf("Vous ne pouvez pas le réutiliser.\n");
            yes = 0;
        } else {
            memcpy(player.pseudo, name, sizename);
            yes = 1;
        }
        free(gamelist);
        free(name);
    } while (yes == 0);
    printf("Vous allez jouer en tant que : %s\n", player.pseudo);
    
    //  // ^ faut demander au joueur un pseudo, si il est pas pris, il le prend, sinon il en choisi un autre.

    time_t debut = time(0);
    int size = get_integer_input("Choisissez la taille de votre grille (4-8): ", 4, 8);
    player.sizegrid = size;

    char** grid = create_grid(size);
    grid = fill_grid_algo(size, grid);
    
    double playtime = (double) get_integer_input("Choisissez le temps de jeu (60s-180s): ", 60, 180);
    player.timeplayed = playtime;
    
    int* wordslen = (int*) malloc(sizeof(int));
    if (wordslen == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for wordslen\n");
        return player;
    }

    int sizewords = 0, iter = 1;
    double dtime = 0;

    print_grid(size, grid);
    do {
        int sizeword = 0, i = 0;
        char* word = NULL;

        get_string_input("Entrez un mot: ", &sizeword, &word);
        if (search2D(size, grid, word)) {
            if (valid_word(word)) {
                printf("Le mot %s est dans la grille\n", word);
                wordslen[i] = sizeword-1;
  
                i++;
                wordslen = realloc(wordslen, sizeof(int)*(i+1));
            } else {
                printf("Le mot %s n'est pas dans le dictionnaire\n", word);
            }
        } else {
           printf("Le mot %s n'est pas dans la grille\n", word);
        }
        if (iter % 3 == 0){
            wait(1);
            clear();
            print_grid(size, grid);
        }
        iter++;

        sizewords = i;
        free(word);
        dtime = difftime(time(0), debut);
        printf("Temps %ccoul%c : %.2fs sur %.2f\n", ACCENT_E, ACCENT_E,dtime, playtime);
    } while (dtime < playtime);
    player.score = score(sizewords, wordslen);
    printf("C'est fini, votre score est de %.2f\n", player.score);
    wait(2);
    save_game(player, "scores.txt");

    free_grid(size, grid);
    free(wordslen);
    return player;
}
