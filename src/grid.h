#ifndef GRID_H
#define GRID_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

typedef struct {
    int arg1, arg2;
} Args;

struct Grid {
    int w, h;
    int **cells;
} grid, grid_aux;

int neighbor(int, int);

void *compute_grid(void *);

void update_grid(int);

void grid_printer();

void simulate(int);

void destroy_grid();

void init_grid(const char *, int, int);
#endif /* GRID_H */
