#include "SchoolsBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;

static shared_ptr<vector<School>> build(const GeoConfiguration& config, shared_ptr<GeoGrid> grid)
{
        auto schools = make_shared<vector<School>>();
        // We assume (overestimate) the fraction of mandatory students to be 25%.
        unsigned int mandatory_students_count = (unsigned int)(0.25 * grid->getTotalPopulation());

        // Every school has an average of 500 students.
        unsigned int school_count = mandatory_students_count / 500;

        // Create the discrete distribution to sample from.
        vector<double> fractions;
        for(UrbanCenter center : *grid) {
                fractions.push_back(double(center.population) / double(grid->getTotalPopulation()));
        }

        // The generator allows for parallelization.
        auto rn_manager = config.getRNManager();
        auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

        // Create and map the schools to their samples.
        for (unsigned int i = 0; i < school_count; i++) {
                schools->push_back(School(grid->at(generator()).coordinate));
        }

        return schools;
}


} // namespace gen
} // namespace stride
