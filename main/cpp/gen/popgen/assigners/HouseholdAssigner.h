#include "pop/Population.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include "util/Regions.h"
#include <vector>

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

/// Assigns households to urban centers.
/// @param population   Households that will be assigned.
/// @param grid         The geogrid containing all centers that a household can be assigned to.
/// @param region       The region that the households will be assigned in.
void AssignHouseholds(std::shared_ptr<Population> population, const GeoGrid& grid, std::shared_ptr<util::Region> region);

} // assigner
} // popgen
} // namespace gen
} // namespace stride
