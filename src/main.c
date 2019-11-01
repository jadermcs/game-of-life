#include "grid.h"

int main(int argc, char* argv[]) {
    int n_jobs, buffer_w, buffer_h; char* filename;

    if (argc > 2)
    {
        filename = argv[1];
        buffer_w = atoi(argv[2]);
        buffer_h = atoi(argv[3]);

        printf("File name: %s\n", filename);
        printf("Grid width: %d\n", buffer_w);
        printf("Grid height: %d\n", buffer_h);
    }
    else
    {
        printf("Please enter number of threads and initial state file.\n");
        return -1;
    }

    grid_aux.w = grid.w = buffer_w;
    grid_aux.h = grid.h = buffer_h;
    grid.cells = (uint8_t**) malloc(grid.w * sizeof(uint8_t*));
    grid_aux.cells = (uint8_t**) malloc(grid.w * sizeof(uint8_t*));
    for (int i = 0; i < grid.w; ++i) {
        grid.cells[i] = (uint8_t *) malloc(grid.h * sizeof(uint8_t));
        grid_aux.cells[i] = (uint8_t *) malloc(grid.h * sizeof(uint8_t));
    }

    init_grid(filename);

    while (1) {
        update_grid(n_jobs);
        grid_printer();
    }

    for(int i = 0; i < grid.w; i++) {
        free(grid.cells[i]);
        free(grid_aux.cells[i]);
    }
    free(grid.cells);
    free(grid_aux.cells);

    return 0;
}
