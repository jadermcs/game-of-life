#ifndef GRID_H
#define GRID_H
#include <stdio.h>
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

void buffer_clear(uint32_t);

void buffer_draw_sprite(size_t, size_t, uint32_t);

int neighbor(uint8_t **, int, int);

void *compute_grid(void *);

void update_grid(int);

void grid_printer();

void init_grid(char *);
#endif /* GRID_H */
