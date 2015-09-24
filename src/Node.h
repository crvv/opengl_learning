#ifndef MYSCENEGRAPH_NODE_H_
#define MYSCENEGRAPH_NODE_H_

#include <vector>
#include <memory>

class Node {
public:
    virtual void draw();

    virtual void addChild(std::shared_ptr<Node>);

    virtual void removeChild(std::shared_ptr<Node>);

private:
    std::vector<std::shared_ptr<Node>> children;
};

#endif //MYSCENEGRAPH_NODE_H
