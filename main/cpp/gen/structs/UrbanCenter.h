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

        unsigned int population = 0;
        std::string name = "";
        int province = -1;
};

} // namespace gen
} // namespace stride
