#include "StellarPopulation.h"

#include "MassDistribution.h"

#include <algorithm>

std::vector<StellarRepresentation>
StellarPopulation::generateHighMassPopulation(
    float totalMass,
    float minimumMass,
    float maximumMass,
    float powerLawExponent
)
{
    std::vector<StellarRepresentation> population;

    float remainingMass = totalMass;

    while (remainingMass > 0.0f)
    {
        const float sampledMass =
            MassDistribution::samplePowerLaw(
                minimumMass,
                maximumMass,
                powerLawExponent
            );

        const float acceptedMass =
            std::min(sampledMass, remainingMass);

        population.push_back({
            acceptedMass,
            true
        });

        remainingMass -= acceptedMass;
    }

    return population;
}

std::vector<StellarRepresentation>
StellarPopulation::generateLowMassPopulation(
    float totalMass,
    float minimumMass,
    float maximumMass,
    float powerLawExponent,
    int starsPerPoint
)
{
    std::vector<StellarRepresentation> population;

    if (starsPerPoint <= 0)
    {
        return population;
    }

    float remainingMass = totalMass;
    int starsInCurrentGroup = 0;
    float currentGroupMass = 0.0f;

    while (remainingMass > 0.0f)
    {
        const float sampledMass =
            MassDistribution::samplePowerLaw(
                minimumMass,
                maximumMass,
                powerLawExponent
            );

        const float acceptedMass =
            std::min(sampledMass, remainingMass);

        currentGroupMass += acceptedMass;
        ++starsInCurrentGroup;

        remainingMass -= acceptedMass;

        if (starsInCurrentGroup == starsPerPoint ||
            remainingMass <= 0.0f)
        {
            population.push_back({
                currentGroupMass,
                false
            });

            currentGroupMass = 0.0f;
            starsInCurrentGroup = 0;
        }
    }

    return population;
}
