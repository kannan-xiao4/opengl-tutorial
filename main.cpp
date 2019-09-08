#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "load_window.hpp"
#include "class/Shape.h"
#include "class/Window.h"

static constexpr Object::Vertex rectangleVertex[] = {
    { -0.5f, -0.5f }, { 0.5f, -0.5f }, { 0.5f, 0.5f }, { -0.5f, 0.5f }
};

int main(int argc, const char * argv[]) {
    char dir[255];
    getcwd(dir,255);
    std::cout << "Current Directory : " << dir << std::endl;

    if (glfwInit() == GL_FALSE) {
        std::cerr << "Cant initialize GLFW" << std::endl;
        return 1;
    }
    
    atexit(glfwTerminate);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    Window window;
    
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
 
    const GLuint program(loadProgram("point.vert", "point.frag"));
    
    const GLint sizeLoc(glGetUniformLocation(program, "size"));
    const GLint scaleLoc(glGetUniformLocation(program, "scale"));
    const GLint locationLoc(glGetUniformLocation(program, "location"));

    std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));

    while (window) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        
        glUniform2fv(sizeLoc, 1, window.getSize());
        glUniform1f(scaleLoc, window.getScale());
        glUniform2fv(locationLoc, 1, window.getLocation());
        
        shape->draw();
        
        window.swapBuffers();
    }
}
