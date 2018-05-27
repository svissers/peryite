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
/// @param region       The region that the population will be created in.
/// @param population   The population that will be seeded.
void BuildPopulation(std::shared_ptr<util::Region> region, std::shared_ptr<Population>& population);

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
