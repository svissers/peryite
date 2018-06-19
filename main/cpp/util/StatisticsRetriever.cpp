#include "StatisticsRetriever.h"

#include <iostream>

namespace stride {
namespace util {

StatisticsRetriever::StatisticsRetriever(std::shared_ptr<Population>& pop) {
    m_sortedPopByIndex = std::vector<std::vector<unsigned int>>(AMOUNTOFBANDSPOP);
    // Getting the maximum and minimum longitudes
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
    //going over the population and inserting the id/index of every person according to where they live
    for(unsigned int index = 0; index < pop->size(); index ++) {
        auto person = pop->at(index);
        for(unsigned int i = 0; i < AMOUNTOFBANDSPOP; i++) {
            double offset = (i+1) * m_bandLength;
            if(person.GetCoordinate().get<1>() < m_min_long + offset) {
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

    for(auto band : m_sortedPopByIndex){
       std::cout << "band size: " << band.size() << std::endl;
    }
}

std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>
    StatisticsRetriever::GetStatisticsOfArea(std::shared_ptr<Population>& pop, util::spherical_point center, double radius) {

    std::cout << "input latitude: " << center.get<0>() << std::endl;
    std::cout << "input longitude: " << center.get<1>() << std::endl;
    std::cout << "input radius: " << radius << std::endl;

    //the statistics this funditon will return
    std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int> returnVal;

    double m_bandLengthInKm = m_bandLength * 111.320 * cos(center.get<0>());
    //calculating which parts of the sorted pop are possibly in range of the chosen area
    auto radiusofBands = uint(ceil(radius/m_bandLengthInKm));

    auto band_of_center = uint( (center.get<1>() - m_min_long) / m_bandLength );
    unsigned int beginband = std::max(uint(0), band_of_center-radiusofBands);
    auto endband = std::min(uint(AMOUNTOFBANDSPOP-1), band_of_center+radiusofBands);

    std::cout << "radius: " << radiusofBands << std::endl;
    std::cout << "begin: " << beginband << std::endl;
    std::cout << "end: " << endband << std::endl;
    std::cout << "m_bandlength: " << m_bandLength << std::endl;

    unsigned int popInRadius = 0;
    unsigned int sickPeople = 0;
    unsigned int schoolPeople = 0;
    unsigned int sickSchoolPeople = 0;
    unsigned int workPeople = 0;
    unsigned int sickWorkPeople = 0;
    unsigned int unemployedPeople = 0;
    unsigned int sickUnemployedPeople = 0;

    //going over the chosen parts of the population,
    for(unsigned int i = beginband; i <= endband; i++){
        unsigned int misses = 0;
        std::cout << m_sortedPopByIndex[i].size() << std::endl;
        for (unsigned int j : m_sortedPopByIndex[i]) {
            auto person = pop->at(j);
            auto personCoord = person.GetCoordinate();
            if(calculateDistance(personCoord, center) <= radius){
                misses = 0;
                //Fill in statistics
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
            else if(center.get<0>() < personCoord.get<0>()){    // check if it got out of range of the area, so it doesn't check when all checks will be false
                misses++;
                if (misses >= 1000){
                    break;
                }
            }

        }

    }

    std::cout << "popInRadius: " << popInRadius << std::endl;
    std::cout << "sickPeople: " << sickPeople << std::endl;

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