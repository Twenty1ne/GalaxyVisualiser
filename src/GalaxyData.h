#pragma once

#include "SimulationCell.h"

#include <string>
#include <vector>

class GalaxyData{
public:
    static std::vector<SimulationCell> loadFirstTimestep(const std::string filename);
};
