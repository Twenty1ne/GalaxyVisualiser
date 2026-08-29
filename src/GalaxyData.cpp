#include "GalaxyData.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

std::vector<std::vector<SimulationCell>> GalaxyData::load(const std::string filename){
    std::ifstream file(filename);

    if(!file.is_open()){
        throw std::runtime_error("Could not open galaxy data file: " + filename);
    }

    std::vector<std::vector<SimulationCell>> timesteps;
    std::vector<SimulationCell> currentTimestep;
    std::string line;

    while(std::getline(file, line)){
        // Blank line == end of timestep
        if(line.empty()){
            if(!currentTimestep.empty()){
                timesteps.push_back(std::move(currentTimestep));
                currentTimestep.clear();
            }

            continue;
        }

        std::istringstream stream(line);

        SimulationCell cell;

        stream
            >> cell.ring
            >> cell.theta
            >> cell.highMass
            >> cell.lowMass;

        currentTimestep.push_back(cell);
    }

    if(!currentTimestep.empty()){
        timesteps.push_back(std::move(currentTimestep));
    }

    return timesteps;
}
