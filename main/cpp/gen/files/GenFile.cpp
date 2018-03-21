#include "GenFile.h"

namespace stride {
namespace gen {

GenFile::GenFile(GeoConfiguration& config) 
{
    // Get the output directory for this configuration.
    string config_path = config.getPath();
    m_out_dir = "output/"+config_path.substr(0, config_path.find_last_of("."));
    // Create it if it doesn't exist.    
    try {
            create_directories(m_out_dir);
    } catch (std::exception& e) {
            std::cout << "GeoGenerator::generate> Exception while creating output directory:  {}", e.what();
            throw;
    }
}

} // namespace gen
} // namespace stride
