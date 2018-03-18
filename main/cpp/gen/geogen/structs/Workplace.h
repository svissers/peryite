#pragma once
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct WorkPlace
{
        unsigned int id;
        util::GeoCoordinate coordinate = util::GeoCoordinate();
        WorkPlace(unsigned int wp_id, util::GeoCoordinate coord) : id(wp_id),coordinate(coord) {}


};

} // namespace gen
} // namespace stride
