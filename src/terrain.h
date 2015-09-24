#ifndef MYSCENEGRAPH_TERRAIN_H_
#define MYSCENEGRAPH_TERRAIN_H_

#include <string>

#include "glheader.h"
#include "node.h"
#include "texture.h"

class Terrain: public Node {

public:
    Terrain();

private:
    void beforeDraw(Renderer *) override;
    void afterDraw(Renderer *) override;
    void drawGeometry() override;
    void loadModel();

    std::shared_ptr<Texture> texture;
    GLboolean frameEnable = GL_FALSE;
    GLfloat terrainSideLength;
    GLsizei vertexCount;
};

#endif
