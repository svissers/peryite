#pragma once
#include "structs/UrbanCenter.h"
#include <vector>

namespace stride {
namespace gen {

class GeoGrid : public std::vector<std::shared_ptr<UrbanCenter>>
{
public:
    //unsigned int getTotalPopulation() { return m_total_population; }
    void addPopulation(unsigned int pop) { m_total_population+=pop; }
    double m_min_long;
    double m_max_long;
    double m_longitude_band_width;

    /// iterators
    using std::vector<std::shared_ptr<UrbanCenter>>::begin;
    using std::vector<std::shared_ptr<UrbanCenter>>::end;
    using std::vector<std::shared_ptr<UrbanCenter>>::size;
private:
    unsigned int m_total_population = 0;
};

} // namespace gen
} // namespace stride
