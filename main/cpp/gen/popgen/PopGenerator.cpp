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

void Generate(files::GenDirectory& dir, shared_ptr<Population>& population)
{
    auto& pool_sys    = population->GetContactPoolSys();

    unsigned int next_cp_id_communities = 0;
    unsigned int next_cp_id_schools = 0;
    unsigned int next_cp_id_universities = 0;
    unsigned int next_cp_id_workplaces = 0;
    unsigned int amount_regions = dir.GetAmountOfRegions();

    unsigned int cp_id = 1;
    for(unsigned int current_region_nr = 0;current_region_nr < amount_regions; current_region_nr++) {
        int first_person_id = dir.GetFirstInRegion(current_region_nr);
        int next_first_person_id = dir.GetFirstInRegion(current_region_nr + 1);
        if(next_first_person_id == -1){
            next_first_person_id = population->size();
        }

        // --------------------------------------------------
        // Get the population and structs (by file or memory)
        // --------------------------------------------------
        auto config = dir.GetConfig()[current_region_nr];
        dir.GetPopulationFile(current_region_nr)[current_region_nr]->Read(population);
        auto grid = dir.GetGeoGridFile(current_region_nr)[current_region_nr]->ReadGrid();
        auto schools = dir.GetSchoolFile(current_region_nr)[current_region_nr]->Read();
        auto universities = dir.GetUniversityFile(current_region_nr)[current_region_nr]->Read();
        auto workplaces = dir.GetWorkplaceFile(current_region_nr)[current_region_nr]->Read();
        auto communities = dir.GetCommunityFile(current_region_nr)[current_region_nr]->Read();

        // -------------------
        // Assign ContactPools
        // -------------------
        assigner::AssignHouseholds(population, grid, config, first_person_id,next_first_person_id);
        next_cp_id_schools = assigner::AssignSchools(schools, population, config, grid, next_cp_id_schools,first_person_id,next_first_person_id);

        std::tuple<unsigned int, unsigned int> assignUniReturnVal = assigner::AssignUniversities(universities,
                                                                                                 population, config,
                                                                                                 grid,
                                                                                                 next_cp_id_universities,first_person_id,next_first_person_id);
        unsigned int total_commuting_students = std::get<0>(assignUniReturnVal);
        next_cp_id_universities = std::get<1>(assignUniReturnVal);

        next_cp_id_workplaces = assigner::AssignWorkplaces(workplaces, population, config, grid,
                                                           total_commuting_students, next_cp_id_workplaces,first_person_id,next_first_person_id);

        next_cp_id_communities = assigner::AssignCommunities(communities, population, config, grid,
                                                             next_cp_id_communities,first_person_id,next_first_person_id);

        // -------------------
        // Fill ContactPoolSys
        // -------------------
        // Households
        for (int i = first_person_id; i < next_first_person_id; ++i) {
            auto &person = population->at(i);
            auto hh_id = person.GetPoolId(ContactPoolType::Id::Household);
            auto pool = ContactPool(cp_id, ContactPoolType::Id::Household);
            pool_sys[ContactPoolType::Id::Household].emplace_back(pool);
            while (person.GetPoolId(ContactPoolType::Id::Household) == hh_id) {
                if (++i >= next_first_person_id)
                    break;
            }
            cp_id++;
        }
        // Schools
        for (const auto &band : schools) {
            for (const auto &g_struct : band) {
                auto school = std::static_pointer_cast<School>(g_struct);
                for (const auto &pool : school->pools) {
                    pool_sys[ContactPoolType::Id::School].emplace_back(*pool);
                    cp_id++;
                }
            }
        }
        // Universities
        for (auto &band : universities) {
            for (auto &g_struct : band) {
                auto university = std::static_pointer_cast<University>(g_struct);
                // Create the contactpools for every university
                for (const auto &pool : university->pools) {
                    pool_sys[ContactPoolType::Id::School].emplace_back(*pool);
                    cp_id++;
                }
            }
        }
        // Workplaces
        for (auto &band : workplaces) {
            for (auto &g_struct : band) {
                auto workplace = std::static_pointer_cast<WorkPlace>(g_struct);
                pool_sys[ContactPoolType::Id::Work].emplace_back(*(workplace->pool));
                cp_id++;
            }
        }
        // Communities
        for (auto &band : communities) {
            for (auto &g_struct : band) {
                auto community = std::static_pointer_cast<Community>(g_struct);
                auto com_id = ContactPoolType::Id::PrimaryCommunity;
                if (!community->is_primary)
                    com_id = ContactPoolType::Id::SecondaryCommunity;
                for (const auto &pool : community->pools) {
                    pool_sys[com_id].emplace_back(*pool);
                    cp_id++;
                }
            }
        }

        // -------------
        // Write persons
        // -------------
        auto write = config.GetTree().get<bool>("write_population");
        if (write) {
            auto output_file = files::PopulationFile(
                    config,
                    population
            );
            output_file.Write();
        }
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
