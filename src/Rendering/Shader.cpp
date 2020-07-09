#include <fstream>

#include <GlobalLogger.hpp>

#include "Exception/ShaderCompilationFailureException.h"
#include "Rendering/Shader.h"

using Logger::globalLogger;

Shader::Shader(GLenum type, std::string fileName) : _id(glCreateShader(type)), _fileName(fileName) {}

Shader::~Shader() {
    glDeleteShader(_id);
}

std::string Shader::getCode() {
    std::ifstream inputStream;
    inputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        inputStream.open(_fileName);
        std::stringstream ss;
        ss << inputStream.rdbuf();
        inputStream.close();
        return ss.str();
    } catch (std::ifstream::failure& e) {
        throw ShaderCompilationFailureException(_fileName, "Could not read from file");
    }
}

void Shader::compile() {
    std::string shaderCode = getCode();
    const char* shaderCodeCstr = shaderCode.c_str();
    glShaderSource(_id, 1, &shaderCodeCstr, NULL);
    glCompileShader(_id);

    int success;
    glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(_id, 512, NULL, infoLog);
        throw ShaderCompilationFailureException(_fileName, infoLog);
    } else {
        globalLogger.info("Successfully compiled ", _fileName, "!");
    }
}
