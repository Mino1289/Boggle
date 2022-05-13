#include <grid.h>
#include <game.h>


int main(int argc, char* argv[]) {
	time_t seed = time(0);
	if (argc > 1) {
		seed = atoi(argv[1]);
	}
	printf("%lld\n", seed);
	srand(seed); 
	int size = 8;
	char** grid = create_grid(size);
	grid = initialize_grid(size, grid);
	grid = fill_grid_algo(size, grid);

	print_grid(size, grid, stdout);

	// TEST |
	// char word[24];
	// scanf("%s", word);
	// printf("%s\n", word);
	// if (contains_word(size, grid, word)) {
	// 	printf("%s is in the grid\n", word);
	// } else {
	// 	printf("%s is not in the grid\n", word);
	// }
	//TEST ^

	free_grid(size, grid);
	return EXIT_SUCCESS;
}