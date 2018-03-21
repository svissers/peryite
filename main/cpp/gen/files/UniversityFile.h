#pragma once
#include "GenFile.h"
#include "../structs/University.h"
#include "../GeoConfiguration.h"
#include "../GeoGrid.h"

namespace stride {
namespace gen {

/**
 * Builds a list of communities using the Geogrid and some configured values.
 */
class UniversityFile : public GenFile
{
public:
    UniversityFile(GeoConfiguration& config, std::vector<std::shared_ptr<University>> universities, std::shared_ptr<GeoGrid> geo);
    
    void write();
    std::vector<std::shared_ptr<University>> read();

private:
    const std::string m_file_name = "Universities.csv";
    std::shared_ptr<GeoGrid> m_grid;
    std::vector<std::vector<std::shared_ptr<University>>> m_sorted_universities;

    void sortUniversities(std::vector<std::shared_ptr<University>> universities, std::shared_ptr<GeoGrid> geo);
};

} // namespace gen
} // namespace stride
