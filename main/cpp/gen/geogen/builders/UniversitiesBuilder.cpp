#include "UniversitiesBuilder.h"
#include <algorithm>

namespace stride {
namespace gen {

using namespace std;
using namespace util;
using namespace trng;

static std::shared_ptr<vector<University>> build(GeoConfiguration& config, shared_ptr<GeoGrid> grid)
{
        auto universities = make_shared<vector<University>>();
        unsigned int mandatory_students_count = (unsigned int)(config.getUniversityFraction() * grid->getTotalPopulation());

        // Every university has an average of 3000 students
        unsigned int school_count = mandatory_students_count / 3000;

        // Sort cities based on population size, biggest to smallest.
        auto compare_population = [](const UrbanCenter& a, const UrbanCenter b) { return a.population > b.population; };
        sort(grid->begin(), grid->end(), compare_population);

        // Take the 10 biggest cities
        int city_count = min(10, grid->size());
        auto biggest = grid->begin();
        auto smallest = grid->begin() + city_count;
        auto big_cities(biggest, smallest);
        /*
        unsigned int total_city_population = 0;
        for(UrbanCenter center : big_cities) {
                total_city_population += center.population;
        }
        */

        // TODO: Assignment is a bit vague here.

        return universities;
}


} // namespace gen
} // namespace stride
