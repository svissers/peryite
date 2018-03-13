#pragma once
#include "../structs/Community.h"
#include "../GeoConfiguration.h"
#include "../GeoGrid.h"
#include <vector>

namespace stride {
namespace gen {

/**
 * Builds a list of communities using the Geogrid and some configured values.
 */
class CommunitiesBuilder
{
public:
        std::shared_ptr<std::vector<Community >> communities;
        double minLong;
        double maxLong;
        double LongitudeBandWidth;
        CommunitiesBuilder();
        /// Builds a list of communities using the Geogrid and some configuration
        /// values.
        /// @param config       TODO
        /// @param locations    TODO
        std::shared_ptr<std::vector<Community>> build(const GeoConfiguration& config, std::shared_ptr<GeoGrid> grid);
        void write(std::string outputFile);
};

} // namespace gen
} // namespace stride
