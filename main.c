#include <grid.h>
#include <game.h>


int main(int argc, char* argv[]) {
	long long seed = time(0);
	if (argc > 1) {
		seed = atoi(argv[1]);
	}
	printf("%lld\n", seed);
	srand(seed); 
	int size = 8;
	char** grid = create_grid(size);
	grid = fill_grid_algo(size, grid);

	print_grid(size, grid);


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

	// Player player = play();
	// printf("%s\t%.2f\n", player.pseudo, player.score);

	free_grid(size, grid);
	return EXIT_SUCCESS;
}