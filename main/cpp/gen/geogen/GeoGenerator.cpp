#include "GeoGenerator.h"
#include "builders/PopulationBuilder.h"

using namespace boost::filesystem;
namespace stride {
namespace gen {
namespace geogen {

using namespace std;
using namespace files;
using namespace util;

void Generate(GenDirectory& dir, shared_ptr<Population>& population)
{
    population->SetRegions(dir.GetRegions());
    auto write = dir.GetConfig().GetTree().get<bool>("write_geogrid");
    // Build and write each region.
    shared_ptr<Region> prev_region = nullptr;
    for(auto & region : dir.GetRegions()) {
        if (prev_region) {
            region->first_person_id = prev_region->last_person_id+1;
            auto type = ContactPoolType::Id::Household;
            region->first_cps[type] = prev_region->last_cps[type] + 1;
        }

        // Build
        builder::BuildPopulation(region, population);
        std::cout << "building region: " << region->name << std::endl;
        std::cout << "  working: " <<std::flush;
        auto geogrid        = builder::BuildGeoGrid(region->config);
        std::cout << "-"<< std::flush;
        auto schools        = builder::BuildSchools(region->config, geogrid, population, region->first_person_id);
        std::cout << "-"<< std::flush;
        auto universities   = builder::BuildUniversities(region->config, geogrid);
        std::cout << "-"<< std::flush;
        auto workplaces     = builder::BuildWorkplaces(region->config, geogrid, population, region->first_person_id);
        std::cout << "-"<< std::flush;
        auto communities    = builder::BuildCommunities(region->config, geogrid, population, region->first_person_id);
        std::cout << "-"<< std::flush;

        // Write
        auto config     = region->config;
        string suffix   = to_string(region->id);
        shared_ptr<GeoGridFile> geo_grid_file = make_shared<GeoGridFile>(
                config,
                geogrid,
                suffix
        );
        std::cout << "-"<< std::flush;


        auto school_file = make_shared<SchoolFile>(
                config,
                vector<GenStruct::ptr>(schools.begin(), schools.end()),
                geogrid,
                suffix
        );
        std::cout << "-"<< std::flush;

        auto university_file = make_shared<UniversityFile>(
                config,
                vector<GenStruct::ptr>(universities.begin(), universities.end()),
                geogrid,
                suffix
        );
        std::cout << "-"<< std::flush;

        auto workplace_file = make_shared<WorkplaceFile>(
                config,
                vector<GenStruct::ptr>(workplaces.begin(), workplaces.end()),
                geogrid,
                suffix
        );
        std::cout << "-"<< std::flush;

        auto community_file = make_shared<CommunityFile>(
                config,
                vector<GenStruct::ptr>(communities.begin(), communities.end()),
                geogrid,
                suffix
        );
        std::cout << "-"<< std::flush;

        if (write) {
            geo_grid_file->Write();
            school_file->Write();
            university_file->Write();
            workplace_file->Write();
            community_file->Write();
        }
        std::cout << "-"<< std::flush;


        // Initialize
        dir.InitializeRegion(
            region->id,
            geo_grid_file, school_file,
            university_file, workplace_file, community_file
        );

        prev_region = region;
        std::cout << " finished." <<std::endl;

    }
    // Write the final population file
    std::cout << "writing population file... "<< std::flush;

    auto population_file = make_shared<PopulationFile>(
        dir.GetConfig(),
        population
    );
    if (write)
        population_file->Write();
    dir.InitializePopulationFile(population_file);
    std::cout << "finished." <<std::endl;

}

} // namespace geogen
} // namespace gen
} // namespace stride
