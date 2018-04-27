#include "householdAssigner.h"
#include "../../structs/Community.h"
#include "../PopGenerator.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

using namespace std;
using namespace gen;

void AssignCommunities(
    vector<vector<shared_ptr<GenStruct>>> communities, const vector<shared_ptr<Household>>& households,
    const GenConfiguration& config, const GeoGrid& grid)
{
    // -------------
    // Contactpools
    // -------------
    const unsigned int community_size      = 2000;
    const unsigned int community_cp_size   = 20;
    // Create the contactpools for every community
    unsigned int cp_id = 0;
    unsigned int community_amount = 0;
    for (auto& band : communities) {
        community_amount+=band.size();
    }
    for (auto& band : communities) {
        for (auto& g_struct : band) {
            auto community  = std::static_pointer_cast<Community>(g_struct);
            if(community->id+1 >= community_amount/2){
                community->is_primary = false;
            }
            else{
                community->is_primary = true;
            }
            for(unsigned int size = 0; size < community_size; size += community_cp_size) {
                auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::PrimaryCommunity);
                community->pools.push_back(pool);
                cp_id++;
            }
        }
    }
    // ------------------------------
    // Assign persons to communities
    // ------------------------------
    for (const auto& household : households) {
        auto home_coord = household->coordinate;
        std::vector<shared_ptr<GenStruct>> closest_communities = GetClosestStructs(home_coord, communities, grid);
        if (closest_communities.empty()) {
            std::cout << "closest_communities is empty: " << std::endl;
            continue;
        }
        // Create a uniform distribution to select a primary community
        auto rn_manager             = config.GetRNManager();
        auto community_generator    = rn_manager->GetGenerator(trng::fast_discrete_dist(closest_communities.size()));

        auto community_index        = community_generator();
        auto community              = static_pointer_cast<Community>(closest_communities.at(community_index));
        while(true){
            if(community->is_primary){
                break;
            }
            //std::cout << community_index << std::endl;
            //std::cout << community->is_primary << std::endl;
            community_index        = community_generator();
            community              = static_pointer_cast<Community>(closest_communities.at(community_index));
        }

        // Create a uniform distribution to select a contactpool in the selected community
        auto cp_generator           = rn_manager->GetGenerator(trng::fast_discrete_dist(community->pools.size()));
        auto cp_index               = cp_generator();
        auto pool                   = community->pools[cp_index];
        for (const auto& person : household->persons) {
            // Add person to the selected primary community
            person->setPoolId(ContactPoolType::Id::PrimaryCommunity, pool->GetId());
            pool->AddMember(person.get());
        }
        // Remove the pool from the list once it's full
        if (pool->GetSize() >= community_cp_size) {
            auto pools = community->pools;
            pools.erase(pools.begin() + cp_index);
            community->full_pools.push_back(pool);
            if (community->pools.empty()) {
                communities.erase(communities.begin() + community_index);
            }
        }
        for (const auto& person : household->persons) {
            if (closest_communities.empty()) {
                std::cout << "closest_communities is empty: " << std::endl;
                continue;
            }
            // Create a uniform distribution to select a secondary community
            auto secondary_rn_manager             = config.GetRNManager();
            auto secondary_community_generator    = secondary_rn_manager->GetGenerator(trng::fast_discrete_dist(closest_communities.size()));

            auto secondary_community_index        = secondary_community_generator();
            auto secondary_community              = static_pointer_cast<Community>(closest_communities.at(secondary_community_index));
            unsigned int counter = 0;
            while(true){
                counter++;
                if(!secondary_community->is_primary){
                    break;
                }
                // ugly bandaid solution to infinite loop
                if(counter>=20){
                    break;
                }
                secondary_community_index        = secondary_community_generator();
                secondary_community              = static_pointer_cast<Community>(closest_communities.at(secondary_community_index));
            }
            // Create a uniform distribution to select a contactpool in the selected community
            auto secondary_cp_generator           = secondary_rn_manager->GetGenerator(trng::fast_discrete_dist(secondary_community->pools.size()));
            auto secondary_cp_index               = secondary_cp_generator();
            auto secondary_pool                   = secondary_community->pools[secondary_cp_index];

            // Add person to the selected secondary community
            person->setPoolId(ContactPoolType::Id::SecondaryCommunity, secondary_pool->GetId());
            secondary_pool->AddMember(person.get());

            // Remove the pool from the list once it's full
            if (secondary_pool->GetSize() >= community_cp_size) {
                auto pools = secondary_community->pools;
                pools.erase(pools.begin() + secondary_cp_index);
                secondary_community->full_pools.push_back(secondary_pool);
                if (secondary_community->pools.empty()) {
                    std::cout << "deleting secondary com" << std::endl;
                    communities.erase(communities.begin() + secondary_community_index);
                }
            }
        }

    }
}

} // assigner
} // popgen
} // namespace gen
} // namespace stride

