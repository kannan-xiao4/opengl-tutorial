#ifndef load_window_hpp
#define load_window_hpp

#include <fstream>
#include <vector>
#include <GL/glew.h>

GLuint createProgram(const char *vsrc, const char *fsrc);
bool readShaderSource(const char *name, std::vector<GLchar> &buffer);
GLuint loadProgram(const char *vert, const char *frag);

#endif /* load_window_hpp */
