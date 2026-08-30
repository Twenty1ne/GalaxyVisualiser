#pragma once

#include "Point.h"
#include "SimulationCell.h"

#include <vector>

class StarGenerator{
public:
    static float siteTheta(int ring, int site);
    static void generate(
        const SimulationCell& cell,
        std::vector<Point>& highMassPoints,
        std::vector<Point>& lowMassPoints
    );
};
