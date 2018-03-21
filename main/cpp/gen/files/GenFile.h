#pragma once
#include "../structs/GenStruct.h"
#include "../GenConfiguration.h"
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
    GenFile(GenConfiguration& config);
    GenFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, std::shared_ptr<GeoGrid> geo);

    void write();
    std::vector<std::vector<std::shared_ptr<GenStruct>>> read();

private:
    boost::filesystem::path m_out_dir;
    const std::string m_file_name;
    const std::initializer_list<std::string> m_labels;
    std::vector<std::vector<std::shared_ptr<GenStruct>>> m_sorted_structs;

    std::shared_ptr<GenStruct> getStruct(util::CSVRow const & row);
    std::vector<std::string> getValues(std::shared_ptr<GenStruct>);

    void insertStructs(std::vector<std::shared_ptr<GenStruct>> structs, std::shared_ptr<GeoGrid> geo);
};

} // namespace gen
} // namespace stride
