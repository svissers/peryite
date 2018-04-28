#include "householdAssigner.h"
#include "../../structs/School.h"
#include "../PopGenerator.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

using namespace std;
using namespace gen;

void AssignSchools(
    vector<vector<shared_ptr<GenStruct>>>& schools, const shared_ptr<Population> population,
    const GenConfiguration& config, const GeoGrid& grid)
{
    const unsigned int school_size      = 500;
    const unsigned int school_cp_size   = 20;
    // Create the contactpools for every school
    unsigned int cp_id = 0;
    for (const auto& band : schools) {
        for (const auto& g_struct : band) {
            auto school = std::static_pointer_cast<School>(g_struct);
            for(unsigned int size = 0; size < school_size; size += school_cp_size) {
                auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::School);
                school->pools.push_back(pool);
                cp_id++;
            }
        }
    }
    // Assign young students to schools
    for (auto& person : *population) {
        auto age = person.GetAge();
        if (age >= 3 && age < 18) {
            auto home_coord = person.GetCoordinate();
            // Find the closest schools
            std::vector<shared_ptr<GenStruct>> closest_schools = GetClosestStructs(home_coord, schools, grid);
            if (closest_schools.empty()) {
                std::cout << "closest_schools is empty: " << age << std::endl;
                continue;
            }
            // Create a uniform distribution to select a school
            auto rn_manager         = config.GetRNManager();
            auto school_generator   = rn_manager->GetGenerator(trng::fast_discrete_dist(closest_schools.size()));
            auto school = static_pointer_cast<School>(closest_schools.at(school_generator()));
            // Create a uniform distribution to select a contactpool in the selected school
            auto cp_generator   = rn_manager->GetGenerator(trng::fast_discrete_dist(school->pools.size()));
            auto pool           = school->pools.at(cp_generator());
            person.setPoolId(ContactPoolType::Id::School, pool->GetId());
            pool->AddMember(&person);
        }
    }
}

} // assigner
} // popgen
} // namespace gen
} // namespace stride

