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
private:
    double minLong;
    double maxLong;
    double LongitudeBandWidth;
public:
        GeoGridBuilder();
        std::shared_ptr<GeoGrid> geo_grid;
        /// Builds the GeoGrid using city data.
        /// @param config       The configuration for a geogrid.
        /// @return             A vector of urban centers representing a geogrid.
        std::shared_ptr<GeoGrid> build(const GeoConfiguration& config);
};

} // namespace gen
} // namespace stride
