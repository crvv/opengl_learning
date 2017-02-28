#include <iostream>
#include <fstream>

#include "json.hpp"
#include "terrain.h"
#include "model.h"

Terrain::Terrain(Controller* controller) {
    drawable = true;
    std::vector<Program::Shader> shaders = {
            {GL_VERTEX_SHADER,          "glsl/terrain.vert.glsl", 0},
            {GL_TESS_CONTROL_SHADER,    "glsl/terrain.cont.glsl", 0},
            {GL_TESS_EVALUATION_SHADER, "glsl/terrain.eval.glsl", 0},
            {GL_FRAGMENT_SHADER,        "glsl/terrain.frag.glsl", 0},
    };
    program = Program::newProgram(shaders);
    vao = std::make_shared<VAO>();

    loadModel();
    controller->setKeyboardEventCallback(SDL_SCANCODE_F,
            new std::function<void()>([this]() {
                if (frameEnable == GL_TRUE) frameEnable = GL_FALSE;
                else frameEnable = GL_TRUE;
            }));
    controller->setKeyboardEventCallback(SDL_SCANCODE_1, getPointSelectFunction(-1));
    controller->setKeyboardEventCallback(SDL_SCANCODE_2, getPointSelectFunction(1));
    controller->setKeyboardEventCallback(SDL_SCANCODE_UP, getPointMoveFunction(0, 1));
    controller->setKeyboardEventCallback(SDL_SCANCODE_LEFT, getPointMoveFunction(-1, 0));
    controller->setKeyboardEventCallback(SDL_SCANCODE_DOWN, getPointMoveFunction(0, -1));
    controller->setKeyboardEventCallback(SDL_SCANCODE_RIGHT, getPointMoveFunction(1, 0));
    controller->setKeyboardEventCallback(SDL_SCANCODE_SPACE, new std::function<void()>([this]() {
        std::vector<std::vector<double>> jsonData(xs.size());
        int i = 0;
        std::for_each(jsonData.begin(), jsonData.end(), [this, &i](auto& v) {
            v.push_back(xs[i]);
            v.push_back(ys[i]);
            i++;
        });
        std::ofstream(roadPointFilename) << nlohmann::json(jsonData);
        std::cout << "saved" << std::endl;
    }));
    controller->setKeyboardEventCallback(SDL_SCANCODE_N, new std::function<void()>([this]() {
        ys.insert(ys.begin() + currentPoint + 1, ys[currentPoint]);
        double newX;
        if (currentPoint == xs.size() - 1) {
            newX = (xs[currentPoint] + 1) / 2;
        } else {
            newX = (xs[currentPoint] + xs[currentPoint + 1]) / 2;
        }
        xs.insert(xs.begin() + currentPoint + 1, newX);
        updateSplineCoff();
    }));
    controller->setKeyboardEventCallback(SDL_SCANCODE_M, new std::function<void()>([this]() {
        xs.erase(xs.begin() + currentPoint);
        ys.erase(ys.begin() + currentPoint);
        if (currentPoint == xs.size()) {
            currentPoint = static_cast<int>(xs.size() - 1);
        }
        updateSplineCoff();
    }));
}

std::function<void()>* Terrain::getPointMoveFunction(float dx, float dy) {
    return new std::function<void()>([this, dx, dy]() {
        xs[currentPoint] += dx * 0.002;
        ys[currentPoint] += dy * 0.002;
        std::cout << xs[currentPoint] << ", " << ys[currentPoint] << std::endl;
        updateSplineCoff();
    });
}
std::function<void()>* Terrain::getPointSelectFunction(int delta) {
    return new std::function<void()>([this, delta]() {
        currentPoint += delta;
        int max = static_cast<int>(xs.size());
        if (currentPoint == -1) {
            currentPoint = max - 1;
        }
        if (currentPoint == max) {
            currentPoint = 0;
        }
    });
}

void Terrain::beforeDraw(Renderer* renderer) {
    Node::beforeDraw(renderer);
    program->setUniform("screenSize", renderer->getScreenSize());
    program->setUniform("frameEnable", &frameEnable);
    program->setUniform("terrainSideLength", &terrainSideLength);
    program->setUniform("len", static_cast<int>(coff.xs.size()));
    program->setUniform("xs", coff.xs);
    program->setUniform("ys", coff.ys);
    program->setUniform("as", coff.as);
    program->setUniform("bs", coff.bs);
    program->setUniform("cs", coff.cs);
    program->setUniform("currentPoint", currentPoint);

    if (frameEnable) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void Terrain::afterDraw(Renderer*) {
    if (frameEnable) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Terrain::drawGeometry() {
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDrawElements(GL_PATCHES, vertexCount, GL_UNSIGNED_INT, nullptr);
}

void Terrain::loadModel() {
    std::ifstream patchFile("model/patch", std::ios::binary);
    std::ifstream terrainFile("model/terrain", std::ios::binary);
    if (!patchFile.good() || !terrainFile.good()) {
        throw std::runtime_error("unable to read file");
    }
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> terrain;
    GLuint terrainSidePointCount;

    ModelUtils::readFileToValue(patchFile, vertices, indices);
    ModelUtils::readFileToValue(terrainFile, terrainSideLength, terrainSidePointCount, terrain);

    vertexCount = static_cast<GLsizei>(indices.size());
    vao->addElementBuffer(indices, GL_STATIC_DRAW);
    vao->addVertexAttribBuffer(0, 2, GL_FLOAT, vertices, GL_STATIC_DRAW);

    texture = std::make_shared<Texture>(GL_TEXTURE_2D, terrainSidePointCount, terrain);

    std::ifstream roadPointFile(roadPointFilename);
    nlohmann::json roadData;
    roadPointFile >> roadData;
    for (auto point : roadData) {
        xs.push_back(point[0]);
        ys.push_back(point[1]);
    }
    updateSplineCoff();
}

void Terrain::updateSplineCoff() {
    tk::spline s;
    s.set_points(xs, ys);
    coff = s.getCoefficients();
}
