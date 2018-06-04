#include "pop/Population.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include "util/Regions.h"
#include <vector>
#include "../../files/GenDirectory.h"


namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

/// Builds a set of households for the population.
/// @param population               The persons that will be assigned to travel.
/// @param regions                   the regions that the simulated area consists of.
    void AssignTravellers(
            const std::shared_ptr<Population> population,
            files::GenDirectory& dir
    );

} // assigner
} // popgen
} // namespace gen
} // namespace stride
