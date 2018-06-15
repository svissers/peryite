#include "PopulationBuilder.h"
#include "../../files/PopulationFile.h"
#include "trng/fast_discrete_dist.hpp"
#include <omp.h>
#include <iostream>

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
    // auto end_of_while = current_p_id+pop_size;
    unsigned int toGenerate = pop_size/omp_get_num_threads();
    // Create a uniform distribution for the household reference set.
    auto rn_manager = config.GetRNManager();
    auto generator  = rn_manager->GetGenerator(trng::fast_discrete_dist(hh_reference.size()));

    // Build the households
    auto current_hh_id  = region->first_cps[ContactPoolType::Id::Household];
    std::cout << omp_get_num_threads() << std::endl;
#pragma omp parallel for shared(generator, hh_reference, current_hh_id, current_p_id, toGenerate, population, cout) default(none)
    for (unsigned int i = 0; i < omp_get_num_threads(); ++i){
        std::cout << omp_get_num_threads() << std::endl;
        unsigned int generated = 0;
        while (generated < toGenerate) {
                // Select a household from the reference set
                // #pragma omp atomic
                int index           = generator();
                auto household_ref  = hh_reference.at(index);

                // Build the selected household
                // vector<shared_ptr<Person>> hh_persons;
                for (unsigned int age : household_ref) {
                    #pragma omp critical
                    population->CreatePerson(
                        current_p_id++, age, current_hh_id,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, -1, -1
                    );
                    // ++current_p_id;
                    ++generated;
                }
                #pragma omp atomic
                current_hh_id++;
        }
    }
    region->last_person_id = current_p_id - 1;
    region->last_cps[ContactPoolType::Id::Household] = current_hh_id-1;
}

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
