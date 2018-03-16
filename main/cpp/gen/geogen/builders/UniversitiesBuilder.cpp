#include "UniversitiesBuilder.h"
#include "../structs/UrbanCenter.h"
#include <algorithm>

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;


    UniversitiesBuilder::UniversitiesBuilder(){};

std::shared_ptr<vector<University>> UniversitiesBuilder::build(GeoConfiguration& config, shared_ptr<GeoGrid> grid)
{
        universities = make_shared<vector<University>>();
        unsigned int mandatory_students_count = (unsigned int)(config.getUniversityFraction() * grid->getTotalPopulation());

        // Every university has an average of 3000 students
        unsigned int school_count = mandatory_students_count / 3000;

        // Sort cities based on population size, biggest to smallest.
        auto compare_population = [](const UrbanCenter& a, const UrbanCenter b) { return a.population > b.population; };
        sort(grid->begin(), grid->end(), compare_population);

        // Take the 10 biggest cities
        int city_count = min(10, (int)grid->size());
        auto biggest = grid->begin();
        auto smallest = grid->begin() + city_count;
        vector<UrbanCenter> big_cities(biggest, smallest);

        unsigned int total_city_population = 0;
        for(UrbanCenter center : big_cities) {
                total_city_population += center.population;
        }
        unsigned int counter = 0;
        for(UrbanCenter center : big_cities){
                unsigned int SchoolsToAdd = int((school_count* (center.population / total_city_population))+0.5);
                counter+=SchoolsToAdd;
                if(counter==school_count){
                        for(unsigned int i = 0; i < SchoolsToAdd; i++)
                                universities->push_back(University(i, center.coordinate));
                        break;
                }
                if(counter>school_count) {
                        while (counter > school_count) {
                                SchoolsToAdd--;
                                counter--;
                        }
                        for(unsigned int i = 0; i < SchoolsToAdd; i++)
                                universities->push_back(University(i, center.coordinate));
                        break;
                }
        }




        return universities;
}

    void UniversitiesBuilder::write(std::string UniversityFile){
            std::vector<std::vector<University> > sortedUniversities;
            for(auto it = universities->begin(); it < universities->end();it++){
                    for(unsigned int i=0; i < AMOUNTOFBANDS; i++){
                            if(it->coordinate.m_longitude<minLong+((i+1)*LongitudeBandWidth)){
                                    for(unsigned int j=0; j<sortedUniversities[i].size(); j++){
                                            if(sortedUniversities[i][j].coordinate.m_latitude> it->coordinate.m_latitude){
                                                    j--;
                                                    sortedUniversities[i].insert(sortedUniversities[i].begin()+j, *it);
                                                    break;
                                            }
                                    }
                                    break;
                            }
                    }
            }
            ofstream my_file{UniversityFile};
            if(my_file.is_open()){
                    for(unsigned int i = 0; i < sortedUniversities.size(); i++){
                            for (unsigned int j = 0; j < sortedUniversities[i].size(); j++){
                                    my_file<< "||university|| ID: " << sortedUniversities[i][j].id << " , "
                                           << sortedUniversities[i][j].coordinate << " " ;

                            }
                            my_file<< std::endl;
                    }
            }



    }

} // namespace gen
} // namespace stride
