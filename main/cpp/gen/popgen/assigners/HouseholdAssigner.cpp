#include "HouseholdAssigner.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

using namespace std;
using namespace gen;
using namespace util;

void AssignHouseholds(
        shared_ptr<Population> population, const GeoGrid &grid, shared_ptr<Region> region) {
    auto config             = region->config;
    auto total_population   = config.GetTree().get<unsigned int>("population_size");

    // Create the discrete distribution to sample from.
    vector<double> fractions;
    for (const auto &center : grid) {
        fractions.push_back(double(center->population) / double(total_population));
    }
    if (fractions.empty())
        return;

    // The RNManager allows for parallelization.
    auto rn_manager = config.GetRNManager();
    std::function<int()> generator = rn_manager->GetGenerator(
            trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Map the households to their samples.
    for (std::size_t i = region->first_person_id; i <= region->last_person_id; i++) {
        auto hh_id = population->at(i).GetPoolId(ContactPoolType::Id::Household);
        auto index = grid.at(generator());
        auto center = index;
        auto coord = center->coordinate;
        if (coord.get<0>() == 0) {
            std::cout << "lat is 0 " << coord << std::endl;
            std::cout << "index: " << index << std::endl;
            std::cout << "hh_id: " << hh_id << std::endl;
        }
        if (center->is_fragmented) {
            // Select one of the fragments
            vector<double> f_fractions;
            for (const auto &f_pop_size : center->fragmented_populations)
                f_fractions.push_back(double(f_pop_size) / double(center->population));
            auto frag_gen = rn_manager->GetGenerator(
                    trng::fast_discrete_dist(f_fractions.begin(), f_fractions.end()));
            coord = center->fragmented_coords.at(frag_gen());
        }
        while (population->at(i).GetPoolId(ContactPoolType::Id::Household) == hh_id) {
            population->at(i).SetCoordinate(coord);
            if (++i >= population->size())
                break;
        }
        i--;
    }
}

} // assigner
} // popgen
} // namespace gen
} // namespace stride

