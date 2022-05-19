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

Boolean search2D(int size, char** grid, const char* word) {
    int x[8] = { -1, -1, -1,  0, 0,  1, 1, 1 };
    int y[8] = { -1,  0,  1, -1, 1, -1, 0, 1 };

    int sizeword = strlen(word);
    // int sizecoords;
    // int** coords = locate_char(size, grid, word[0], &sizecoords);
    int j = 0;
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            
        }
    }
    /*
    while (j < sizecoords) {
        int row = coords[j][0], col = coords[j][1];
        if (grid[row][col] != word[0]) {
            return FALSE;
        }

        for (int dir = 0; dir < sizecoords; dir++) {
            int k, rd = x[dir], cd = y[dir];
    
            for (k = 1; k < sizeword; k++) {
                if (rd >= size || rd < 0 || cd >= size || cd < 0) {
                    break;
                }
                if (grid[rd][cd] != word[k]) {
                    break;
                }
                rd += x[dir], cd += y[dir];
            }
            printf("%d\n", k);
            if (k == sizecoords) {
                return TRUE;
            }
        }
        j++;
    }
    */
    return FALSE;
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
        if (strcmp(line, word) == 0) {
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
    fprintf(file, "%s\t%.2f", player.pseudo, player.score);
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

void print_playerlist(FILE* stream, Player* playerlist, int size) {
    for (int i = 0; i < size; i++) {
        fprintf(stream, "%s\t%.2f\n", playerlist[i].pseudo, playerlist[i].score);
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
    printf("%d\n", k);
    (*input) = malloc(sizeof(char)*k);

    for (i = 0; i < k; i++) {
        (*input)[i] = temp[i];
    }
    (*input)[k] = '\0';
    *size = k;
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
            player = gamelist[i];
            printf("Vous avez déjà joué avec %s. Votre score était de %.2f\n", player.pseudo, player.score);
            printf("Vous allez rejouer avec le même pseudo et perdre votre ancien score.\n");
            yes = get_integer_input("Voulez-vous continuer ? (1 pour oui, 0 pour non) : ", 0, 1);
        }
        memcpy(player.pseudo, name, sizename);
        free(gamelist);
        free(name);
    } while (yes == 0);
    printf("Vous allez jouer en tant que : %s\n", player.pseudo);
    
    //  // ^ faut demander au joueur un pseudo, si il est pas pris, il le prend, sinon il en choisi un autre.

    time_t debut = time(0);
    int size = get_integer_input("Choisissez la taille de votre grille (4-8): ", 4, 8);
    char** grid = create_grid(size);
    grid = fill_grid_algo(size, grid);
    double playtime = (double) get_integer_input("Choisissez le temps de jeu (60s-180s): ", 60, 180);
    
    int* wordslen = (int*) malloc(sizeof(int));
    int sizewords = 0;
    print_grid(size, grid);
    do {
        int sizeword = 0, i = 0;
        char* word = NULL;

        get_string_input("Entrez un mot: ", &sizeword, &word);
        if (search2D(size, grid, word) && valid_word(word)) {
            printf("Le mot %s est dans la grille\n", word);
            wordslen[i] = sizeword;
            i++;
            wordslen = realloc(wordslen, sizeof(char*)*(i));
        } else {
            printf("Le mot %s n'est pas dans la grille\n", word);
        }
        if ((i+1) % 3 == 0) {
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
        print_grid(size, grid);
        }
        sizewords = i;
        free(word);
    } while (difftime(debut, time(0)) < playtime);
    player.score = score(sizewords, wordslen);
    printf("C'est fini, votre score est de %.2f\n", player.score);
    save_game(player, "scores.txt");

    free(wordslen);
    return player;
}
