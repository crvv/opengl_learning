#include "node.h"

void Node::draw(Renderer *renderer) {
    if (drawable) {
        program->makeCurrent();
        program->setUniform("mvp", renderer->getMvp());
        beforeDraw(renderer);
        drawGeometry();
        afterDraw(renderer);
    }
    for (auto &node : children) {
        node->draw(renderer);
    }
}

void Node::addChild(std::shared_ptr<Node> child) {
    children.push_back(child);
}

void Node::removeChild(std::shared_ptr<Node> child) {
    for (auto node = children.cbegin(); node!=children.cend(); ++node) {
        if (*node==child) {
            children.erase(node);
            break;
        }
    }
}
void Node::beforeDraw(Renderer *) { }
void Node::drawGeometry() { }
void Node::afterDraw(Renderer *) { }
