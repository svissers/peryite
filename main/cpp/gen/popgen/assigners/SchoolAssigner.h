#include "pop/Population.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include "util/Regions.h"
#include <vector>

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

/// Builds a set of households for the population.
/// @param schools      The schools that persons can be asssigned to.
/// @param population   The persons that will be assigned to schools.
/// @param region       The region that the schools will be assigned in.
/// @param grid         The geogrid that was used to build the schools.
void AssignSchools(
    std::vector<std::vector<std::shared_ptr<GenStruct>>>& schools,
    const std::shared_ptr<Population> population,
    std::shared_ptr<util::Region> region,
    const GeoGrid &grid
    );

} // assigner
} // popgen
} // namespace gen
} // namespace stride
