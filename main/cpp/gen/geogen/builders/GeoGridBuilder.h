#pragma once
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

/// Builds the GeoGrid using city data.
/// @param config       The configuration for a geogrid.
/// @return             A vector of urban centers representing a geogrid.
GeoGrid BuildGeoGrid(const GenConfiguration& config);

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
