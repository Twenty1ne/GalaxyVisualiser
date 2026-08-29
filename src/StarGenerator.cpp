#include "StarGenerator.h"

#include <algorithm>
#include <cmath>
#include <random>

namespace
{
    constexpr float pi = 3.14159265358979323846f;

    // How much simulated mass corresponds to one rendered star.
    constexpr float massPerStar = 500.0f;

    // Visual size of one rendered star.
    constexpr float starSize = 0.08f;

    // Small variation in where stars appear along the Y axis.
    constexpr float halfDepth = 0.5f;

    const glm::vec3 highMassColour(
        0.75f,
        0.90f,
        1.00f
    );

    const glm::vec3 lowMassColour(
        0.80f,
        0.25f,
        0.10f
    );

    std::mt19937& randomGenerator()
    {
        static std::mt19937 generator(
            std::random_device{}()
        );

        return generator;
    }

    float calculateDeltaTheta(int ring)
    {
        const int siteCount = 6 * ring;

        return 2.0f * pi /
               static_cast<float>(siteCount);
    }

    float calculateThetaOffset(int ring)
    {
        if (ring % 2 == 0)
        {
            return calculateDeltaTheta(ring) / 2.0f;
        }

        return 0.0f;
    }

    glm::vec3 randomPositionInCell(
        int ring,
        float theta
    )
    {
        std::uniform_real_distribution<float> uniform01(
            0.0f,
            1.0f
        );

        // Central cell.
        if (ring == 0)
        {
            const float radius =
                0.5f * std::sqrt(uniform01(randomGenerator()));

            const float angle =
                2.0f * pi * uniform01(randomGenerator());

            const float y =
                std::uniform_real_distribution<float>(
                    -halfDepth,
                    halfDepth
                )(randomGenerator());

            return glm::vec3(
                radius * std::cos(angle),
                y,
                radius * std::sin(angle)
            );
        }

        const float deltaTheta =
            calculateDeltaTheta(ring);

        const float innerRadius =
            static_cast<float>(ring) - 0.5f;

        const float outerRadius =
            static_cast<float>(ring) + 0.5f;

        const float thetaMin =
            theta - deltaTheta / 2.0f;

        const float thetaMax =
            theta + deltaTheta / 2.0f;

        // Uniform in area rather than radius.
        const float radius =
            std::sqrt(
                innerRadius * innerRadius +
                uniform01(randomGenerator()) *
                (
                    outerRadius * outerRadius -
                    innerRadius * innerRadius
                )
            );

        const float angle =
            thetaMin +
            uniform01(randomGenerator()) *
            (thetaMax - thetaMin);

        const float y =
            std::uniform_real_distribution<float>(
                -halfDepth,
                halfDepth
            )(randomGenerator());

        return glm::vec3(
            radius * std::cos(angle),
            y,
            radius * std::sin(angle)
        );
    }

    void addPopulation(
        std::vector<Point>& points,
        int ring,
        float theta,
        float mass,
        const glm::vec3& colour
    )
    {
        if (mass <= 0.0f)
        {
            return;
        }

        const int starCount =
            std::max(
                1,
                static_cast<int>(
                    std::round(mass / massPerStar)
                )
            );

        for (int i = 0; i < starCount; ++i)
        {
            Point point;

            point.position =
                randomPositionInCell(
                    ring,
                    theta
                );

            point.color = colour;

            point.size = starSize;

            points.push_back(point);
        }
    }
}

float StarGenerator::siteTheta(
    int ring,
    int site
)
{
    if (ring == 0)
    {
        return 0.0f;
    }

    const float deltaTheta =
        calculateDeltaTheta(ring);

    const float thetaOffset =
        calculateThetaOffset(ring);

    return thetaOffset +
           static_cast<float>(site) *
           deltaTheta;
}

std::vector<Point> StarGenerator::generate(const SimulationCell& cell, std::vector<Point>& points){
        addPopulation(
        points,
        cell.ring,
        cell.theta,
        cell.highMass,
        highMassColour
    );

    addPopulation(
        points,
        cell.ring,
        cell.theta,
        cell.lowMass,
        lowMassColour
    );

    return points;
}
