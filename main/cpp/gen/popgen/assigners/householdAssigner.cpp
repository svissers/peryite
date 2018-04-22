#include "householdAssigner.h"
#include "../../files/HouseholdFile.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace popgen {
namespace hh_assign {

using namespace std;
using namespace gen;

vector<shared_ptr<Household>> BuildHouseholds(const GenConfiguration& config)
{
    auto hh_reference           = files::getHouseholds(config);
    unsigned int current_hh_id  = 0;
    unsigned int current_p_id   = 0;
    auto pop_size       = config.getTree().get<unsigned int>("population_size");

    // Create a uniform distribution for the household reference set.
    auto rn_manager = config.getRNManager();
    auto generator  = rn_manager->GetGenerator(trng::fast_discrete_dist(hh_reference.size()));

    // Build the households
    vector<shared_ptr<Household>> result;
    while (current_p_id < pop_size) {
            // Select a household from the reference set
            int index           = generator();
            auto household_ref  = hh_reference.at(index);

            // Build the selected household
            auto household = make_shared<Household>(current_hh_id);
            vector<shared_ptr<Person>> hh_persons;
            for (unsigned int age : household_ref) {
                auto person = make_shared<Person>(
                    current_p_id, age, current_hh_id,
                    0, 0, 0, 0
                );
                hh_persons.push_back(person);
                current_p_id++;
            }
            household->persons = hh_persons;
            result.push_back(household);
            current_hh_id++;
    }
    return result;
}

void AssignHouseholds (
    vector<shared_ptr<Household>>& households, const GeoGrid& grid, const GenConfiguration& config)
{
    auto total_population = config.getTree().get<unsigned int>("population_size");

    // Create the discrete distribution to sample from.
    vector<double> fractions;
    for(const auto& center : grid) {
        fractions.push_back(double(center->population) / double(total_population));
    }
    if (fractions.empty()) {
        return;
    }

    // The RNManager allows for parallelization.
    auto rn_manager = config.getRNManager();
    std::function<int()> generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Map the households to their samples.
    for (const auto& household : households) {
        auto center = grid.at(generator());
        auto coords = center->coordinate;
        if (center->is_fragmented) {
            // Select one of the fragments
            vector<double> f_fractions;
            for(const auto& population : center->fragmented_populations)
                f_fractions.push_back(double(population) / double(center->population));
            auto frag_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(f_fractions.begin(), f_fractions.end()));
            coords = center->fragmented_coords.at(frag_gen());
        }
        household->coordinate = coords;
    }
}

} // hh_assign
} // popgen
} // namespace gen
} // namespace stride

