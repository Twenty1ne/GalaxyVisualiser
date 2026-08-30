#pragma once

#include "Geometry.h"
#include "Shader.h"

#include <cstddef>
#include <glm/mat4x4.hpp>
#include <vector>

class Renderer{
public:
    Renderer();
    ~Renderer();

    std::size_t upload(const Geometry& geometry);
    void render(std::size_t geometryId, float aspectRatio, const glm::mat4& view, bool glow = false);

private:
    struct GpuGeometry{
        unsigned int vao;
        unsigned int vbo;
        unsigned int ebo;

        std::size_t indexCount;
        std::size_t vertexCount;

        PrimitiveType primitiveType;
    };

    std::vector<GpuGeometry> m_geometries;

    Shader m_pointShader;
    Shader m_lineShader;
    Shader m_triangleShader;
};
