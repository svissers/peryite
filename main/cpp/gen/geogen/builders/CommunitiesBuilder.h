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
        shared_ptr<vector<Community >> communities;
        CommunitiesBuilder();
        /// Builds a list of communities using the Geogrid and some configuration
        /// values.
        /// @param config       TODO
        /// @param locations    TODO
        static std::shared_ptr<std::vector<Community>> build(GeoConfiguration& config, std::shared_ptr<GeoGrid> grid);
        void write(std::string outputFile);

};

} // namespace gen
} // namespace stride
