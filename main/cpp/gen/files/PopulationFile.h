#pragma once
#include "../GenConfiguration.h"
#include "pop/Population.h"
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

    void SetFileName(std::string newname);

private:
    /// The labels for the first line in a csv file.
    std::vector<std::string> m_labels;
    /// The path to the file.
    boost::filesystem::path m_file_path;
    /// The name of the file
    std::string m_file_name;
    /// The population that the file contains.
    std::shared_ptr<Population> m_population;

    /// Returns the values of a person in string format.
    /// @param person           The person that contains the values.
    /// @return                 Vector of the values of the person in string format.
    std::vector<std::string> GetValues(const Person& person);
};

/// Returns the reference households from the file defined in the configuration.
/// @param config               The geopop configuration to be used.
/// @return                     Vector of households containing a vector of ages.
std::vector<std::vector<unsigned int>> GetReferenceHouseholds(const GenConfiguration& config);

typedef std::shared_ptr<PopulationFile> PopulationFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
