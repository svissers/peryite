#pragma once
#include "GenStruct.h"
#include "pool/ContactPool.h"

namespace stride {
namespace gen {

/**
 * A school.
 */
struct School : public GenStruct
{
    /// Constructor
    /// @param id           The identification number for the school.
    /// @param coord        The coordinates that the school has been assigned to.
    School(unsigned int id, util::GeoCoordinate coord)
    : GenStruct(id, coord)
    {};

    /// The contactpools associated with this school.
    std::vector<std::shared_ptr<ContactPool>> pools;
};

} // namespace gen
} // namespace stride
