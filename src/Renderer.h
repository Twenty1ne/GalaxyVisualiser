#pragma once

#include "Geometry.h"
#include "Shader.h"

#include <cstddef>
#include <glm/mat4x4.hpp>

class Renderer{
public:
    Renderer();
    ~Renderer();

    void upload(const Geometry& geometry);
    void render(float aspectRatio, const glm::mat4& view);

private:
    unsigned int m_vao;
    unsigned int m_vbo;
    std::size_t m_vertexCount;

    Shader m_shader;
};
