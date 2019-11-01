#include "grid.h"

static volatile int keep_running = 1;

void intHandler(int dummy) {
    keep_running = 0;
}

void simulate(int n_jobs)
{
    //disable cursor block
    signal(SIGINT, intHandler);
    printf("\e[?25l");
    while (keep_running) {
        update_grid(n_jobs);
        grid_printer();
        usleep(100*1000);
    }
    //enable cursor block
    printf("\e[?25h");
}

void destroy_grid()
{
    for(int i = 0; i < grid.w; i++) {
        free(grid.cells[i]);
        free(grid_aux.cells[i]);
    }
    free(grid.cells);
    free(grid_aux.cells);
}

int neighbor(int i, int j) {
    int count = 0;
    for (int ix = -1; ix <= 1; ix++)
        for (int jx = -1; jx <= 1; jx++)
            if ((ix || jx) && grid.cells[i+ix][j+jx])
                count++;
    return count;
}

void *compute_grid(void *arguments) {
    Args args = *((Args *) arguments);
    int N = args.arg1 == 0 ? 1 : args.arg1, M = args.arg2,
        alive;

    for (int j = 1; j < grid.h; j++)
        for (int i = N; i < M; i++) {
            alive = neighbor(i, j);

            if (alive == 2) grid_aux.cells[i][j] = grid.cells[i][j];
            if (alive == 3) grid_aux.cells[i][j] = 1;
            if (alive < 2)  grid_aux.cells[i][j] = 0;
            if (alive > 3)  grid_aux.cells[i][j] = 0;
        }
    pthread_exit(0);
}

void update_grid(int n_jobs) {
    pthread_t updaters[n_jobs];
    Args *args;
    int **temp;
    int bound = (grid.w) / n_jobs;
    for (int i = 0; i < n_jobs; ++i) {
        args = (Args *) malloc(sizeof(Args));
        args->arg1 = i * bound;

        if (i == n_jobs-1) args->arg2 = grid.w - 1;
        else               args->arg2 = (i+1) * bound;

        pthread_create(&updaters[i], NULL, compute_grid, args);
    }

    for (int i = 0; i < n_jobs; ++i)
        pthread_join(updaters[i], NULL);

    temp = grid.cells;
    grid.cells = grid_aux.cells;
    grid_aux.cells = temp;
}

void grid_printer() {
    system("@cls||clear");
    for (int y = 0; y < grid.h; ++y)
    {
        for (int x = 0; x < grid.w; ++x)
            if (grid.cells[x][y]) printf("\u2588");
            else printf(" ");
        printf("\n");
    }
}

void init_grid(const char *filename, int buffer_w, int buffer_h) {
    FILE *fp = fopen(filename, "r");
    char ch;
    grid_aux.w = grid.w = buffer_w;
    grid_aux.h = grid.h = buffer_h;
    grid.cells = (int **) malloc(grid.w * sizeof(int *));
    grid_aux.cells = (int **) malloc(grid.w * sizeof(int *));
    for (int i = 0; i < grid.w; ++i) {
        grid.cells[i] = (int *) malloc(grid.h * sizeof(int));
        grid_aux.cells[i] = (int *) malloc(grid.h * sizeof(int));
    }
    if (fp == NULL) {
        perror("Can't open file.\n");
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < grid.h; ++y) {
        for (int x = 0; x < grid.w; ++x) {
            ch = fgetc(fp);
            if (ch == '\n')
                ch = fgetc(fp);
            grid.cells[x][y] = (ch - '0');
        }
    }
    fclose(fp);
}
