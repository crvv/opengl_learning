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
