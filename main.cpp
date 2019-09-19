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
#include "class/Matrix.h"

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
    
    const GLint modelViewLoc(glGetUniformLocation(program, "modelview"));
    const GLint projectionLoc(glGetUniformLocation(program, "projection"));

    std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));

    while (window) {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        
        const GLfloat *const size(window.getSize());
        const GLfloat scale(window.getScale() * 2.0f);
        const GLfloat w(size[0]/scale), h(size[1]/scale);
        const Matrix projection(Matrix::orthogonal(-w, w, -h, h, 1.0f, 10.0f));

        const GLfloat *const location(window.getLocation());
        const Matrix model(Matrix::translate(location[0], location[1], 0.0f));
        
        const Matrix view(Matrix::lookat(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
        
        const Matrix modelview(view * model);
        
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, modelview.data());
        
        shape->draw();
        
        window.swapBuffers();
    }
}
