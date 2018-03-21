#pragma once
#include "GenStruct.h"
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct Community : public GenStruct
{
        Community(unsigned int id, bool primary, util::GeoCoordinate coord)
                : is_primary(primary), id(id), coordinate(coord) {}
        bool is_primary = true;
        unsigned int id;
        util::GeoCoordinate coordinate = util::GeoCoordinate();
};


} // namespace gen
} // namespace stride
