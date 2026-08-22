#include "Renderer.h"
#include "Geometry.h"

#include <glad/gl.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

Renderer::Renderer() :
    m_shader("shaders/basic.vert", "shaders/basic.frag"),
    m_primitiveType(PrimitiveType::Triangles),
    m_indexCount(0),
    m_vertexCount(0),
    m_vao(0),
    m_vbo(0),
    m_ebo(0){

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

Renderer::~Renderer(){
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void Renderer::upload(const Geometry& geometry){
    const auto& vertices = geometry.vertices();
    const auto& indices = geometry.indices();

    m_indexCount = geometry.indexCount();
    m_vertexCount = geometry.vertexCount();
    m_primitiveType = geometry.primitiveType();

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    if(!indices.empty()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(unsigned int),
            indices.data(),
            GL_STATIC_DRAW
        );
    }

    glVertexAttribPointer(
        0,
        Geometry::PositionComponents,
        GL_FLOAT,
        GL_FALSE,
        geometry.floatsPerVertex() * sizeof(float),
        nullptr
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        Geometry::ColorComponents,
        GL_FLOAT,
        GL_FALSE,
        geometry.floatsPerVertex() * sizeof(float),
        reinterpret_cast<void*>(Geometry::PositionComponents * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    if(geometry.primitiveType() == PrimitiveType::Points){
        glVertexAttribPointer(
            2,
            Geometry::SizeComponents,
            GL_FLOAT,
            GL_FALSE,
            geometry.floatsPerVertex() * sizeof(float),
            reinterpret_cast<void*>((Geometry::PositionComponents + Geometry::ColorComponents) * sizeof(float))
        );

        glEnableVertexAttribArray(2);
    }

    glBindVertexArray(0);
}

void Renderer::render(float aspectRatio, const glm::mat4& view){
    m_shader.use();

    glm::mat4 model(1.0f);

    m_shader.setMat4("model", model);
    m_shader.setMat4("view", view);

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        aspectRatio,
        0.1f,
        100.0f
    );

    m_shader.setMat4("projection", projection);

    m_shader.setBool("isPoint", m_primitiveType == PrimitiveType::Points);

    glBindVertexArray(m_vao);

    if(m_primitiveType == PrimitiveType::Points){
        glDrawArrays(
            GL_POINTS,
            0,
            m_vertexCount
        );
    }
    else if(m_primitiveType == PrimitiveType::Triangles){
        glDrawElements(
            GL_TRIANGLES,
            m_indexCount,
            GL_UNSIGNED_INT,
            nullptr
        );
    }

    glBindVertexArray(0);
}
