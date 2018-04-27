#pragma once
#include "pop/Population.h"
#include "../../GenConfiguration.h"
#include <vector>

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

/// Builds a set of households for the population.
/// @param config     The configuration file that specifies parameters used in building households.
/// @return           The newly constructed population.
std::shared_ptr<Population> BuildPopulation(const GenConfiguration& config, const ptree& belief_pt);

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
