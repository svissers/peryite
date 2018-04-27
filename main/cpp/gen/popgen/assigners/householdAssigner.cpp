#include "householdAssigner.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace popgen {
namespace assigner {

using namespace std;
using namespace gen;

void AssignHouseholds (
    vector<shared_ptr<Household>>& households, const GeoGrid& grid, const GenConfiguration& config)
{
    auto total_population = config.GetTree().get<unsigned int>("population_size");

    // Create the discrete distribution to sample from.
    vector<double> fractions;
    for(const auto& center : grid) {
        fractions.push_back(double(center->population) / double(total_population));
    }
    if (fractions.empty()) {
        return;
    }

    // The RNManager allows for parallelization.
    auto rn_manager = config.GetRNManager();
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

} // assigner
} // popgen
} // namespace gen
} // namespace stride

