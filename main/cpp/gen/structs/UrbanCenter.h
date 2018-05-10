#pragma once
#include "GenStruct.h"
#include <vector>

namespace stride {
namespace gen {

/**
 * An urban center.
 */
struct UrbanCenter : GenStruct
{
    /// Constructor
    /// @param uc_id        The identification number for the urban center.
    /// @param pop          The population count of the center.
    /// @param uc_name      The name of the urban center.
    /// @param prov         The province id that the center belongs to.
    /// @param coord        The coordinates that the center has been assigned to.
    UrbanCenter(unsigned int uc_id, unsigned int pop, std::string uc_name, int prov, util::spherical_point coord)
    : GenStruct(uc_id, coord), population(pop), name(uc_name), province(prov)
    {}

    /// The total population of a UrbanCenter.
    unsigned int population = 0;
    /// Whether the center is fragmented.
    bool is_fragmented = false;
    /// The populations of the fragmented subsets of a center.
    std::vector<unsigned int> fragmented_populations;
    /// The coordinates of the fragmented subsets of a center.
    std::vector<util::spherical_point> fragmented_coords;
    /// The name of an UrbanCenter.
    std::string name = "";
    /// The province identifier.
    int province = -1;
};

} // namespace gen
} // namespace stride
