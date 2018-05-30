#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include "pop/Population.h"
#include "util/Regions.h"
#include <vector>

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

/// Builds a set of households for the population.
/// @param communities  The communities that persons can be asssigned to.
/// @param population   The persons that will be assigned to communities.
/// @param region       The region that the communities will be assigned in.
/// @param grid         The geogrid that was used to build the communities.
void AssignCommunities
        (std::vector<std::vector<std::shared_ptr<GenStruct>>> communities,
         const std::shared_ptr<Population> population,
         std::shared_ptr<util::Region> region, const GeoGrid &grid);

} // assigner
} // popgen
} // namespace gen
} // namespace stride
