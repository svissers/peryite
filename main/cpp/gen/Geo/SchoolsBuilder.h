#include "structs/School.h"

/**
 * Builds a list of schools using the Geogrid and some configured values.
 */
class SchoolsBuilder
{
public:
        /// Builds a list of schools using the Geogrid and some configuration values.
        /// @param config       TODO
        /// @param locations    TODO
        static vector<School> build(GeoConfiguration config, vector<UrbanCenter>);
};
