#include "pop/Population.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include <vector>
#include <tuple>

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

/// Builds a set of households for the population.
/// @param universities The universities that persons can be asssigned to.
/// @param population   The persons that will be assigned to universities.
/// @param config       Property_tree with general configuration settings.
/// @param grid         The geogrid that was used to build the universities.
std::tuple<unsigned int, unsigned int> AssignUniversities
    (std::vector<std::vector<std::shared_ptr<GenStruct>>>& universities, const std::shared_ptr<Population> population,
    const GenConfiguration& config, const GeoGrid& grid, unsigned int start_cp_id, unsigned int first_person_id, unsigned int next_first_person_id);

} // assigner
} // popgen
} // namespace gen
} // namespace stride
