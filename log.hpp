#ifndef log_hpp
#define log_hpp

#include <GL/glew.h>

GLboolean printProgramInfoLog(GLuint program);
GLboolean printShaderInfoLog(GLuint shader, const char *str);

#endif /* log_hpp */
