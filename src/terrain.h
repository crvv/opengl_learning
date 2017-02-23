#ifndef SIMPLESCENEGRAPH_TERRAIN_H_
#define SIMPLESCENEGRAPH_TERRAIN_H_

#include <string>

#include "spline.h"
#include "glheader.h"
#include "node.h"
#include "texture.h"
#include "controller.h"

class Terrain : public Node {

public:
    Terrain(Controller*);

private:
    void beforeDraw(Renderer *) override;
    void afterDraw(Renderer *) override;
    void drawGeometry() override;
    void loadModel();

    tk::spline::coefficients coff;
    std::shared_ptr<Texture> texture;
    GLboolean frameEnable = GL_FALSE;
    GLfloat terrainSideLength;
    GLsizei vertexCount;
};

#endif
