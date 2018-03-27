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

unsigned int assignUniversity
(std::vector<std::vector<std::shared_ptr<GenStruct>>>& universities, const std::vector<std::shared_ptr<Household>>& households, const GenConfiguration& config);

void assignWorkplace
(std::vector<std::vector<std::shared_ptr<GenStruct>>>& workplaces, const std::vector<std::shared_ptr<Household>>& households,
 const GenConfiguration& config, unsigned int total_commuting_students);


} // namespace gen
} // namespace stride
