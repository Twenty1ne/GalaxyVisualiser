#pragma once

#include "Point.h"
#include "SimulationCell.h"

#include <vector>

class StarGenerator{
public:
    static float siteTheta(int ring, int site);
    static std::vector<Point> generate(const SimulationCell& cell, std::vector<Point>& points);
};
