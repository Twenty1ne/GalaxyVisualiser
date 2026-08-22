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
    unsigned int m_ebo;

    std::size_t m_indexCount;
    std::size_t m_vertexCount;

    PrimitiveType m_primitiveType;

    Shader m_shader;
};
