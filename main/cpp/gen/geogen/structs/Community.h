#pragma once
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct Community
{
        Community(bool primary, util::GeoCoordinate coord)
                : is_primary(primary), coordinate(coord) {}
        bool is_primary = true;
        util::GeoCoordinate coordinate = util::GeoCoordinate();
};

} // namespace gen
} // namespace stride
