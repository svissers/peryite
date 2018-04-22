#pragma once
#include "GenStruct.h"
#include "pool/ContactPool.h"

namespace stride {
namespace gen {

struct WorkPlace : public GenStruct
{
    WorkPlace(unsigned int wp_id, util::GeoCoordinate coord)
    : GenStruct(wp_id, coord)
    {}
    std::shared_ptr<ContactPool> pool;
};

} // namespace gen
} // namespace stride
