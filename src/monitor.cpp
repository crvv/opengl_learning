#include <iostream>
#include <SDL2/SDL.h>

#include "monitor.h"

void Monitor::die(std::string msg) {
    std::cerr << msg << std::endl;
    SDL_Quit();
    exit(1);
}

void Monitor::checkError(int line = -1) {
    std::string error(SDL_GetError());
    if (!error.empty()) {
        std::cout << "SDL Error: " << error << std::endl;
        if (line != -1) {
            std::cout << "line: " << line << std::endl;
        }
        SDL_ClearError();
    }
}

Monitor::Monitor(int w, int h) : width(w), height(h) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        die("Unable to initialize SDL video subsystem");
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    window = SDL_CreateWindow("OpenGL SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_OPENGL);
    if (!window) {
        die("Unable to create window");
    }

    checkError(__LINE__);
    context = SDL_GL_CreateContext(window);
    checkError(__LINE__);
    SDL_GL_SetSwapInterval(0);

    glewExperimental = GL_TRUE;
    auto err = glewInit();
    if (err != GLEW_OK) {
        std::string message(reinterpret_cast<const char *>(glewGetErrorString(err)));
        throw std::runtime_error(message);
    }

    std::cout << glGetString(GL_RENDERER) << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

}

void Monitor::setShouldExit() {
    sdlShouldRunning = false;
}

Monitor::~Monitor() {
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Monitor::handleEvent() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                setShouldExit();
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        setShouldExit();
                        break;
                    default:
                        break;
                }
            default:
                controller->handleEvent(&event);
                break;
        }
    }
}

void Monitor::display(Renderer &r) {
    renderer = &r;

    SDL_WarpMouseInWindow(window, centerX, centerY);
    controller = Controller::getController(renderer, this);
    SDL_ShowCursor(0);

    while (sdlShouldRunning) {
        std::cout << "\r" << std::fixed << counter.addFrame(SDL_GetTicks()) << " ms/frame";
        handleEvent();
        controller->keyboard();
        renderer->draw();
        SDL_GL_SwapWindow(window);
    }
}