#pragma once
#include "structs/UrbanCenter.h"
#include <vector>

namespace stride {
namespace gen {

class GeoGrid : public std::vector<UrbanCenter>
{
public:
        unsigned int getTotalPopulation() { return m_total_population; }
private:
        unsigned int m_total_population = 0;
};

} // namespace gen
} // namespace stride
