#include <iostream>

#include "terrain.h"
#include "program.h"

Terrain::Terrain() {
    drawable = true;
    std::vector<Program::Shader> shaders = {
            {GL_VERTEX_SHADER,          "glsl/terrain.vert.glsl"},
            {GL_TESS_CONTROL_SHADER,    "glsl/terrain.cont.glsl"},
            {GL_TESS_EVALUATION_SHADER, "glsl/terrain.eval.glsl"},
            {GL_FRAGMENT_SHADER,        "glsl/terrain.frag.glsl"},
    };
    program = Program::newProgram(shaders);
}

void Terrain::beforeDraw(Renderer *renderer) {
    program->setUniform("screenSize", renderer->getScreenSize());
    program->setUniform("frameEnable", &frameEnable);
    program->setUniform("terrainSideLength", terrainSideLength);

}
//Renderer::~Renderer() {
//    glDeleteBuffers(1, &vertexBufferID);
//    glDeleteBuffers(1, &elementBufferID);
//    glDeleteBuffers(1, &texCoordsBufferID);
//    glDeleteBuffers(1, &normalsBufferID);
//    glDeleteTextures(1, &textureID);
//    glDeleteProgram(program);
//}

void Terrain::drawGeometry(){
    glDrawElements(GL_PATCHES, vertexCount, GL_UNSIGNED_INT, 0);
}

void Renderer::toggleBomb() {
    bombEnable = !bombEnable;
}

void Renderer::toggleFrame() {
    if (frameEnable) {
        program1 = terrainProgram;
        program2 = bombProgram;
    } else {
        program1 = terrainProgramFrame;
        program2 = bombProgramFrame;
    }
    frameEnable = !frameEnable;

    setUniformVariable();
}

void Renderer::transform() {
    viewTrans = glm::lookAt(cameraPosition, cameraDestination, cameraUp);
    projectionTrans = glm::perspective(fov, aspectRatio, 0.00001f, 36.0f);
    modelTrans = glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    auto mvp = projectionTrans * viewTrans * modelTrans;
    glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
}

void Renderer::loadModel() {
    model = new Model(filename);
    auto& vertices = model->vertices;
    auto& indices = model->indices;
    auto& terrain = model->terrain;
    auto& terrainSidePointCount = model->terrainSidePointCount;
    terrainSideLength = model->terrainSideLength;

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glPatchParameteri(GL_PATCH_VERTICES, 4);

    glGenBuffers(1, &elementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
    vertexCount = (GLsizei) indices.size();

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, terrainSidePointCount, terrainSidePointCount,
                 0, GL_RED, GL_FLOAT, &terrain[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLsizeiptr bombDataSize = model->bombFloatCount * sizeof(GLfloat);
    glGenBuffers(1, &bombPositionID);
    glBindBuffer(GL_ARRAY_BUFFER, bombPositionID);
    glBufferData(GL_ARRAY_BUFFER, bombDataSize, NULL, GL_STATIC_DRAW);
    auto bombDataPointer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    model->readBomb(bombDataPointer);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    bombCount = (GLsizei) model->bombFloatCount / 2;

    delete model;
    model = NULL;
}
