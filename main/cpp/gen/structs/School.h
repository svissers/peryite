#pragma once
#include "GenStruct.h"
#include "geo/GeoCoordinate.h"
#include "core/ContactPool.h"

namespace stride {
namespace gen {

struct School : public GenStruct
{
    School(unsigned int id, util::GeoCoordinate coord)
    : GenStruct(id, coord)
    {};

    std::vector<std::shared_ptr<ContactPool>> pools;
};

} // namespace gen
} // namespace stride
