#pragma once
#include "UrbanCenter.h"
#include "geo/GeoCoordinate.h"
#include <vector>

namespace stride {
namespace gen {

struct UrbanCenter
{
        UrbanCenter(unsigned int uc_id, unsigned int pop, std::string uc_name, int prov, util::GeoCoordinate coord)
        {
                id = uc_id;
                population = pop;
                name = uc_name;
                province = prov;
                coordinate = coord;
        }

        unsigned int id = -1;
        unsigned int population = 0;
        std::string name = "";
        int province = -1;
        util::GeoCoordinate coordinate = util::GeoCoordinate();

};

} // namespace gen
} // namespace stride