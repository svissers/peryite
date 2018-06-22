#include "PopGenerator.h"
#include "assigners/HouseholdAssigner.h"
#include "assigners/SchoolAssigner.h"
#include "assigners/UniversityAssigner.h"
#include "assigners/WorkplaceAssigner.h"
#include "assigners/CommunityAssigner.h"
#include "assigners/TravelAssigner.h"

namespace stride {
namespace gen {
namespace popgen {

using namespace std;
using namespace gen;
using namespace util;

void Generate(files::GenDirectory& dir, shared_ptr<Population>& population)
{
    auto& pool_sys = population->GetContactPoolSys();
    dir.GetPopulationFile()->Read(population);
    population->SetRegions(dir.GetRegions());

    // Assign contactpools per region.
    shared_ptr<Region> prev_region = nullptr;
    for(auto & region : dir.GetRegions()) {
        if (prev_region) {
            for(auto typ : ContactPoolType::IdList)
                region->first_cps[typ] = prev_region->last_cps[typ] + 1;
        }

        auto config = region->config;
        // --------------------------------------------------
        // Get the population and structs (by file or memory)
        // --------------------------------------------------
        auto grid           = dir.GetGeoGridFile(region->id)->ReadGrid();
        auto schools        = dir.GetSchoolFile(region->id)->Read();
        auto universities   = dir.GetUniversityFile(region->id)->Read();
        auto workplaces     = dir.GetWorkplaceFile(region->id)->Read();
        auto communities    = dir.GetCommunityFile(region->id)->Read();

        // -------------------
        // Assign ContactPools
        // -------------------
        std::cout << "assigning population in region: " << region->name << std::endl;
        std::cout << "  working: " <<std::flush;

        assigner::AssignHouseholds(population, grid, region);
        std::cout << "-"<< std::flush;

        assigner::AssignSchools(schools, population, region, grid);
        std::cout << "-"<< std::flush;

        auto total_commuting_students = assigner::AssignUniversities(universities, population, region, grid);
        std::cout << "-"<< std::flush;

        assigner::AssignWorkplaces(workplaces, population, region, grid, total_commuting_students);
        std::cout << "--"<< std::flush;

        assigner::AssignCommunities(communities, population, region, grid);
        std::cout << "-"<< std::flush;

        // -------------------
        // Fill ContactPoolSys
        // -------------------
        // Households
        unsigned int hh_id = 0;
        for (auto i = region->first_person_id; i <= region->last_person_id; ++i) {
            auto &person    = population->at(i);
            hh_id           = person.GetPoolId(ContactPoolType::Id::Household);
            auto coord      = person.GetCoordinate();
            auto pool       = ContactPool(hh_id, ContactPoolType::Id::Household, coord);
            pool_sys[ContactPoolType::Id::Household].emplace_back(pool);
            while (true) {
                auto j = i + 1;
                if (j > region->last_person_id)
                    break;
                if (population->at(j).GetPoolId(ContactPoolType::Id::Household) != hh_id)
                    break;
                i = j;
            }
        }
        region->last_cps[ContactPoolType::Id::Household] = hh_id;
        std::cout << "-"<< std::flush;

        // Schools
        for (const auto &band : schools) {
            for (const auto &g_struct : band) {
                auto school = std::static_pointer_cast<School>(g_struct);
                for (const auto &pool : school->pools) {
                    pool_sys[ContactPoolType::Id::School].emplace_back(*pool);
                }
            }
        }
        std::cout << "-"<< std::flush;

        // Universities
        for (auto &band : universities) {
            for (auto &g_struct : band) {
                auto university = std::static_pointer_cast<University>(g_struct);
                // Create the contactpools for every university
                for (const auto &pool : university->pools) {
                    pool_sys[ContactPoolType::Id::School].emplace_back(*pool);
                }
            }
        }
        std::cout << "-"<< std::flush;

        // Workplaces
        for (auto &band : workplaces) {
            for (auto &g_struct : band) {
                auto workplace = std::static_pointer_cast<WorkPlace>(g_struct);
                for (const auto &pool : workplace->pools) {
                    pool_sys[ContactPoolType::Id::Work].emplace_back(*pool);
                }
            }
        }
        std::cout << "-"<< std::flush;

        // Communities
        for (auto &band : communities) {
            for (auto &g_struct : band) {
                auto community  = std::static_pointer_cast<Community>(g_struct);
                auto com_id     = ContactPoolType::Id::PrimaryCommunity;
                if (!community->is_primary)
                    com_id = ContactPoolType::Id::SecondaryCommunity;
                for (const auto &pool : community->pools) {
                    pool_sys[com_id].emplace_back(*pool);
                }
            }
        }
        std::cout << "-"<< std::flush;

        prev_region = region;
        std::cout << " finished." << std::endl;

    }

    if(dir.GetRegions().size() > 1) {
        std::cout << "assigning inter regio travellers... "<< std::flush;

        assigner::AssignTravellers(population, dir);
        std::cout << "finished." << std::endl;
    }

    // ---------------------------------------
    // Write personsFile combining all regions
    // ---------------------------------------
    auto write = dir.GetConfig().GetTree().get<bool>("write_population");
    if (write) {
        std::cout<<"writing population... " << std::flush;

        auto output_file = files::PopulationFile(
                dir.GetConfig(),
                population
        );
        output_file.Write();
        output_file.WriteRegions(dir.GetConfig().GetOutputPrefix(), dir.GetRegions());
        output_file.WritePoolSys(dir.GetConfig().GetOutputPrefix(), pool_sys);
        std::cout << "finished." << std::endl;
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
            lastband = uint(structs.size() - 1);
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
