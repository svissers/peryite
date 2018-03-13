#pragma once
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct University
{
        University(unsigned int id, util::GeoCoordinate coord) {
            coordinate(coord);
            ID = id;
        }
        util::GeoCoordinate coordinate = util::GeoCoordinate();
        unsigned int ID;
};

} // namespace gen
} // namespace stride
