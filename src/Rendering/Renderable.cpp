#include <GlobalLogger.hpp>
#include <filesystem>

#include "Rendering/Renderable.h"
#include "Rendering/stb_image.h"
#include "Rendering/Image.h"

using Logger::globalLogger;

std::map<std::string, GLint> Renderable::_textureIndexes{};

struct VertexAttribsCalculator {
    struct VertexAttrib {
        int index;
        int count;
        void* offset;
        GLenum type;
    };

    std::vector<VertexAttrib> attribs;
    int currIndex = 0;
    size_t totalSize = 0;

    template<typename T>
    void addVertexAttrib(GLenum type, int count) {
        VertexAttrib attrib;
        attrib.index = currIndex++;
        attrib.count = count;
        attrib.offset = (void *)totalSize;
        attrib.type = type;
        attribs.push_back(attrib);

        totalSize += sizeof(T) * count;
    }

    template<typename T>
    void addVertexAttrib(int count);

    template<>
    void addVertexAttrib<GLbyte>(int count) { addVertexAttrib<GLbyte>(GL_BYTE, count); }
    template<>
    void addVertexAttrib<GLubyte>(int count) { addVertexAttrib<GLubyte>(GL_UNSIGNED_BYTE, count); }
    template<>
    void addVertexAttrib<GLshort>(int count) { addVertexAttrib<GLshort>(GL_SHORT, count); }
    template<>
    void addVertexAttrib<GLushort>(int count) { addVertexAttrib<GLushort>(GL_UNSIGNED_SHORT, count); }
    template<>
    void addVertexAttrib<GLint>(int count) { addVertexAttrib<GLint>(GL_INT, count); }
    template<>
    void addVertexAttrib<GLuint>(int count) { addVertexAttrib<GLuint>(GL_UNSIGNED_INT, count); }
    template<>
    void addVertexAttrib<GLfloat>(int count) { addVertexAttrib<GLfloat>(GL_FLOAT, count); }
    template<>
    void addVertexAttrib<GLdouble>(int count) { addVertexAttrib<GLdouble>(GL_DOUBLE, count); }

    void send() {
        for (VertexAttrib attrib : attribs) {
            glVertexAttribPointer(attrib.index, attrib.count, attrib.type, GL_FALSE /* normalized */, totalSize, attrib.offset);
        }
    }
};

void Renderable::initBuffer() {
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    VertexAttribsCalculator vAttribs;
    vAttribs.addVertexAttrib<float>(3);
    vAttribs.addVertexAttrib<float>(3);
    vAttribs.addVertexAttrib<float>(3);
    vAttribs.addVertexAttrib<float>(2);
    vAttribs.send();

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
