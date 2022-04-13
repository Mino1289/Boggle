#include <grid.h>


int main(void) {
	int size = 8;
	char** grid = create_grid(size);
	grid = initialize_grid(grid, size);

	print_grid(size, grid);

	free_grid(size, grid);
	return EXIT_SUCCESS;
}
