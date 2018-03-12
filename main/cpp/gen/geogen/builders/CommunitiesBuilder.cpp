#include "CommunitiesBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;

static shared_ptr<vector<Community>> build(const GeoConfiguration& config, shared_ptr<GeoGrid> grid)
{
        auto communities = make_shared<vector<Community>>();
        // Every community has an average of 2000 members.
        unsigned int community_count = grid->getTotalPopulation() / 2000;

        // Create the discrete distribution to sample from.
        vector<double> fractions;
        for(UrbanCenter center : *grid) {
                fractions.push_back(double(center.population) / double(grid->getTotalPopulation()));
        }

        // The generator allows for parallelization.
        auto rn_manager = config.getRNManager();
        auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

        // Create and map the communities to their samples.
        for (unsigned int i = 0; i < community_count; i++) {
                // TODO: Currently only primary communities.
                communities->push_back(Community(true, grid->at(generator()).coordinate));
        }

        return communities;
}


} // namespace gen
} // namespace stride
