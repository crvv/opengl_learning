#include "glheader.h"
#include "renderer.h"


Renderer::Renderer(int x, int y) :
    screenSize(x, y) {
    aspectRatio = static_cast<float>(x)/static_cast<float>(y);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    root = std::make_shared<Node>();
    setVariable();
}

void Renderer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    transform();
    root->draw(this);
}

void Renderer::transform() {
    viewTrans = glm::lookAt(cameraPosition, cameraDestination, cameraUp);
    projectionTrans = glm::perspective(fov, aspectRatio, 0.00001f, 36.0f);

    mvp = projectionTrans*viewTrans;
}

void Renderer::setVariable() {
    fov = 0.8f;
    cameraDestination = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraPosition = glm::vec3(0.0f, 0.0f, 1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    scale = 1.0f;
}

glm::mat4 *Renderer::getMvp() {
    return &mvp;
}
glm::vec2 *Renderer::getScreenSize() {
    return &screenSize;
}