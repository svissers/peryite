#pragma once
#include "GenFile.h"

namespace stride {
namespace gen {

/**
 *
 */
class GeoGridFile : public GenFile
{
private:
    const std::string m_file_name = "Geogrid.csv";
    const std::initializer_list<std::string> m_labels = {"id","latitude","longitude","name","province","population","band"};

    void write() {}
    std::vector<std::vector<std::shared_ptr<GenStruct>>> read() {}
};

} // namespace gen
} // namespace stride
