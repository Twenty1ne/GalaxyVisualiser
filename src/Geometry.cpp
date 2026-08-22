#include "Geometry.h"

Geometry::Geometry(const std::vector<float>& vertices, std::size_t floatsPerVertex) :
    m_vertices(vertices),
    m_floatsPerVertex(floatsPerVertex){}

const std::vector<float>& Geometry::vertices() const{
    return m_vertices;
}

std::size_t Geometry::vertexCount() const{
    return m_vertices.size() / m_floatsPerVertex;
}

std::size_t Geometry::floatsPerVertex() const{
    return m_floatsPerVertex;
}
