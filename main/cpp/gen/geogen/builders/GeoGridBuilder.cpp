#include "GeoGridBuilder.h"
#include "../structs/UrbanCenter.h"
#include "util/CSV.h"
#include "geo/GeoCoordinate.h"
namespace stride {
namespace gen {

using namespace std;

shared_ptr<GeoGrid> GeoGridBuilder::build(const GeoConfiguration& config)
{
        geo_grid = make_shared<GeoGrid>();
        // Construct the urban centers from the city data
        util::CSV cities_data = util::CSV(config.getCitiesFileName());
        maxLong = 0;
        minLong = 90;
        for (util::CSVRow& row : cities_data) {
                auto id         = row.getValue<unsigned int>("city_id");
                auto population = row.getValue<unsigned int>("population");
                auto name       = row.getValue<string>("city_name");
                auto province   = row.getValue<unsigned int>("province");
                auto longitude  = row.getValue<double>("longitude");
                auto latitude   = row.getValue<double>("latitude");
                UrbanCenter center = UrbanCenter(id, population, name, province, util::GeoCoordinate(longitude, latitude));
                geo_grid->push_back(center);
                if(longitude > maxLong)
                        maxLong = longitude;
                if(longitude < minLong)
                        minLong = longitude;
        }
        LongitudeBandWidth = (maxLong - minLong)/AMOUNTOFBANDS;
        geo_grid->m_longitude_band_width = LongitudeBandWidth;
        geo_grid->m_max_long = maxLong;
        geo_grid->m_min_long = minLong;
        return geo_grid;
}

GeoGridBuilder::GeoGridBuilder() = default;


} // namespace gen
} // namespace stride
