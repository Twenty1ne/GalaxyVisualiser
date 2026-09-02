#include "StellarPopulation.h"

#include "MassDistribution.h"

#include <algorithm>

std::vector<StellarRepresentation>
StellarPopulation::generatePopulation(
    float totalMass,
    float minimumMass,
    float maximumMass,
    float powerLawExponent,
    bool isHighMass,
    std::size_t maxCount
)
{
    std::vector<StellarRepresentation> population;

    float remainingMass = totalMass;

    while (remainingMass > 0.0f && population.size() < maxCount)
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
            isHighMass
        });

        remainingMass -= acceptedMass;
    }

    return population;
}
