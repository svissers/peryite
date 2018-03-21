#pragma once
#include "GenStruct.h"
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct WorkPlace : public GenStruct
{
        unsigned int id;
        util::GeoCoordinate coordinate = util::GeoCoordinate();
        WorkPlace(unsigned int wp_id, util::GeoCoordinate coord) : id(wp_id),coordinate(coord) {}
};

} // namespace gen
} // namespace stride
