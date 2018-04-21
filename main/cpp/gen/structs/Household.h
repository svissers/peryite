#pragma once
#include "GenStruct.h"
#include "pop/Person.h"

namespace stride {
namespace gen {

struct Household : GenStruct
{
    Household(unsigned int h_id)
    {
        id = h_id;
    }
    Household(unsigned int id, std::vector<std::shared_ptr<Person>> persons, util::GeoCoordinate coord)
    : GenStruct(id, coord), persons(persons)
    {}
    std::vector<std::shared_ptr<Person>> persons;
};

} // namespace gen
} // namespace stride
