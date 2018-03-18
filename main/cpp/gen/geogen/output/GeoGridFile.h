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

namespace stride{
namespace gen{

std::ostream& operator<<(std::ostream& os, Community& com);
std::ostream& operator<<(std::ostream& os, School& school);
std::ostream& operator<<(std::ostream& os, University& uni);
std::ostream& operator<<(std::ostream& os, UrbanCenter& uc);
std::ostream& operator<<(std::ostream& os, WorkPlace& wp);

template  <class T>
void writefiles(std::shared_ptr<T> toWrite, std::shared_ptr<GeoGrid> geo, std::string to_write_file){
    //toWrite: shared ptr of vector of <schools/...>
    std::vector<T> sorted; // vector of vectors  of <schools/...>
    sorted.push_back(*toWrite);
    // std::cout << "bandwidth " << geo->m_longitude_band_width << std::endl;
    // std::cout << "min long " << geo->m_min_long << std::endl;
    for(auto it = toWrite->begin(); it < toWrite->end();it++){
        for(unsigned int i=0; i < AMOUNTOFBANDS; i++) {
            if (it == toWrite->begin()){
                // std::cout << "m_long " << it->coordinate.m_longitude << std::endl;
                // std::cout << "calc " << geo->m_min_long+((i+1)* geo->m_longitude_band_width) << std::endl;

            }

            if(it->coordinate.m_longitude < geo->m_min_long+((i+1)*geo->m_longitude_band_width)){
                for(unsigned int j=0; j<sorted[i].size(); j++){
                    if(sorted[i][j].coordinate.m_latitude> it->coordinate.m_latitude){
                        j--;
                        sorted[i].insert(sorted[i].begin()+j, *it);
                        break;
                    }
                }
                break;
            }
        }
    }

    // for (auto i : sorted) {
    //     std::cout << "" << std::endl;
    //     for (auto j : i) {
    //         std::cout << j << std::endl;
    //     }
    // }
    std::ofstream my_file{to_write_file};
    if(my_file.is_open()){
        for(unsigned int i = 0; i < sorted.size(); i++){
            for (unsigned int j = 0; j < sorted[i].size(); j++){
                my_file<< sorted[i][j];
            }
            my_file<< std::endl;
        }
        my_file.close();
    }

}

}
}
