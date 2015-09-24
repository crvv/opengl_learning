#ifndef MYSCENEGRAPH_RENDERER_H_
#define MYSCENEGRAPH_RENDERER_H_

#include <string>
#include <memory>

#include "glheader.h"
#include "node.h"

class Renderer {
    friend class Controller;

public:
    Renderer(int, int);
    void draw();
    glm::mat4 *getMvp();
    glm::vec2 *getScreenSize();

private:
    std::shared_ptr<Node> root;

    glm::vec2 screenSize;
    float aspectRatio;

    glm::mat4 mvp;
    glm::mat4 projectionTrans;
    glm::mat4 viewTrans;
    float fov;

    glm::vec3 cameraDestination;
    glm::vec3 cameraPosition;
    glm::vec3 cameraUp;
    float scale;

    void transform();
    void setVariable();
};

#endif
