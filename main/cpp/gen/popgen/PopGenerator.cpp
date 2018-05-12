#include "PopGenerator.h"
#include "assigners/HouseholdAssigner.h"
#include "assigners/SchoolAssigner.h"
#include "assigners/UniversityAssigner.h"
#include "assigners/WorkplaceAssigner.h"
#include "assigners/CommunityAssigner.h"
#include "util/GeoCoordCalculator.h"


namespace stride {
namespace gen {
namespace popgen {

using namespace std;
using namespace gen;

void Generate(files::GenDirectory& dir, shared_ptr<Population>& population, bool write)
{
    auto& pool_sys    = population->GetContactPoolSys();
    // --------------------------------------------------
    // Get the population and structs (by file or memory)
    // --------------------------------------------------
    auto config         = dir.GetConfig();
    population          = dir.GetPopulationFile()->Read(dir.GetBeliefConfig());
    auto grid           = dir.GetGeoGridFile()->ReadGrid();
    auto schools        = dir.GetSchoolFile()->Read();
    auto universities   = dir.GetUniversityFile()->Read();
    auto workplaces     = dir.GetWorkplaceFile()->Read();
    auto communities    = dir.GetCommunityFile()->Read();

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
    for (std::size_t i = 0; i < population->size(); ++i) {
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
    // Schools
    for (const auto& band : schools) {
        for (const auto& g_struct : band) {
            auto school = std::static_pointer_cast<School>(g_struct);
            for(const auto& pool : school->pools) {
                pool_sys[ContactPoolType::Id::School].emplace_back(*pool);
                cp_id++;
            }
        }
    }
    // Universities
    for (auto& band : universities) {
        for (auto& g_struct : band) {
            auto university = std::static_pointer_cast<University>(g_struct);
            // Create the contactpools for every university
            for(const auto& pool : university->pools) {
                pool_sys[ContactPoolType::Id::School].emplace_back(*pool);
                cp_id++;
            }
        }
    }
    // Workplaces
    for (auto& band : workplaces) {
        for (auto& g_struct : band) {
            auto workplace = std::static_pointer_cast<WorkPlace>(g_struct);
            pool_sys[ContactPoolType::Id::Work].emplace_back(*(workplace->pool));
            cp_id++;
        }
    }
    // Communities
    for (auto& band : communities) {
        for (auto& g_struct : band) {
            auto community  = std::static_pointer_cast<Community>(g_struct);
            auto com_id     = ContactPoolType::Id::PrimaryCommunity;
            if(!community->is_primary)
                com_id = ContactPoolType::Id::SecondaryCommunity;
            for(const auto& pool : community->pools) {
                pool_sys[com_id].emplace_back(*pool);
                cp_id++;
            }
        }
    }

    // -------------
    // Write persons
    // -------------
    if (write) {
        auto output_file = files::PopulationFile(
            config,
            population
        );
        output_file.Write();
    }
}

vector<shared_ptr<GenStruct>> GetClosestStructs(const util::spherical_point& home_coord, const vector<vector<shared_ptr<GenStruct>>>& structs, const GeoGrid& grid)
{
    vector<shared_ptr<GenStruct>> closest_structs;
    // The amount of bands doubles every iteration
    unsigned int band_range = 2;
    // The default search range is 10 km
    unsigned int search_range = 10;

    auto band_of_hh = uint( (home_coord.get<1>() - grid.m_min_long) / grid.m_longitude_band_width );
    if (band_of_hh >= structs.size()) {
        //std::cout << "home_coord: " << home_coord << std::endl;
        //std::cout << "grid.m_min_long: " << grid.m_min_long << std::endl;
        //std::cout << "grid.m_longitude_band_width: " << grid.m_longitude_band_width << std::endl;
        //std::cout << "band_of_hh: " << band_of_hh << " / " << structs.size() << std::endl;
        return closest_structs;
    }

    // Keep doubling search space until a struct is found
    while(closest_structs.empty()){
        // The first and last band define the search space
        unsigned int firstband = 0;
        unsigned int lastband = band_of_hh + band_range;
        if (band_of_hh > band_range)
            firstband = band_of_hh - band_range;
        if (lastband >= structs.size()) {
            lastband = structs.size() - 1;
            if (firstband == 0)
                break;
        }
        // Go over the search space
        for (unsigned int index = firstband; index <= lastband; index++) {
            for (const auto& gstruct : structs[index]) {
                if (util::calculateDistance(gstruct->coordinate, home_coord) <= search_range)
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
