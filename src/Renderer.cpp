#include "Renderer.h"

#include <glad/gl.h>

Renderer::Renderer() :
    m_shader("shaders/basic.vert", "shaders/basic.frag"),
    m_vertexCount(0),
    m_vao(0),
    m_vbo(0){

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
}

Renderer::~Renderer(){
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void Renderer::upload(const Geometry& geometry){
    const auto& vertices = geometry.vertices();

    m_vertexCount = geometry.vertexCount();

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Renderer::render(){
    m_shader.use();

    glBindVertexArray(m_vao);

    glDrawArrays(
        GL_TRIANGLES,
        0,
        m_vertexCount
    );

    glBindVertexArray(0);
}
