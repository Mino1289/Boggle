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

Boolean search2D(int size, char** grid, Word word) {

    int sizeword = word.length;
    int sizecoords;
    int** coords = locate_char(size, grid, word.str[0], &sizecoords);
    int k = 0;
    Boolean found = FALSE;
    while (k < sizecoords && !found) {
        int row = coords[k][0];
        int col = coords[k][1];
        int i = 1;
        while (i < sizeword-1 && is_around(size, grid, &row, &col, word.str[i-1], word.str[i])) {
            i++;
        }
        if (i == sizeword-1) {
            found = TRUE;
        }
        k++;
    }
    
    return found;
}

GrpWords* read_dico() {
    const char* file_path = "dico.txt";
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Could not open file %s\n", file_path);
        return NULL;
    }
    GrpWords* grpwords = malloc(sizeof(GrpWords)*NB_LETTER); // we know there are 26 groups (26 letters)
    if (grpwords == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for grpwords.\n");
        return NULL;
    }
    for (int i = 0; i < NB_LETTER; i++) {
        grpwords[i].nbWord = 1;
        grpwords[i].words = (Word*) malloc(sizeof(Word));
        grpwords[i].lettre = 'a' + i;
    }
    int i = 0, k = 0;
    Boolean good = TRUE;
    while (good) {
        char buffer [MAX_CHAR_ARRAY_LENGTH];
        int len;
        fscanf(file, "%s\t%d", buffer, &len);
        if (i != buffer[0] - 'a') { // changement de lettre => on remet k à 0
            k = 0;  
            i = buffer[0] - 'a'; 
        }

        grpwords[i].words[k].length = len; // longueur
        grpwords[i].words[k].str = malloc(sizeof(char)*(grpwords[i].words[k].length+1)); //with the '\0'
        strncpy(grpwords[i].words[k].str, buffer, grpwords[i].words[k].length+1); 
        
        grpwords[i].nbWord++;
        grpwords[i].words = realloc(grpwords[i].words, sizeof(Word)*(grpwords[i].nbWord));
        
        if (feof(file)) {
            good = FALSE;
        }
        k++; //should be == grpwords[i].nbWord;
    }
    fclose(file);
    return grpwords;
}

Boolean valid_word(const char* word, GrpWords* grpwords) {
    char frstletter = word[0];
    int indexletter = frstletter - 'a';

    Boolean found = FALSE;
    int bas = 0;
    int haut = grpwords[indexletter].nbWord - 1;
    int milieu;
    while (bas <= haut && !found) {
        milieu = (bas + haut) / 2;
        int cmp = strcmp(grpwords[indexletter].words[milieu].str, word);
        if (cmp == 0) {
            found = TRUE;
        } else if (cmp < 0) {
            bas = milieu + 1;
        } else {
            haut = milieu - 1;
        }
    }
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

Player* orderPlayerlist(Player* playerlist, int size, int (*playercmp)(Player p1, Player p2)) {
    //on bubble 
    Boolean weswap;
    int loop = 0;
    do {
        weswap = FALSE;
        for (int i = 0; i < size-1-loop; i++) {
            if (playercmp(playerlist[i], playerlist[i+1]) < 0) {
                swapPlayer(playerlist, i, i+1);
                weswap = TRUE;
            }
        }
        loop++;
    } while (weswap);
    return playerlist;
}

int playercmpscore(Player p1, Player p2) {
    if (p1.score > p2.score) {
        return 1;
    } else if (p1.score < p2.score) {
        return -1;
    }
    return 0; // equal
}

int playercmpscoreANDsize(Player p1, Player p2) {
    int cmpscore = playercmpscore(p1, p2);
    if (cmpscore == 0) {
        if (p1.sizegrid < p2.sizegrid) {
            return 1;
        } else if (p1.sizegrid > p2.sizegrid) {
            return -1;
        }
    }
    return cmpscore;
}

int playercmpscoreANDtime(Player p1, Player p2) {
    int cmpscore = playercmpscore(p1, p2);
    if (cmpscore == 0) {
        if (p1.timeplayed < p2.timeplayed) {
            return 1;
        } else if (p1.timeplayed > p2.timeplayed) {
            return -1;
        }
    }
    return cmpscore;
}

int playercmppseudo(Player p1, Player p2) {
    return -strcmp(p1.pseudo, p2.pseudo);
}

void printPlayerlist(FILE* stream, Player* playerlist, int size) {
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
        fflush(stdin);
        tmp = atoi(inputstring);
        if (tmp >= min && tmp <= max) {
            input = tmp;
        }
    } while (input == 0);

    return input;
}

Word get_string_input(const char* message) {
    printf("%s\n", message);
    char temp[MAX_CHAR_ARRAY_LENGTH];
    scanf("%s", temp);
    fflush(stdin);
    int k = strlen(temp)+1;
    char* str = (char*) malloc(sizeof(char)*k);
    Word word = (Word) {k, str};

    strncpy(word.str, temp, k);
        
    return word;
}

void print_logo() {
    printf("\n    ____  ____  ______________    ______  \n");
    printf("   / __ )/ __ \\/ ____/ ____/ /   / ____/ \n");
    printf("  / __  / / / / / __/ / __/ /   / __/     \n");
    printf(" / /_/ / /_/ / /_/ / /_/ / /___/ /___     \n");
    printf("/_____/\\____/\\____/\\____/_____/_____/\n\n");
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

void freeWord(Word *word) {
    free(word->str);
    *word = (Word) {0, NULL}; // Légal ?
}

void freeGrpWords(GrpWords *grpwords) {
    for (int j = 0; j < grpwords->nbWord-1; j++) {
        freeWord(&(grpwords->words[j]));
    }
    free(grpwords->words);
    *grpwords = (GrpWords) {'\0', NULL, 0};
}

Player play() {
    Player player;
    int yes=1;
    do {
        Word pseudo = get_string_input("Choisissez un pseudo : ");
        
        int n, i = 0;
        Player* gamelist = (Player*) malloc(sizeof(Player));
        read_games("scores.txt", &n, &gamelist);
        while (i <= n && strcmp(gamelist[i].pseudo, pseudo.str) != 0) {
            i++;
        }

        if (i <= n) {
            printf("Le pseudo %s a d%cja %ct%c utilis%c.\n", pseudo.str, ACCENT_E, ACCENT_E, ACCENT_E, ACCENT_E);
            printf("Vous ne pouvez pas le r%cutiliser.\n", ACCENT_E);
            yes = 0;
        } else {
            strcpy(player.pseudo, pseudo.str);
            yes = 1;
        }
        free(gamelist);
        free(pseudo.str);
    } while (yes == 0);
    printf("Vous allez jouer en tant que : %s\n", player.pseudo);
    
    //  // ^ faut demander au joueur un pseudo, si il est pas pris, il le prend, sinon il en choisi un autre.

    int size = get_integer_input("Choisissez la taille de votre grille (4-8): ", 4, 8);
    player.sizegrid = size;

    char** grid = create_grid(size);
    grid = fill_grid_algo(size, grid);
    
    double playtime = (double) get_integer_input("Choisissez le temps de jeu (60s-180s): ", 60, 180);
    player.timeplayed = playtime;
    
    int* wordslen = (int*) malloc(sizeof(int)); // tableau de taille de mots -> sera realloc
    if (wordslen == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for wordslen\n");
        return player;
    }
    char** words = (char**) malloc(sizeof(char*)); // tableau de mots -> sera realloc
    if (words == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for words\n");
        return player;
    }

    int wordsize = 0; // taille du tableau de taille de mot et du tableau de mots
    int iter = 1; // nombre d'itération
    double dtime = 0; // temps écoulé

    clear();
    print_logo();
    print_grid(size, grid);
    time_t debut = time(0);
    GrpWords* grpwords = read_dico();
    do {
        Word word = get_string_input("Entrez un mot: ");

        if (search2D(size, grid, word)) {
        
            // mots dans la grille
            if (valid_word(word.str, grpwords)) {
                // mots dans le dico
                Boolean found = FALSE;
                int i = 0;
                while (i < wordsize && !found) {
                    if (strcmp(words[i], word.str) == 0) {
                        found = TRUE;
                    }
                    i++;
                }
                if (!found) {
                    // mots pas encore trouvé
                    printf("Le mot %s est dans la grille\n", word.str);
                    wordslen[wordsize] = word.length-1;
                    words[wordsize] = malloc(sizeof(char)*word.length);
                    if (words[wordsize] == NULL) {
                        fprintf(stderr, "ERROR: Could not allocate memory for words[%d]\n", wordsize);
                        return player;
                    }
                    strcpy(words[wordsize], word.str);

                    wordsize++;

                    wordslen = realloc(wordslen, sizeof(int)*(wordsize+1));
                    wordslen[wordsize] = 0;

                    words = realloc(words, sizeof(char*)*(wordsize+1));
                    // free(word); // ça fait crash alo ?
                } else {
                    // mots déja trouvé
                    printf("Vous avez d%cja trouv%c le mot %s\n", ACCENT_E, ACCENT_E, word.str);
                }
            } else {
                printf("Le mot %s n'est pas dans le dictionnaire\n", word.str);
            }
        } else {
           printf("Le mot %s n'est pas dans la grille\n", word.str);
        }
        freeWord(&word);
        if (iter % 2 == 0){
            wait(1);
            clear();
            print_logo();
            print_grid(size, grid);
            for (int i = 0; i < wordsize; i++) {
                printf("%s ", words[i]);
            }
            printf("\n");
        }
        iter++;

        dtime = difftime(time(0), debut);
        printf("Temps %ccoul%c : %.0fs sur %.0fs\n", ACCENT_E, ACCENT_E, dtime, playtime);
    } while (dtime < playtime);
    printf("%d mots trouv%cs:\n", wordsize, ACCENT_E);
    printf("Mot : Taille\n");   
    for (int i = 0; i < wordsize; i++) {
        printf("%s : %d\n", words[i], wordslen[i]);
    }
    printf("\n");
    player.score = score(wordsize, wordslen);
    printf("C'est fini, votre score est de %.2f\n", player.score);

    Word input = get_string_input("Appuyez sur une touche pour continuer");
    if (input.str != NULL) {
        freeWord(&input);
    }
    save_game(player, "scores.txt");

    free_grid(size, grid);
    free(wordslen);

    for (int i = 0; i < NB_LETTER; i++) {
        freeGrpWords(&(grpwords[i]));
    }
    free(grpwords);
    return player;
}
