#pragma once
#include "GenStruct.h"
#include "UrbanCenter.h"
#include <vector>

namespace stride {
namespace gen {

struct UrbanCenter : GenStruct
{
        UrbanCenter(unsigned int uc_id, unsigned int pop, std::string uc_name, int prov, util::GeoCoordinate coord)
        : GenStruct(uc_id, coord)
        {
                population = pop;
                name = uc_name;
                province = prov;
        }

        /// The total population of a UrbanCenter.
        unsigned int population = 0;
        /// Whether the center is fragmented.
        bool is_fragmented = false;
        /// The populations of the fragmented subsets of a center.
        std::vector<unsigned int> fragmented_populations;
        /// The coordinates of the fragmented subsets of a center.
        std::vector<util::GeoCoordinate> fragmented_coords;
        /// The name of an UrbanCenter.
        std::string name = "";
        /// The province, currently not used.
        int province = -1;
};

} // namespace gen
} // namespace stride
