#pragma once
#include "GenStruct.h"
#include "pool/ContactPool.h"

namespace stride {
namespace gen {

struct WorkPlace : public GenStruct
{
    /// Constructor
    /// @param wp_id        The identification number for the workplace.
    /// @param coord        The coordinates that the workplace has been assigned to.
    WorkPlace(unsigned int wp_id, util::spherical_point coord)
    : GenStruct(wp_id, coord)
    {}

    /// The contactpools associated with the workplace.
    std::shared_ptr<ContactPool> pool;
};

} // namespace gen
} // namespace stride
