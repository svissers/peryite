#include "WorkplaceAssigner.h"
#include "../../structs/Workplace.h"
#include "../PopGenerator.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

using namespace std;
using namespace gen;

void AssignWorkplaces(
        vector<vector<shared_ptr<GenStruct>>> &workplaces, const shared_ptr<Population> population,
        const GenConfiguration &config, const GeoGrid &grid, unsigned int total_commuting_students) {
    // -------------
    // Contactpools
    // -------------
    unsigned int cp_id = 0;
    for (auto &band : workplaces) {
        for (auto &g_struct : band) {
            auto workplace = std::static_pointer_cast<WorkPlace>(g_struct);
            auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::Work);
            workplace->pool = pool;
            cp_id++;
        }
    }
    // -------------
    // Distributions
    // -------------
    auto rn_manager = config.GetRNManager();

    auto total_population   = config.GetTree().get<unsigned int>("population_size");
    auto student_fraction   = config.GetTree().get<double>("university.student_fraction");
    auto work_fraction      = config.GetTree().get<double>("work.work_fraction");
    auto commute_fraction   = config.GetTree().get<double>("work.commute_fraction");
    auto student_fractions  = vector<double>{student_fraction, 1.0 - student_fraction};
    auto work_fractions     = vector<double>{work_fraction, 1.0 - work_fraction};
    auto commute_fractions  = vector<double>{commute_fraction, 1.0 - commute_fraction};
    auto student_gen        = rn_manager->GetGenerator(
            trng::fast_discrete_dist(student_fractions.begin(), student_fractions.end()));
    auto work_gen           = rn_manager->GetGenerator(
            trng::fast_discrete_dist(work_fractions.begin(), work_fractions.end()));
    auto commute_gen        = rn_manager->GetGenerator(
            trng::fast_discrete_dist(commute_fractions.begin(), commute_fractions.end()));

    // Commuting distributions
    unsigned int working_age_people = 0;
    for(size_t i; i < population->size(); i++){
        if(population->at(i).GetAge() >=18 && population->at(i).GetAge() <= 65)
            working_age_people++;
    }
    unsigned int total_commuting_actives    = working_age_people * commute_fraction;
    double commuting_student_active_ratio   = total_commuting_students / total_commuting_actives;

    util::CSV commuting_data    = util::CSV(config.GetTree().get<string>("geoprofile.commuters"));
    size_t column_count         = commuting_data.GetColumnCount();
    vector<int> relative_commute(column_count, 0);
    vector<unsigned int> total_commute(column_count, 0);
    if (commuting_data.size() > 1) {
        // Access each element in the matrix
        for (size_t row_index = 0; row_index < commuting_data.size(); row_index++) {
            for (size_t col_index = 0; col_index < column_count; col_index++) {
                // Ignore commuting towards itself
                if (row_index == col_index)
                    continue;
                util::CSVRow row    = *(commuting_data.begin() + row_index);
                auto commute_count  = row.GetValue<unsigned int>(col_index);
                // Remove commuting students
                commute_count       = (commuting_student_active_ratio * commute_count);
                // TODO: ask
                relative_commute[col_index] -= commute_count;
                relative_commute[row_index] += commute_count;
                total_commute[col_index]    += commute_count;
            }
        }
    }

    // Create a distribution to select a workplace city.
    vector<double> wpc_fractions;
    for (size_t i = 0; i < relative_commute.size(); i++)
        wpc_fractions.push_back(double(relative_commute[i]) / double(total_commute[i]));
    auto city_gen = rn_manager->GetGenerator(
            trng::fast_discrete_dist(wpc_fractions.begin(), wpc_fractions.end()));

    //if (fractions.empty()) {}

    // --------------------------------
    // Assign employables to workplaces.
    // --------------------------------
    for (auto &person : *population) {
        auto age = person.GetAge();
        if (age >= 18 && age < 26 && person.GetPoolId(ContactPoolType::Id::School) != 0) {
            // Students are not employable
            continue;
        }
        if (age >= 18 && age < 65) {
            if (work_gen() == 1) {
                // The person is non active
                person.setPoolId(ContactPoolType::Id::Work, 0);
                continue;
            }
            // The person is active
            shared_ptr<ContactPool> pool;
            if (commute_gen() == 0) {
                // Commuting
                auto destination = grid[city_gen()];
                auto dest_coord = destination->coordinate;
                vector<shared_ptr<WorkPlace>> dest_workplaces;
                for (auto &band : workplaces) {
                    for (auto &g_struct : band) {
                        auto workplace = std::static_pointer_cast<WorkPlace>(g_struct);
                        if (workplace->coordinate == dest_coord) {
                            dest_workplaces.push_back(workplace);
                        }
                    }
                }
                // TODO: ask
                if (dest_workplaces.empty())
                    continue;
                // Create a uniform distribution to select a workplace
                auto wp_generator   = rn_manager->GetGenerator(
                        trng::fast_discrete_dist(dest_workplaces.size()));
                auto workplace      = dest_workplaces[wp_generator()];
                pool                = workplace->pool;
            } else {
                // Non-commuting
                auto home_coord = person.GetCoordinate();
                std::vector<shared_ptr<GenStruct>> closest_workplaces = GetClosestStructs(home_coord,
                                                                                          workplaces,
                                                                                          grid);
                if (closest_workplaces.empty()) {
                    //std::cout << "closest_workplaces is empty: " << age << std::endl;
                    continue;
                }
                // Create a uniform distribution to select a workplace
                std::function<int()> wp_generator = rn_manager->GetGenerator(
                        trng::fast_discrete_dist(closest_workplaces.size()));
                auto workplace = static_pointer_cast<WorkPlace>(closest_workplaces.at(wp_generator()));
                pool = workplace->pool;
                //std::cout << "non commutor" <<std::endl;
            }
            //std::cout << "assigned workplace pool id: " <<pool->GetId() << std::endl;
            person.setPoolId(ContactPoolType::Id::Work, pool->GetId());
            pool->AddMember(&person);
        }
    }
}
} // assigner
} // popgen
} // namespace gen
} // namespace stride

