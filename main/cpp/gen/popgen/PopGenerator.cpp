#include "PopGenerator.h"
#include "../files/HouseholdFile.h"
#include "trng/fast_discrete_dist.hpp"
#include "util/RNManager.h"
#include "pop/Person.h"
#include "../structs/School.h"

namespace stride {
namespace gen {

using namespace std;

void generate(files::GenDirectory& dir, unsigned int thread_count)
{
    auto config = dir.getConfig();

    auto households = buildHouseholds(config);

    auto geoGridFile    = dir.getGeoGridFile();
    auto grid           = geoGridFile->readGrid();

    assignHouseholds(households, grid, config);

    auto schoolsFile    = dir.getSchoolFile();
    auto schools     = schoolsFile->read();

    assignSchools(schools, households, config);
}

vector<shared_ptr<Household>> buildHouseholds(const GenConfiguration& config)
{
    auto hh_reference           = files::getHouseholds(config);
    unsigned int current_hh_id  = 0;
    unsigned int current_p_id   = 0;
    unsigned int pop_size       = config.getTree().get<unsigned int>("population_size");

    // Create a uniform distribution for the household reference set.
    auto rn_manager = config.getRNManager();
    std::function<int()> generator = rn_manager->GetGenerator(trng::fast_discrete_dist(hh_reference.size()));

    // Build the households
    vector<shared_ptr<Household>> result;
    while (current_p_id < pop_size) {
            // Select a household from the reference set
            int index           = generator();
            auto household_ref  = hh_reference.at(index);

            // Build the selected household
            auto household = make_shared<Household>(current_hh_id);
            vector<shared_ptr<Person>> hh_persons;
            for (unsigned int age : household_ref) {
                auto person = make_shared<Person>(
                    current_p_id, age, current_hh_id,
                    0, 0, 0, 0, 0, 0, 0, 0
                );
                hh_persons.push_back(person);
                current_p_id++;
            }
            result.push_back(household);
            current_hh_id++;
    }
    return result;
}

void assignHouseholds (vector<shared_ptr<Household>>& households, const GeoGrid& grid, const GenConfiguration& config)
{
    unsigned int total_population = config.getTree().get<unsigned int>("population_size");

    // Create the discrete distribution to sample from.
    vector<double> fractions;
    for(auto center : grid) {
        fractions.push_back(double(center->population) / double(total_population));
    }
    if (fractions.empty()) {
        return;
    }

    // The RNManager allows for parallelization.
    auto rn_manager = config.getRNManager();
    std::function<int()> generator = rn_manager->GetGenerator(trng::fast_discrete_dist(fractions.begin(), fractions.end()));

    // Map the households to their samples.
    for (auto household : households) {
            int index = generator();
            household->coordinate = grid.at(index)->coordinate;
    }
}


void assignSchools(vector<vector<shared_ptr<GenStruct>>>& schools, const vector<shared_ptr<Household>>& households, const GenConfiguration& config)
{
    const unsigned int school_size      = 500;
    const unsigned int school_cp_size   = 20;
    // Create the contactpools for every school
    unsigned int cp_id = 0;
    for (auto& band : schools) {
        for (auto& g_struct : band) {
            auto school = std::static_pointer_cast<School>(g_struct);
            for(unsigned int size = 0; size < school_size; size += school_cp_size) {
                auto pool = make_shared<ContactPool>(cp_id, ContactPoolType::Id::Household, ContactProfiles());
                school->pools.push_back(pool);
            }
        }
    }

    // Assign young students to schools
    for (auto household : households) {
        for (auto person : household->persons) {
            auto age = person->GetAge();
            if (age > 3 && age < 18) {
                auto home_coord = household->coordinate;
                // TODO
                // Find the bands within 10 km of home
                // Keep doubling until found

                std::vector<shared_ptr<School>> closest_schools;
                // Create a uniform distribution to select a school
                auto rn_manager = config.getRNManager();
                std::function<int()> school_generator = rn_manager->GetGenerator(trng::fast_discrete_dist(closest_schools.size()));
                auto school = closest_schools.at(school_generator());
                // Create a uniform distribution to select a contactpool in the school
                std::function<int()> cp_generator = rn_manager->GetGenerator(trng::fast_discrete_dist(school->pools.size()));
                auto pool = school->pools.at(cp_generator());
                pool->AddMember(person.get());
            }
        }
    }


}


} // namespace gen
} // namespace stride
