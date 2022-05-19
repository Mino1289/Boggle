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
		int reponse = get_integer_input("Que voulez vous faire ?\nJOUER\t1\nSCORE\t2\nQUITTER\t3\n", 1, 3);
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
				int n;
				char* input = NULL;
				get_string_input("Appuyez sur une touche pour continuer",&n, &input);
				if (input != NULL) {
					free(input);
					break;
				}
				break;
			case 3 : // si l'utilisateur souhaite quitter le jeu
				exit(EXIT_SUCCESS);
				break;
		}
		wait(2);
	} while (1);
	return EXIT_SUCCESS;
}