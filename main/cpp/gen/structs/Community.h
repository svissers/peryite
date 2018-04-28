#pragma once
#include "GenStruct.h"
#include "pool/ContactPool.h"
#include <vector>

namespace stride {
namespace gen {

struct Community : public GenStruct
{
    Community(unsigned int id, bool primary, util::GeoCoordinate coord)
            : GenStruct(id, coord), is_primary(primary) {}
    bool is_primary = true;

    std::vector<std::shared_ptr<ContactPool>> pools;
    std::vector<std::shared_ptr<ContactPool>> full_pools;
};


} // namespace gen
} // namespace stride
