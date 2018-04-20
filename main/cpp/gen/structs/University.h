#pragma once
#include "GenStruct.h"
#include "GeoCoordinate.h"
#include "pool/ContactPool.h"
#include <vector>

namespace stride {
namespace gen {

struct University : public GenStruct
{
    University(unsigned int id, unsigned int u_id, util::GeoCoordinate coord)
    : GenStruct(id, coord)
    {
        urban_id = u_id;
    }
    // The vector index of the UrbanCenter, not the identifier.
    unsigned int urban_id = -1;
    std::vector<std::shared_ptr<ContactPool>> pools;
};

} // namespace gen
} // namespace stride
