#ifndef MYSCENEGRAPH_TERRAIN_H_
#define MYSCENEGRAPH_TERRAIN_H_

#include <string>

#include "glheader.h"
#include "node.h"

class Terrain: public Node {

public:
    Terrain();

private:
    void beforeDraw(Renderer *) override;
    void drawGeometry() override;

    GLboolean frameEnable = GL_TRUE;
    GLfloat terrainSideLength;

    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint elementBufferID;
    GLuint bombPositionID;
    GLuint textureID;
    GLsizei vertexCount;
    GLsizei bombCount;
};

#endif
