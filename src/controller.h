#ifndef SIMPLESCENEGRAPH_CONTROLLER_H_
#define SIMPLESCENEGRAPH_CONTROLLER_H_


#include <SDL2/SDL_scancode.h>

#include "renderer.h"
#include "monitor.h"

class Monitor;

class Controller {

public:
    static Controller *getController(Renderer *, Monitor *);
    void handleEvent(SDL_Event *);
    void keyboard();

private:
    Controller(Renderer *, Monitor *);

    void mouseMotion(int, int);
    void mouseWheelFov(int);
    void mouseWheelUp(float);

    Renderer *renderer;
    Monitor *monitor;

    static Controller *self;

    float mouseSpeed = 0.001f;
    float walkSpeed = 9.8f / 16000.0f;
    float mouseWheelFovSpeed = 0.1f;
    float mouseWheelUpSpeed = 0.314159f / 2.0f;
    float keyboardUpSpeed = 0.04f;

    Uint32 lastFrameTime = 0;
    Uint32 duration = 0;

    const Uint8 *keyboardState = NULL;
};

#endif
