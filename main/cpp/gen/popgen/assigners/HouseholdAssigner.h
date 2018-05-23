#include "pop/Population.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include <vector>

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

/// Assigns households to urban centers.
/// @param population   Households that will be assigned.
/// @param grid         The geogrid containing all centers that a household can be assigned to.
/// @param config       Property_tree with general configuration settings.
void AssignHouseholds
    (std::shared_ptr<Population> population, const GeoGrid& grid, const GenConfiguration& config, unsigned int first_person_id, unsigned int next_first_person_id);

} // assigner
} // popgen
} // namespace gen
} // namespace stride
