#include "PopulationBuilder.h"
#include "../../files/PopulationFile.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

using namespace std;
using namespace util;

void BuildPopulation(shared_ptr<Region> region, shared_ptr<Population>& population)
{
    auto config                 = region->config;
    auto hh_reference           = files::GetReferenceHouseholds(config);
    auto current_p_id           = region->first_person_id;
    auto pop_size = config.GetTree().get<unsigned int>("population_size");

    // Create a uniform distribution for the household reference set.
    auto rn_manager = config.GetRNManager();
    auto generator  = rn_manager->GetGenerator(trng::fast_discrete_dist(hh_reference.size()));

    // Build the households
    auto current_hh_id  = region->first_cps[ContactPoolType::Id::Household];
    while (current_p_id < pop_size) {
            // Select a household from the reference set
            int index           = generator();
            auto household_ref  = hh_reference.at(index);

            // Build the selected household
            vector<shared_ptr<Person>> hh_persons;
            for (unsigned int age : household_ref) {
                population->CreatePerson(
                    current_p_id++, age, current_hh_id,
                    0, 0, 0, 0, 0, 0, 0, 0, 0
                );
            }
            current_hh_id++;
    }
    region->last_person_id = current_p_id - 1;
    region->last_cps[ContactPoolType::Id::Household] = current_hh_id-1;
}

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
