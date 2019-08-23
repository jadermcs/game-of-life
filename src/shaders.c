#include "utils.h"
#include "shaders.h"

void validate_shader(GLuint shader, const char *file){
    static const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    GLsizei length = 0;
    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);

    if(length > 0) printf("Shader %d(%s) compile error: %s\n",
                            shader, (file? file: ""), buffer);
}

_Bool validate_program(GLuint program) {
    static const GLsizei BUFFER_SIZE = 512;
    GLchar buffer[BUFFER_SIZE];
    GLsizei length = 0;
    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
    if(length > 0) {
        printf("Program %d link error: %s\n", program, buffer);
        return 0;
    }
    return 1;
}
