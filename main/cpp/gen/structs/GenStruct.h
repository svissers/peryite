#pragma once
#include "util/GeoCoordinate.h"
#include <memory>

namespace stride {
namespace gen {

/**
 * Abstract struct.
 * All structs built during the first phase of the generator derive from this.
 */
struct GenStruct
{
    typedef std::shared_ptr<GenStruct> ptr;

    /// Default constructor.
    GenStruct() {}

    /// Constructor
    /// @param id           The identification number for the genstruct.
    /// @param coord        The coordinates that the genstruct has been assigned to.
    GenStruct(unsigned int id, util::GeoCoordinate coord)
    : id(id), coordinate(coord)
    {}

    /// The identification number for the genstruct.
    unsigned int id = -1;
    /// The coordinates that the genstruct has been assigned to.
    util::GeoCoordinate coordinate = util::GeoCoordinate();
};

} // namespace gen
} // namespace stride
