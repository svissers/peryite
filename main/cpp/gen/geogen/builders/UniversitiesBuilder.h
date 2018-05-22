#pragma once
#include "../../structs/University.h"
#include "../../GenConfiguration.h"
#include "../../GeoGrid.h"
#include <vector>
#include <tuple>

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

/// Builds a list of universities using the Geogrid and some configuration
/// values.
/// Note: The input grid will leave the method sorted on population size (big to small).
/// @param config       The configuration file that specifies parameters used in building workplaces.
/// @param grid         The geogrid that the universities will be mapped to.
/// @return             A vector of newly created universities.
std::tuple<std::vector<std::shared_ptr<University>>, unsigned int> BuildUniversities(GenConfiguration& config, GeoGrid& grid, unsigned int start_uni_id);

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
