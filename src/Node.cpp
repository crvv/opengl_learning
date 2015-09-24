#include "node.h"

void Node::draw()
{
    for (auto node : children) {
       node->draw();
    }
}

void Node::addChild(std::shared_ptr<Node> child) {
    children.push_back(child);
}

void Node::removeChild(std::shared_ptr<Node> child) {
    for (auto node = children.cbegin(); node != children.cend(); ++node) {
        if (*node == child) {
            children.erase(node);
            break;
        }
    }
}