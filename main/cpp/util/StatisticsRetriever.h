#pragma once

#include <iostream>
#include <pop/Population.h>
#include "GeometryGeoCoord.h"


namespace stride {
namespace util {

class StatisticsRetriever{
public:

    explicit StatisticsRetriever(std::shared_ptr<Population>& pop);
/// Returns a tuple with statistics of the regions inquired about.
/// @return tuple with: total population of area, number of sick people, number of working people, number of school going people (including university)
    std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>
        GetStatisticsOfArea(std::shared_ptr<Population>& pop, util::spherical_point center, double radius);

private:
    std::vector<std::vector<unsigned int>> m_sortedPopByIndex;
    double m_min_long;
    double m_max_long;
    double m_bandLength;
};
}
}

