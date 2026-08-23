#include "GeometryBuilder.h"

Geometry GeometryBuilder::makePoints(const std::vector<Point>& points){
    std::vector<float> vertices;

    vertices.reserve(points.size() * 7);

    for(const Point& point : points){
        vertices.push_back(point.position.x);
        vertices.push_back(point.position.y);
        vertices.push_back(point.position.z);

        vertices.push_back(point.color.r);
        vertices.push_back(point.color.g);
        vertices.push_back(point.color.b);

        vertices.push_back(point.size);
    }

    return Geometry(vertices, 7, PrimitiveType::Points);
}

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

Geometry GeometryBuilder::makeAxes(){
    std::vector<float> vertices = {
        // X axis
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
        5.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

        // Y axis
        0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        0.0f, 5.0f, 0.0f,   0.0f, 1.0f, 0.0f,

        // Z axis
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 5.0f,   0.0f, 0.0f, 1.0f
    };

    return Geometry(vertices, 6, PrimitiveType::Lines);
}

Geometry GeometryBuilder::makeGrid(){
    std::vector<float> vertices;

    const float gridMin = -5.0f;
    const float gridMax = 5.0f;
    const float gridStep = 1.0f;

    const float gridColorR = 0.25f;
    const float gridColorG = 0.25f;
    const float gridColorB = 0.25f;

    for(float x = gridMin; x <= gridMax; x += gridStep){
        // Line parallel to the Z axis.
        vertices.insert(
            vertices.end(),
            {
                x, 0.0f, gridMin,
                gridColorR, gridColorG, gridColorB,

                x, 0.0f, gridMax,
                gridColorR, gridColorG, gridColorB
            }
        );
    }

    for(float z = gridMin; z <= gridMax; z += gridStep){
        // Line parallel to the X axis.
        vertices.insert(
            vertices.end(),
            {
                gridMin, 0.0f, z,
                gridColorR, gridColorG, gridColorB,

                gridMax, 0.0f, z,
                gridColorR, gridColorG, gridColorB
            }
        );
    }

    return Geometry(vertices, 6, PrimitiveType::Lines);
}
