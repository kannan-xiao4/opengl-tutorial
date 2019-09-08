#include "log.hpp"
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLboolean printProgramInfoLog(GLuint program){
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::cerr << "link error " << std::endl;
    }
    
    GLsizei bufSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
    
    if (bufSize > 1) {
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }
    
    return static_cast<GLboolean>(status);
}

GLboolean printShaderInfoLog(GLuint shader, const char *str){
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::cerr << "compile error in " << str << std::endl;
    }
    
    GLsizei bufSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
    
    if (bufSize > 1) {
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }
    
    return static_cast<GLboolean>(status);
}
