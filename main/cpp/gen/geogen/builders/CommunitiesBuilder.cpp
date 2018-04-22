#include "CommunitiesBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;

vector<shared_ptr<Community>> CommunitiesBuilder::Build(const GenConfiguration& config, GeoGrid& grid)
{
    vector<shared_ptr<Community>> communities = vector<shared_ptr<Community>>();
    unsigned int total_population = config.getTree().get<unsigned int>("population_size");
    // Every community has an average of 2000 members.
    unsigned int community_count = total_population / 2000;

    // Create the discrete distribution to sample from.
    vector<double> fractions;
    for(auto center : grid) {
            fractions.push_back(double(center->population) / double(total_population));
    }

    // The RNManager allows for parallelization.
    auto rn_manager = config.getRNManager();
    auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Create and map the communities to their samples.
    for (unsigned int i = 0; i < community_count; i++) {
            auto randomval = generator();
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
        auto community           = make_shared<Community>(Community(i, true, coords));
        auto community_secondary = make_shared<Community>(Community(i+community_count, false, coords));
        communities.push_back(community);
        communities.push_back(community_secondary);
    }

    return communities;
}





} // namespace gen
} // namespace stride
