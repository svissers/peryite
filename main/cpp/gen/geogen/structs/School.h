#pragma once
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

struct School
{
        util::GeoCoordinate coordinate = util::GeoCoordinate();
        unsigned int id;
        School(unsigned int sch_id, util::GeoCoordinate coord) : id(sch_id), coordinate(coord) {};



};

} // namespace gen
} // namespace stride
