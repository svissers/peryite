#include "GenFile.h"
#include "util/GeoCoordinate.h"
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/join.hpp>

namespace stride {
namespace gen {
namespace files {

using namespace std;
using namespace util;

GenFile::GenFile(GenConfiguration& config)
{
    // Get the output directory for this configuration.
    string config_path = config.GetPath();
    m_out_dir = "output/"+config_path.substr(0, config_path.find_last_of("."));
    try {
        create_directories(m_out_dir);
    } catch (exception& e) {
        cout << "GeoGenerator::generate> Exception while creating output directory:  {}", e.what();
        throw;
    }
}

GenFile::GenFile(GenConfiguration& config, vector<shared_ptr<GenStruct>> structs, GeoGrid& geo)
: GenFile(config)
{
    insertStructs(structs, geo);
}

void GenFile::write()
{
    if (m_sorted_structs.size() == 0)
        return;
    string file_path = m_out_dir.string()+"/"+m_file_name;
    std::ofstream my_file{file_path};
    if(my_file.is_open()) {
        my_file << boost::algorithm::join(m_labels,",") << "\n";
        for (unsigned int band = 0; band < m_sorted_structs.size(); band++) {
            for (auto g_struct : m_sorted_structs.at(band)) {
                my_file << boost::algorithm::join(getValues(g_struct),",");
                my_file << "," << to_string(band) << "\n";
            }
        }
        my_file.close();
    }
}

vector<vector<shared_ptr<GenStruct>>> GenFile::read()
{
    if (m_sorted_structs.size() != 0)
        return m_sorted_structs;
    // Populate the struct vector and return it.
    m_sorted_structs = vector<vector<shared_ptr<GenStruct>>>(AMOUNTOFBANDS);
    string file_path = m_out_dir.string()+"/"+m_file_name;
    CSV struct_data(file_path);
    for (CSVRow const & row : struct_data) {
        auto g_struct = getStruct(row);
        auto band = row.GetValue<unsigned int>("band");
        m_sorted_structs.at(band).push_back(g_struct);
    }
    return m_sorted_structs;
}

void GenFile::insertStructs(vector<shared_ptr<GenStruct>> structs, GeoGrid& geo)
{
    auto sorted = vector<vector<shared_ptr<GenStruct>>>(AMOUNTOFBANDS);
    for(auto g_struct : structs) {
        for(unsigned int i = 0; i < AMOUNTOFBANDS; i++) {
            double offset = (i+1) * geo.m_longitude_band_width;
            if(g_struct->coordinate.m_longitude < geo.m_min_long+offset) {
                if (sorted.at(i).size() == 0) {
                    // The band is empty, simply insert.
                    sorted.at(i).push_back(g_struct);
                }
                else {
                    // The band is not empty, sort using latitude.
                    bool inserted = false;
                    for(unsigned int j = 0; j < sorted.at(i).size(); j++){
                        if(sorted.at(i).at(j)->coordinate.m_latitude > g_struct->coordinate.m_latitude){
                            sorted.at(i).insert(sorted.at(i).begin()+j, g_struct);
                            break;
                        }
                    }
                    if (!inserted)
                        sorted.at(i).push_back(g_struct);
                }
                break;
            }
        }
    }
    m_sorted_structs = sorted;
}

} // namespace files
} // namespace gen
} // namespace stride
