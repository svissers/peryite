#pragma once
#include "GenStruct.h"
#include "pool/ContactPool.h"
#include <vector>

namespace stride {
namespace gen {

/**
 * A university.
 */
struct University : public GenStruct
{
    /// Constructor
    /// @param id           The identification number for the university.
    /// @param u_id         The index of the city (that the university belongs to) in the geogrid
    /// @param coord        The coordinates that the university has been assigned to.
    University(unsigned int id, unsigned int u_id, util::GeoCoordinate coord)
    : GenStruct(id, coord)
    {
        urban_id = u_id;
    }

    // The vector index of the UrbanCenter, not the identifier.
    unsigned int urban_id = -1;
    /// The contactpools associated with this university.
    std::vector<std::shared_ptr<ContactPool>> pools;
};

} // namespace gen
} // namespace stride
