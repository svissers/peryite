#include "CommunitiesBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;

shared_ptr<vector<Community>> CommunitiesBuilder::build(const GeoConfiguration& config, shared_ptr<GeoGrid> grid)
{
        shared_ptr<vector<Community>> communities = make_shared<vector<Community>>();
        unsigned int total_population = config.getTree().get<unsigned int>("population_size");
        // Every community has an average of 2000 members.
        unsigned int community_count = total_population / 2000;

        // Create the discrete distribution to sample from.
        vector<double> fractions;
        for(UrbanCenter center : *grid) {
                fractions.push_back(double(center.population) / double(total_population));
        }

        // The generator allows for parallelization.
        auto rn_manager = config.getRNManager();
        auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

        // Create and map the communities to their samples.
        for (unsigned int i = 0; i < community_count; i++) {
                // TODO: Currently only primary communities.
                communities->push_back(Community(i, true, grid->at(generator()).coordinate));
        }

        return communities;
}





} // namespace gen
} // namespace stride
