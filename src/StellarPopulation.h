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
    static std::vector<StellarRepresentation> generatePopulation(
        float totalMass,
        float minimumMass,
        float maximumMass,
        float powerLawExponent,
        bool isHighMass
    );
};
