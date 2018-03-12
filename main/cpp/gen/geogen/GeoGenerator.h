#pragma once
#include <string>

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
        static void generate(std::string config_path);
};

} // namespace gen
} // namespace stride

