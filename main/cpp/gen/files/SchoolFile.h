#pragma once
#include "GenFile.h"
#include "../structs/School.h"
#include "../GeoConfiguration.h"
#include "../GeoGrid.h"

namespace stride {
namespace gen {

/**
 * Builds a list of communities using the Geogrid and some configured values.
 */
class SchoolFile : public GenFile
{
public:
    SchoolFile(GeoConfiguration& config, std::vector<std::shared_ptr<School>> schools, std::shared_ptr<GeoGrid> geo);
    
    void write();
    std::vector<std::shared_ptr<School>> read();

private:
    const std::string m_file_name = "Schools.csv";
    std::shared_ptr<GeoGrid> m_grid;
    std::vector<std::vector<std::shared_ptr<School>>> m_sorted_schools;

    void sortSchools(std::vector<std::shared_ptr<School>> schools, std::shared_ptr<GeoGrid> geo);
};

} // namespace gen
} // namespace stride
