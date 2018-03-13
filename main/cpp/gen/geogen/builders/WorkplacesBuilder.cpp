#include "WorkplacesBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"
#include "util/CSV.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;

shared_ptr<vector<WorkPlace>> build(GeoConfiguration& config, shared_ptr<GeoGrid> grid)
{
        auto workplaces = make_shared<vector<WorkPlace>>();

        // Commuter data
        // currently we assume there are about as many commutors leaving as there are coming in, so it balances out
        // later we'll specify the commutors into a commuting.csv

        // Work fraction (Probably rename to commuteFraction?)
        unsigned int workforce = (unsigned int) config.getWorkFraction()*grid->getTotalPopulation();

        //every workplace has 20 workers
        unsigned int work_count =  workforce/20;
        // Create the discrete distribution to sample from.
        vector<double> fractions;
        for(UrbanCenter center : *grid) {
                fractions.push_back(double(center.population) / double(grid->getTotalPopulation()));
        }

        // The generator allows for parallelization.
        auto rn_manager = config.getRNManager();
        auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

        // Create and map the workplaces to their samples.
        for (unsigned int i = 0; i < work_count; i++) {
                workplaces->push_back(WorkPlace(grid->at(generator()).coordinate));
        }

        return workplaces;
}


} // namespace gen
} // namespace stride
