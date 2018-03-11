#include "structs/UrbanCenter.h"
#include <boost/property_tree/ptree.hpp>

/**
 * Builds a geogrid using city data.
 */
class GeoGridBuilder
{
public:
        /// Builds the GeoGrid using city data.
        /// @param cities_tree     A property tree that contains city data.
        static vector<UrbanCenter> build(boost::property_tree::ptree cities_tree);
};
