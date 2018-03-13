#pragma once
#include "../structs/University.h"
#include "../GeoConfiguration.h"
#include "../GeoGrid.h"
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
        shared_ptr<vector<University>> universities;
        UniversitiesBuilder();
        /// Builds a list of universities using the Geogrid and some configuration
        /// values.
        /// Note: The input grid will leave the method sorted on population size (big to small).
        /// @param config       TODO
        /// @param grid         TODO
        static std::shared_ptr<std::vector<University>> build(GeoConfiguration& config, std::shared_ptr<GeoGrid> grid);
        void write(std::string UniversityFile);

};

} // namespace gen
} // namespace stride
