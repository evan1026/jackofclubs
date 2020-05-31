#pragma once

#include <filesystem>

class Image {
    unsigned char* _data;
    int _width;
    int _height;
    int _numChannels;

    public:

        Image(std::filesystem::path path);

        Image(const Image& other) = delete;
        Image& operator=(const Image& other) = delete;
        ~Image();

        Image(Image&& other);
        Image& operator=(Image&& other);

        const unsigned char* const getData() const { return _data; }
        int getWidth() const { return _width; }
        int getHeight() const { return _height; }
        int getNumChannels() const { return _numChannels; }
};
