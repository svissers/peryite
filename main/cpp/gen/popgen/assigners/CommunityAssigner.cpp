#include "CommunityAssigner.h"
#include "../../structs/Community.h"
#include "../PopGenerator.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

using namespace std;
using namespace gen;
using namespace util;

unsigned int AssignCommunities(
        vector<vector<shared_ptr<GenStruct>>> communities, const shared_ptr<Population> population,
        const GenConfiguration &config, const GeoGrid &grid, unsigned int start_cp_id, unsigned int first_person_id , unsigned int next_first_person_id) {
    // -------------
    // Contactpools
    // -------------
    const unsigned int community_size = 2000;
    const unsigned int community_cp_size = 20;
    // Create the contactpools for every community
    unsigned int cp_id = start_cp_id;
    for (auto &band : communities) {
        for (auto &g_struct : band) {
            auto community = std::static_pointer_cast<Community>(g_struct);
            auto com_id = ContactPoolType::Id::PrimaryCommunity;
            if (!community->is_primary)
                com_id = ContactPoolType::Id::SecondaryCommunity;
            for (unsigned int size = 0; size < community_size; size += community_cp_size) {
                auto pool = make_shared<ContactPool>(cp_id, com_id);
                community->pools.push_back(pool);
                cp_id++;
            }
        }
    }
    // ------------------------------
    // Assign persons to communities
    // ------------------------------
    for (std::size_t i = first_person_id; i < next_first_person_id; i++) {
        auto hh_id = population->at(i).GetPoolId(ContactPoolType::Id::Household);
        auto home_coord = population->at(i).GetCoordinate();
        auto closest_communities = GetClosestStructs(home_coord, communities, grid);
        if (closest_communities.empty()) {

            continue;
        }
        // Create a uniform distribution to select a secondary community
        auto rn_manager = config.GetRNManager();
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
        // Iterate over the household in the population
        while (population->at(i).GetPoolId(ContactPoolType::Id::Household) == hh_id) {
            // Add entire household to the same secondary community.
            // ------------------------------
            population->at(i).setPoolId(ContactPoolType::Id::SecondaryCommunity, pool->GetId());
            pool->AddMember(&population->at(i));

            // Add Persons from the same household to different primary communities.
            // ------------------------------
            // Create a uniform distribution to select a primary community
            auto primary_rn_manager = config.GetRNManager();
            auto primary_community_generator = primary_rn_manager->GetGenerator(
                    trng::fast_discrete_dist(closest_communities.size()));

            auto primary_community_index = primary_community_generator();
            auto primary_community = static_pointer_cast<Community>(
                    closest_communities.at(primary_community_index));
            unsigned int s_counter = 0;
            // Search uniformly for a primary community in the vector of closest communities.
            while (s_counter < closest_communities.size() * 2 && primary_community->is_primary) {
                primary_community_index = primary_community_generator();
                primary_community = static_pointer_cast<Community>(
                        closest_communities.at(primary_community_index));
                s_counter++;
            }
            // Create a uniform distribution to select a contactpool within the selected community
            auto primary_cp_generator = primary_rn_manager->GetGenerator(
                    trng::fast_discrete_dist(primary_community->pools.size()));
            auto primary_cp_index = primary_cp_generator();
            auto primary_pool = primary_community->pools[primary_cp_index];

            // Add person to the selected primary community
            population->at(i).setPoolId(ContactPoolType::Id::PrimaryCommunity,
                                        primary_pool->GetId());
            primary_pool->AddMember(&population->at(i));

            if (++i >= population->size())
                break;
        }
        i--;

    }
    return cp_id;
}
} // assigner
} // popgen
} // namespace gen
} // namespace stride

