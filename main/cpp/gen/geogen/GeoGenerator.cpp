#include "GeoGenerator.h"
#include "builders/PopulationBuilder.h"
#include <papi.h>

using namespace boost::filesystem;
namespace stride {
namespace gen {
namespace geogen {

using namespace std;
using namespace files;
using namespace util;

void Generate(GenDirectory& dir, shared_ptr<Population>& population)
{
    /* PAPI Events */
    /*
    int Events[5] =
    {PAPI_TOT_INS, PAPI_LD_INS, PAPI_SR_INS, PAPI_BR_INS, PAPI_TOT_CYC};
    long_long values[5];
    /* Start counting events */
    /*
    if (PAPI_start_counters(Events, 5) != PAPI_OK)
        throw runtime_error(string(__func__) + "> Papi error occured while building population.");
    */
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
        /*
        if (PAPI_read_counters(values, 5) != PAPI_OK)
            throw runtime_error(string(__func__) + "> Papi error occured while reading counters after building population.");
        std::cout << "BuildPopulation Counters: " << std::endl;
        std::cout << "Total instructions: " << values[0] << std::endl;
        std::cout << "Load instructions: " << values[1] << std::endl;
        std::cout << "Store Instructions: " << values[2] << std::endl;
        std::cout << "Branch Instructions: " << values[3] << std::endl;
        std::cout << "Total cycles: " << values[4] << std::endl;
        */
        std::cout << "building region: " << region->name << std::endl;
        //std::cout << "  working: " << std::flush;
        auto geogrid        = builder::BuildGeoGrid(region->config);
        /*
        if (PAPI_read_counters(values, 5) != PAPI_OK)
            throw runtime_error(string(__func__) + "> Papi error occured while reading counters after building population.");
        std::cout << "BuildGeoGrid Counters: " << std::endl;
        std::cout << "Total instructions: " << values[0] << std::endl;
        std::cout << "Load instructions: " << values[1] << std::endl;
        std::cout << "Store Instructions: " << values[2] << std::endl;
        std::cout << "Branch Instructions: " << values[3] << std::endl;
        std::cout << "Total cycles: " << values[4] << std::endl;
        */

        //std::cout << "-"<< std::flush;
        auto schools        = builder::BuildSchools(region->config, geogrid, population, region->first_person_id);
        /*
        if (PAPI_read_counters(values, 5) != PAPI_OK)
            throw runtime_error(string(__func__) + "> Papi error occured while reading counters after building population.");
        std::cout << "BuildSchools Counters: " << std::endl;
        std::cout << "Total instructions: " << values[0] << std::endl;
        std::cout << "Load instructions: " << values[1] << std::endl;
        std::cout << "Store Instructions: " << values[2] << std::endl;
        std::cout << "Branch Instructions: " << values[3] << std::endl;
        std::cout << "Total cycles: " << values[4] << std::endl;
        */
        //std::cout << "-"<< std::flush;
        auto universities   = builder::BuildUniversities(region->config, geogrid);
        /*
        if (PAPI_read_counters(values, 5) != PAPI_OK)
            throw runtime_error(string(__func__) + "> Papi error occured while reading counters after building population.");
        std::cout << "BuildUniversities Counters: " << std::endl;
        std::cout << "Total instructions: " << values[0] << std::endl;
        std::cout << "Load instructions: " << values[1] << std::endl;
        std::cout << "Store Instructions: " << values[2] << std::endl;
        std::cout << "Branch Instructions: " << values[3] << std::endl;
        std::cout << "Total cycles: " << values[4] << std::endl;
        */
        //std::cout << "-"<< std::flush;
        auto workplaces     = builder::BuildWorkplaces(region->config, geogrid, population, region->first_person_id);
        /*
        if (PAPI_read_counters(values, 5) != PAPI_OK)
            throw runtime_error(string(__func__) + "> Papi error occured while reading counters after building population.");
        std::cout << "BuildWorkplaces Counters: " << std::endl;
        std::cout << "Total instructions: " << values[0] << std::endl;
        std::cout << "Load instructions: " << values[1] << std::endl;
        std::cout << "Store Instructions: " << values[2] << std::endl;
        std::cout << "Branch Instructions: " << values[3] << std::endl;
        std::cout << "Total cycles: " << values[4] << std::endl;
        */
        //std::cout << "-"<< std::flush;
        auto communities    = builder::BuildCommunities(region->config, geogrid, population, region->first_person_id);
        /*
        if (PAPI_read_counters(values, 5) != PAPI_OK)
            throw runtime_error(string(__func__) + "> Papi error occured while reading counters after building population.");
        std::cout << "BuildCommunities Counters: " << std::endl;
        std::cout << "Total instructions: " << values[0] << std::endl;
        std::cout << "Load instructions: " << values[1] << std::endl;
        std::cout << "Store Instructions: " << values[2] << std::endl;
        std::cout << "Branch Instructions: " << values[3] << std::endl;
        std::cout << "Total cycles: " << values[4] << std::endl;
        */
        //std::cout << "-"<< std::flush;

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
    /*
    if (PAPI_read_counters(values, 5) != PAPI_OK)
        throw runtime_error(string(__func__) + "> Papi error occured while reading counters after building population.");
    std::cout << "Geogen rest Counters: " << std::endl;
    std::cout << "Total instructions: " << values[0] << std::endl;
    std::cout << "Load instructions: " << values[1] << std::endl;
    std::cout << "Store Instructions: " << values[2] << std::endl;
    std::cout << "Branch Instructions: " << values[3] << std::endl;
    std::cout << "Total cycles: " << values[4] << std::endl;
    */
}

} // namespace geogen
} // namespace gen
} // namespace stride
