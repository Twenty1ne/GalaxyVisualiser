#include "StarGenerator.h"

#include "StellarPopulation.h"

#include <cmath>
#include <random>

namespace
{
    constexpr float pi = 3.14159265358979323846f;

    constexpr float powerLawExponent = 2.35f;

    constexpr float highMassMinimum = 8.0f;
    constexpr float highMassMaximum = 100.0f;

    constexpr float lowMassMinimum = 50.0f;
    constexpr float lowMassMaximum = 350.0f;

    constexpr float highMassSizeScale = 0.08f;
    constexpr float lowMassSizeScale = 0.03;

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
        const std::vector<StellarRepresentation>& population,
        const glm::vec3& colour,
        float sizeScale
    )
    {
        for (const StellarRepresentation& star : population)
        {
            Point point;
    
            point.position =
                randomPositionInCell(
                    ring,
                    theta
                );
    
            point.color = colour;
    
            point.size =
                sizeScale *
                std::cbrt(star.representedMass);
    
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

void StarGenerator::generate(const SimulationCell& cell, std::vector<Point>& highMassPoints, std::vector<Point>& lowMassPoints){
    const std::vector<StellarRepresentation> highMassPopulation =
        StellarPopulation::generatePopulation(
            cell.highMass,
            highMassMinimum,
            highMassMaximum,
            powerLawExponent,
            true
        );

    const std::vector<StellarRepresentation> lowMassPopulation =
        StellarPopulation::generatePopulation(
            cell.lowMass,
            lowMassMinimum,
            lowMassMaximum,
            powerLawExponent,
            false
        );

    // High mass stars rendered individually
    addPopulation(
        highMassPoints,
        cell.ring,
        cell.theta,
        highMassPopulation,
        highMassColour,
        highMassSizeScale
    );

    // Low mass stars grouped into batches of lowMassStarsPerPoint before rendering
    addPopulation(
        lowMassPoints,
        cell.ring,
        cell.theta,
        lowMassPopulation,
        lowMassColour,
        lowMassSizeScale
    );
}
