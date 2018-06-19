#pragma once

#include <iostream>
#include <pop/Population.h>
#include "GeometryGeoCoord.h"

#define AMOUNTOFBANDSPOP 100

namespace stride {
namespace util {

class StatisticsRetriever{
public:

    explicit StatisticsRetriever(std::shared_ptr<Population>& pop);
/// Returns a tuple with statistics of the regions inquired about.
/// @return tuple with: total population of area, number of sick people, number of working people, sick working people, number of school going people (including university), sick school people, number of unemployed people, sick unemployed people
    std::tuple<unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int>
        GetStatisticsOfArea(std::shared_ptr<Population>& pop, util::spherical_point center, double radius);

private:
    /// A vector that acts like the sorted version of a population. It holds the indexes of persons in the original population vector.
    std::vector<std::vector<unsigned int>> m_sortedPopByIndex;
    /// The lowest longitude of a person in the population.
    double m_min_long;
    /// The highest longitude of a person in the population.
    double m_max_long;
    /// The amount of longitudinal degrees in every band.
    double m_bandLength;
};
}
}

