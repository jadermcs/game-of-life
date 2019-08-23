#ifndef UTILS_H
#define UTILS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
    int arg1, arg2;
} Args;

extern void error_callback(int, const char*);

extern void key_callback(GLFWwindow*, int, int, int, int);

extern uint32_t rgb_to_uint32(uint8_t, uint8_t, uint8_t);
#endif /* UTILS_H */
