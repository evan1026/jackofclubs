#pragma once

#include <string>

#include <GL/glew.h>

class Shader {
    unsigned int _id;
    std::string _fileName;

    std::string getCode();

    public:
        Shader(GLenum type, std::string fileName);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void compile();

        auto getId() const { return _id; }
};
