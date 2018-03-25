#pragma once
#include "GenStruct.h"
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct University : public GenStruct
{
        University(unsigned int id, util::GeoCoordinate coord)
        : GenStruct(id, coord)
        {}
};

} // namespace gen
} // namespace stride
