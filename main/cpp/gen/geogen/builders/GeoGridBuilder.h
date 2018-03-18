#pragma once
#include "../GeoGrid.h"
#include "../GeoConfiguration.h"

namespace stride {
namespace gen {

/**
 * Builds a geogrid using city data.
 */
class GeoGridBuilder
{
public:
        /// Builds the GeoGrid using city data.
        /// @param config       The configuration for a geogrid.
        /// @return             A vector of urban centers representing a geogrid.
        static std::shared_ptr<GeoGrid> build(const GeoConfiguration& config);
};

} // namespace gen
} // namespace stride
