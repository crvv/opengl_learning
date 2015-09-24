#include <vector>

#include "texture.h"

Texture::Texture(GLenum t, GLsizei width, std::vector<GLfloat> terrain) {
    glGenTextures(1, &id);
    target = t;
    makeCurrent();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, width, width,
                 0, GL_RED, GL_FLOAT, &terrain[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::makeCurrent() {
    glBindTexture(target, id);
}
