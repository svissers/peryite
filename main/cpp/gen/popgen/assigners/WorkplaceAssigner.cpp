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
using namespace util;

void AssignWorkplaces(
        vector<vector<shared_ptr<GenStruct>>> &workplaces, const shared_ptr<Population> population,
        shared_ptr<Region> region, const GeoGrid &grid, unsigned int total_commuting_students) {
    // -------------
    // Contactpools
    // -------------
    auto config = region->config;
    const auto wokrplace_size      = config.GetTree().get<unsigned int>("workplace_size");
    const auto workplace_cp_size   = config.GetTree().get<unsigned int>("workplace_cp_size");
    unsigned int cp_id = region->first_cps[ContactPoolType::Id::Work];
    for (auto &band : workplaces) {
        for (auto &g_struct : band) {
            auto workplace  = std::static_pointer_cast<WorkPlace>(g_struct);
            auto coord      = workplace->coordinate;
            //auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::Work, coord);
            for (unsigned int size = 0; size < wokrplace_size; size += workplace_cp_size) {
                auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::Work, coord);
                workplace->pools.push_back(pool);
                cp_id++;
            }
        }
    }
    region->last_cps[ContactPoolType::Id::Work] = cp_id-1;

    // -------------
    // Distributions
    // -------------

    auto rn_manager = config.GetRNManager();

    //auto total_population   = config.GetTree().get<unsigned int>("population_size");
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
    for(size_t i = region->first_person_id; i <= region->last_person_id; i++){
        if(population->at(i).GetAge() >=18 && population->at(i).GetAge() <= 65)
            working_age_people++;
    }

    auto total_commuting_actives    = uint(std::ceil(working_age_people * commute_fraction));
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
                commute_count       = uint(round(commuting_student_active_ratio * commute_count));
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
    for (unsigned int i = region->first_person_id; i <= region->last_person_id; i++) {
        auto &person = population->at(i);
        auto age = person.GetAge();
        if (person.GetPoolId(ContactPoolType::Id::School) != 0) {
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
                auto band_of_interest = uint( (dest_coord.get<1>() - grid.m_min_long) / grid.m_longitude_band_width );

                // if the city is not fragmented, use its coordinate
                if(!destination->is_fragmented){


                    for (const auto& gstruct : workplaces[band_of_interest]) {
                        auto workplace = std::static_pointer_cast<WorkPlace>(gstruct);

                        if (util::calculateDistance(workplace->coordinate, dest_coord) == 0) {
                            dest_workplaces.push_back(workplace);
                        }
                    }
                }
                else{
                    // if it is fragmente check the coordinates of its  fragments
                    for(auto fragment : destination->fragmented_coords){
                        band_of_interest = uint( (fragment.get<1>() - grid.m_min_long) / grid.m_longitude_band_width );


                        for (const auto& gstruct : workplaces[band_of_interest]) {
                            auto workplace = std::static_pointer_cast<WorkPlace>(gstruct);

                            if (util::calculateDistance(workplace->coordinate, fragment) == 0) {
                                dest_workplaces.push_back(workplace);
                            }
                        }
                    }

                }

                //if no workplaces are found in the city that the person would commute to or the city does not exist
                //find workplaces close to that city instead
                if (dest_workplaces.empty()) {
                    std::vector<shared_ptr<GenStruct>> dest_workplaces = GetClosestStructs(dest_coord,
                                                                                              workplaces,
                                                                                              grid);
                    //std::cout << "closest instead." << std::endl;
                    continue;
                }
                // Create a uniform distribution to select a workplace
                auto wp_generator   = rn_manager->GetGenerator(
                        trng::fast_discrete_dist(uint(dest_workplaces.size())));
                auto workplace      = dest_workplaces[wp_generator()];
                auto cp_generator = rn_manager->GetGenerator(
                        trng::fast_discrete_dist(uint(workplace->pools.size())));
                pool                = workplace->pools.at(uint(cp_generator()));
            } else {
                // Non-commuting
                auto home_coord = person.GetCoordinate();
                std::vector<shared_ptr<GenStruct>> closest_workplaces = GetClosestStructs(home_coord,
                                                                                          workplaces,
                                                                                          grid);
                if (closest_workplaces.empty()) {
                    continue;
                }
                // Create a uniform distribution to select a workplace
                std::function<int()> wp_generator = rn_manager->GetGenerator(
                        trng::fast_discrete_dist(uint(closest_workplaces.size())));
                auto workplace = static_pointer_cast<WorkPlace>(closest_workplaces.at(uint(wp_generator())));
                auto cp_generator = rn_manager->GetGenerator(
                        trng::fast_discrete_dist(uint(workplace->pools.size())));
                pool                = workplace->pools.at(uint(cp_generator()));
            }
            person.setPoolId(ContactPoolType::Id::Work, uint(pool->GetId()));
            pool->AddMember(&person);
        }
    }
}
} // assigner
} // popgen
} // namespace gen
} // namespace stride

