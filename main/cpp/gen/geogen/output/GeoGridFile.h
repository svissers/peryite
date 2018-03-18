#include "../structs/Community.h"
#include "../structs/School.h"
#include "../structs/University.h"
#include "../structs/UrbanCenter.h"
#include "../structs/Workplace.h"
#include "../GeoGrid.h"
#include "../GeoConfiguration.h"
#include <iostream>
#include <string>
#include <memory>
#include <boost/algorithm/string/join.hpp>
#include <string>
#include <algorithm>    // std::sort

namespace stride{
namespace gen{

std::ostream& operator<<(std::ostream& os, Community& com);
std::ostream& operator<<(std::ostream& os, School& school);
std::ostream& operator<<(std::ostream& os, University& uni);
std::ostream& operator<<(std::ostream& os, UrbanCenter& uc);
std::ostream& operator<<(std::ostream& os, WorkPlace& wp);

std::string getHeader(UrbanCenter& uc);
std::string getHeader(Community& com);
std::string getHeader(School& school);
std::string getHeader(University& uni);
std::string getHeader(WorkPlace& wp);

template <class T>
void writefiles(std::shared_ptr<T> toWrite, std::shared_ptr<GeoGrid> geo, std::string to_write_file){
    //toWrite: shared ptr of vector of <schools/...>
    std::vector<T> sorted(AMOUNTOFBANDS); // vector of vectors  of <schools/...>
    for(auto it = toWrite->begin(); it < toWrite->end();it++){
        for(unsigned int i=0; i < AMOUNTOFBANDS; i++) {
            if(it->coordinate.m_longitude < geo->m_min_long+((i+1)*geo->m_longitude_band_width)){
                if (sorted.at(i).size() == 0){
                    sorted.at(i).push_back(*it);
                }else{
                    for(unsigned int j=0; j < sorted.at(i).size(); j++){
                        if(sorted.at(i).at(j).coordinate.m_latitude > it->coordinate.m_latitude){
                            //geen j--, want j begint op 0
                            sorted.at(i).insert(sorted.at(i).begin()+j, *it);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }

    std::ofstream my_file{to_write_file};
    if(my_file.is_open()){
        if (toWrite->size() != 0){
            my_file << getHeader(toWrite->at(0)) << "\n";
        }
        for(unsigned int i = 0; i < sorted.size(); i++){
            for (unsigned int j = 0; j < sorted[i].size(); j++){
                my_file<< sorted[i][j];
            }
        }
        my_file.close();
    }

}

}
}
