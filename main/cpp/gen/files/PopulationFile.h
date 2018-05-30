#pragma once
#include "../GenConfiguration.h"
#include "pop/Population.h"
#include "pool/ContactPoolSys.h"
#include "util/Regions.h"
#include <boost/property_tree/ptree.hpp>
#include <vector>


namespace stride {
namespace gen {
namespace files {

/**
 * A class that can read and write population data files.
 */
class PopulationFile
{
public:
    /// Constructor. Constructs the PopulationFile using the config.
    /// @param config           The geopop configuration to be used.
    PopulationFile(GenConfiguration& config);

    /// Constructor. Constructs the PopulationFile object using an existing population.
    /// @param config           The geopop configuration to be used.
    /// @param population       The population that the file will contain.
    PopulationFile(GenConfiguration& config, std::shared_ptr<Population> population);

    /// Writes the population that the file contains to a file.
    void Write();

    /// Reads the population from a file or returns it if it already exists
    /// @param population        The population to be seeded or replaced.
    void Read(std::shared_ptr<Population>& population);

    /// Writes the regions of the population to a file.
    /// @param output_prefix    The prefix used by the region file.
    /// @param regions          The regions to be written to a file.
    static void WriteRegions(std::string output_prefix, util::Regions& regions);

    /// Read the regions of the population from a file.
    /// @param config_pt        Property_tree with general configuration settings.
    /// @return                 The regions contained in the file.
    static util::Regions ReadRegions(const boost::property_tree::ptree& config_pt);

    /// Writes the Contactpoolsys to a file.
    /// @param output_prefix    The prefix used by the region file.
    /// @param pool_sys         The poolsys to be written to a file.
    static void WritePoolSys(std::string output_prefix, const ContactPoolSys& pool_sys);

    /// Reads the Contactpoolsys from a file.
    /// @param config_pt        Property_tree with general configuration settings.
    /// @param pool_sys         The poolsys to be seeded.
    /// @return                 The poolsys contained in the file.
    static void ReadPoolSys(const boost::property_tree::ptree& config_pt, ContactPoolSys& pool_sys);

private:
    /// The labels for the first line in a csv file.
    std::vector<std::string> m_labels;
    /// The path to the file.
    boost::filesystem::path m_file_path;
    /// The name of the file
    std::string m_file_name;
    /// The population that the file contains.
    std::shared_ptr<Population> m_population;
    /// The prefix for the file path.
    std::string m_output_prefix;

    /// Returns the values of a person in string format.
    /// @param person           The person that contains the values.
    /// @return                 Vector of the values of the person in string format.
    std::vector<std::string> GetValues(const Person& person);

    /// Returns the values of a region in string format.
    /// @param region           The region that contains the values.
    /// @return                 Vector of the values of the region in string format.
    static std::vector<std::string> GetValues(const std::shared_ptr<util::Region> region);

    /// Returns the values of a contactpool in string format.
    /// @param pool             The contactpool that contains the values.
    /// @return                 Vector of the values of the contactpool in string format.
    static std::vector<std::string> GetValues(const ContactPool& pool);
};

/// Returns the reference households from the file defined in the configuration.
/// @param config               The geopop configuration to be used.
/// @return                     Vector of households containing a vector of ages.
std::vector<std::vector<unsigned int>> GetReferenceHouseholds(const GenConfiguration& config);

typedef std::shared_ptr<PopulationFile> PopulationFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
