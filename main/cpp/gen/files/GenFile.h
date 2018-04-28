#pragma once
#include "../structs/GenStruct.h"
#include "../GenConfiguration.h"
#include "../GeoGrid.h"
#include "util/CSV.h"

namespace stride {
namespace gen {
namespace files {

/**
 * A class that can read and write generator data files.
 */
class GenFile
{
public:
    GenFile(GenConfiguration& config);
    GenFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo);

    virtual void Write();
    virtual std::vector<std::vector<std::shared_ptr<GenStruct>>> read();

protected:
    std::vector<std::string> m_labels;
    boost::filesystem::path m_file_path;
    std::string m_output_prefix;
    std::string m_file_name;
    std::vector<std::vector<std::shared_ptr<GenStruct>>> m_sorted_structs;

    virtual std::shared_ptr<GenStruct> getStruct(util::CSVRow const & row) =0;
    virtual std::vector<std::string> getValues(std::shared_ptr<GenStruct>) =0;
    void insertStructs(std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo);
};

} // namespace files
} // namespace gen
} // namespace stride
