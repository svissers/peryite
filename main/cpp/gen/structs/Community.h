#pragma once
#include "GenStruct.h"
#include "pool/ContactPool.h"
#include <vector>

namespace stride {
namespace gen {

/**
 * A primary or secondary community.
 */
struct Community : public GenStruct
{
    /// Constructor
    /// @param id           The identification number for the community.
    /// @param primary      Indicates if the community is primary or secondary.
    /// @param coord        The coordinates that the community has been assigned to.
    Community(unsigned int id, bool primary, util::GeoCoordinate coord)
            : GenStruct(id, coord), is_primary(primary)
    {}

    /// Indicates if the community is primary or secondary.
    bool is_primary = true;
    /// The contactpools associated with this community.
    std::vector<std::shared_ptr<ContactPool>> pools;
    /// The contactpools in this community that have been filled up.
    std::vector<std::shared_ptr<ContactPool>> full_pools;
};


} // namespace gen
} // namespace stride
