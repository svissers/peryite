#include <boost/property_tree/ptree.hpp>

/**
 * Generator class for the geo component.
 */
class GeoGenerator
{
public:
        /// Generates the GeoGrid and places schools, universities, workplaces and communities
        /// And then writes them to their corresponding files.
        /// @param config_file     The configuration xml file to use.
        static void generate(const boost::filesystem::path& config_path);
};
