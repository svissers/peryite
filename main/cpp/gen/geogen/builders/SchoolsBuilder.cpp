#include "SchoolsBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

using namespace std;
using namespace util;
using namespace trng;

vector<shared_ptr<School>> BuildSchools(const GenConfiguration& config, GeoGrid& grid)
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
    auto generator  = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Create and map the schools to their samples.
    for (unsigned int i = 0; i < school_count; i++) {
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
        auto school = make_shared<School>(School(i, coords));
        schools.push_back(school);
    }

    return schools;
}

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
