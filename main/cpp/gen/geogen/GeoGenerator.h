#pragma once
#include <string>
#include "./builders/CommunitiesBuilder.h"
#include "./builders/GeoGridBuilder.h"
#include "./builders/SchoolsBuilder.h"
#include "./builders/UniversitiesBuilder.h"
#include "./builders/WorkplacesBuilder.h"
#include "../files/GenDirectory.h"
#include <memory>

namespace stride {
namespace gen {
namespace geogen {

/// Generates the GeoGrid and places schools, universities, workplaces and
/// communities
/// And then writes them to their corresponding files.
/// @param dir             The directory that the files will be generated in.
/// @param thread_count    The amount of threads that will be used.
void Generate(files::GenDirectory& dir);

} // namespace geogen
} // namespace gen
} // namespace stride
