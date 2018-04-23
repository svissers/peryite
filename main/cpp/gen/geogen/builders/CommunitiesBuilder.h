#pragma once
#include "../../structs/Community.h"
#include "../../GenConfiguration.h"
#include "../../GeoGrid.h"
#include <vector>

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

/// Builds a vector of communities using the Geogrid and a configuration.
/// @param config       The configuration file that specifies parameters used in building communities.
/// @param grid         The geogrid that the communities will be mapped to.
/// @return             A vector of newly created communities.
std::vector<std::shared_ptr<Community>> BuildCommunities(const GenConfiguration& config, GeoGrid& grid);

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
