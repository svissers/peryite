#include "structs/Community.h"

/**
 * Builds a list of communities using the Geogrid and some configured values.
 */
class CommunitiesBuilder
{
public:
        /// Builds a list of communities using the Geogrid and some configuration values.
        /// @param config       TODO
        /// @param locations    TODO
        static vector<Community> build(GeoConfiguration config, vector<UrbanCenter>);
};
