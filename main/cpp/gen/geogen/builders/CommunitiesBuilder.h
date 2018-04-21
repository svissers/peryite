#pragma once
#include "../../structs/Community.h"
#include "../../GenConfiguration.h"
#include "../../GeoGrid.h"
#include <vector>

namespace stride {
namespace gen {

/**
 * Builds a list of communities using the Geogrid and some configured values.
 */
class CommunitiesBuilder
{
public:
        /// Builds a vector of communities using the Geogrid and a configuration.
        /// @param config       TODO
        /// @param locations    TODO
        static std::vector<std::shared_ptr<Community>> build(const GenConfiguration& config, GeoGrid& grid);
};

} // namespace gen
} // namespace stride
