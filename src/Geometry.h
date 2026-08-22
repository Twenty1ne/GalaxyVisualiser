#pragma once

#include <cstddef>
#include <vector>

enum class PrimitiveType{
    Points,
    Triangles
};

class Geometry{
public:
    static constexpr std::size_t PositionComponents = 3;
    static constexpr std::size_t ColorComponents = 3;


    Geometry(const std::vector<float>& vertices, std::size_t floatsPerVertex, PrimitiveType primitiveType);

    Geometry(const std::vector<float>& vertices, std::size_t floatsPerVertex, const std::vector<unsigned int>& indices, PrimitiveType primitiveType);

    const std::vector<float>& vertices() const;
    const std::vector<unsigned int>& indices() const;

    std::size_t vertexCount() const;
    std::size_t indexCount() const;
    std::size_t floatsPerVertex() const;

    PrimitiveType primitiveType() const;

private:
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;

    std::size_t m_floatsPerVertex;
    PrimitiveType m_primitiveType;
};
