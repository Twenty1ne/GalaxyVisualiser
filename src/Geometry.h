#pragma once

#include <cstddef>
#include <vector>

class Geometry{
public:
    Geometry(const std::vector<float>& vertices, std::size_t floatsPerVertex, const std::vector<unsigned int>& indices);

    const std::vector<float>& vertices() const;
    const std::vector<unsigned int>& indices() const;

    std::size_t vertexCount() const;
    std::size_t indexCount() const;
    std::size_t floatsPerVertex() const;

private:
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;

    std::size_t m_floatsPerVertex;
};
