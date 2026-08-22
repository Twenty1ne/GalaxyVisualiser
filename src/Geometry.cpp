#include "Geometry.h"

Geometry::Geometry(const std::vector<float>& vertices, std::size_t floatsPerVertex, PrimitiveType primitiveType) :
    m_vertices(vertices),
    m_floatsPerVertex(floatsPerVertex),
    m_primitiveType(primitiveType){}

Geometry::Geometry(const std::vector<float>& vertices, std::size_t floatsPerVertex, const std::vector<unsigned int>& indices, PrimitiveType primitiveType) :
    m_vertices(vertices),
    m_indices(indices),
    m_floatsPerVertex(floatsPerVertex),
    m_primitiveType(primitiveType){}

const std::vector<float>& Geometry::vertices() const{
    return m_vertices;
}

const std::vector<unsigned int>& Geometry::indices() const{
    return m_indices;
}

std::size_t Geometry::vertexCount() const{
    return m_vertices.size() / m_floatsPerVertex;
}

std::size_t Geometry::indexCount() const{
    return m_indices.size();
}

std::size_t Geometry::floatsPerVertex() const{
    return m_floatsPerVertex;
}

PrimitiveType Geometry::primitiveType() const{
    return m_primitiveType;
}
