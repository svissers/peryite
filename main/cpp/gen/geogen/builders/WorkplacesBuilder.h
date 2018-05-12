#pragma once
#include "../../structs/Workplace.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include <vector>
#include "pop/Population.h"


namespace stride {
namespace gen {
namespace geogen {
namespace builder {

/// Builds a list of workplaces using the Geogrid and some configuration
/// values.
/// @param config       The configuration file that specifies parameters used in building workplaces.
/// @param grid         The geogrid that the workplaces will be mapped to.
/// @return             A vector of newly created workplaces.
std::vector<std::shared_ptr<WorkPlace>> BuildWorkplaces(GenConfiguration& config, GeoGrid& grid, std::shared_ptr<Population> pop);

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
