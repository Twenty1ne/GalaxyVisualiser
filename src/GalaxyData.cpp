#include "GalaxyData.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<SimulationCell> GalaxyData::loadFirstTimestep(const std::string filename){
    std::ifstream file(filename);

    if(!file.is_open()){
        throw std::runtime_error("Could not open galaxy data file: " + filename);
    }

    std::vector<SimulationCell> cells;
    std::string line;

    while(std::getline(file, line)){
        // Blank line == end of timestep
        if(line.empty()) break;

        std::istringstream stream(line);

        SimulationCell cell;

        stream
            >> cell.ring
            >> cell.theta
            >> cell.highMass
            >> cell.lowMass;

        cells.push_back(cell);
    }

    return cells;
}
