#pragma once

#include "Geometry.h"
#include "Point.h"

class GeometryBuilder{
public:
    static Geometry makePoints(const std::vector<Point>& points);

    static Geometry makeCube();

    static Geometry makeAxes();

    static Geometry makeGrid();
};
