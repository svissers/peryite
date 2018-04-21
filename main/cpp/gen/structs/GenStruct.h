#pragma once
#include "util/GeoCoordinate.h"
#include <memory>

namespace stride {
namespace gen {

struct GenStruct
{
    typedef std::shared_ptr<GenStruct> ptr;
    GenStruct() {}
    GenStruct(unsigned int id, util::GeoCoordinate coord)
    : id(id), coordinate(coord)
    {}

    unsigned int id = -1;
    util::GeoCoordinate coordinate = util::GeoCoordinate();
};

} // namespace gen
} // namespace stride
