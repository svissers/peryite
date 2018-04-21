#include "SchoolsBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;

vector<shared_ptr<School>> SchoolsBuilder::build(const GenConfiguration& config, GeoGrid& grid)
{
    auto schools = vector<shared_ptr<School>>();
    unsigned int total_population = config.getTree().get<unsigned int>("population_size");

    // We assume (overestimate) the fraction of mandatory students to be 25%.
    unsigned int mandatory_students_count = (unsigned int)(0.25 * total_population);

    // Every school has an average of 500 students.
    unsigned int school_count = mandatory_students_count / 500;

    // Create the discrete distribution to sample from.
    vector<double> fractions;
    for(auto& center : grid) {
            fractions.push_back(double(center->population) / double(total_population));
    }
    if (fractions.empty()) {
        return schools;
    }

    // The RNManager allows for parallelization.
    auto rn_manager = config.getRNManager();
    std::function<int()> generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Create and map the schools to their samples.
    for (unsigned int i = 0; i < school_count; i++) {
            int index = generator();
            auto school = make_shared<School>(School(i, grid.at(index)->coordinate));
            schools.push_back(school);
    }

    return schools;
}


} // namespace gen
} // namespace stride
