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
/// @param population               The persons that will be assigned to travel.
/// @param regions                   the regions that the simulated area consists of.
    void AssignTravellers(
            const std::shared_ptr<Population> population,
            std::shared_ptr<util::Regions> regions
    );

} // assigner
} // popgen
} // namespace gen
} // namespace stride
