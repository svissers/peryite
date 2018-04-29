#include "../../structs/Household.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include <vector>

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

/// Builds a set of households for the population.
/// @param communities  The communities that persons can be asssigned to.
/// @param population   The persons that will be assigned to communities.
/// @param config       The configuration file that specifies parameters used in assigning communities.
/// @param grid         The geogrid that was used to build the communities.
void AssignCommunities
        (std::vector<std::vector<std::shared_ptr<GenStruct>>> communities,
         const std::shared_ptr<Population> population,
         const GenConfiguration &config, const GeoGrid &grid);

} // assigner
} // popgen
} // namespace gen
} // namespace stride
