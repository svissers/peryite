#include "../../structs/Household.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include <vector>

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

/// Assigns households to urban centers.
/// @param households   Households that will be assigned.
/// @param grid         The geogrid containing all centers that a household can be assigned to.
/// @param config       Property_tree with general configuration settings.
void AssignHouseholds
(std::vector<std::shared_ptr<Household>>& households, const GeoGrid& grid, const GenConfiguration& config);

} // assigner
} // popgen
} // namespace gen
} // namespace stride
