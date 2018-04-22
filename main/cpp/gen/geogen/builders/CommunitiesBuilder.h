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
/// @param config       TODO
/// @param locations    TODO
std::vector<std::shared_ptr<Community>> Build(const GenConfiguration& config, GeoGrid& grid);

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
