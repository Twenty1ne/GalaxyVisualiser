#pragma once

#include "Geometry.h"
#include "Shader.h"

#include <cstddef>

class Renderer{
public:
    Renderer();
    ~Renderer();

    void upload(const Geometry& geometry);
    void render();

private:
    unsigned int m_vao;
    unsigned int m_vbo;
    std::size_t m_vertexCount;

    Shader m_shader;
};
