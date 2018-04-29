#pragma once
#include "GenStruct.h"
#include "pop/Person.h"

namespace stride {
namespace gen {

/**
 * A household containing people.
 */
struct Household : GenStruct
{
    /// Constructor
    /// @param id           The identification number for the household.
    Household(unsigned int h_id)
    {
        id = h_id;
    }
    /// Constructor
    /// @param id           The identification number for the genstruct.
    /// @param persons      The people in this household.
    /// @param coord        The coordinates that the household has been assigned to.
    Household(unsigned int id, std::vector<std::shared_ptr<Person>> persons, util::GeoCoordinate coord)
    : GenStruct(id, coord), persons(persons)
    {}

    /// The people in this household.
    std::vector<std::shared_ptr<Person>> persons;
};

} // namespace gen
} // namespace stride
