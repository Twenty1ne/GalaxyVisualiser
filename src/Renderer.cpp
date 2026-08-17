#include "Renderer.h"

#include <glad/gl.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

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

void Renderer::render(float aspectRatio){
    m_shader.use();

    glm::mat4 model(1.0f);

    model = glm::translate(
        model,
        glm::vec3(0.0, 0.0f, -2.0f)
    );

    model = glm::rotate(
        model,
        glm::radians(45.0f),
        glm::vec3(0.0f, 0.0f, 1.0f)
    );

    m_shader.setMat4("model", model);

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        aspectRatio,
        0.1f,
        100.0f
    );

    m_shader.setMat4("projection", projection);

    glBindVertexArray(m_vao);

    glDrawArrays(
        GL_TRIANGLES,
        0,
        m_vertexCount
    );

    glBindVertexArray(0);
}
