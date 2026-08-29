#pragma once

#include "SimulationCell.h"

#include <string>
#include <vector>

class GalaxyData{
public:
    static std::vector<std::vector<SimulationCell>> load(const std::string filename);
};
