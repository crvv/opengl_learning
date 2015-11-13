#ifndef SIMPLESCENEGRAPH_MONITOR_H_
#define SIMPLESCENEGRAPH_MONITOR_H_

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>

#include "renderer.h"
#include "controller.h"
#include "fps.h"

class Controller;

class Monitor {
    friend class Controller;

public:
    Monitor(int, int);
    ~Monitor();
    void display(Renderer &);
    void setShouldExit();

private:
    SDL_Window *window;
    SDL_GLContext context;

    const int width, height;
    const int centerX = width / 2, centerY = height / 2;

    bool sdlShouldRunning = true;
    SDL_Event event;

    FpsCounter counter;
    Controller *controller;
    Renderer *renderer;

    void die(std::string);
    void checkError(int);
    void handleEvent();
};

#endif
