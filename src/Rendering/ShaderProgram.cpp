#include "ShaderProgram.h"
#include "Exception/ShaderCompilationFailureException.h"
#include "Logger/GlobalLogger.hpp"

using Logger::globalLogger;

ShaderProgram::ShaderProgram(std::string vertexShaderFileName, std::string fragmentShaderFileName) :
    _vertexFilename(vertexShaderFileName), _fragmentFilename(fragmentShaderFileName)
{}

void ShaderProgram::compile() {
    Shader vertexShader(GL_VERTEX_SHADER, _vertexFilename);
    Shader fragmentShader(GL_FRAGMENT_SHADER, _fragmentFilename);

    vertexShader.compile();
    fragmentShader.compile();

    _id.emplace(glCreateProgram());

    glAttachShader(*_id, vertexShader.getId());
    glAttachShader(*_id, fragmentShader.getId());
    glLinkProgram(*_id);

    int success;
    glGetProgramiv(*_id, GL_LINK_STATUS, &success);
    if (!success) {
        char infolog[512];
        glGetProgramInfoLog(*_id, 512, NULL, infolog);
        throw ShaderCompilationFailureException("", infolog);
    } else {
        globalLogger.info("Successfully linked shaders!");
    }
}

void ShaderProgram::bind() {
    if (_id) {
        glUseProgram(*_id);
    }
}

void ShaderProgram::unbind() {
    if (_id) {
        glUseProgram(0);
    }
}
