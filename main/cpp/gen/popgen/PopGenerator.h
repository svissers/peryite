#pragma once
#include "../files/GenDirectory.h"
#include "../structs/Household.h"

namespace stride {
namespace gen {
namespace popgen {

void generate(files::GenDirectory& dir, unsigned int thread_count);

std::vector<std::shared_ptr<Household>>  buildHouseholds(const GenConfiguration& config);

void assignHouseholds
(std::vector<std::shared_ptr<Household>>& households, const GeoGrid& grid, const GenConfiguration& config);

void assignSchools
(std::vector<std::vector<std::shared_ptr<GenStruct>>>& schools, const std::vector<std::shared_ptr<Household>>& households,
 const GenConfiguration& config, const GeoGrid& grid);

unsigned int assignUniversities
(std::vector<std::vector<std::shared_ptr<GenStruct>>>& universities, const std::vector<std::shared_ptr<Household>>& households,
 const GenConfiguration& config, const GeoGrid& grid);

void assignWorkplaces
(std::vector<std::vector<std::shared_ptr<GenStruct>>>& workplaces, const std::vector<std::shared_ptr<Household>>& households,
 const GenConfiguration& config, const GeoGrid& grid, unsigned int total_commuting_students);

void assignCommunities
(std::vector<std::vector<std::shared_ptr<GenStruct>>> communities, const std::vector<std::shared_ptr<Household>>& households,
 const GenConfiguration& config, const GeoGrid& grid);

void writePopulation(std::vector<std::shared_ptr<Household>> households, const GenConfiguration& config);

} // namespace popgen
} // namespace gen
} // namespace stride
