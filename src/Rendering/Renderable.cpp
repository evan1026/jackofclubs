#include <GlobalLogger.hpp>
#include <filesystem>

#include "Rendering/Renderable.h"
#include "Rendering/stb_image.h"
#include "Rendering/Image.h"

using Logger::globalLogger;

std::map<std::string, GLint> Renderable::_textureIndexes{};

void Renderable::initBuffer() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    // TODO function to calculate this part
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);

    _initialized = true;
    _bufferSize = 0;
}

void Renderable::setBufferData(const std::vector<Vertex>& vertexes) {
    if (_initialized) {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexes.size() * sizeof(Vertex), &vertexes[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        _bufferSize = vertexes.size();
    }
}

void Renderable::drawFromBuffer(GLenum type) {
    if (_initialized) {
        glBindVertexArray(_vao);
        glDrawArrays(type, 0, _bufferSize);
        glBindVertexArray(0);
    }
}

bool Renderable::loadTexture(std::string name) {
    std::filesystem::path imagePath("resources");
    imagePath /= name;
    imagePath += ".png";
    Image img(imagePath);

    if (!img.getData()) {
        globalLogger.error("Could not load texture named \"", name, "\". Error loading: ", imagePath);
        return false;
    }

    // Set up texture
    unsigned int textureIndex;
    glGenTextures(1, &textureIndex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIndex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.getWidth(), img.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.getData());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    if(glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
        GLfloat fLargest;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    _textureIndexes[name] = textureIndex;
    globalLogger.info("Successfully loaded ", name);
    return true;
}

void Renderable::setTexture(std::string name) {
    if (_textureIndexes.find(name) != _textureIndexes.end() || loadTexture(name)) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _textureIndexes[name]);
    }
}
