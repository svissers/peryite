#include "structs/University.h"

/**
 * /// Builds a list of universities using the Geogrid and some configured values.
 */
class UniversitiesBuilder
{
public:
        /// Builds a list of universities using the Geogrid and some configuration values.
        /// @param config       TODO
        /// @param locations    TODO
        static vector<University> build(GeoConfiguration config, vector<UrbanCenter> locations);
};
