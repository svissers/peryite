#pragma once

#include <iostream>
#include <boost/geometry.hpp>

namespace stride {
namespace util {

    typedef boost::geometry::model::point<double, 2, boost::geometry::cs::spherical_equatorial< boost::geometry::degree > > spherical_point;

    std::ostream& operator<<(std::ostream &s, const spherical_point &coord);

    double calculateDistance(spherical_point point1, spherical_point point2);

}
}