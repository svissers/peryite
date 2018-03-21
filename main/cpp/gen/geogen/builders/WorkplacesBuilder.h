#pragma once
#include "../../structs/Workplace.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include <vector>

namespace stride {
namespace gen {

/**
 *  Builds a list of workplaces using the Geogrid and some configured values.
 */
class WorkplacesBuilder
{
public:
        /// Builds a list of workplaces using the Geogrid and some configuration
        /// values.
        /// @param config       TODO
        /// @param grid         TODO
        static std::shared_ptr<std::vector<WorkPlace>> build(GenConfiguration& config, std::shared_ptr<GeoGrid> grid);
};

} // namespace gen
} // namespace stride
