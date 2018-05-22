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
    unsigned int start_person_id = 0;
    unsigned int next_hh_id = 0;
    unsigned int amount_of_regions = dir.GetAmountOfRegions();
    unsigned int nextIdCommunity = 0;
    unsigned int nextIdWorkplace = 0;
    unsigned int nextIdUniversity = 0;
    unsigned int nextIdSchool = 0;
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
        std::tuple<vector<shared_ptr<School>>, unsigned int> schoolReturnVal = builder::BuildSchools(config, geogrid, population, nextIdSchool);
        vector<shared_ptr<School>> schools = get<0>(schoolReturnVal);
        nextIdUniversity = get<1>(schoolReturnVal);
        std::tuple<vector<shared_ptr<University>>, unsigned int> uniReturnVal = builder::BuildUniversities(config, geogrid,  nextIdUniversity);
        vector<shared_ptr<University>> universities = get<0>(uniReturnVal);
        nextIdSchool = get<1>(uniReturnVal);
        tuple<vector<shared_ptr<WorkPlace>>, unsigned int> workReturnVal = builder::BuildWorkplaces(config, geogrid, population, nextIdWorkplace, start_person_id, next_person_id);
        vector<shared_ptr<WorkPlace>> workplaces = get<0>(workReturnVal);
        nextIdWorkplace = get<1>(workReturnVal);
        tuple<vector<shared_ptr<Community>>, unsigned int> commieReturnVal = builder::BuildCommunities(config, geogrid, population, nextIdCommunity);
        vector<shared_ptr<Community>> communities = get<0>(commieReturnVal);
        nextIdCommunity = get<1>(commieReturnVal);

        start_person_id = next_person_id;

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
        dir.Initialize(population_file, geo_grid_file, school_file, university_file, workplace_file, community_file);
        std::cout << "going to write" << std::endl;

        auto write = config.GetTree().get<bool>("write_geogrid");
        if (write) {
            population_file->Write();
            geo_grid_file->Write();
            school_file->Write();
            university_file->Write();
            workplace_file->Write();
            community_file->Write();
        }
        std::cout << "completed building region: " << current_region_nr << std::endl;
    }
}

} // namespace geogen
} // namespace gen
} // namespace stride
