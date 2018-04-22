#include "../../structs/Household.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include <vector>

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

/// Builds a set of households for the population.
/// @param universities The universities that persons can be asssigned to.
/// @param households   The households that contain the persons that will be assigned to universities.
/// @param config       Property_tree with general configuration settings.
/// @param grid         The geogrid that was used to build the universities.
unsigned int AssignUniversities
(std::vector<std::vector<std::shared_ptr<GenStruct>>>& universities, const std::vector<std::shared_ptr<Household>>& households,
 const GenConfiguration& config, const GeoGrid& grid);

} // assigner
} // popgen
} // namespace gen
} // namespace stride
