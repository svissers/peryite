#include "GeoGridBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "trng/uniform_dist.hpp"

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

using namespace std;

GeoGrid BuildGeoGrid(const GenConfiguration& config)
{
        GeoGrid geo_grid = GeoGrid();

        // Reference set of cities
        util::CSV cities_data = util::CSV(config.GetTree().get<string>("geoprofile.cities"));;
        // Total population from reference set
        unsigned int total_ref_population   = 0;
        // Total population to be generated
        auto total_population       = config.GetTree().get<unsigned int>("population_size");
        // Longitude bounds for efficient distance based searches
        double max_longitude = 0;
        double min_longitude = 90;
        // Load the centers from the cities file
        for (util::CSVRow const & row : cities_data) {
            auto longitude  = row.GetValue<double>("longitude");
            auto latitude   = row.GetValue<double>("latitude");
            auto center = make_shared<UrbanCenter>(UrbanCenter(
                row.GetValue<unsigned int>("id"),
                row.GetValue<unsigned int>("population"),
                row.GetValue<string>("name"),
                row.GetValue<unsigned int>("province"),
                util::spherical_point(latitude, longitude)
            ));
            geo_grid.push_back(center);

            total_ref_population += center->population;
            if(longitude > max_longitude) {
                    max_longitude = longitude;
            }
            else if(longitude < min_longitude) {
                    min_longitude = longitude;
            }
        }
        // Calculate actual population
        for (auto& center : geo_grid) {
            double ref_pop          = center->population;
            double relative_pop     = (ref_pop / total_ref_population);
            unsigned int actual_pop = relative_pop * total_population;
            center->population = actual_pop;
        }

        // Fragmentation parameters
        auto fragment_centers = config.GetTree().get<bool>("fragment_centers");
        if (fragment_centers) {
            const unsigned int fragmentation_bound  = 10000;
            const unsigned int nr_to_fragment       = 50;
            const unsigned int fragment_dist[4]     = {40,40,15,5};
            const unsigned int fragment_amounts[4]  = {2,3,4,5};
            const double lat_lon_diff               = 0.1;
            auto rn_manager         = config.GetRNManager();
            auto frag_center_gen    = rn_manager->GetGenerator(trng::fast_discrete_dist(geo_grid.size()));
            auto frag_amount_gen    = rn_manager->GetGenerator(trng::fast_discrete_dist(std::begin(fragment_dist), std::end(fragment_dist)));
            auto latlon_diff_gen    = rn_manager->GetGenerator(trng::uniform_dist<>(-lat_lon_diff, lat_lon_diff));
            unsigned int nr_fragmented = 0;

            while(nr_fragmented < nr_to_fragment) {
                // Uniformly pick center from grid
                auto center = geo_grid.at(frag_center_gen());
                if (center->population <= fragmentation_bound) {
                    // Fragment center
                    center->is_fragmented = true;
                    int frag_amount = fragment_amounts[frag_amount_gen()];
                    int frag_size   = center->population / frag_amount;
                    std::vector<unsigned int> frag_pop;
                    std::vector<util::spherical_point> frag_coords;

                    for (int i = 0; i < frag_amount; i++) {
                        frag_pop.push_back(frag_size);
                        double lat = center->coordinate.get<0>() + latlon_diff_gen();
                        double lon = center->coordinate.get<1>() + latlon_diff_gen();
                        frag_coords.emplace_back(util::spherical_point(lat, lon));
                        if(lon > max_longitude) {
                            max_longitude = lon;
                        }
                        else if(lon < min_longitude) {
                            min_longitude = lon;
                        }
                    }
                    center->fragmented_populations  = frag_pop;
                    center->fragmented_coords       = frag_coords;
                    nr_fragmented++;
                }
            }
        }

        // Initialize the bands which allow for efficient distance based search.
        double longitude_bandwidth = (max_longitude - min_longitude)/AMOUNTOFBANDS;
        geo_grid.m_longitude_band_width = longitude_bandwidth;
        geo_grid.m_max_long = max_longitude;
        geo_grid.m_min_long = min_longitude;
        return geo_grid;
}

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
