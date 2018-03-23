#include "SchoolsBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;

shared_ptr<vector<School>> SchoolsBuilder::build(const GeoConfiguration& config, shared_ptr<GeoGrid> grid)
{
        auto schools = make_shared<vector<School>>();
        unsigned int total_population = config.getTree().get<unsigned int>("population_size");

        // We assume (overestimate) the fraction of mandatory students to be 25%.
        double percentage = config.getTree().get<double>("students.compulsory");
        unsigned int mandatory_students_count = (unsigned int)(percentage * total_population);

        // Every school has an average of 500 students.
        unsigned int school_count = mandatory_students_count / 500;

        // Create the discrete distribution to sample from.
        vector<double> fractions;
        for(UrbanCenter center : *grid) {
                fractions.push_back(double(center.population) / double(total_population));
        }
        if (fractions.empty()) {
            return schools;
        }

        // The generator allows for parallelization.
        auto rn_manager = config.getRNManager();
        std::function<int()> generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

        // Create and map the schools to their samples.
        for (unsigned int i = 0; i < school_count; i++) {
                int index = generator();
                schools->push_back(School(i, grid->at(index).coordinate));
        }

        return schools;
}


} // namespace gen
} // namespace stride
