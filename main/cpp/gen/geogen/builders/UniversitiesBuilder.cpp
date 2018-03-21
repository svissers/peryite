#include "UniversitiesBuilder.h"
#include "trng/fast_discrete_dist.hpp"
#include "../../structs/UrbanCenter.h"
#include <algorithm>

namespace stride {
namespace gen {

using namespace std;

std::shared_ptr<vector<University>> UniversitiesBuilder::build(GenConfiguration& config, shared_ptr<GeoGrid> grid)
{
        auto universities = make_shared<vector<University>>();
        unsigned int total_population = config.getTree().get<unsigned int>("population_size");

        // The total amount of university students is given.
        double universityFraction = config.getTree().get<double>("university.fraction");
        unsigned int student_count = (unsigned int)(universityFraction * total_population);

        // Every university has an average of 3000 students
        unsigned int uni_count = student_count / 3000;

        // Sort cities based on population size, biggest to smallest.
        auto compare_population = [](const UrbanCenter& a, const UrbanCenter b) { return a.population > b.population; };
        sort(grid->begin(), grid->end(), compare_population);

        // Take the 10 biggest cities
        int city_count = min(10, (int)grid->size());
        auto biggest = grid->begin();
        auto smallest = grid->begin() + city_count;
        vector<UrbanCenter> big_cities(biggest, smallest);

        // The distribution will be relative to the top ten city population (not total).
        unsigned int total_city_population = 0;
        for(UrbanCenter center : big_cities) {
                total_city_population += center.population;
        }

        // Create the discrete distribution to sample from.
        vector<double> fractions;
        for(UrbanCenter center : big_cities){
                fractions.push_back(center.population/total_city_population);
        }

        // The generator allows for parallelization.
        auto rn_manager = config.getRNManager();
        auto generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

        // Create and map the universities to their samples.
        for (unsigned int i = 0; i < uni_count; i++) {
                universities->push_back(University(i, grid->at(generator()).coordinate));
        }

        return universities;
}


} // namespace gen
} // namespace stride
