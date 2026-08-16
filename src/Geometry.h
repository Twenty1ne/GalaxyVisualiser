#pragma once

#include <cstddef>
#include <vector>

class Geometry{
public:
    Geometry(const std::vector<float>& vertices);

    const std::vector<float>& vertices() const;
    std::size_t vertexCount() const;

private:
    std::vector<float> m_vertices;
};
