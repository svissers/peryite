#pragma once
#include "../../structs/Community.h"
#include "../../GenConfiguration.h"
#include "../../GeoGrid.h"
#include <vector>
#include <pop/Population.h>
#include <tuple>

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

/// Builds a vector of communities using the Geogrid and a configuration.
/// @param config       The configuration file that specifies parameters used in building communities.
/// @param grid         The geogrid that the communities will be mapped to.
/// @return             A vector of newly created communities.
std::tuple<std::vector<std::shared_ptr<Community>>, unsigned int> BuildCommunities(const GenConfiguration& config, GeoGrid& grid, std::shared_ptr<Population>& population, unsigned int startId);

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
