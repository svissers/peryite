#pragma once
#include "../structs/University.h"
#include "../GeoConfiguration.h"
#include "../GeoGrid.h"
#include <vector>

namespace stride {
namespace gen {

/**
 * /// Builds a list of universities using the Geogrid and some configured
 * values.
 */
class UniversitiesBuilder
{
public:
        std::shared_ptr<std::vector<University>> universities;
        double minLong;
        double maxLong;
        double LongitudeBandWidth;
        UniversitiesBuilder();
        /// Builds a list of universities using the Geogrid and some configuration
        /// values.
        /// Note: The input grid will leave the method sorted on population size (big to small).
        /// @param config       TODO
        /// @param grid         TODO
        std::shared_ptr<std::vector<University>> build(GeoConfiguration& config, std::shared_ptr<GeoGrid> grid);

};

} // namespace gen
} // namespace stride
