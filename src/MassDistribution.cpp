#include "MassDistribution.h"

#include <cmath>
#include <random>

namespace{
    std::mt19937& randomGenerator(){
        static std::mt19937 generator(std::random_device{}());

        return generator;
    }
}

float MassDistribution::samplePowerLaw(float minimumMass, float maximumMass, float exponent){
    std::uniform_real_distribution<float> uniform01(0.0f, 1.0f);

    const float oneMinusExponent = 1.0f - exponent;

    const float minimumPower = std::pow(minimumMass, oneMinusExponent);

    const float maximumPower = std::pow(maximumMass, oneMinusExponent);

    const float randomValue = uniform01(randomGenerator());

    return std::pow(minimumPower + randomValue * (maximumPower - minimumPower), 1.0f / oneMinusExponent);
}
