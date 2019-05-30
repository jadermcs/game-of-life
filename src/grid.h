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

void grid_printer(Grid* grid, Buffer* buffer, Sprite* sprite) {
    for (size_t x = 0; x < grid->width; ++x)
        for (size_t y = 0; y < grid->height; ++y)
            if (grid->cells[(y*grid->width)+x])
                buffer_draw_sprite(buffer, *sprite, x*10, y*10,
                                   rgb_to_uint32(0, 0, 0));
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
