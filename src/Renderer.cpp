#include "Renderer.h"
#include "Geometry.h"

#include <glad/gl.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

Renderer::Renderer() :
    m_pointShader("shaders/point.vert", "shaders/point.frag"),
    m_lineShader("shaders/line.vert", "shaders/line.frag"),
    m_triangleShader("shaders/triangle.vert", "shaders/triangle.frag"){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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

void Renderer::update(std::size_t geometryId, const Geometry& geometry){
    GpuGeometry& gpuGeometry = m_geometries.at(geometryId);

    const auto& vertices = geometry.vertices();
    const auto& indices = geometry.indices();

    glBindBuffer(GL_ARRAY_BUFFER, gpuGeometry.vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    if(!indices.empty()){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpuGeometry.ebo);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(unsigned int),
            indices.data(),
            GL_STATIC_DRAW
        );
    }

    gpuGeometry.indexCount = geometry.indexCount();
    gpuGeometry.vertexCount = geometry.vertexCount();
    gpuGeometry.primitiveType = geometry.primitiveType();
}

void Renderer::render(std::size_t geometryId, float aspectRatio, const glm::mat4& view, bool glow){
    const GpuGeometry& geometry = m_geometries.at(geometryId);

    const Shader* shader = nullptr;

    GLenum drawMode = GL_POINTS;

    switch(geometry.primitiveType){
        case PrimitiveType::Points:
            shader = &m_pointShader;
            drawMode = GL_POINTS;
            break;

        case PrimitiveType::Lines:
            shader = &m_lineShader;
            drawMode = GL_LINES;
            break;

        case PrimitiveType::Triangles:
            shader = &m_triangleShader;
            drawMode = GL_TRIANGLES;
            break;
    }

    shader->use();

    glm::mat4 model(1.0f);

    shader->setMat4("model", model);
    shader->setMat4("view", view);

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        aspectRatio,
        0.1f,
        1000.0f
    );

    shader->setMat4("projection", projection);

    if(geometry.primitiveType == PrimitiveType::Points){
        shader->setBool("glow", glow);
    }

    if(glow){
        glDepthMask(GL_FALSE);
    }

    glBindVertexArray(geometry.vao);

    if(geometry.primitiveType == PrimitiveType::Triangles){
        glDrawElements(
            drawMode,
            geometry.indexCount,
            GL_UNSIGNED_INT,
            nullptr
        );
    }
    else{
        glDrawArrays(
            drawMode,
            0,
            geometry.vertexCount
        );
    }

    glBindVertexArray(0);

    if(glow){
        glDepthMask(GL_TRUE);
    }
}
