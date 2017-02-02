#ifndef VERTEX_H
#define VERTEX_H

/*!
 * Simple data structure defining a vertex that gets pushed
 * to OpenGL. Each vertex has a position, a color, and a normal
 * vector (normal is used for lighting calculations)
 */
struct Vertex {
    /*! Position of this vertex */
    float pos[3];
    /*! Color of this vertex */
    float color[3];
    /*! Normal of this vertex */
    float normal[3];

    /*! Initializes new vertex */
    Vertex(float _pos[3], float _color[3], float _normal[3]) :
        pos   {_pos[0],    _pos[1],    _pos[2]},
        color {_color[0],  _color[1],  _color[2]},
        normal{_normal[0], _normal[1], _normal[2]} {}

    /*! Initializes new vertex */
    Vertex() : pos{0,0,0}, color{0,0,0}, normal{0,0,0} {}
};

#endif
