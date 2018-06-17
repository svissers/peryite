#include "SchoolsBuilder.h"
#include "trng/fast_discrete_dist.hpp"

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

using namespace std;
using namespace util;
using namespace trng;

vector<shared_ptr<School>> BuildSchools(const GenConfiguration& config, GeoGrid& grid, std::shared_ptr<Population> pop, unsigned int firstPerson)
{
    auto schools = vector<shared_ptr<School>>();
    auto total_population = config.GetTree().get<unsigned int>("population_size");

    // We check the amount of mandatory students in the population
    unsigned int mandatory_students_count = 0;
    for(size_t i = firstPerson; i < pop->size(); i++){
        if(pop->at(i).GetAge() >=3 && pop->at(i).GetAge() <= 18)
            mandatory_students_count++;
    }

    // Every school has the same average amount of students.
    auto school_size            =  config.GetTree().get<unsigned int>("school_size");
    unsigned int school_count   = mandatory_students_count / school_size;

    // Create the discrete distribution to sample from.
    vector<double> fractions;
    for(auto& center : grid) {
            fractions.push_back(double(center->population) / double(total_population));
    }
    if (fractions.empty()) {
        return schools;
    }

    // The RNManager allows for parallelization.
    auto rn_manager = config.GetRNManager();
    auto generator  = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Create and map the schools to their samples.
    for (unsigned int i = 0; i < school_count; i++) {
        auto center = grid.at(uint(generator()));
        auto coords = center->coordinate;
        if (center->is_fragmented) {
            // Select one of the fragments
            vector<double> f_fractions;
            for(const auto& population : center->fragmented_populations)
                f_fractions.push_back(double(population) / double(center->population));
            auto frag_gen = rn_manager->GetGenerator(trng::fast_discrete_dist(f_fractions.begin(), f_fractions.end()));
            coords = center->fragmented_coords.at(uint(frag_gen()));
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
