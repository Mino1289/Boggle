#include <grid.h>
#include <game.h>


int main(int argc, char* argv[]) {
	long long seed = time(0);
	if (argc > 1) {
		seed = atoi(argv[1]);
	}
	printf("%lld\n", seed);
	srand(seed); 
	/*int size = 4; 
	char** grid = create_grid(size);
	grid = fill_grid_algo(size, grid);

	print_grid(size, grid); */
	char c = gen_rand_char();
	printf("%d = %c\n", c,c);


	// TEST |
	char* word=NULL;// = (char*) malloc(sizeof(char));
	int wordlen;
	get_string_input("Find a word in the grid:", &wordlen, &word);
	printf("%s\n", word);
	if (search2D(size, grid, word)) {
		printf("%s is in the grid\n", word);
	} else {
		printf("%s is not in the grid\n", word);
	}
	//TEST ^

	// int k;
	// Player* playerlist = read_games("scores.txt", &k);
	// for (int i = 0; i < k; i++) {
	// 	printf("%s\t%.2f\n", playerlist[i].pseudo, playerlist[i].score);
	// }

	// free_grid(size, grid);
	int reponse_utilisateur; // valeur entrée par l'utilisateur 
	// Au démarrage on propose à l’utilisateur un menu affichant trois options:
		do{
		printf ("bienvenu");
		printf("Que souhaitez-vous effectuer?\n Si vous souhaitez démarrer une partie tapez 1 \n Si vous souhaitez afficher les meilleurs scores tapez 2\n Si vous souhaitez quitter tapez 3 "); // question de départ à l'utilisateur 
		}while (reponse_utilisateur==1 || reponse_utilisateur ==2 || reponse_utilisateur ==3);

		do {
            scanf("%d",&reponse_utilisateur);    //on lit le choix du joueur
            if ((reponse_utilisateur!=1) && (reponse_utilisateur!=2) && (reponse_utilisateur!=3))  //on verifie ce que l'utilisateur a entré avec un message d'erreur
            {
                printf("Impossible, choisir 0, 1 ou 2\n");
            }
        }while ((reponse_utilisateur!=1) && (reponse_utilisateur!=2) && (reponse_utilisateur!=3));
		
		switch (reponse_utilisateur){
			case 1 :	// si l'utilisateur souhaite démarer une partie
				break;
			case 2 : // si l'utilisateur souhaite afficher les scores 
				break;
			case 3 : // si l'utilisateur souhaite quitter et sauvegarder 
			/*if (save(g1, i,joueur1,joueur2) == 1) { //on appelle la fonction save pour que les donne rentre dna sle fichier
                            printf("Sauvegarde reussie");
                            return 0;
                        } else {
                            printf("Erreur de sauvegarde...");
                            return 0;
                        }*/
				break;
		}


	// Player player = play();
	// printf("%s\t%.2f\n", player.pseudo, player.score);

	free_grid(size, grid);
	return EXIT_SUCCESS;
}