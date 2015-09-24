#ifndef MYSCENEGRAPH_NODE_H_
#define MYSCENEGRAPH_NODE_H_

#include <vector>
#include <memory>

#include "program.h"
#include "renderer.h"

class Node {
public:
    void draw(Renderer *) final;

    void addChild(std::shared_ptr<Node>) final;
    void removeChild(std::shared_ptr<Node>) final;

protected:
    std::vector<std::shared_ptr<Node>> children;
    std::shared_ptr<Program> program;

    bool drawable = false;
    virtual void beforeDraw(Renderer *);
    virtual void drawGeometry();
    virtual void afterDraw(Renderer *);
};

#endif //MYSCENEGRAPH_NODE_H
