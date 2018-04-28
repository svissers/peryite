#pragma once
#include "../GenConfiguration.h"
#include "pop/Population.h"
#include <boost/property_tree/ptree.hpp>
#include <vector>


namespace stride {
namespace gen {
namespace files {

/**
 *
 */
class PopulationFile
{
public:
    PopulationFile(GenConfiguration& config);
    PopulationFile(GenConfiguration& config, std::shared_ptr<Population> population);
    void Write();
    std::shared_ptr<Population> Read(const boost::property_tree::ptree& belief_pt);

private:
    std::vector<std::string> m_labels;
    boost::filesystem::path m_file_path;
    std::string m_file_name;
    std::shared_ptr<Population> m_population;

    std::vector<std::string> getValues(const Person& person);
};

std::vector<std::vector<unsigned int>> GetReferenceHouseholds(const GenConfiguration& config);

typedef std::shared_ptr<PopulationFile> PopulationFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
