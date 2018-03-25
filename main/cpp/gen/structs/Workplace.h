#pragma once
#include "GenStruct.h"
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct WorkPlace : public GenStruct
{
        WorkPlace(unsigned int wp_id, util::GeoCoordinate coord)
        : GenStruct(wp_id, coord)
        {}
};

} // namespace gen
} // namespace stride
