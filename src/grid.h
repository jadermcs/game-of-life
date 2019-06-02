#ifndef GRID_H
#define GRID_H
#include "utils.h"

struct Buffer {
    size_t width, height;
    uint32_t* data;
};

struct Sprite {
    size_t width, height;
    uint8_t* data;
};

struct Grid {
    size_t width, height;
    uint8_t* cells;
};

void buffer_draw_sprite(Buffer* buffer, const Sprite& sprite, size_t x,
        size_t y, uint32_t color) {
    for(size_t xi = 0; xi < sprite.width; ++xi)
        for(size_t yi = 0; yi < sprite.height; ++yi)
            if(sprite.data[yi * sprite.width + xi] &&
               (sprite.height - 1 + y - yi) < buffer->height &&
               (x + xi) < buffer->width)
                buffer->data[(sprite.height - 1 + y - yi) *
                    buffer->width + (x + xi)] = color;
}

void compute_grid(Grid* grid, Grid* grid_aux, int N, int M) {
    int count;
    for (int i = 1; i <=grid->height; i++) {
        for (int j = N; j < M; ++j) {
            count = 0;
            if (grid->cells[(i-1)*grid->height+j-1]) count++;
            if (grid->cells[(i-1)*grid->height+j]) count++;
            if (grid->cells[(i-1)*grid->height+j+1]) count++;
            if (grid->cells[i*grid->height+j-1]) count++;
            if (grid->cells[i*grid->height+j+1]) count++;
            if (grid->cells[(i+1)*grid->height+j-1]) count++;
            if (grid->cells[(i+1)*grid->height+j]) count++;
            if (grid->cells[(i+1)*grid->height+j+1]) count++;

            if (count <= 1 || count >= 4)
                grid_aux->cells[i*grid->height+j] = 0;
            else if (grid->cells[i*grid->height+j] && (count == 2||count == 3))
                grid_aux->cells[i*grid->height+j] = 1;
            else if (not grid->cells[i*grid->height+j] && count == 3)
                grid_aux->cells[i*grid->height+j] = 1;
            else
                grid_aux->cells[i*grid->height+j] = 0;
        }
    }
}

void update_grid(Grid* grid, Grid* grid_aux, int n_jobs) {
    std::thread updaters[n_jobs];
    for (int i = 0; i < n_jobs; ++i) {
        updaters[i] = std::thread(compute_grid, grid, grid_aux, i, i);
    }
    for (auto &th : updaters)
        th.join();
    grid = grid_aux;
}

void grid_printer(Grid* grid, Buffer* buffer, Sprite* sprite) {
    for (size_t x = 0; x < grid->width; ++x)
        for (size_t y = 0; y < grid->height; ++y)
            if (grid->cells[(y*grid->width)+x])
                buffer_draw_sprite(buffer, *sprite, x*10, y*10,
                                   rgb_to_uint32(0, 0, 0));
}

void init_grid(Grid* grid) {
    for (int x = 0; x < (int)grid->width; ++x)
        for (int y = 0; y < (int)grid->height; ++y)
            if (rand() % 6 == 0)
                grid->cells[y*grid->width+x] = 1;
            else
                grid->cells[y*grid->width+x] = 0;
}

uint8_t bac_sprite[100] =
{
    1,1,1,1,1,0,0,0,0,0,
    1,1,1,1,1,0,1,0,0,0,
    1,1,1,1,1,0,1,1,0,0,
    1,1,1,1,1,0,1,1,1,0,
    1,1,1,1,1,0,1,1,1,0,
    0,0,0,0,0,0,1,1,1,0,
    0,1,1,1,1,1,0,1,1,0,
    0,0,1,1,1,1,1,0,1,0,
    0,0,0,1,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,0,0,
};
#endif /* GRID_H */
