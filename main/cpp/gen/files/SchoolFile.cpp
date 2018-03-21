#include "SchoolFile.h"
#include "GenFile.h"
#include "util/CSV.h"
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;

SchoolFile::SchoolFile(GeoConfiguration& config, vector<shared_ptr<School>> schools, shared_ptr<GeoGrid> geo)
: GenFile(config)
{
    sortSchools(schools, geo);
}

void SchoolFile::write() 
{
    if (m_sorted_schools.size() == 0)
        return;
    string file_path = out_dir.string()+"/"+m_file_name;
    ofstream my_file{file_path};
    if(my_file.is_open()) {
        my_file << "id, latitude, longitude, band" << "\n";
        for (unsigned int i = 0; i < m_sorted_schools.size(); i++) {
            for (auto& school : m_sorted_schools.at(i)) {
                my_file << to_string(school->id) << ", ";
                my_file << to_string(school->coordinate.m_latitude) << ", ";
                my_file << to_string(school->coordinate.m_longitude) << ", ";
                my_file << i << "\n";
            }
        }
        my_file.close();
    }
}

std::vector<std::vector<std::shared_ptr<School>>> SchoolFile::read() 
{
    if (m_sorted_schools.size() != 0) {
        return m_sorted_schools;
    }
    sorted = vector<vector<shared_ptr<School>>>(AMOUNTOFBANDS);
    string file_path = out_dir.string()+"/"+m_file_name;    
    CSV schools_data = CSV(file_path);
    for (CSVRow const & row : schools_data) {
        auto latitude   = row.getValue<double>("latitude");
        auto longitude  = row.getValue<double>("longitude");
        auto band       = row.getValue<unsigned int>("band");
        School school = make_shared(School(
            row.getValue<unsigned int>("id"),  
            util::GeoCoordinate(
                latitude, longitude)
            );
        sorted.at(i).push_back(school);
    }
}

void SchoolFIle::sortSchools(vector<shared_ptr<School>> schools, shared_ptr<GeoGrid> geo) 
{
    sorted = vector<vector<shared_ptr<School>>>(AMOUNTOFBANDS);
    for(auto school : schools) {
        for(unsigned int i = 0; i < AMOUNTOFBANDS; i++) {
            double offset = (i+1) * geo->m_longitude_band_width;
            if(school->coordinate.m_longitude < geo->m_min_long+offset) {
                if (sorted.at(i).size() == 0) {
                    // The band is empty, simply insert.
                    sorted.at(i).push_back(school);
                }
                else {
                    // The band is not empty, sort using latitude.
                    for(unsigned int j = 0; j < sorted.at(i).size(); j++){
                        if(sorted.at(i).at(j).coordinate.m_latitude > it->coordinate.m_latitude){
                            sorted.at(i).insert(sorted.at(i).begin()+j, *it);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    m_sorted_schools = sorted;
}


} // namespace gen
} // namespace stride
