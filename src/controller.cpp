#include <SDL2/SDL_timer.h>

#include "glheader.h"
#include "controller.h"


Controller *Controller::self = NULL;

Controller::Controller(Renderer *r, Monitor *m) : renderer(r), monitor(m) {
    keyboardState = SDL_GetKeyboardState(NULL);
}

void Controller::handleEvent(SDL_Event *event) {
    switch (event->type) {
        case SDL_MOUSEMOTION:
            mouseMotion(event->motion.x - monitor->centerX, event->motion.y - monitor->centerY);
            SDL_WarpMouseInWindow(monitor->window, monitor->centerX, monitor->centerY);
            break;
        case SDL_MOUSEWHEEL:
            mouseWheelFov(event->wheel.y);
            mouseWheelUp(static_cast<float>(event->wheel.x));
            break;
        default:
            break;
    }
}
Controller *Controller::getController(Renderer *r, Monitor *m) {
    if (self==NULL) {
        self = new Controller(r, m);
    }
    return self;
}

void Controller::mouseMotion(int x, int y) {
    auto up = renderer->cameraUp;
    auto pos = renderer->cameraPosition;
    auto des = renderer->cameraDestination;

    auto direction = des - pos;

    auto transX = glm::rotate(glm::mat4(1.0f), (float) x*mouseSpeed, -up);
    auto trans = glm::rotate(transX, (float) y*mouseSpeed, glm::cross(up, direction));

    direction = glm::vec3(trans*glm::vec4(direction, 0));
    up = glm::vec3(trans*glm::vec4(up, 0));
    des = direction + pos;

    renderer->cameraDestination = des;
    renderer->cameraUp = up;
}

void Controller::mouseWheelFov(int y) {
    renderer->fov *= (1.0f - mouseWheelFovSpeed*(float) y);
    if (renderer->fov > 3.14f) {
        renderer->fov = 3.14f;
    }
}

void Controller::mouseWheelUp(float x) {
    auto direction = renderer->cameraDestination - renderer->cameraPosition;
    auto trans = glm::rotate(glm::mat4(1.0f), mouseWheelUpSpeed*x, direction);
    renderer->cameraUp = glm::vec3(trans*glm::vec4(renderer->cameraUp, 0));
}

void Controller::keyboard() {
    auto pos = renderer->cameraPosition;
    auto up = renderer->cameraUp;
    auto des = renderer->cameraDestination;
    auto direction = des - pos;

    float ahead = 0.0f, right = 0.0f, high = 0.0f;
    if (keyboardState[SDL_SCANCODE_W]) {
        ahead += walkSpeed*pos[2];
    }
    if (keyboardState[SDL_SCANCODE_S]) {
        ahead -= walkSpeed*pos[2];
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        right += walkSpeed*pos[2];
    }
    if (keyboardState[SDL_SCANCODE_A]) {
        right -= walkSpeed*pos[2];
    }
    if (keyboardState[SDL_SCANCODE_Q]) {
        high += walkSpeed*pos[2];
    }
    if (keyboardState[SDL_SCANCODE_Z]) {
        high -= walkSpeed*pos[2];
    }
    if (keyboardState[SDL_SCANCODE_E]) {
        mouseWheelUp(-keyboardUpSpeed);
    }
    if (keyboardState[SDL_SCANCODE_R]) {
        mouseWheelUp(keyboardUpSpeed);
    }
    duration = SDL_GetTicks() - lastFrameTime;
    lastFrameTime = SDL_GetTicks();
    ahead *= static_cast<float>(duration);
    right *= static_cast<float>(duration);
    high *= static_cast<float>(duration);

    auto move = glm::normalize(direction)*ahead
        + glm::normalize(glm::cross(direction, up))*right
        + glm::normalize(up)*high;
    auto trans = glm::translate(glm::mat4(1.0f), move);

    pos = glm::vec3(trans*glm::vec4(pos, 1));
    des = glm::vec3(trans*glm::vec4(des, 1));

    renderer->cameraDestination = des;
    renderer->cameraPosition = pos;
}


