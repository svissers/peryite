#pragma once
#include "structs/UrbanCenter.h"
#include <vector>

namespace stride {
namespace gen {

class GeoGrid : public std::vector<std::shared_ptr<UrbanCenter>>
{
public:
    double m_min_long;
    double m_max_long;
    double m_longitude_band_width;

    /// iterators
    using std::vector<std::shared_ptr<UrbanCenter>>::begin;
    using std::vector<std::shared_ptr<UrbanCenter>>::end;
    using std::vector<std::shared_ptr<UrbanCenter>>::size;
};

} // namespace gen
} // namespace stride
