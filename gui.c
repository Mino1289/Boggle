#include <gui.h>


#ifdef _WIN32
void print_grid(int size, char** grid) {
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            printf("%c%c%c%c", k==0 ? i==0 ? 201 : 204 : i==0 ? 203 : 206, 205, 205, 205);
        }
        printf("%c\n%c", i==0 ? 187 : 185, 186);
        for (int j = 0; j < size; j++) {
            printf( " %c %c", grid[i][j], 186);
        }
        printf("\n");
    }
    for (int i = 0; i < size; i++) {
        printf("%c%c%c%c", i==0 ? 200 : 202, 205, 205, 205);
    }
    printf("%c\n", 188);
}
#else
void print_grid(int size, char** grid) {
    for (int i = 0; i < size; i++) {
        for (int k = 0; k < size; k++) {
            printf("+---");
        }
        printf("+\n|");
        for (int j = 0; j < size; j++) {
            printf(" %c |", grid[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < size; i++) {
        printf("+---");
    }
    printf("+\n\n");
}
#endif

void printPlayer(FILE* stream, Player player) {
    fprintf(stream, "%s\t%.2f\t%d\t%d\n", player.pseudo, player.score, player.sizegrid, player.timeplayed);
}

void printPlayerlist(FILE* stream, Player* playerlist, int size) {
    for (int i = 0; i < size; i++) {
        printPlayer(stream, playerlist[i]);
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
    int k = strlen(temp);
    char* str = (char*) malloc(sizeof(char)*k);
    Word word = (Word) {k, str};

    strncpy(word.str, temp, k+1);
        
    return word;
}

void validate(const char* message) {
    printf("%s\n", message);
    #ifdef _WIN32
        getchar();
    #else
        FILE* f = fopen("/dev/tty", "r"); // done by M. Gaud
        if (f != NULL) {
            int c = fgetc(f);
            (void) c;
        }
    #endif
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
            printf("Le pseudo %s %c d%cja %ct%c utilis%c.\n", pseudo.str, ACCENT_E, ACCENT_A, ACCENT_E, ACCENT_E, ACCENT_E);
            printf("Vous ne pouvez pas le r%cutiliser.\n", ACCENT_E);
            yes = 0;
        } else {
            strcpy(player.pseudo, pseudo.str);
            yes = 1;
        }
        free(gamelist);
        freeWord(&pseudo);
    } while (yes == 0); // we ask for an unused nickname, if not, we ask again
    printf("Vous allez jouer en tant que : %s\n", player.pseudo);
    

    int size = get_integer_input("Choisissez la taille de votre grille (4-8): ", 4, 8);
    player.sizegrid = size;

    char** grid = create_grid(size);
    grid = fill_grid_algo(size, grid);
    
    double playtime = (double) get_integer_input("Choisissez le temps de jeu (60s-180s): ", 60, 180);
    player.timeplayed = playtime;
    
    Word* words = (Word*) malloc(sizeof(Word)); // array of Word -> will be realloc'ed
    if (words == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for words\n");
        return player;
    }

    int wordsize = 0; // size of the array of words
    int iter = 1; // number of iterations
    double dtime = 0; // time passed since the beginning of the game

    clear();
    print_logo();
    print_grid(size, grid);
    time_t debut = time(0);
    GrpWords* grpwords = read_dico();
    do {
        Word word = get_string_input("Entrez un mot: ");

        if (search2D(size, grid, word)) {
        
            // the word is in the grid
            if (valid_word(word.str, grpwords)) {
                // the word is also in the dictionary
                Boolean found = FALSE;
                int i = 0;
                while (i < wordsize && !found) {
                    if (strcmp(words[i].str, word.str) == 0) {
                        found = TRUE;
                    }
                    i++;
                }
                if (!found) {
                    // this word has not been found yet
                    printf("Le mot %s est dans la grille\n", word.str);
                    words[wordsize].length = word.length;
                    words[wordsize].str = malloc(sizeof(char)*(word.length+1));
                    strncpy(words[wordsize].str, word.str, word.length+1); // maybe use memcpy ?

                    wordsize++;

                    words = realloc(words, sizeof(Word)*(wordsize+1));
                } else {
                    // word already found
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
                printf("%s ", words[i].str);
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
        printf("%s : %d\n", words[i].str, words[i].length);
    }
    printf("\n");
    player.score = score(wordsize, words);
    printf("C'est fini, votre score est de %.2f\n", player.score);

    validate("Appuyez sur une touche pour continuer");

    save_game(player, "scores.txt");

    free_grid(size, grid);
    
    for (int i = 0; i < wordsize; i++) {
        freeWord(&words[i]);
    }
    free(words);
    
    for (int i = 0; i < NB_LETTER; i++) {
        freeGrpWords(&(grpwords[i]));
    }
    free(grpwords);
    return player;
}