#ifndef SIMPLESCENEGRAPH_VAO_H_
#define SIMPLESCENEGRAPH_VAO_H_

#include <vector>

#include "glheader.h"

class VAO {
public:
    VAO();
    ~VAO();
    void makeCurrent();

    template<typename T>
    void addElementBuffer(std::vector<T> &indices, GLenum usage) {
        makeCurrent();
        glGenBuffers(1, &elementBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(T), &indices[0], usage);
    }

    template<typename T>
    void addVertexAttribBuffer(GLuint index, GLint size, GLenum type, std::vector<T> data, GLenum usage) {
        GLuint id;
        glGenBuffers(1, &id);
        attribs.push_back(id);
        makeCurrent();
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), &data[0], usage);
        glVertexAttribPointer(index, size, type, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(index);
    }

private:
    GLuint id;
    GLuint elementBufferID = 0;
    std::vector<GLuint> attribs;
};


#endif
