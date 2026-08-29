#pragma once

#include <vector>

struct StellarRepresentation
{
    float representedMass;
    bool isHighMass;
};

class StellarPopulation
{
public:
    static std::vector<StellarRepresentation> generateHighMassPopulation(
        float totalMass,
        float minimumMass,
        float maximumMass,
        float powerLawExponent
    );

    static std::vector<StellarRepresentation> generateLowMassPopulation(
        float totalMass,
        float minimumMass,
        float maximumMass,
        float powerLawExponent,
        int starsPerPoint
    );
};
