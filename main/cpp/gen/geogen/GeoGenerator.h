#pragma once
#include <string>
#include "./builders/CommunitiesBuilder.h"
#include "./builders/GeoGridBuilder.h"
#include "./builders/SchoolsBuilder.h"
#include "./builders/UniversitiesBuilder.h"
#include "./builders/WorkplacesBuilder.h"
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
        /// @param config_path     The path to the configuration xml file to use.
        /// @param thread_count    The amount of threads that will be used.
        static void generate(std::string config_path, unsigned int thread_count);
};

} // namespace gen
} // namespace stride
