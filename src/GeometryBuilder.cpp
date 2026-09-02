#include "GeometryBuilder.h"
#include "DiskThickness.h"

#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

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

Geometry GeometryBuilder::makeSimulationGrid()
{
    constexpr int maxRadius = 50;

    constexpr float radialWidth = 1.0f;

    constexpr float gridColorR = 0.20f;
    constexpr float gridColorG = 0.20f;
    constexpr float gridColorB = 0.20f;

    constexpr int arcSegments = 12;
    constexpr float pi = 3.14159265358979323846f;

    std::vector<float> vertices;

    auto addVertex =
        [&](float x, float y, float z)
    {
        vertices.insert(
            vertices.end(),
            {
                x, y, z,
                gridColorR,
                gridColorG,
                gridColorB
            }
        );
    };

    auto addLine =
        [&](float x1, float y1, float z1,
            float x2, float y2, float z2)
    {
        addVertex(x1, y1, z1);
        addVertex(x2, y2, z2);
    };

    // Polar coordinates mapped into the X/Z plane.
    //
    // theta = 0 points along +X.
    // theta increases towards +Z.
    // Y is the depth/thickness of the grid.
    auto point =
        [](float radius, float theta, float y)
    {
        return glm::vec3(
            radius * std::cos(theta),
            y,
            radius * std::sin(theta)
        );
    };

    // Central cell: radius 0 to 0.5.
    {
        constexpr float radius = 0.5f;

        const float halfDepth = DiskThickness::halfDepth(0);

        for (int segment = 0;
             segment < arcSegments;
             ++segment)
        {
            const float theta1 =
                2.0f * pi *
                static_cast<float>(segment) /
                static_cast<float>(arcSegments);

            const float theta2 =
                2.0f * pi *
                static_cast<float>(segment + 1) /
                static_cast<float>(arcSegments);

            const glm::vec3 front1 =
                point(radius, theta1, -halfDepth);

            const glm::vec3 front2 =
                point(radius, theta2, -halfDepth);

            const glm::vec3 back1 =
                point(radius, theta1, halfDepth);

            const glm::vec3 back2 =
                point(radius, theta2, halfDepth);

            // Lower boundary.
            addLine(
                front1.x, front1.y, front1.z,
                front2.x, front2.y, front2.z
            );

            // Upper boundary.
            addLine(
                back1.x, back1.y, back1.z,
                back2.x, back2.y, back2.z
            );

            // Depth connector.
            addLine(
                front1.x, front1.y, front1.z,
                back1.x, back1.y, back1.z
            );
        }
    }

    // Annular rings.
    for (int r = 1; r <= maxRadius; ++r)
    {
        const float innerRadius =
            static_cast<float>(r) - radialWidth / 2.0f;

        const float outerRadius =
            static_cast<float>(r) + radialWidth / 2.0f;

        const int siteCount = 6 * r;

        const float deltaTheta =
            2.0f * pi /
            static_cast<float>(siteCount);

        // Even rings are offset by half a site's angular width.
        const float thetaOffset =
            (r % 2 == 0)
                ? deltaTheta / 2.0f
                : 0.0f;

        const float halfDepth = DiskThickness::halfDepth(r);

        for (int site = 0;
             site < siteCount;
             ++site)
        {
            const float thetaCentre =
                thetaOffset +
                static_cast<float>(site) * deltaTheta;

            const float thetaMin =
                thetaCentre - deltaTheta / 2.0f;

            const float thetaMax =
                thetaCentre + deltaTheta / 2.0f;

            const glm::vec3 innerMinFront =
                point(innerRadius, thetaMin, -halfDepth);

            const glm::vec3 innerMaxFront =
                point(innerRadius, thetaMax, -halfDepth);

            const glm::vec3 outerMinFront =
                point(outerRadius, thetaMin, -halfDepth);

            const glm::vec3 outerMaxFront =
                point(outerRadius, thetaMax, -halfDepth);

            const glm::vec3 innerMinBack =
                point(innerRadius, thetaMin, halfDepth);

            const glm::vec3 innerMaxBack =
                point(innerRadius, thetaMax, halfDepth);

            const glm::vec3 outerMinBack =
                point(outerRadius, thetaMin, halfDepth);

            const glm::vec3 outerMaxBack =
                point(outerRadius, thetaMax, halfDepth);

            // Radial boundaries on the lower surface.
            addLine(
                innerMinFront.x,
                innerMinFront.y,
                innerMinFront.z,

                outerMinFront.x,
                outerMinFront.y,
                outerMinFront.z
            );

            addLine(
                innerMaxFront.x,
                innerMaxFront.y,
                innerMaxFront.z,

                outerMaxFront.x,
                outerMaxFront.y,
                outerMaxFront.z
            );

            // Radial boundaries on the upper surface.
            addLine(
                innerMinBack.x,
                innerMinBack.y,
                innerMinBack.z,

                outerMinBack.x,
                outerMinBack.y,
                outerMinBack.z
            );

            addLine(
                innerMaxBack.x,
                innerMaxBack.y,
                innerMaxBack.z,

                outerMaxBack.x,
                outerMaxBack.y,
                outerMaxBack.z
            );

            // Curved inner and outer boundaries.
            for (int segment = 0;
                 segment < arcSegments;
                 ++segment)
            {
                const float t1 =
                    thetaMin +
                    deltaTheta *
                    static_cast<float>(segment) /
                    static_cast<float>(arcSegments);

                const float t2 =
                    thetaMin +
                    deltaTheta *
                    static_cast<float>(segment + 1) /
                    static_cast<float>(arcSegments);

                const glm::vec3 inner1Front =
                    point(innerRadius, t1, -halfDepth);

                const glm::vec3 inner2Front =
                    point(innerRadius, t2, -halfDepth);

                const glm::vec3 outer1Front =
                    point(outerRadius, t1, -halfDepth);

                const glm::vec3 outer2Front =
                    point(outerRadius, t2, -halfDepth);

                const glm::vec3 inner1Back =
                    point(innerRadius, t1, halfDepth);

                const glm::vec3 inner2Back =
                    point(innerRadius, t2, halfDepth);

                const glm::vec3 outer1Back =
                    point(outerRadius, t1, halfDepth);

                const glm::vec3 outer2Back =
                    point(outerRadius, t2, halfDepth);

                // Inner boundary, lower.
                addLine(
                    inner1Front.x,
                    inner1Front.y,
                    inner1Front.z,

                    inner2Front.x,
                    inner2Front.y,
                    inner2Front.z
                );

                // Outer boundary, lower.
                addLine(
                    outer1Front.x,
                    outer1Front.y,
                    outer1Front.z,

                    outer2Front.x,
                    outer2Front.y,
                    outer2Front.z
                );

                // Inner boundary, upper.
                addLine(
                    inner1Back.x,
                    inner1Back.y,
                    inner1Back.z,

                    inner2Back.x,
                    inner2Back.y,
                    inner2Back.z
                );

                // Outer boundary, upper.
                addLine(
                    outer1Back.x,
                    outer1Back.y,
                    outer1Back.z,

                    outer2Back.x,
                    outer2Back.y,
                    outer2Back.z
                );
            }

            // Connect lower and upper surfaces.
            addLine(
                innerMinFront.x,
                innerMinFront.y,
                innerMinFront.z,

                innerMinBack.x,
                innerMinBack.y,
                innerMinBack.z
            );

            addLine(
                innerMaxFront.x,
                innerMaxFront.y,
                innerMaxFront.z,

                innerMaxBack.x,
                innerMaxBack.y,
                innerMaxBack.z
            );

            addLine(
                outerMinFront.x,
                outerMinFront.y,
                outerMinFront.z,

                outerMinBack.x,
                outerMinBack.y,
                outerMinBack.z
            );

            addLine(
                outerMaxFront.x,
                outerMaxFront.y,
                outerMaxFront.z,

                outerMaxBack.x,
                outerMaxBack.y,
                outerMaxBack.z
            );
        }
    }

    return Geometry(
        vertices,
        6,
        PrimitiveType::Lines
    );
}
