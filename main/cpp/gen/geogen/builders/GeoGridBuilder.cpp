#include "GeoGridBuilder.h"
#include "../structs/UrbanCenter.h"
#include "util/CSV.h"
#include "geo/GeoCoordinate.h"

namespace stride {
namespace gen {

using namespace std;

shared_ptr<GeoGrid> GeoGridBuilder::build(const GeoConfiguration& config)
{
        shared_ptr<GeoGrid> geo_grid = make_shared<GeoGrid>();

        // Construct the urban centers from the city data
        util::CSV cities_data = util::CSV(config.getTree().get<string>("geoprofile.cities"));
        double max_long = 0;
        double min_long = 90;
        for (util::CSVRow const & row : cities_data) {
            auto longitude  = row.getValue<double>("longitude");
            auto latitude   = row.getValue<double>("latitude");
            UrbanCenter center = UrbanCenter(
                row.getValue<unsigned int>("id"),
                row.getValue<unsigned int>("population"),
                row.getValue<string>("name"),
                row.getValue<unsigned int>("province"),
                util::GeoCoordinate(
                    latitude, longitude)
                );
            geo_grid->push_back(center);
            geo_grid->addPopulation(center.population);
            if(longitude > max_long)
                    max_long = longitude;
            if(longitude < min_long)
                    min_long = longitude;
        }
        // Initialize the bands which allow for efficient lookup.
        // TODO discuss bands, add AMOUNTOFBANDS to config file
        double longitude_bandwidth = (max_long - min_long)/AMOUNTOFBANDS;
        geo_grid->m_longitude_band_width = longitude_bandwidth;
        geo_grid->m_max_long = max_long;
        geo_grid->m_min_long = min_long;
        return geo_grid;
}

} // namespace gen
} // namespace stride
