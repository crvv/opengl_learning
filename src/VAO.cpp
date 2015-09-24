#include "vao.h"

VAO::VAO() {
    glGenVertexArrays(1, &id);
}
VAO::~VAO() {
    if (elementBufferID!=0) {
        glDeleteBuffers(1, &elementBufferID);
    }
    glDeleteBuffers(static_cast<GLsizei>(attribs.size()), &attribs[0]);
    glDeleteVertexArrays(1, &id);
}
void VAO::makeCurrent() {
    glBindVertexArray(id);
}

