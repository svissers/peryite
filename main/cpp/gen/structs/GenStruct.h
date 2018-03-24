#pragma once
#include "geo/GeoCoordinate.h"
#include <memory>

namespace stride {
namespace gen {

struct GenStruct
{
    typedef std::shared_ptr<GenStruct> ptr;
    util::GeoCoordinate coordinate = util::GeoCoordinate();
};

} // namespace gen
} // namespace stride
