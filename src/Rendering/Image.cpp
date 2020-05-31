#include <filesystem>

#include "Rendering/Image.h"
#include "Rendering/stb_image.h"

Image::Image(std::filesystem::path path) {
    _data = stbi_load(path.c_str(), &_width, &_height, &_numChannels, 0);
}

Image::~Image() {
    if (_data) {
        stbi_image_free(_data);
    }
}

Image::Image(Image&& other) {
    _data = other._data;
    other._data = nullptr;
}

Image& Image::operator=(Image&& other) {
    if (_data) {
        stbi_image_free(_data);
    }
    _data = other._data;
    other._data = nullptr;
    return *this;
}

