#pragma once
#include "../../structs/Workplace.h"
#include "../../GeoGrid.h"
#include "../../GenConfiguration.h"
#include <vector>

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

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
        static std::vector<std::shared_ptr<WorkPlace>> Build(GenConfiguration& config, GeoGrid& grid);
};

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
