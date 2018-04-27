#pragma once
#include "../GenConfiguration.h"
#include "pop/Population.h"
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

    std::shared_ptr<Population> Read();

    std::vector<std::vector<unsigned int>> GetReferenceHouseholds(const GenConfiguration& config);


private:
    std::vector<std::string> m_labels;
    boost::filesystem::path m_out_dir;
    std::string m_file_name;
    std::shared_ptr<Population> m_population;

    std::vector<std::string> getValues(std::shared_ptr<Person> person);
};

typedef std::shared_ptr<PopulationFile> PopulationFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
