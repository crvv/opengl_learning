#ifndef SIMPLESCENEGRAPH_TERRAIN_H_
#define SIMPLESCENEGRAPH_TERRAIN_H_

#include <functional>
#include <string>

#include "json.hpp"
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
    void updateSplineCoff();

    std::function<void()>* getPointSelectFunction(int);
    std::function<void()>* getPointMoveFunction(float, float);

    nlohmann::json vectorData;
    std::string vectorDataFilename = "vector.json";
    int currentPoint = 0;
    std::vector<double> xs, ys;
    tk::spline::coefficients coff;
    std::shared_ptr<Texture> texture;
    GLboolean frameEnable = GL_FALSE;
    GLfloat terrainSideLength;
    GLsizei vertexCount;
};

#endif
