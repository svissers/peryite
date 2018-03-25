#pragma once
#include "GenStruct.h"
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct Community : public GenStruct
{
        Community(unsigned int id, bool primary, util::GeoCoordinate coord)
                : GenStruct(id, coord), is_primary(primary) {}
        bool is_primary = true;
};


} // namespace gen
} // namespace stride
