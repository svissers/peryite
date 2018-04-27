#pragma once
#include "../files/GenDirectory.h"
#include "../structs/Household.h"
#include "util/GeoCoordinate.h"

namespace stride {
namespace gen {
namespace popgen {

void Generate(files::GenDirectory& dir, std::shared_ptr<Population>& population, ContactPoolSys& pool_sys, bool write=true);

void WritePopulation(std::vector<std::shared_ptr<Household>> households, const GenConfiguration& config);

std::vector<std::shared_ptr<GenStruct>> GetClosestStructs(const util::GeoCoordinate& home_coord, const std::vector<std::vector<std::shared_ptr<GenStruct>>>& structs, const GeoGrid& grid);

} // namespace popgen
} // namespace gen
} // namespace stride
