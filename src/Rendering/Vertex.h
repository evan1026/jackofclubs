#ifndef VERTEX_H
#define VERTEX_H

#include <SFML/System.hpp>

struct Vertex {
    float pos[3];
    float color[3];

    Vertex(float _pos[3], float _color[3]) :
        pos{_pos[0], _pos[1], _pos[2]},
        color{_color[0], _color[1], _color[2]} {}

    Vertex() : pos{0,0,0}, color{0,0,0} {}
};

#endif
