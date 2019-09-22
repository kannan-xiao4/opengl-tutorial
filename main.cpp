#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "load_window.hpp"
#include "class/Object.h"
#include "class/Shape.h"
#include "class/ShapeIndex.h"
#include "class/SolidShapeIndex.h"
#include "class/SolidShape.h"
#include "class/Window.h"
#include "class/Matrix.h"

static constexpr Object::Vertex rectangleVertex[] = {
    { -0.5f, -0.5f }, { 0.5f, -0.5f }, { 0.5f, 0.5f }, { -0.5f, 0.5f }
};

constexpr Object::Vertex octahedronVertex[] = {
    { 0.0f, 1.0f, 0.0f },
    { -1.0f, 0.0f, 0.0f },
    { 0.0f, -1.0f, 0.0f },
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f },
    { 0.0f, -1.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f },
    { -1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f },
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f }
};

constexpr Object::Vertex cubeVertex[] = {
    { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f }, // (0)
    { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.8f }, // (1)
    { -1.0f, 1.0f, 1.0f, 0.0f, 0.8f, 0.0f }, // (2)
    { -1.0f, 1.0f, -1.0f, 0.0f, 0.8f, 0.8f }, // (3)
    { 1.0f, 1.0f, -1.0f, 0.8f, 0.0f, 0.0f }, // (4)
    { 1.0f, -1.0f, -1.0f, 0.8f, 0.0f, 0.8f }, // (5)
    { 1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.0f }, // (6)
    { 1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.8f } // (7)
};

// 六面体の稜線の両端点のインデックス
constexpr GLuint wireCubeIndex[] = {
    1, 0, // (a)
    2, 7, // (b)
    3, 0, // (c)
    4, 7, // (d)
    5, 0, // (e)
    6, 7, // (f)
    1, 2, // (g)
    2, 3, // (h)
    3, 4, // (i)
    4, 5, // (j)
    5, 6, // (k)
    6, 1 // (l)
};

constexpr Object::Vertex solidCubeVertex[] = {
    // 左
    { -1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.1f },
    { -1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.1f },
    { -1.0f, 1.0f, 1.0f, 0.1f, 0.8f, 0.1f },
    { -1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.1f },
    { -1.0f, 1.0f, 1.0f, 0.1f, 0.8f, 0.1f },
    { -1.0f, 1.0f, -1.0f, 0.1f, 0.8f, 0.1f },
    // 裏
    { 1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f },
    { -1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f },
    { -1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f },
    { 1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f },
    { -1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f },
    { 1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f },
    // 下
    { -1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f },
    { 1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f },
    { 1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f },
    { -1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f },
    { 1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f },
    { -1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f },
    // 右
    { 1.0f, -1.0f, 1.0f, 0.1f, 0.1f, 0.8f },
    { 1.0f, -1.0f, -1.0f, 0.1f, 0.1f, 0.8f },
    { 1.0f, 1.0f, -1.0f, 0.1f, 0.1f, 0.8f },
    { 1.0f, -1.0f, 1.0f, 0.1f, 0.1f, 0.8f },
    { 1.0f, 1.0f, -1.0f, 0.1f, 0.1f, 0.8f },
    { 1.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.8f },
    // 上
    { -1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f },
    { -1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f },
    { 1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f },
    { -1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f },
    { 1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f },
    { 1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f },
    // 前
    { -1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f },
    { 1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f },
    { 1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f },
    { -1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f },
    { 1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f },
    { -1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f }
};

constexpr GLuint solidCubeIndex[] = {
    0, 1, 2, 3, 4, 5, // 左
    6, 7, 8, 9,10,11,//裏
    12, 13, 14, 15, 16, 17, // 下
    18, 19, 20, 21, 22, 23, //右
    24, 25, 26, 27, 28, 29, // 上
    30,31,32,33,34,35 //前
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
    
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
 
    const GLuint program(loadProgram("point.vert", "point.frag"));
    
    const GLint modelViewLoc(glGetUniformLocation(program, "modelview"));
    const GLint projectionLoc(glGetUniformLocation(program, "projection"));

    std::unique_ptr<const Shape> shape(new SolidShapeIndex(3, 36, solidCubeVertex, 36, solidCubeIndex));

    glfwSetTime(0.0);
    
    while (window) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        
        const GLfloat *const size(window.getSize());
        const GLfloat fovy(window.getScale() * 0.01f);
        const GLfloat aspect(size[0] / size[1]);
        const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));

        const GLfloat *const location(window.getLocation());
        const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()), 0.0f, 1.0f, 0.0f));
        const Matrix model(Matrix::translate(location[0], location[1], 0.0f) * r);
        
        const Matrix view(Matrix::lookat(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
        
        const Matrix modelview(view * model);
        
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
        glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, modelview.data());
        
        shape->draw();
        
        const Matrix modelview1(modelview * Matrix::translate(0.0f, 0.0f, 3.0f));
        
        glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, modelview1.data());
        
        shape->draw();
        
        window.swapBuffers();
    }
}
