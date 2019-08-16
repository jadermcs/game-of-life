#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaders.h"
#include "grid.h"
#include <stdlib.h>


int main(int argc, char* argv[]) {
    int n_jobs; char* filename;
    const size_t buffer_w = 640;
    const size_t buffer_h = 280;

    if (argc > 2) {
        n_jobs = atoi(argv[1]);
        filename = argv[2];
        printf("Number of threads: %d\n", n_jobs);
        printf("File name: %s\n", filename);
    }
    else {
        printf("Please enter number of threads and initial state file.\n");
        return -1;
    }

    grid.w = buffer_w/10;
    grid.h = buffer_h/10;
    grid.cells = (uint8_t**) malloc(grid.w * sizeof(uint8_t*));
    for (int i = 0; i < grid.w; ++i) {
        grid.cells[i] = (uint8_t *) malloc(grid.h * sizeof(uint8_t));
    }

    grid_aux.w = buffer_w/10;
    grid_aux.h = buffer_h/10;
    grid_aux.cells = (uint8_t**) malloc(grid.w * sizeof(uint8_t*));
    for (int i = 0; i < grid_aux.w; ++i) {
        grid_aux.cells[i] = (uint8_t *) malloc(grid.h * sizeof(uint8_t));
    }

    sprite.w = 10;
    sprite.h = 10;
    sprite.data = bac_sprite;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(buffer_w, buffer_h,
                                          "Game of Life", NULL, NULL);
    if(!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(5);

    GLenum err = glewInit();
    if(err != GLEW_OK) {
        fprintf(stderr, "Error initializing GLEW.\n");
        glfwTerminate();
        return -1;
    }
    int glVersion[2] = {-1, 1};
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

    printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);
    printf("Renderer used: %s\n", glGetString(GL_RENDERER));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Create graphics buffer
    buffer.w  = buffer_w;
    buffer.h = buffer_h;
    buffer.data   = (uint32_t *) malloc(buffer.w * buffer.h *sizeof(uint32_t));

    buffer_clear(0);

    // Create texture for presenting buffer to OpenGL
    GLuint buffer_texture;
    glGenTextures(1, &buffer_texture);
    glBindTexture(GL_TEXTURE_2D, buffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, buffer.w, buffer.h, 0,
                 GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    // Create vao for generating fullscreen triangle
    GLuint fullscreen_triangle_vao;
    glGenVertexArrays(1, &fullscreen_triangle_vao);


    // Create shader for displaying buffer

    GLuint shader_id = glCreateProgram();

    {
        //Create vertex shader
        GLuint shader_vp = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(shader_vp, 1, &vertex_shader, 0);
        glCompileShader(shader_vp);
        validate_shader(shader_vp, vertex_shader);
        glAttachShader(shader_id, shader_vp);

        glDeleteShader(shader_vp);
    }

    {
        //Create fragment shader
        GLuint shader_fp = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(shader_fp, 1, &fragment_shader, 0);
        glCompileShader(shader_fp);
        validate_shader(shader_fp, fragment_shader);
        glAttachShader(shader_id, shader_fp);

        glDeleteShader(shader_fp);
    }

    glLinkProgram(shader_id);

    if(!validate_program(shader_id)) {
        fprintf(stderr, "Error while validating shader.\n");
        glfwTerminate();
        glDeleteVertexArrays(1, &fullscreen_triangle_vao);
        free(buffer.data);
        return -1;
    }

    glUseProgram(shader_id);

    GLint location = glGetUniformLocation(shader_id, "buffer");
    glUniform1i(location, 0);


    //OpenGL setup
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(fullscreen_triangle_vao);

    // Prepare game
    uint32_t clear_color = rgb_to_uint32(255, 255, 255);
    init_grid(filename);

    while (!glfwWindowShouldClose(window)) {
        buffer_clear(clear_color);

        update_grid(n_jobs);
        grid_printer();

        glTexSubImage2D(
            GL_TEXTURE_2D, 0, 0, 0,
            buffer.w, buffer.h,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
            buffer.data
        );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    glDeleteVertexArrays(1, &fullscreen_triangle_vao);

    for(int i = 0; i < grid.w; i++)
        free(grid.cells[i]);
    free(grid.cells);

    for(int i = 0; i < grid_aux.w; i++)
        free(grid_aux.cells[i]);
    free(grid_aux.cells);

    free(buffer.data);

    return 0;
}

