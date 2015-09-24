#ifndef SIMPLESCENEGRAPH_PROGRAM_H_
#define SIMPLESCENEGRAPH_PROGRAM_H_

#include <string>
#include <vector>
#include <memory>

#include "glheader.h"

class Program {
public:
    struct Shader {
        GLenum type;
        std::string filename;
        GLuint shaderID;
    };

    static std::shared_ptr<Program> newProgram(std::vector<Shader> &);
    Program(std::vector<Shader> &);
    ~Program();

    void makeCurrent();
    void setUniform(std::string, GLfloat *);
    void setUniform(std::string, glm::vec2 *);
    void setUniform(std::string, glm::vec3 *);
    void setUniform(std::string, glm::vec4 *);
    void setUniform(std::string, GLboolean *);
    void setUniform(std::string, GLint *);
    void setUniform(std::string, glm::ivec2 *);
    void setUniform(std::string, glm::ivec3 *);
    void setUniform(std::string, glm::ivec4 *);
    void setUniform(std::string, GLuint *);
    void setUniform(std::string, glm::uvec2 *);
    void setUniform(std::string, glm::uvec3 *);
    void setUniform(std::string, glm::uvec4 *);
    void setUniform(std::string, glm::mat2 *);
    void setUniform(std::string, glm::mat3 *);
    void setUniform(std::string, glm::mat4 *);

private:
    GLuint programID = 0;

    bool isValid();

    void load(std::vector<Shader> &);
    void clearShaders(std::vector<Shader> &);

    static const std::string readShaderSrc(const std::string);
};

#endif
