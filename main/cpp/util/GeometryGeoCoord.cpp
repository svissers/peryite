#include "GeometryGeoCoord.h"

using namespace stride;
using namespace util;

/// earth radius in km
unsigned int earth_radius = 6371;


std::ostream& stride::util::operator<<(std::ostream& s, const spherical_point& coord){
    s << "latitude: " << coord.get<0>() << ", longitude: " << coord.get<1>();
    return s;
}

double stride::util::calculateDistance(spherical_point point1, spherical_point point2){
    return boost::geometry::distance(point1, point2) * earth_radius;
}
