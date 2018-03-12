#pragma once
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct School
{
        School(util::GeoCoordinate coord) : coordinate(coord) {}
        util::GeoCoordinate coordinate = util::GeoCoordinate();
};

} // namespace gen
} // namespace stride
