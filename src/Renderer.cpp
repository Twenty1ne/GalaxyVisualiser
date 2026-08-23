#include "Renderer.h"
#include "Geometry.h"

#include <glad/gl.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

Renderer::Renderer() :
    m_shader("shaders/basic.vert", "shaders/basic.frag"){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
}

Renderer::~Renderer(){
    for(const GpuGeometry& geometry : m_geometries){
        glDeleteVertexArrays(1, &geometry.vao);
        glDeleteBuffers(1, &geometry.vbo);

        if(geometry.ebo != 0){
            glDeleteBuffers(1, &geometry.ebo);
        }
    }
}

std::size_t Renderer::upload(const Geometry& geometry){
    GpuGeometry gpuGeometry{};

    gpuGeometry.vao = 0;
    gpuGeometry.vbo = 0;
    gpuGeometry.ebo = 0;

    gpuGeometry.indexCount = geometry.indexCount();
    gpuGeometry.vertexCount = geometry.vertexCount();
    gpuGeometry.primitiveType = geometry.primitiveType();

    const auto& vertices = geometry.vertices();
    const auto& indices = geometry.indices();

    glGenVertexArrays(1, &gpuGeometry.vao);
    glGenBuffers(1, &gpuGeometry.vbo);

    glBindVertexArray(gpuGeometry.vao);
    glBindBuffer(GL_ARRAY_BUFFER, gpuGeometry.vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    if(!indices.empty()){
        glGenBuffers(1, &gpuGeometry.ebo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpuGeometry.ebo);

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

    m_geometries.push_back(gpuGeometry);

    return m_geometries.size() - 1;
}

void Renderer::render(std::size_t geometryId, float aspectRatio, const glm::mat4& view){
    const GpuGeometry& geometry = m_geometries.at(geometryId);

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

    m_shader.setBool("isPoint", geometry.primitiveType == PrimitiveType::Points);

    glBindVertexArray(geometry.vao);

    if(geometry.primitiveType == PrimitiveType::Points){
        glDrawArrays(
            GL_POINTS,
            0,
            geometry.vertexCount
        );
    }
    else if(geometry.primitiveType == PrimitiveType::Lines){
        glDrawArrays(
            GL_LINES,
            0,
            geometry.vertexCount
        );
    }
    else if(geometry.primitiveType == PrimitiveType::Triangles){
        glDrawElements(
            GL_TRIANGLES,
            geometry.indexCount,
            GL_UNSIGNED_INT,
            nullptr
        );
    }

    glBindVertexArray(0);
}
