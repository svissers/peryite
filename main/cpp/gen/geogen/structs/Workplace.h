#pragma once
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct WorkPlace
{
        util::GeoCoordinate coordinate = util::GeoCoordinate();
        unsigned int id = -1;
        WorkPlace(unsigned int wp_id, util::GeoCoordinate coord) : id(wp_id),coordinate(coord) {}


};

} // namespace gen
} // namespace stride
