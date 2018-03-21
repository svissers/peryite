#pragma once
#include "../structs/GenStruct.h"
#include "../GeoConfiguration.h"
#include "../GeoGrid.h"
#include "util/CSV.h"

namespace stride {
namespace gen {

/**
 * A class that can read and write generator data files.
 */
class GenFile
{
public:
    GenFile(GeoConfiguration& config);
    GenFile(GeoConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, std::shared_ptr<GeoGrid> geo);

private:
    boost::filesystem::path m_out_dir;
    const std::string m_file_name;
    const std::initializer_list<std::string> labels;
    std::vector<std::vector<std::shared_ptr<GenStruct>>> m_sorted_structs;

    std::shared_ptr<GenStruct> getStruct(CSVRow const & row);
    void insertStructs(std::vector<std::shared_ptr<GenStruct>> structs, std::shared_ptr<GeoGrid> geo);
};

} // namespace gen
} // namespace stride
