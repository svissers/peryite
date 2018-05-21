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


    unsigned int next_person_id = 0;
    unsigned int next_hh_id = 0;
    unsigned int amount_of_regions = dir.GetAmountOfRegions();
    for(unsigned int current_region_nr = 0; current_region_nr < amount_of_regions; current_region_nr++) {
        dir.AddFirstInRegion(next_person_id);
        auto config = dir.GetConfig()[current_region_nr];
        // Build
        std::tuple<unsigned int, unsigned int> PopulationReturnVal = builder::BuildPopulation(config, population,
                                                                                              next_person_id,
                                                                                              next_hh_id);
        next_person_id = std::get<0>(PopulationReturnVal);
        next_hh_id = std::get<1>(PopulationReturnVal);
        GeoGrid geogrid = builder::BuildGeoGrid(config);
        vector<shared_ptr<School>> schools = builder::BuildSchools(config, geogrid, population);
        vector<shared_ptr<University>> universities = builder::BuildUniversities(config, geogrid, schools.size());
        vector<shared_ptr<WorkPlace>> workplaces = builder::BuildWorkplaces(config, geogrid, population);
        vector<shared_ptr<Community>> communities = builder::BuildCommunities(config, geogrid, population);

        // Write
        auto population_file = make_shared<PopulationFile>(
                config,
                population
        );
        std::string popfilename = "pop";
        popfilename.append(std::to_string(current_region_nr));
        popfilename.append(".csv");
        population_file->SetFileName(popfilename);

        auto geo_grid_file = make_shared<GeoGridFile>(
                config,
                geogrid
        );
        std::string geofilename = "Geogrid";
        geofilename.append(std::to_string(current_region_nr));
        geofilename.append(".csv");
        geo_grid_file->SetFileName(geofilename);

        auto school_file = make_shared<SchoolFile>(
                config,
                vector<GenStruct::ptr>(schools.begin(), schools.end()),
                geogrid
        );
        std::string schoolfilename = "Schools";
        schoolfilename.append(std::to_string(current_region_nr));
        schoolfilename.append(".csv");
        school_file->SetFileName(schoolfilename);

        auto university_file = make_shared<UniversityFile>(
                config,
                vector<GenStruct::ptr>(universities.begin(), universities.end()),
                geogrid
        );
        std::string unifilename = "Universities";
        unifilename.append(std::to_string(current_region_nr));
        unifilename.append(".csv");
        university_file->SetFileName(unifilename);

        auto workplace_file = make_shared<WorkplaceFile>(
                config,
                vector<GenStruct::ptr>(workplaces.begin(), workplaces.end()),
                geogrid
        );
        std::string workfilename = "Workplaces";
        workfilename.append(std::to_string(current_region_nr));
        workfilename.append(".csv");
        workplace_file->SetFileName(workfilename);

        auto community_file = make_shared<CommunityFile>(
                config,
                vector<GenStruct::ptr>(communities.begin(), communities.end()),
                geogrid
        );
        std::string commiefilename = "Communities";
        commiefilename.append(std::to_string(current_region_nr));
        commiefilename.append(".csv");
        community_file->SetFileName(commiefilename);

        // Initialize
        // TODO change to vectors; push back
        dir.Initialize(population_file, geo_grid_file, school_file, university_file, workplace_file, community_file);

        auto write = config.GetTree().get<bool>("write_geogrid");
        if (write) {
            population_file->Write();
            geo_grid_file->Write();
            school_file->Write();
            university_file->Write();
            workplace_file->Write();
            community_file->Write();
        }
    }
}

} // namespace geogen
} // namespace gen
} // namespace stride
