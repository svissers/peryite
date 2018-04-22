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

/**
 * Generator class for the geo component.
 */
class GeoGenerator
{
public:
        /// Generates the GeoGrid and places schools, universities, workplaces and
        /// communities
        /// And then writes them to their corresponding files.
        /// @param dir             The directory that the files will be generated in.
        /// @param thread_count    The amount of threads that will be used.
        static void Generate(files::GenDirectory& dir, unsigned int thread_count);
};

} // namespace gen
} // namespace stride
