#ifndef MYSCENEGRAPH_TEXTURE_H
#define MYSCENEGRAPH_TEXTURE_H

#include "glheader.h"

class Texture {
public:
    Texture(GLenum, GLsizei, std::vector<GLfloat>);
    ~Texture();
    void makeCurrent();
private:
    GLuint id;
    GLenum target;
};


#endif //MYSCENEGRAPH_TEXTURE_H
