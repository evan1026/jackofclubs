#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>
#include <optional>
#include <functional>
#include <map>
#include "glm/glm.hpp"

#include "Shader.h"

class ShaderProgram {

    std::optional<unsigned int> _id;

    std::string _vertexFilename,
                _fragmentFilename;

    std::map<std::string, GLint> _uniformLocations;

    GLint getUniformLocation(std::string name);

    void safeCallVector(std::function<void(GLint, GLsizei, const GLfloat*)>, std::string, const GLfloat*);
    void safeCallMatrix(std::function<void(GLint, GLsizei, GLboolean, const GLfloat*)>, std::string, GLboolean, const GLfloat*);

    public:
        ShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName);

        void compile();
        void bind();
        void unbind();

        void setVec2(const std::string& name, const glm::vec2& vec2) { safeCallVector(glUniform2fv, name, &vec2[0]); }
        void setVec3(const std::string& name, const glm::vec3& vec3) { safeCallVector(glUniform3fv, name, &vec3[0]); }
        void setVec4(const std::string& name, const glm::vec4& vec4) { safeCallVector(glUniform4fv, name, &vec4[0]); }
        void setMat2(const std::string& name, const glm::mat2& mat2) { safeCallMatrix(glUniformMatrix2fv, name, GL_FALSE, &mat2[0][0]); }
        void setMat3(const std::string& name, const glm::mat3& mat3) { safeCallMatrix(glUniformMatrix3fv, name, GL_FALSE, &mat3[0][0]); }
        void setMat4(const std::string& name, const glm::mat4& mat4) { safeCallMatrix(glUniformMatrix4fv, name, GL_FALSE, &mat4[0][0]); }
};

#endif
