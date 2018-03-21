#pragma once
#include "GenStruct.h"
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct University : public GenStruct
{
        University(unsigned int uni_id, util::GeoCoordinate coord) {
            coordinate = coord;
            id = uni_id;
        }
        util::GeoCoordinate coordinate = util::GeoCoordinate();
        unsigned int id;


};

} // namespace gen
} // namespace stride
