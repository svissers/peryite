#include "SchoolAssigner.h"
#include "../../structs/School.h"
#include "../PopGenerator.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

using namespace std;
using namespace gen;
using namespace util;

void AssignSchools(
        vector<vector<shared_ptr<GenStruct>>> &schools,
        const shared_ptr<Population> population,
        shared_ptr<Region> region,
        const GeoGrid &grid
        )
{
    auto config                         = region->config;
    const unsigned int school_size      = config.GetTree().get<unsigned int>("school_size");
    const unsigned int school_cp_size   = config.GetTree().get<unsigned int>("school_cp_size");
    // Create the contactpools for every school
    unsigned int cp_id = region->first_cps[ContactPoolType::Id::School];
    for (const auto &band : schools) {
        for (const auto &g_struct : band) {
            auto school = std::static_pointer_cast<School>(g_struct);
            auto coord  = school->coordinate;
            for (unsigned int size = 0; size < school_size; size += school_cp_size) {
                auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::School, coord);
                school->pools.push_back(pool);
                cp_id++;
            }
        }
    }
    region->last_cps[ContactPoolType::Id::School] = cp_id-1;

    // Assign students to schools
    for (unsigned int i = region->first_person_id; i < region->last_person_id; i++) {
        auto &person = population->at(i);
        auto age = person.GetAge();
        if (age >= 3 && age < 18) {
            auto home_coord = person.GetCoordinate();
            // Find the closest schools
            std::vector<shared_ptr<GenStruct>> closest_schools = GetClosestStructs(home_coord, schools,
                                                                                   grid);
            if (closest_schools.empty()) {
                continue;
            }
            // Create a uniform distribution to select a school
            auto rn_manager = config.GetRNManager();
            auto school_generator = rn_manager->GetGenerator(
                    trng::fast_discrete_dist(uint(closest_schools.size())));
            auto school = static_pointer_cast<School>(closest_schools.at(uint(school_generator())));
            // Create a uniform distribution to select a contactpool in the selected school
            auto cp_generator = rn_manager->GetGenerator(
                    trng::fast_discrete_dist(uint(school->pools.size())));
            auto pool = school->pools.at(uint(cp_generator()));
            person.setPoolId(ContactPoolType::Id::School, uint(pool->GetId()));
            pool->AddMember(&person);
        }
    }
}

} // assigner
} // popgen
} // namespace gen
} // namespace stride

