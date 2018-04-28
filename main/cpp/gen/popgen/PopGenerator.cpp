#include "PopGenerator.h"
#include "../files/PopulationFile.h"
#include "assigners/householdAssigner.h"
#include "assigners/schoolAssigner.h"
#include "assigners/universityAssigner.h"
#include "assigners/workplaceAssigner.h"
#include "assigners/communityAssigner.h"
#include "pool/ContactPoolSys.h"
#include "pop/Population.h"
#include "util/GeoCoordCalculator.h"
#include "trng/fast_discrete_dist.hpp"
#include <map>


namespace stride {
namespace gen {
namespace popgen {

using namespace std;
using namespace gen;

void Generate(files::GenDirectory& dir, shared_ptr<Population>& population, ContactPoolSys& pool_sys, bool write)
{
    // --------------------------------------------------
    // Get the population and structs (by file or memory)
    // --------------------------------------------------
    auto config         = dir.getConfig();
    population          = dir.GetPopulationFile()->Read();
    auto grid           = dir.getGeoGridFile()->readGrid();
    auto schools        = dir.getSchoolFile()->read();
    auto universities   = dir.getUniversityFile()->read();
    auto workplaces     = dir.getWorkplaceFile()->read();
    auto communities    = dir.getCommunityFile()->read();

    // -------------------
    // Assign ContactPools
    // -------------------
    std::cout << "Assign households to coordinates" << std::endl;
    assigner::AssignHouseholds(population, grid, config);
    std::cout << "Assigning schools" << std::endl;
    assigner::AssignSchools(schools, population, config, grid);
    std::cout << "Assigning universities" << std::endl;
    unsigned int total_commuting_students = assigner::AssignUniversities(universities, population, config, grid);
    std::cout << "Assigning workplaces" << std::endl;
    assigner::AssignWorkplaces(workplaces, population, config, grid, total_commuting_students);
    std::cout << "Assigning communities" << std::endl;
    assigner::AssignCommunities(communities, population, config, grid);

    // -------------------
    // Fill ContactPoolSys
    // -------------------
    unsigned int cp_id = 1;
    // Households
    for (std::size_t i = 0; i != population->size(); ++i) {
        auto& person    = population->at(i);
        auto hh_id  = person.GetPoolId(ContactPoolType::Id::Household);
        auto pool   = ContactPool(cp_id, ContactPoolType::Id::Household);
        pool_sys[ContactPoolType::Id::Household].emplace_back(pool);
        while ( person.GetPoolId(ContactPoolType::Id::Household) == hh_id ) {
            if (++i >= population->size())
                break;
        }
        cp_id++;
    }


    // -------------
    // Write persons
    // -------------
    if (write) {
        auto output_file = make_shared<files::PopulationFile>(
            config,
            population
        );
        output_file->Write();
    }
}

vector<shared_ptr<GenStruct>> GetClosestStructs(const util::GeoCoordinate& home_coord, const vector<vector<shared_ptr<GenStruct>>>& structs, const GeoGrid& grid)
{
    vector<shared_ptr<GenStruct>> closest_structs;
    const util::GeoCoordCalculator& calculator = util::GeoCoordCalculator::getInstance();
    // The amount of bands doubles every iteration
    unsigned int band_range = 2;
    // The default search range is 10 km
    unsigned int search_range = 10;
    auto band_of_hh = uint( (home_coord.m_longitude - grid.m_min_long) / grid.m_longitude_band_width );
    // Keep doubling search space until a struct is found
    while(closest_structs.empty()){
        // The first and last band define the search space
        unsigned int firstband = 0;
        unsigned int lastband = band_of_hh + band_range;
        if (band_of_hh > band_range)
            firstband = band_of_hh - band_range;
        if (lastband >= structs.size())
            lastband = structs.size() - 1;
        // Go over the search space
        for (unsigned int index = firstband; index <= lastband; index++) {
            for (const auto& gstruct : structs[index]) {
                if (calculator.getDistance(gstruct->coordinate, home_coord) <= search_range)
                    closest_structs.push_back(gstruct);
            }
        }
        search_range = search_range*2;
        band_range = band_range*2;
    }
    return closest_structs;
}

} // namespace popgen
} // namespace gen
} // namespace stride
