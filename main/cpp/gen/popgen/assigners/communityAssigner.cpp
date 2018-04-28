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
    vector<vector<shared_ptr<GenStruct>>> communities, const shared_ptr<Population> population,
    const GenConfiguration& config, const GeoGrid& grid)
{
    const unsigned int community_cp_size = 20;
    // ------------------------------
    // Assign persons to communities
    // ------------------------------
    for (std::size_t i = 0; i != population->size(); ++i) {
        auto& person    = population->at(i);
        auto hh_id      = person.GetPoolId(ContactPoolType::Id::Household);
        auto home_coord = person.GetCoordinate();
        std::vector<shared_ptr<GenStruct>> closest_communities = GetClosestStructs(home_coord, communities, grid);
        if (closest_communities.empty()) {
            std::cout << "closest_communities is empty: " << std::endl;
            continue;
        }
        // Create a uniform distribution to select a primary community
        auto rn_manager             = config.GetRNManager();
        auto community_generator    = rn_manager->GetGenerator(trng::fast_discrete_dist(closest_communities.size()));

        shared_ptr<Community> community;
        unsigned int community_index    = 0;
        unsigned int p_counter          = 0;
        while(p_counter < closest_communities.size() * 2 && !community->is_primary) {
            community_index        = community_generator();
            community              = static_pointer_cast<Community>(closest_communities.at(community_index));
            p_counter++;
        }

        // Create a uniform distribution to select a contactpool in the selected community
        auto cp_generator           = rn_manager->GetGenerator(trng::fast_discrete_dist(community->pools.size()));
        auto cp_index               = cp_generator();
        auto pool                   = community->pools[cp_index];

        // Iterate over the household in the population
        while ( person.GetPoolId(ContactPoolType::Id::Household) == hh_id ) {
            // Add entire household to the same primary community.
            // ------------------------------
            person.setPoolId(ContactPoolType::Id::PrimaryCommunity, pool->GetId());
            pool->AddMember(&person);

            // Add Persons from the same household to different secondary communities.
            // ------------------------------
            // Create a uniform distribution to select a secondary community
            auto secondary_rn_manager             = config.GetRNManager();
            auto secondary_community_generator    = secondary_rn_manager->GetGenerator(trng::fast_discrete_dist(closest_communities.size()));

            auto secondary_community_index        = secondary_community_generator();
            auto secondary_community              = static_pointer_cast<Community>(closest_communities.at(secondary_community_index));
            unsigned int s_counter = 0;
            // Search uniformly for a secondary community in the vector of closest communities.
            while(s_counter < closest_communities.size() * 2 && !secondary_community->is_primary) {
                secondary_community_index        = secondary_community_generator();
                secondary_community              = static_pointer_cast<Community>(closest_communities.at(secondary_community_index));
                s_counter++;
            }
            // Create a uniform distribution to select a contactpool within the selected community
            auto secondary_cp_generator           = secondary_rn_manager->GetGenerator(trng::fast_discrete_dist(secondary_community->pools.size()));
            auto secondary_cp_index               = secondary_cp_generator();
            auto secondary_pool                   = secondary_community->pools[secondary_cp_index];

            // Add person to the selected secondary community
            person.setPoolId(ContactPoolType::Id::SecondaryCommunity, secondary_pool->GetId());
            secondary_pool->AddMember(&person);

            // Remove the secondary community pool from the list once it's full
            if (secondary_pool->GetSize() >= community_cp_size) {
                auto pools = secondary_community->pools;
                pools.erase(pools.begin() + secondary_cp_index);
                secondary_community->full_pools.push_back(secondary_pool);
                if (secondary_community->pools.empty()) {
                    std::cout << "Deleting secondary community" << std::endl;
                    communities.erase(communities.begin() + secondary_community_index);
                }
            }

            if (++i >= population->size())
                break;
            person = population->at(i);
        }

        // Remove the primary community pool from the list once it's full
        if (pool->GetSize() >= community_cp_size) {
            auto pools = community->pools;
            pools.erase(pools.begin() + cp_index);
            community->full_pools.push_back(pool);
            if (community->pools.empty())
                communities.erase(communities.begin() + community_index);
        }
    }
}

} // assigner
} // popgen
} // namespace gen
} // namespace stride

