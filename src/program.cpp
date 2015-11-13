#include <iostream>
#include <fstream>
#include <sstream>

#include "program.h"

std::shared_ptr<Program> Program::newProgram(std::vector<Shader> &shaders) {
    auto program = std::make_shared<Program>(shaders);
    if (program->isValid()) {
        return program;
    }
    return NULL;
}

bool Program::isValid() {
    return programID != 0;
}

Program::Program(std::vector<::Program::Shader> &shaders) {
    load(shaders);
}

Program::~Program() {
    glDeleteProgram(programID);
}

void Program::makeCurrent() {
    glUseProgram(programID);
}

const std::string Program::readShaderSrc(const std::string filename) {
    std::ifstream file(filename);
    if (file) {
        std::ostringstream sourceCode;
        sourceCode << file.rdbuf();
        return sourceCode.str();
    }
    throw std::runtime_error("unable to open glsl file " + filename);
}

void Program::load(std::vector<Shader> &shaders) {
    if (shaders.empty()) {
        return;
    }
    programID = glCreateProgram();

    for (auto &shader : shaders) {
        const GLchar *source;
        std::string src;
        try {
            src = readShaderSrc(shader.filename);
            source = src.c_str();
        } catch (std::runtime_error err) {
            clearShaders(shaders);
            return;
        }
        GLuint shaderID = glCreateShader(shader.type);
        shader.shaderID = shaderID;
        glAttachShader(programID, shaderID);
        glShaderSource(shaderID, 1, &source, nullptr);
        glCompileShader(shaderID);
        GLint compileSuccess;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileSuccess);
        if (!compileSuccess) {
            GLsizei length;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
            GLchar *log = new GLchar[length + 1];
            glGetShaderInfoLog(shaderID, length, &length, log);
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete[] log;
            clearShaders(shaders);
            return;
        }
    }

    glLinkProgram(programID);
    GLint linkSuccess;
    glGetProgramiv(programID, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        GLsizei len;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &len);
        GLchar *log = new GLchar[len + 1];
        glGetProgramInfoLog(programID, len, &len, log);
        std::cerr << "Shader linking failed: " << log << std::endl;
        delete[] log;
        clearShaders(shaders);
        return;
    }
    clearShaders(shaders);
}

void Program::clearShaders(std::vector<Shader> &shaders) {
    for (auto &shader : shaders) {
        if (shader.shaderID != 0) {
            glDetachShader(programID, shader.shaderID);
            glDeleteShader(shader.shaderID);
        }
    }
}

void Program::setUniform(std::string name, GLfloat *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform1fv(location, 1, value);
}

void Program::setUniform(std::string name, glm::vec2 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform2fv(location, 1, &(*value)[0]);
}

void Program::setUniform(std::string name, glm::vec3 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform3fv(location, 1, &(*value)[0]);
}

void Program::setUniform(std::string name, glm::vec4 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform4fv(location, 1, &(*value)[0]);
}

void Program::setUniform(std::string name, GLboolean *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform1i(location, *value);
}

void Program::setUniform(std::string name, GLint *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform1iv(location, 1, value);
}

void Program::setUniform(std::string name, glm::ivec2 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform2iv(location, 1, &(*value)[0]);
}

void Program::setUniform(std::string name, glm::ivec3 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform3iv(location, 1, &(*value)[0]);
}

void Program::setUniform(std::string name, glm::ivec4 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform4iv(location, 1, &(*value)[0]);
}

void Program::setUniform(std::string name, GLuint *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform1uiv(location, 1, value);
}

void Program::setUniform(std::string name, glm::uvec2 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform2uiv(location, 1, &(*value)[0]);
}

void Program::setUniform(std::string name, glm::uvec3 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform3uiv(location, 1, &(*value)[0]);
}

void Program::setUniform(std::string name, glm::uvec4 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniform4uiv(location, 1, &(*value)[0]);
}

void Program::setUniform(std::string name, glm::mat2 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix2fv(location, 1, GL_FALSE, &(*value)[0][0]);
}

void Program::setUniform(std::string name, glm::mat3 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, &(*value)[0][0]);
}

void Program::setUniform(std::string name, glm::mat4 *value) {
    auto location = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &(*value)[0][0]);
}
