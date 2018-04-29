#pragma once
#include "structs/UrbanCenter.h"
#include <vector>

namespace stride {
namespace gen {

class GeoGrid : public std::vector<std::shared_ptr<UrbanCenter>>
{
public:
    /// The minimum longitude used in the geogrid.
    double m_min_long;
    /// The maximum longitude used in the geogrid.
    double m_max_long;
    /// The width of the bands that are used for optimized searching in the grid.
    double m_longitude_band_width;

    /// iterators
    using std::vector<std::shared_ptr<UrbanCenter>>::begin;
    using std::vector<std::shared_ptr<UrbanCenter>>::end;
    using std::vector<std::shared_ptr<UrbanCenter>>::size;
};

} // namespace gen
} // namespace stride
