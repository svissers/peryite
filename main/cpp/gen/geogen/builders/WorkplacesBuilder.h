#pragma once
#include "../../structs/Workplace.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include <vector>
#include "pop/Population.h"
#include <tuple>


namespace stride {
namespace gen {
namespace geogen {
namespace builder {

/// Builds a list of workplaces using the Geogrid and some configuration
/// values.
/// @param config       The configuration file that specifies parameters used in building workplaces.
/// @param grid         The geogrid that the workplaces will be mapped to.
/// @return             A vector of newly created workplaces.
std::tuple<std::vector<std::shared_ptr<WorkPlace>>, unsigned int> BuildWorkplaces(GenConfiguration& config, GeoGrid& grid, std::shared_ptr<Population> pop, unsigned int nextWorkplaceId, unsigned int start_person_id, unsigned int next_person_id);

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
