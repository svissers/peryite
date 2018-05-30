#pragma once
#include "../files/GenDirectory.h"

namespace stride {
namespace gen {
namespace popgen {

/// Generates the contactpools for a population.
/// @param dir             The directory used for input and output.
/// @param population      The population that will be assigned contactpools.
void Generate(files::GenDirectory& dir, std::shared_ptr<Population>& population);


/// Returns a list of the closests structs to the home_coord.
/// @param home_coord   The coordinate where the search begins.
/// @param struct       The list of structs to search through.
/// @param grid         The grid that contains parameters for the search procedure.
std::vector<std::shared_ptr<GenStruct>> GetClosestStructs(const util::spherical_point& home_coord, const std::vector<std::vector<std::shared_ptr<GenStruct>>>& structs, const GeoGrid& grid);

} // namespace popgen
} // namespace gen
} // namespace stride
