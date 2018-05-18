#include "GenFile.h"
#include "util/FileSys.h"

namespace stride {
namespace gen {
namespace files {

using namespace std;
using namespace util;
using namespace boost::filesystem;

GenFile::GenFile(GenConfiguration& config)
{
    // Get the output directory for this configuration.
    m_output_prefix = config.GetOutputPrefix();
    try {
        create_directories(m_output_prefix);
    } catch (exception& e) {
        cout << "GeoGenerator::generate> Exception while creating output directory:  {}", e.what();
        throw;
    }
}

GenFile::GenFile(GenConfiguration& config, vector<shared_ptr<GenStruct>> structs, GeoGrid& geo)
: GenFile(config)
{
    // Get the output directory for this configuration.
    m_output_prefix = config.GetOutputPrefix();
    m_file_path = FileSys::BuildPath(m_output_prefix, m_file_name);
    try {
        create_directories(m_output_prefix);
    } catch (exception& e) {
        cout << "GeoGenerator::generate> Exception while creating output directory:  {}", e.what();
        throw;
    }
    insertStructs(structs, geo);
}

GenFile::~GenFile() {}

void GenFile::Write()
{
    if (m_sorted_structs.empty())
        return;
    m_file_path = FileSys::BuildPath(m_output_prefix, m_file_name);
    std::ofstream my_file{m_file_path.string()};
    if(my_file.is_open()) {
        my_file << boost::algorithm::join(m_labels,",") << "\n";
        for (unsigned int band = 0; band < m_sorted_structs.size(); band++) {
            for (const auto& g_struct : m_sorted_structs.at(band)) {
                my_file << boost::algorithm::join(GetValues(g_struct),",");
                my_file << "," << to_string(band) << "\n";
            }
        }
        my_file.close();
    }
}

vector<vector<shared_ptr<GenStruct>>> GenFile::Read()
{
    if (! m_sorted_structs.empty())
        return m_sorted_structs;
    // Populate the struct vector and return it.
    m_sorted_structs = vector<vector<shared_ptr<GenStruct>>>(AMOUNTOFBANDS);
    m_file_path = FileSys::BuildPath(m_output_prefix, m_file_name);
    CSV struct_data(m_file_path.string());
    for (CSVRow const & row : struct_data) {
        auto g_struct = GetStruct(row);
        auto band = row.GetValue<unsigned int>("band");
        m_sorted_structs.at(band).push_back(g_struct);
    }
    return m_sorted_structs;
}

void GenFile::insertStructs(vector<shared_ptr<GenStruct>>& structs, GeoGrid& geo)
{
    auto sorted = vector<vector<shared_ptr<GenStruct>>>(AMOUNTOFBANDS);
    for(const auto& g_struct : structs) {
        for(unsigned int i = 0; i < AMOUNTOFBANDS; i++) {
            double offset = (i+1) * geo.m_longitude_band_width;
            if(g_struct->coordinate.get<1>() < geo.m_min_long+offset) {
                if (sorted.at(i).empty()) {
                    // The band is empty, simply insert.
                    sorted.at(i).push_back(g_struct);
                }
                else {
                    // The band is not empty, sort using latitude.
                    bool inserted = false;
                    for(unsigned int j = 0; j < sorted.at(i).size(); j++){
                        if(sorted.at(i).at(j)->coordinate.get<0>() > g_struct->coordinate.get<0>()){
                            sorted.at(i).insert(sorted.at(i).begin()+j, g_struct);
                            inserted = true;
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
