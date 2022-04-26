#include <grid.h>
#include <generation.h>


int main(void) {
	srand(time(0)); // SEED
	int size = 8;
	char** grid = create_grid(size);
	grid = initialize_grid(size, grid);
	print_grid(size, grid);
	grid = fill_grid_algo(size, grid);

	print_grid(size, grid);

	free_grid(size, grid);
	return EXIT_SUCCESS;
}