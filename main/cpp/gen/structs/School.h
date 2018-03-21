#pragma once
#include "GenStruct.h"
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {
 
struct School : public GenStruct
{
        unsigned int id;
        util::GeoCoordinate coordinate = util::GeoCoordinate();

        School(unsigned int sch_id, util::GeoCoordinate coord) : id(sch_id), coordinate(coord) {};
};

} // namespace gen
} // namespace stride
