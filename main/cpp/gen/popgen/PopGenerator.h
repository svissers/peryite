#pragma once
#include "../files/GenDirectory.h"
#include "../structs/Household.h"

namespace stride {
namespace gen {

void generate(const files::GenDirectory& dir, unsigned int thread_count);

std::vector<std::shared_ptr<Household>>  buildHouseholds(const GenConfiguration& config);

void assignHouseholds
(std::vector<std::shared_ptr<Household>>& households, const GeoGrid& grid, const GenConfiguration& config);

void assignSchools
(std::vector<std::vector<std::shared_ptr<GenStruct>>>& schools, const std::vector<std::shared_ptr<Household>>& households, const GenConfiguration& config);

} // namespace gen
} // namespace stride
