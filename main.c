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
		printf("Bienvenue dans :");
        print_logo();
		int reponse = get_integer_input("Que souhaitez-vous faire ?\nJOUER\t\t\t1\nAFFICHER SCORES\t\t2\nAFFICHER REGLES\t\t3\nQUITTER LE JEU\t\t4\n", 1, 5);
		clear();
		print_logo();

		int k=0, scoretype, order, index;
		Player* playerlist = (Player*) malloc(sizeof(Player));

		Word pseudotofind;
		switch (reponse) {
		case 1 :	// si l'utilisateur souhaite démarer une partie
			play();
			break;
		case 2 : // si l'utilisateur souhaite afficher les scores // trie etc
			read_games("scores.txt", &k, &playerlist);
			if (k <= 0) {
				printf("k = %d\n", k);
				printf("Il n'y a pas de score sauvegard%c pour l'instant.\n", ACCENT_E);
				wait(3);
				continue;
			}
			scoretype = get_integer_input("Afficher tous les scores\t1\nChercher un joueur\t\t2\nQuitter\t\t\t\t3\n", 1, 3);
			clear();
			print_logo();
			switch (scoretype) {
			case 1:
				order = get_integer_input("Trier la liste de joueur par :\nSCORE\t\t1\nTEMPS+SCORE\t2\nTAILLE+SCORE\t3\nPSEUDO\t\t4\n", 1, 4);
				clear();
				print_logo();
				if (order >= 2 && order <= 3) {
					printf("Tri par %s, et si m%cme valeur, par score.\n", order == 2 ? "temps croissant" : "taille croissante", ACCENT_E2);
				}
				printf("Voici la liste des scores :\n");
				printf("Nom\tScore\tTaille\tTemps\n");

				switch (order) {
				case 1:
					playerlist = orderPlayerlist(playerlist, k, playercmpscore);
					break;
				case 2:
					playerlist = orderPlayerlist(playerlist, k, playercmptimeANDscore);
					break;
				case 3:
					playerlist = orderPlayerlist(playerlist, k, playercmpsizeANDscore);
					break;
				case 4:
					playerlist = orderPlayerlist(playerlist, k, playercmppseudo);
					break;
				default:
					break;
				}
				
				printPlayerlist(stdout, playerlist, k);


				wait(2);
				
				validate("Appuyez sur une touche pour continuer");
				continue;
			case 2:
				pseudotofind = get_string_input("Donnez le pseudo d'un joueur pour le trouver.");
				playerlist = orderPlayerlist(playerlist, k, playercmppseudo);

				
				// bineary search through the playerlist who is sort by pseudo.
				Boolean found = isPseudoInList(playerlist, k, pseudotofind, &index);
				
				if (found && index != -1) {
					printf("\nNom\tScore\tTaille\tTemps\n");
					printPlayer(stdout, playerlist[index]);
				} else {
					printf("Le joueur %s, n'est pas dans la liste.\n", pseudotofind.str);
				}
				
				wait(2);
				
				validate("\nAppuyez sur une touche pour continuer");
				continue;
			default:
				continue;
			}
		case 3 : // Afficher les règles
			printf("\n R%cgles du jeu :\n", ACCENT_E1);
			printf("\tLe but du jeu est de trouver des mots en utilisant des lettres accol%ces dans la grille.\n", ACCENT_E);
			printf("\tPour ce faire, vous pouvez utiliser les lettres dans tous les sens possibles.\n");
			printf("\tTant qu'il est possible de tracer un chemin passant par toutes les lettres.\n");
			srand(15435);
			char** grid = create_grid(4);
			grid = fill_grid_algo(4, grid);
			srand(seed);
			print_grid(4, grid);
			#ifdef _WIN32
				printf("On retrouve le mot \"maire\", en comman%cant en haut %c gauche vers la droite.\n", CEDILLE, ACCENT_A);
				printf("Puis, au i on d%cscend d'une ligne et on continue vers la gauche jusqu'au 'e'.\n\n", ACCENT_E);
			#else
				printf("On retrouve le mot \"dans\" avec le 'd' de la premiere colone,\n");
				printf("puis dans la diagonale haut-droite le 'a', le 'n' juste au dessus, puis le 's' a droite.\n");
				printf("On peut aussi trouver le mot \"pour\" en partant du coin en bas à gauche,\n");
				printf("puis on trouve le 'o' a sa droite, le 'u' en haut a droite et le 'r' a gauche toute.\n");
			#endif
			wait(2);
			validate("\nAppuyez sur une touche pour continuer");
			break;
			
		case 4 : // si l'utilisateur souhaite quitter le jeu
			clear();
			exit(EXIT_SUCCESS);
			break;
		case 5 :
			printf("DEBUG\n");
			validate("Appuyez sur une touche pour continuer.");
			break;
		default:
			break;
		}
	} while (1);
	return EXIT_SUCCESS;
}