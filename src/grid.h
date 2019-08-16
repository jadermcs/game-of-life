#ifndef GRID_H
#define GRID_H
#include "utils.h"

struct Buffer {
    //width, height
    size_t w, h;
    uint32_t *data;
} buffer;

struct Sprite {
    int w, h;
    uint8_t *data;
} sprite;

struct Grid {
    int w, h;
    uint8_t **cells;
} grid, grid_aux;

struct Args {
    int arg1, arg2;
};

void buffer_draw_sprite(size_t x, size_t y, uint32_t color) {
    for(int xi = 0; xi < sprite.w; ++xi)
        for(int yi = 0; yi < sprite.h; ++yi)
            if(sprite.data[yi * sprite.w+ xi] &&
               (sprite.h - 1 + y - yi) < buffer.h &&
               (x + xi) < buffer.w)
                buffer.data[(sprite.h - 1 + y - yi) *
                    buffer.w + (x + xi)] = color;
}

int neighbor(uint8_t** grid_local, int i, int j) {
    int count = 0;
    for (int ix = -1; ix < 2; ix++)
        for (int jx = -1; jx < 2; jx++)
            if ((ix || jx) && grid_local[i+ix][j+jx])
                count++;
    return count;
}

void *compute_grid(void *arguments) {
    struct Args args = *((struct Args *) arguments);
    int N = args.arg1 == 0 ? 1 : args.arg1,
        M = args.arg2,
        alive;

    for (int j = 1; j < grid.h; j++)
        for (int i = N; i < M; i++) {
            alive = neighbor(grid.cells, i, j);

            if (alive == 2) grid_aux.cells[i][j] = grid.cells[i][j];
            if (alive == 3) grid_aux.cells[i][j] = 1;
            if (alive < 2)  grid_aux.cells[i][j] = 0;
            if (alive > 3)  grid_aux.cells[i][j] = 0;
        }
    pthread_exit(0);
}

void update_grid(int n_jobs) {
    pthread_t updaters[n_jobs];
    struct Args *args;
    uint8_t **temp;
    int bound = (grid.w) / n_jobs;
    for (int i = 0; i < n_jobs; ++i) {
        args = (struct Args *) malloc(sizeof(struct Args));
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
    for (int y = 0; y < grid.h; ++y)
        for (int x = 0; x < grid.w; ++x)
            if (grid.cells[x][y])
                buffer_draw_sprite(x*sprite.w, y*sprite.h, rgb_to_uint32(0, 0, 0));
}

void init_grid(char *filename) {
    FILE *fp = fopen(filename, "r");
    char ch;
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

uint8_t bac_sprite[100] =
{
    0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,1,1,1,
    0,1,1,1,1,1,1,1,1,1,
    0,1,1,1,1,1,1,1,1,1,
    0,1,1,1,1,1,1,1,1,1,
    0,1,1,1,1,1,1,1,1,1,
    0,1,1,1,1,1,1,1,1,1,
    0,1,1,1,1,1,1,1,1,1,
    0,1,1,1,1,1,1,1,1,1,
    0,1,1,1,1,1,1,1,1,1,
};
#endif /* GRID_H */
