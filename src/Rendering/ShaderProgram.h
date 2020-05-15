#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>
#include <optional>
#include "glm/glm.hpp"

#include "Shader.h"

class ShaderProgram {

    std::optional<unsigned int> _id;

    std::string _vertexFilename,
                _fragmentFilename;

    public:
        ShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName);

        void compile();
        void bind();
        void unbind();

        void setMat4(const std::string& name, const glm::mat4& mat4);
};

#endif
