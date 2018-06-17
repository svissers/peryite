#include "StatisticsRetriever.h"
#include <algorithm>
#define AMOUNTOFBANDSPOP 100
namespace stride {
namespace util {

StatisticsRetriever::StatisticsRetriever(std::shared_ptr<Population>& pop) {
    m_sortedPopByIndex = std::vector<std::vector<unsigned int>>(AMOUNTOFBANDSPOP);
    m_min_long = 90;
    m_max_long = 0;
    for(unsigned int i = 0; i < pop->size(); i ++){
        double templong = pop->at(i).GetCoordinate().get<1>();
        if( templong > m_max_long){
            m_max_long = templong;
        }
        if( templong < m_min_long){
            m_min_long = templong;
        }
    }
    m_bandLength = (m_max_long - m_min_long )/AMOUNTOFBANDSPOP;
    for(unsigned int index = 0; index < pop->size(); index ++) {
        auto person = pop->at(index);
        for(unsigned int i = 0; i < AMOUNTOFBANDSPOP; i++) {
            double offset = (i+1) * m_bandLength;
            if(person.GetCoordinate().get<1>() < m_bandLength+offset) {
                if (m_sortedPopByIndex.at(i).empty()) {
                    // The band is empty, simply insert.
                    m_sortedPopByIndex.at(i).push_back(index);
                }
                else {
                    // The band is not empty, sort using latitude.
                    bool inserted = false;
                    for(unsigned int j = 0; j < m_sortedPopByIndex.at(i).size(); j++){
                        if(pop->at(m_sortedPopByIndex.at(i).at(j)).GetCoordinate().get<0>() > person.GetCoordinate().get<0>()){
                            m_sortedPopByIndex.at(i).insert(m_sortedPopByIndex.at(i).begin()+j, index);
                            inserted = true;
                            break;
                        }
                    }
                    if (!inserted)
                        m_sortedPopByIndex.at(i).push_back(index);
                }
                break;
            }
        }
    }

}

std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>
    StatisticsRetriever::GetStatisticsOfArea(std::shared_ptr<Population>& pop, util::spherical_point center, double radius) {


    std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int> returnVal;
    
    auto radiusofBands = uint(round(radius/m_bandLength));
    auto band_of_center = uint(round( (center.get<1>() - m_min_long) / m_bandLength ));
    unsigned int beginband = std::max(uint(0), band_of_center+radiusofBands);
    auto endband = std::min(uint(AMOUNTOFBANDSPOP-1), band_of_center+radiusofBands);


    unsigned int popInRadius = 0;
    unsigned int sickPeople = 0;
    unsigned int schoolPeople = 0;
    unsigned int sickSchoolPeople = 0;
    unsigned int workPeople = 0;
    unsigned int sickWorkPeople = 0;
    unsigned int unemployedPeople = 0;
    unsigned int sickUnemployedPeople = 0;


    for(unsigned int i = beginband; i <= endband; i++){
        unsigned int misses = 0;
        for(unsigned int j; j < m_sortedPopByIndex[i].size(); j++){
            auto person = pop->at(m_sortedPopByIndex[i][j]);
            auto personCoord = person.GetCoordinate();
            if(calculateDistance(personCoord, center) <= radius){
                misses = 0;
                //Fill in statistics!
                popInRadius++;
                if(person.IsInPool(ContactPoolType::Id::School)){
                    if(person.GetHealth().IsInfected()){
                        sickPeople++;
                        sickSchoolPeople++;
                    }
                    schoolPeople++;
                }
                else if(person.IsInPool(ContactPoolType::Id::Work)){
                    if(person.GetHealth().IsInfected()){
                        sickPeople++;
                        sickWorkPeople++;
                    }
                    workPeople++;
                }
                else{
                    if(person.GetHealth().IsInfected()) {
                        sickPeople++;
                        sickUnemployedPeople++;
                    }
                    unemployedPeople++;
                }
            }
            else if(center.get<0>() < personCoord.get<0>()){    //stop when you are not going to find more people in radius
                misses++;
                if (misses >= 30){
                    break;
                }
            }

        }

    }
    std::get<0>(returnVal) = popInRadius;
    std::get<1>(returnVal) = sickPeople;
    std::get<2>(returnVal) = schoolPeople;
    std::get<3>(returnVal) = sickSchoolPeople;
    std::get<4>(returnVal) = workPeople;
    std::get<5>(returnVal) = sickWorkPeople;
    std::get<6>(returnVal) = unemployedPeople;
    std::get<7>(returnVal) = sickUnemployedPeople;
    return returnVal;


}


}
}