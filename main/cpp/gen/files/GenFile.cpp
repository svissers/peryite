#include "GenFile.h"
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;

GenFile::GenFile(GeoConfiguration& config) 
{
    // Get the output directory for this configuration.
    string config_path = config.getPath();
    m_out_dir = "output/"+config_path.substr(0, config_path.find_last_of("."));
    try {
        create_directories(m_out_dir);
    } catch (std::exception& e) {
        std::cout << "GeoGenerator::generate> Exception while creating output directory:  {}", e.what();
        throw;
    }
}

GenFile::GenFile(GeoConfiguration& config, vector<shared_ptr<GenStruct>> structs, shared_ptr<GeoGrid> geo)
: GenFile(config)
{
    insertStructs(structs, geo);
}

void GenFile::write() 
{
    if (m_sorted_structs.size() == 0)
        return;
    CSV structs_data(m_labels);
    for (unsigned int band = 0; band < m_sorted_structs.size(); band++) {
        for (auto g_struct : m_sorted_structs.at(band)) {
            vector<string> values = getValues(g_struct);
            values.push_back(to_string(band));            
            structs_data.addRow(values);
        }
    }
    string file_path = m_out_dir.string()+"/"+m_file_name;    
    structs_data.write(file_path);
}

std::vector<std::vector<std::shared_ptr<GenStruct>>> GenFile::read() 
{
    if (m_sorted_structs.size() != 0)
        return m_sorted_structs;
    // Populate the struct vector and return it.
    m_sorted_structs = vector<vector<shared_ptr<GenStruct>>>(AMOUNTOFBANDS);
    string file_path = m_out_dir.string()+"/"+m_file_name;    
    CSV struct_data(file_path);
    for (CSVRow const & row : struct_data) {
        auto g_struct = getStruct(row);
        auto band = row.getValue<unsigned int>("band");
        m_sorted_structs.at(band).push_back(g_struct);
    }
    return m_sorted_structs;
}

void GenFile::insertStructs(vector<shared_ptr<GenStruct>> structs, shared_ptr<GeoGrid> geo) 
{
    sorted = vector<vector<shared_ptr<GenStruct>>>(AMOUNTOFBANDS);
    for(auto g_struct : structs) {
        for(unsigned int i = 0; i < AMOUNTOFBANDS; i++) {
            double offset = (i+1) * geo->m_longitude_band_width;
            if(g_struct->coordinate.m_longitude < geo->m_min_long+offset) {
                if (sorted.at(i).size() == 0) {
                    // The band is empty, simply insert.
                    sorted.at(i).push_back(g_struct);
                }
                else {
                    // The band is not empty, sort using latitude.
                    bool inserted = false;                    
                    for(unsigned int j = 0; j < sorted.at(i).size(); j++){
                        if(sorted.at(i).at(j).coordinate.m_latitude > it->coordinate.m_latitude){
                            sorted.at(i).insert(sorted.at(i).begin()+j, *it);
                            break;
                        }
                    }
                    if (!inserted) 
                        sorted.at(i).push_back(school);
                }
                break;
            }
        }
    }
    m_sorted_structs = sorted;
}

} // namespace gen
} // namespace stride
