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
    int k = strlen(temp)+1;
    char* str = (char*) malloc(sizeof(char)*k);
    Word word = (Word) {k, str};

    strncpy(word.str, temp, k);
        
    return word;
}

void validate(const char* message) {
    printf("%s\n", message);
    #ifdef _WIN32
        getchar();
    #else
        FILE* f = fopen("/dev/tty", "r"); // fait par M. Gaud
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

    validate("Appuyez sur une touche pour continuer");

    save_game(player, "scores.txt");

    free_grid(size, grid);
    free(wordslen);

    for (int i = 0; i < NB_LETTER; i++) {
        freeGrpWords(&(grpwords[i]));
    }
    free(grpwords);
    return player;
}