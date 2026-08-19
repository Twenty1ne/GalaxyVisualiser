#include "Geometry.h"

Geometry::Geometry(const std::vector<float>& vertices) :
    m_vertices(vertices){}

const std::vector<float>& Geometry::vertices() const{
    return m_vertices;
}

std::size_t Geometry::vertexCount() const{
    return m_vertices.size() / 3;
}
