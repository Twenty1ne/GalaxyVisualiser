#include "GeometryBuilder.h"

Geometry GeometryBuilder::makeCube(){
    std::vector<float> vertices = {
        // Front face
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

        // Left face
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

        // Right face
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,

        // Bottom face
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,

        // Top face
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f
    };

    std::vector<unsigned int> indices = {
        // Front
         0,  1,  2,
         2,  3,  0,

        // Back
         4,  5,  6,
         6,  7,  4,

        // Left
         8,  9, 10,
        10, 11,  8,

        // Right
        12, 13, 14,
        14, 15, 12,

        // Bottom
        16, 17, 18,
        18, 19, 16,

        // Top
        20, 21, 22,
        22, 23, 20
    };

    return Geometry(vertices, 6, indices, PrimitiveType::Triangles);
}

Geometry GeometryBuilder::makePoints(){
    std::vector<float> vertices = {
        // position          // colour          // size
        -0.8f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.15f,
        -0.4f,  0.2f, 0.0f,   0.0f, 1.0f, 0.0f,   0.30f,
         0.0f, -0.1f, 0.0f,   0.0f, 0.0f, 1.0f,   0.20f,
         0.4f,  0.4f, 0.0f,   1.0f, 1.0f, 0.0f,   0.40f,
         0.8f, -0.3f, 0.0f,   1.0f, 0.0f, 1.0f,   0.10f
    };

    return Geometry(vertices, 7, PrimitiveType::Points);
}
