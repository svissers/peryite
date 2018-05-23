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

unsigned int AssignSchools(
        vector<vector<shared_ptr<GenStruct>>> &schools, const shared_ptr<Population> population,
        const GenConfiguration &config, const GeoGrid &grid, unsigned int start_cp_id, unsigned int first_person_id, unsigned int next_person_id) {
    const unsigned int school_size = config.GetTree().get<unsigned int>("school_size");
    const unsigned int school_cp_size = config.GetTree().get<unsigned int>("school_cp_size");
    // Create the contactpools for every school
    unsigned int cp_id = start_cp_id;
    for (const auto &band : schools) {
        for (const auto &g_struct : band) {
            auto school = std::static_pointer_cast<School>(g_struct);
            for (unsigned int size = 0; size < school_size; size += school_cp_size) {
                auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::School);
                school->pools.push_back(pool);
                cp_id++;
            }
        }
    }

    // Assign young students to schools
    for (unsigned int i = first_person_id; i < next_person_id; i++/*auto &person : *population*/) {

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
                    trng::fast_discrete_dist(closest_schools.size()));
            auto school = static_pointer_cast<School>(closest_schools.at(school_generator()));
            // Create a uniform distribution to select a contactpool in the selected school
            auto cp_generator = rn_manager->GetGenerator(
                    trng::fast_discrete_dist(school->pools.size()));
            auto pool = school->pools.at(cp_generator());
            person.setPoolId(ContactPoolType::Id::School, pool->GetId());
            pool->AddMember(&person);
        }
    }
    return cp_id;
}

} // assigner
} // popgen
} // namespace gen
} // namespace stride

