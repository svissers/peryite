#include "../../structs/Household.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include <vector>

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

/// Builds a set of households for the population.
/// @param workplaces               The workplaces that persons can be asssigned to.
/// @param households               The households that contain the persons that will be assigned to workplaces.
/// @param config                   Property_tree with general configuration settings.
/// @param grid                     The geogrid that was used to build the schools.
/// @param total_commuting_students The total number of students that are commuting to schools/universities.
void AssignWorkplaces
(std::vector<std::vector<std::shared_ptr<GenStruct>>>& workplaces, const std::vector<std::shared_ptr<Household>>& households,
 const GenConfiguration& config, const GeoGrid& grid, unsigned int total_commuting_students);

} // assigner
} // popgen
} // namespace gen
} // namespace stride
