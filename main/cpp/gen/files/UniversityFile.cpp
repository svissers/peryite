#include "UniversityFile.h"
#include "util/CSV.h"
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;

UniversityFile::UniversityFile(GeoConfiguration& config, vector<shared_ptr<University>> universities, shared_ptr<GeoGrid> geo)
: GenFile(config)
{
    sortUniversities(universities, geo);
}

void UniversityFile::write() 
{
    if (m_sorted_universities.size() == 0)
        return;
    string file_path = m_out_dir.string()+"/"+m_file_name;
    ofstream my_file{file_path};
    if(my_file.is_open()) {
        my_file << "id, latitude, longitude, band" << "\n";
        for (unsigned int i = 0; i < m_sorted_universities.size(); i++) {
            for (auto& University : m_sorted_universities.at(i)) {
                my_file << to_string(University->id) << ", ";
                my_file << to_string(University->coordinate.m_latitude) << ", ";
                my_file << to_string(University->coordinate.m_longitude) << ", ";
                my_file << i << "\n";
            }
        }
        my_file.close();
    }
}

std::vector<std::vector<std::shared_ptr<University>>> UniversityFile::read() 
{
    if (m_sorted_universities.size() != 0) {
        return m_sorted_universities;
    }
    sorted = vector<vector<shared_ptr<University>>>(AMOUNTOFBANDS);
    string file_path = m_out_dir.string()+"/"+m_file_name;    
    CSV universities_data = CSV(file_path);
    for (CSVRow const & row : universities_data) {
        auto latitude   = row.getValue<double>("latitude");
        auto longitude  = row.getValue<double>("longitude");
        auto band       = row.getValue<unsigned int>("band");
        University University = make_shared(University(
            row.getValue<unsigned int>("id"),  
            util::GeoCoordinate(
                latitude, longitude)
            );
        sorted.at(i).push_back(University);
    }
}

void UniversityFIle::sortUniversities(vector<shared_ptr<University>> universities, shared_ptr<GeoGrid> geo) 
{
    sorted = vector<vector<shared_ptr<University>>>(AMOUNTOFBANDS);
    for(auto University : universities) {
        for(unsigned int i = 0; i < AMOUNTOFBANDS; i++) {
            double offset = (i+1) * geo->m_longitude_band_width;
            if(University->coordinate.m_longitude < geo->m_min_long+offset) {
                if (sorted.at(i).size() == 0) {
                    // The band is empty, simply insert.
                    sorted.at(i).push_back(University);
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
    m_sorted_universities = sorted;
}


} // namespace gen
} // namespace stride
