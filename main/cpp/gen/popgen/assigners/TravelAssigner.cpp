#include "TravelAssigner.h"
#include "../../structs/School.h"
#include "../PopGenerator.h"
#include "pool/ContactPoolType.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

using namespace std;
using namespace gen;
using namespace util;

void AssignTravellers(
        const std::shared_ptr<Population> population,
        files::GenDirectory& dir, unsigned int num_days
)
{
    Regions regions = dir.GetRegions();
    unsigned int amountRegions = regions.size();
    auto tourist_fraction = regions.at(0)->config.GetTree().get<double>("tourist_fraction");
    auto work_travel_fraction = regions.at(0)->config.GetTree().get<double>("work_travel_fraction");
    auto rn_manager = regions.at(0)->config.GetRNManager();
    auto work_travel_dist = rn_manager->GetGenerator(trng::fast_discrete_dist(1-work_travel_fraction, work_travel_fraction));
    auto tourist_dist = rn_manager->GetGenerator(trng::fast_discrete_dist(1-tourist_fraction, tourist_fraction));

    vector<vector<vector<shared_ptr<GenStruct>>>> travelWorkplaces;
    vector<vector<vector<shared_ptr<GenStruct>>>> travelComs;
    vector<GeoGrid> travelGrids;
    vector<vector<shared_ptr<UrbanCenter>>> travelCities;

    for(unsigned int i = 0; i < regions.size(); i++){
        auto grid = dir.GetGeoGridFile(regions.at(i)->id)->ReadGrid();
        // Sort cities based on population size, biggest to smallest.
        auto compare_population = [](const shared_ptr<UrbanCenter> a, const shared_ptr<UrbanCenter> b) { return a->population > b->population; };
        sort(grid.begin(), grid.end(), compare_population);

        // Take the 10 biggest cities
        int city_count = min(20, (int)grid.size());
        auto biggest = grid.begin();
        auto smallest = grid.begin() + city_count;
        vector<shared_ptr<UrbanCenter>> big_cities(biggest, smallest);
        auto workplaces     = dir.GetWorkplaceFile(regions.at(i)->id)->Read();
        travelWorkplaces.push_back(workplaces);
        auto communities    = dir.GetCommunityFile(regions.at(i)->id)->Read();
        travelComs.push_back(communities);
        auto travelGrid = dir.GetGeoGridFile(regions.at(i)->id)->ReadGrid();


        travelCities.push_back(big_cities);
    }

    auto region_dist = rn_manager->GetGenerator(trng::fast_discrete_dist(travelCities.size()));
    auto city_dist = rn_manager->GetGenerator(trng::fast_discrete_dist(20));
    auto day_dist = rn_manager->GetGenerator(trng::fast_discrete_dist(num_days));

    auto endDayWork_dist = rn_manager->GetGenerator(trng::fast_discrete_dist(5));
    auto endDayTravel_dist = rn_manager->GetGenerator(trng::fast_discrete_dist(14));



    int previousHHid = -1;
    bool vacationGoers = false;
    unsigned int travel_sec_com_id = 0;
    unsigned int tourist_regionNr;
    unsigned int tourist_cityNr;
    unsigned int tourist_dayNr;
    unsigned int tourist_endDay;
    vector<shared_ptr<GenStruct>> closest_communities;

    util::spherical_point travel_coord;

    for (size_t i = 0; i < population->size(); ++i) {
        Person person = population->at(i);

        if(previousHHid != person.GetPoolId(ContactPoolType::Id::Household)){
            previousHHid = person.GetPoolId(ContactPoolType::Id::Household);
            vacationGoers = false;
            if(tourist_dist() == 1){
                vacationGoers = true;


                //assign travel_com_id's            // TODO
                tourist_regionNr = region_dist();  //travel can be in same region
                tourist_cityNr = city_dist();

                travel_coord = travelCities[tourist_regionNr][tourist_cityNr]->coordinate;
                auto communities    = travelComs[tourist_regionNr];
                auto grid    = travelGrids[tourist_regionNr];
                closest_communities = GetClosestStructs(travel_coord, communities, grid);


                //assigning secondary communities

                auto community_generator = rn_manager->GetGenerator(
                        trng::fast_discrete_dist(closest_communities.size() - 1));

                shared_ptr<Community> community = nullptr;
                unsigned int community_index = 0;
                unsigned int p_counter = 0;
                while (p_counter < (closest_communities.size() * 2)) {
                    community_index = community_generator();
                    community = static_pointer_cast<Community>(closest_communities.at(community_index));
                    if (!community->is_primary)
                        break;
                    p_counter++;
                }
                if (!community)
                    break;
                // Create a uniform distribution to select a contactpool in the selected community
                auto cp_generator = rn_manager->GetGenerator(trng::fast_discrete_dist(community->pools.size()));
                auto cp_index = cp_generator();
                auto pool = community->pools[cp_index];
                auto poolID =  pool->GetId();
                travel_sec_com_id = poolID;


                //assigning primary communities

                community = nullptr;
                community_index = 0;
                p_counter = 0;
                while (p_counter < (closest_communities.size() * 2)) {
                    community_index = community_generator();
                    community = static_pointer_cast<Community>(closest_communities.at(community_index));
                    if (community->is_primary)
                        break;
                    p_counter++;
                }
                if (!community)
                    break;
                // Create a uniform distribution to select a contactpool in the selected community
                cp_generator = rn_manager->GetGenerator(trng::fast_discrete_dist(community->pools.size()));
                cp_index = cp_generator();
                pool = community->pools[cp_index];
                poolID =  pool->GetId();
                unsigned int travel_prim_com_id = poolID;


                tourist_dayNr = day_dist();
                tourist_endDay = tourist_dayNr + endDayTravel_dist() +7;

                person.setTravelComIds(travel_prim_com_id, travel_sec_com_id);
            }
        }
        else if(vacationGoers){
            unsigned int travel_prim_com_id = 0;
            person.setTravelComIds(travel_prim_com_id, travel_sec_com_id);
        }
        if(person.GetAge() >= 18 && person.GetAge() < 65 && !vacationGoers) {
            if (work_travel_dist() == 1) {
                //assign secondary workplace
                unsigned int regionNr = region_dist();
                unsigned int cityNr = city_dist();

                //GetClosestStructs
                // : get workplaces of chosen region
                auto work_coord = travelCities[regionNr][cityNr]->coordinate;
                auto workplaces    = travelWorkplaces[regionNr];
                auto grid    = travelGrids[regionNr];
                auto closest_workplaces = GetClosestStructs(work_coord, workplaces, grid);

                // Create a uniform distribution to select a workplace
                std::function<int()> wp_generator = rn_manager->GetGenerator(
                        trng::fast_discrete_dist(closest_workplaces.size()));
                auto workplace = static_pointer_cast<WorkPlace>(closest_workplaces.at(wp_generator()));
                unsigned int sec_work_id = workplace->pool->GetId();


                unsigned int dayNr = day_dist();
                unsigned int endDay = dayNr + endDayWork_dist()+1;
                person.setTravelWorkId(sec_work_id);
            }
        }

    }
}

} // assigner
} // popgen
} // namespace gen
} // namespace stride

