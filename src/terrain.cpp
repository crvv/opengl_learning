#include <iostream>
#include <fstream>

#include "terrain.h"
#include "model.h"

Terrain::Terrain() {
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
}

void Terrain::beforeDraw(Renderer *renderer) {
    Node::beforeDraw(renderer);
    program->setUniform("screenSize", renderer->getScreenSize());
    program->setUniform("frameEnable", &frameEnable);
    program->setUniform("terrainSideLength", &terrainSideLength);
    if (frameEnable) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void Terrain::afterDraw(Renderer *) {
    if (frameEnable) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void Terrain::drawGeometry() {
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glDrawElements(GL_PATCHES, vertexCount, GL_UNSIGNED_INT, 0);
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

}
