#ifndef GRID_H
#define GRID_H
#include "utils.h"

struct Buffer {
    size_t width, height;
    uint32_t* data;
};

struct Sprite {
    int width, height;
    uint8_t* data;
};

struct Grid {
    int width, height;
    uint8_t** cells;
};

void buffer_draw_sprite(Buffer* buffer, const Sprite& sprite, size_t x,
        size_t y, uint32_t color) {
    for(int xi = 0; xi < sprite.width; ++xi)
        for(int yi = 0; yi < sprite.height; ++yi)
            if(sprite.data[yi * sprite.width + xi] and
               (sprite.height - 1 + y - yi) < buffer->height and
               (x + xi) < buffer->width)
                buffer->data[(sprite.height - 1 + y - yi) *
                    buffer->width + (x + xi)] = color;
}

int neighbor(uint8_t** grid, int i, int j) {
    int count = 0;
    for (int ix = -1; ix < 2; ix++)
        for (int jx = -1; jx < 2; jx++)
            if ((ix or jx) and grid[i+ix][j+jx])
                count++;
    return count;
}

void compute_grid(Grid* grid, Grid* grid_aux, int N, int M) {
    int alive, i, j;
    if (N == 0) N = 1;

    for (j = 1; j < grid->height; j++)
        for (i = N; i < M; i++) {
            alive = neighbor(grid->cells, i, j);

            if (alive == 2) grid_aux->cells[i][j] = grid->cells[i][j];
            if (alive == 3) grid_aux->cells[i][j] = 1;
            if (alive < 2)  grid_aux->cells[i][j] = 0;
            if (alive > 3)  grid_aux->cells[i][j] = 0;
        }
}

void update_grid(Grid* grid, Grid* grid_aux, int n_jobs) {
    std::thread updaters[n_jobs];
    uint8_t** temp;
    int i, bound = (grid->width) / n_jobs;
    for (i = 0; i < n_jobs; ++i) {
        if (i == n_jobs-1)
            updaters[i] = std::thread(compute_grid, grid, grid_aux,
                                      i * bound, (grid->width) - 1);
        else
            updaters[i] = std::thread(compute_grid, grid, grid_aux,
                                      i * bound, (i+1) * bound);
    }

    for (auto &th : updaters)
        th.join();

    temp = grid->cells;
    grid->cells = grid_aux->cells;
    grid_aux->cells = temp;
}

void grid_printer(Grid* grid, Buffer* buffer, Sprite* sprite) {
    int wspr = sprite->width, hspr = sprite->height;
    for (int y = 0; y < grid->height; ++y)
        for (int x = 0; x < grid->width; ++x)
            if (grid->cells[x][y])
                buffer_draw_sprite(buffer, *sprite, x*wspr, y*hspr,
                                   rgb_to_uint32(0, 0, 0));
}

void init_grid(Grid* grid, char* filename) {
    FILE* fp = fopen(filename, "r");
    char ch;
    if (fp == NULL) {
        perror("Can't open file.\n");
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < grid->height; ++y) {
        for (int x = 0; x < grid->width; ++x) {
            ch = fgetc(fp);
            if (ch == '\n')
                ch = fgetc(fp);
            grid->cells[x][y] = (ch - '0');
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
