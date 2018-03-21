#pragma once
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include "../../structs/School.h"
#include <vector>

namespace stride {
namespace gen {

/**
 * Builds a list of schools using the Geogrid and some configured values.
 */
class SchoolsBuilder
{
public:
        /// Builds a list of schools using the Geogrid and some configuration values.
        /// @param config       TODO
        /// @param locations    TODO
        static std::shared_ptr<std::vector<School>> build(const GenConfiguration& config, std::shared_ptr<GeoGrid> locations);
};

} // namespace gen
} // namespace stride
