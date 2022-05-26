#include <grid.h>
#include <game.h>


int main(int argc, char* argv[]) {
	long long seed = time(0);
	if (argc > 1) {
		seed = atoi(argv[1]);
	}
	srand(seed); 

	do {
		clear();
		printf("\n    ____  ____  ______________    ______  \n");
		printf("   / __ )/ __ \\/ ____/ ____/ /   / ____/ \n");
		printf("  / __  / / / / / __/ / __/ /   / __/     \n");
		printf(" / /_/ / /_/ / /_/ / /_/ / /___/ /___     \n");
		printf("/_____/\\____/\\____/\\____/_____/_____/\n\n");
                                        
		int reponse = get_integer_input("Que voulez vous faire ?\nJOUER\t1\nSCORE\t2\nQUITTER\t3\n", 1, 4);
		int k;
		Player* playerlist = (Player*) malloc(sizeof(Player));
		switch (reponse){
			case 1 :	// si l'utilisateur souhaite démarer une partie
				play();
				break;
			case 2 : // si l'utilisateur souhaite afficher les scores
				read_games("scores.txt", &k, &playerlist);
				clear();
				printf("Voici la liste des scores :\n");
				printf("Nom\tScore\tTaille\tTemps\n");
				orderAndPrint_playerlist(stdout, playerlist, k);
				wait(2);

				Word input = get_string_input("Appuyez sur une touche pour continuer");
				if (input.str != NULL) {
					freeWord(&input);
					break;
				}
				break;
			case 3 : // si l'utilisateur souhaite quitter le jeu
				exit(EXIT_SUCCESS);
				break;
			case 4 :
				printf("DEBUG\n");
				printf("Aucune code à debug.\n");
				wait(5);
		}
	} while (1);
	return EXIT_SUCCESS;
}