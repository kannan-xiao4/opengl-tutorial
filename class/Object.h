#pragma once
#include <GL/glew.h>

class Object{
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    
public:
    struct Vertex{
        GLfloat position[3];
        GLfloat normal[3];
    };
    
    Object(GLint size, GLsizei vertexcount, const Vertex *vertex, GLsizei indexcount = 0, const GLuint *index = NULL){
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex *>(0)->position);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<Vertex *>(0)->normal);
        glEnableVertexAttribArray(1);

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);
    }
    
    virtual ~Object(){
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }
    
private:
    Object(const Object &o);
    Object &operator=(const Object &o);
    
public:
    void bind() const{
        glBindVertexArray(vao);
    }
};
