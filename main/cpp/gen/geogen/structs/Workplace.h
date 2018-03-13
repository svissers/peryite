#pragma once
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct WorkPlace
{
        WorkPlace(util::GeoCoordinate coord) : coordinate(coord) {}
        util::GeoCoordinate coordinate = util::GeoCoordinate();
};

} // namespace gen
} // namespace stride
