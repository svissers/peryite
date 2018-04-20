#pragma once
#include "../../structs/University.h"
#include "../../GenConfiguration.h"
#include "../../GeoGrid.h"
#include <vector>

namespace stride {
namespace gen {

/**
 * /// Builds a list of universities using the Geogrid and some configured
 * values.
 */
class UniversitiesBuilder
{
public:
        /// Builds a list of universities using the Geogrid and some configuration
        /// values.
        /// Note: The input grid will leave the method sorted on population size (big to small).
        /// @param config       TODO
        /// @param grid         TODO
        static std::vector<std::shared_ptr<University>> build(GenConfiguration& config, GeoGrid& grid);

};

} // namespace gen
} // namespace stride
