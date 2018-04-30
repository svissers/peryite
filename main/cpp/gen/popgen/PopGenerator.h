#pragma once
#include "../files/GenDirectory.h"
#include "pool/ContactPoolSys.h"
#include "util/GeoCoordinate.h"

namespace stride {
namespace gen {
namespace popgen {

void Generate(files::GenDirectory& dir, std::shared_ptr<Population>& population, ContactPoolSys& pool_sys, bool write=true);

std::vector<std::shared_ptr<GenStruct>> GetClosestStructs(const util::GeoCoordinate& home_coord, const std::vector<std::vector<std::shared_ptr<GenStruct>>>& structs, const GeoGrid& grid);

} // namespace popgen
} // namespace gen
} // namespace stride
