#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include "pop/Population.h"
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
unsigned int AssignCommunities
        (std::vector<std::vector<std::shared_ptr<GenStruct>>> communities,
         const std::shared_ptr<Population> population,
         const GenConfiguration &config, const GeoGrid &grid, unsigned int start_cp_id, unsigned int first_person_id, unsigned int next_first_person_id);

} // assigner
} // popgen
} // namespace gen
} // namespace stride
