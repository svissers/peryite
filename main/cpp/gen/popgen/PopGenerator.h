#pragma once
#include "../files/GenDirectory.h"
#include "../structs/Household.h"
#include "util/GeoCoordinate.h"

namespace stride {
namespace gen {
namespace popgen {

void Generate(files::GenDirectory& dir, unsigned int thread_count);

void writePopulation(std::vector<std::shared_ptr<Household>> households, const GenConfiguration& config);

std::vector<std::shared_ptr<GenStruct>> getClosestStructs(const util::GeoCoordinate& home_coord, const std::vector<std::vector<std::shared_ptr<GenStruct>>>& structs, const GeoGrid& grid);

} // namespace popgen
} // namespace gen
} // namespace stride
