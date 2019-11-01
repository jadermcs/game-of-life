#include "grid.h"

int main(int argc, char* argv[]) {
    int n_jobs, buffer_w, buffer_h;
    const char* filename = NULL;

    if (argc > 2)
    {
        n_jobs = 4;
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

    getchar();
    init_grid(filename, buffer_w, buffer_h);
    simulate(n_jobs);
    destroy_grid();

    return 0;
}
