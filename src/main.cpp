#include "monitor.h"

int main() {

    int width = 1024, height = 768;
    Monitor monitor(width, height);
    Renderer renderer(width, height);
    monitor.display(renderer);

    return 0;
}

