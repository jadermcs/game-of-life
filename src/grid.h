#ifndef GRID_H
#define GRID_H
#include "utils.h"

std::mutex mtx;

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
            if(sprite.data[yi * sprite.width + xi] &&
               (sprite.height - 1 + y - yi) < buffer->height &&
               (x + xi) < buffer->width)
                buffer->data[(sprite.height - 1 + y - yi) *
                    buffer->width + (x + xi)] = color;
}

void compute_grid(Grid* grid, Grid* grid_aux, int N, int M) {
    int count;
    if (N == 0)
        N = 1;
    for (int j = 1; j < grid->height; j++) {
        for (int i = N; i < M; i++) {
            count = 0;
            if (grid->cells[i-1][j-1]) count++;
            if (grid->cells[i-1][j]) count++;
            if (grid->cells[i-1][j+1]) count++;
            if (grid->cells[i][j-1]) count++;
            if (grid->cells[i][j+1]) count++;
            if (grid->cells[i+1][j-1]) count++;
            if (grid->cells[i+1][j]) count++;
            if (grid->cells[i+1][j+1]) count++;

            if (count <= 1 || count >= 4)
                grid_aux->cells[i][j] = 0;
            else if (grid->cells[i][j] && (count == 2||count == 3))
                grid_aux->cells[i][j] = 1;
            else if ((not grid->cells[i][j]) && count == 3)
                grid_aux->cells[i][j] = 1;
            else
                grid_aux->cells[i][j] = 0;
        }
    }
}

void update_grid(Grid* grid, Grid* grid_aux, int n_jobs) {
    std::thread updaters[n_jobs];
    int factor = grid->width/n_jobs;
    for (int i = 0; i < n_jobs; ++i) {
        if (i == n_jobs-1)
            updaters[i] = std::thread(compute_grid, grid, grid_aux,
                                      i*factor, grid->width-1);
        else
            updaters[i] = std::thread(compute_grid, grid, grid_aux,
                                      i*factor, (i+1)*factor);
    }
    for (auto &th : updaters)
        th.join();
    *grid = *grid_aux;
}

void grid_printer(Grid* grid, Buffer* buffer, Sprite* sprite) {
    int wspr = sprite->width, hspr = sprite->height;
    for (int y = 0; y < grid->height; ++y) {
        for (int x = 0; x < grid->width; ++x) {
            if (grid->cells[x][y]) {
                buffer_draw_sprite(buffer, *sprite, x*wspr, y*hspr,
                                   rgb_to_uint32(0, 0, 0));
            }
        }
    }
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
