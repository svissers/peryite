#pragma once
#include <string>
#include "./builders/CommunitiesBuilder.h"
#include "./builders/GeoGridBuilder.h"
#include "./builders/SchoolsBuilder.h"
#include "./builders/UniversitiesBuilder.h"
#include "./builders/WorkplacesBuilder.h"
#include "../files/GenDirectory.h"
#include "pop/Population.h"
#include <memory>

namespace stride {
namespace gen {
namespace geogen {

/// Generates the GeoGrid and places schools, universities, workplaces and
/// communities
/// And then writes them to their corresponding files.
/// @param dir             The directory that the files will be generated in.
/// @param population      The population that will be seeded.
void Generate(files::GenDirectory& dir, std::shared_ptr<Population>& population);

} // namespace geogen
} // namespace gen
} // namespace stride
