#include "UniversitiesBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "../../structs/UrbanCenter.h"
#include <algorithm>

namespace stride {
namespace gen {
namespace geogen {
namespace builder {

using namespace std;

vector<shared_ptr<University>> BuildUniversities(GenConfiguration& config, GeoGrid& grid)
{
    auto universities = vector<shared_ptr<University>>();
    unsigned int total_population = config.getTree().get<unsigned int>("population_size");

    // The total amount of university students is given.
    double universityFraction   = config.getTree().get<double>("university.student_fraction");
    unsigned int student_count  = (unsigned int)(universityFraction * total_population);

    // Every university has an average of 3000 students
    unsigned int uni_count = student_count / 3000;

    // Sort cities based on population size, biggest to smallest.
    auto compare_population = [](const shared_ptr<UrbanCenter> a, const shared_ptr<UrbanCenter> b) { return a->population > b->population; };
    sort(grid.begin(), grid.end(), compare_population);

    // Take the 10 biggest cities
    int city_count = min(10, (int)grid.size());
    auto biggest = grid.begin();
    auto smallest = grid.begin() + city_count;
    vector<shared_ptr<UrbanCenter>> big_cities(biggest, smallest);
    if (big_cities.empty())
        return universities;

    // The distribution will be relative to the top ten city population (not total).
    unsigned int total_city_population = 0;
    for(auto center : big_cities) {
            total_city_population += center->population;
    }

    // Create the discrete distribution to sample from.
    vector<double> fractions;
    for(auto center : big_cities) {
            fractions.push_back(center->population / total_city_population);
    }

    // The RNManager allows for parallelization.
    auto rn_manager = config.getRNManager();
    auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Create and map the universities to their samples.
    for (unsigned int i = 0; i < uni_count; i++) {
            auto index      = generator();
            auto center     = grid.at(index);
            auto university = make_shared<University>(University(i, index, center->coordinate));
            universities.push_back(university);
    }

    return universities;
}

} // namespace builder
} // namespace geogen
} // namespace gen
} // namespace stride
