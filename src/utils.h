#ifndef UTILS_H
#define UTILS_H
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    int arg1, arg2;
} Args;

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s code %d\n", description, error);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action,
                         int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b) {
    return (r << 24) | (g << 16) | (b << 8) | 255;
}
#endif /* UTILS_H */
