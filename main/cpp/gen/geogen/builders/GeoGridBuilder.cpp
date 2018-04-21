#include "GeoGridBuilder.h"
#include "../../structs/UrbanCenter.h"
#include "util/CSV.h"
#include "util/GeoCoordinate.h"

namespace stride {
namespace gen {

using namespace std;

GeoGrid GeoGridBuilder::build(const GenConfiguration& config)
{
        GeoGrid geo_grid;

        // Construct the urban centers from the city data
        util::CSV cities_data = util::CSV(config.getTree().get<string>("geoprofile.cities"));
        double max_long = 0;
        double min_long = 90;
        // TODO: Loop to get total, then relative
        for (util::CSVRow const & row : cities_data) {
            auto longitude  = row.GetValue<double>("longitude");
            auto latitude   = row.GetValue<double>("latitude");
            auto center = make_shared<UrbanCenter>(UrbanCenter(
                row.GetValue<unsigned int>("id"),
                row.GetValue<unsigned int>("population"),
                row.GetValue<string>("name"),
                row.GetValue<unsigned int>("province"),
                util::GeoCoordinate(
                    latitude, longitude)
            ));
            geo_grid.push_back(center);
            geo_grid.addPopulation(center->population);
            if(longitude > max_long)
                    max_long = longitude;
            if(longitude < min_long)
                    min_long = longitude;
        }
        // Initialize the bands which allow for efficient lookup.
        // TODO discuss bands, add AMOUNTOFBANDS to config file
        double longitude_bandwidth = (max_long - min_long)/AMOUNTOFBANDS;
        geo_grid.m_longitude_band_width = longitude_bandwidth;
        geo_grid.m_max_long = max_long;
        geo_grid.m_min_long = min_long;
        return geo_grid;
}

} // namespace gen
} // namespace stride
