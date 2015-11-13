#include "monitor.h"
#include "terrain.h"

int main() {
    int width = 1024, height = 768;
    Monitor monitor(width, height);
    Renderer renderer(width, height);
    renderer.addNode(std::make_shared<Terrain>());
    monitor.display(renderer);

    return 0;
}

