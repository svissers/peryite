#pragma once
#include "../structs/School.h"
#include "../GeoConfiguration.h"
#include "../GeoGrid.h"
#include <boost/filesystem.hpp>

namespace stride {
namespace gen {

/**
 * A class that can write and read generator data files.
 */
class GenFile
{
public:
    GenFile(GeoConfiguration& config);

private:
    boost::filesystem::path m_out_dir;
};

} // namespace gen
} // namespace stride
