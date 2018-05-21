#pragma once
#include "pop/Population.h"
#include "../../GenConfiguration.h"
#include "pop/Population.h"
#include <boost/property_tree/ptree.hpp>
#include <vector>
#include <tuple>

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

/// Builds a set of households for the population.
/// @param config     The configuration file that specifies parameters used in building households.
/// @param population The population that will be seeded.
std::tuple<unsigned int, unsigned int> BuildPopulation(const GenConfiguration& config, std::shared_ptr<Population>& population, unsigned int start_person_id, unsigned int start_hh_id);

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
