#include "structs/Workplace.h"

/**
 *  Builds a list of workplaces using the Geogrid and some configured values.
 */
class WorkplacesBuilder
{
public:
        /// Builds a list of workplaces using the Geogrid and some configuration values.
        /// @param config       TODO
        /// @param locations    TODO
        static vector<Workplace> build(GeoConfiguration config, vector<UrbanCenter>);
};
