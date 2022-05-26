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
        print_logo();
		int reponse = get_integer_input("Que voulez vous faire ?\nJOUER\t1\nSCORE\t2\nQUITTER\t3\n", 1, 4);
		clear();
		print_logo();
		int k, scoretype, order;
		Player* playerlist = (Player*) malloc(sizeof(Player));
		Word pseudotofind, input;
		switch (reponse) {
			case 1 :	// si l'utilisateur souhaite démarer une partie
				play();
				break;
			case 2 : // si l'utilisateur souhaite afficher les scores // trie etc
				if (k < 0) {
					printf("k = %d\n", k);
					printf("Il n'y a pas de score sauvegard%c pour l'instant.\n", ACCENT_E);
					wait(3);
					continue;
				}
				scoretype = get_integer_input("Afficher tous les scores\t1\nChercher un joueur\t\t2\nQuitter\t\t\t\t3\n", 1, 3);
				read_games("scores.txt", &k, &playerlist);
				clear();
				print_logo();
				switch (scoretype) {
				case 1:
					order = get_integer_input("Trier la liste de joueur par :\nSCORE\t\t1\nSCORE+TEMPS\t2\nSCORE+TAILLE\t3\nPSEUDO\t\t4\n", 1, 4);
					clear();
					print_logo();
					if (order >= 2 && order <= 3) {
						printf("Tri par score, et si %cgalit%c, par %s.\n", ACCENT_E, ACCENT_E, order == 2 ? "temps croissant" : "taille croissante");
					}
					printf("Voici la liste des scores :\n");
					printf("Nom\tScore\tTaille\tTemps\n");

					switch (order) {
					case 1:
						playerlist = orderPlayerlist(playerlist, k, playercmpscore);
						break;
					case 2:
						playerlist = orderPlayerlist(playerlist, k, playercmpscoreANDtime);
						break;
					case 3:
						playerlist = orderPlayerlist(playerlist, k, playercmpscoreANDsize);
						break;
					case 4:
						playerlist = orderPlayerlist(playerlist, k, playercmppseudo);
						break;
					default:
						break;
					}
					
					printPlayerlist(stdout, playerlist, k);

					wait(2);

					input = get_string_input("Appuyez sur une touche pour continuer");
					if (input.str != NULL) {
						freeWord(&input);
					}
					continue;
				case 2:
					pseudotofind = get_string_input("Donnez le pseudo d'un joueur pour le trouver.");
					playerlist = orderPlayerlist(playerlist, k, playercmppseudo);
					
					// bineary search through the playerlist who is sort by pseudo.
					Boolean found = FALSE;
					int bas = 0;
					int haut = k - 1;
					int milieu;
					while (bas <= haut && !found) {
						milieu = (bas + haut) / 2;
						int cmp = strcmp(playerlist[milieu].pseudo,pseudotofind.str);
						if (cmp == 0) {
							found = TRUE;
						} else if (cmp < 0) {
							bas = milieu + 1;
						} else {
							haut = milieu - 1;
						}
					}
					if (found) {
						// clear();
						printf("\nNom\tScore\tTaille\tTemps\n");
			        	printf("%s\t%.2f\t%d\t%d\n", playerlist[milieu].pseudo, playerlist[milieu].score, playerlist[milieu].sizegrid, playerlist[milieu].timeplayed);
					} else {
						printf("Le joueur %s, n'est pas dans la liste.\n", pseudotofind.str);
					}
					
					wait(2);

					input = get_string_input("Appuyez sur une touche pour continuer");
					if (input.str != NULL) {
						freeWord(&input);
					}
					continue;
				default:
					continue;
				}
				
			case 3 : // si l'utilisateur souhaite quitter le jeu
				exit(EXIT_SUCCESS);
				break;
			case 4 :
				printf("DEBUG\n");
				printf("Aucun code à debug.\n");
				wait(5);
			default:
				break;
		}
	} while (1);
	return EXIT_SUCCESS;
}