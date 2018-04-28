#include "GeoGenerator.h"
#include "builders/PopulationBuilder.h"
#include "builders/SchoolsBuilder.h"
#include "builders/WorkplacesBuilder.h"
#include "../files/PopulationFile.h"
#include "../files/GeoGridFile.h"
#include "../files/SchoolFile.h"
#include "../files/UniversityFile.h"
#include "../files/WorkplaceFile.h"
#include "../files/CommunityFile.h"

using namespace boost::filesystem;
namespace stride {
namespace gen {
namespace geogen {

using namespace std;
using namespace files;
using namespace util;

void Generate(GenDirectory& dir)
{
    auto config = dir.getConfig();

    // Build
    std::cout << "Building population" << std::endl;
    shared_ptr<Population> population = builder::BuildPopulation(config, dir.getBeliefConfig());
    std::cout << "Building geogen" << std::endl;
    GeoGrid geogrid = builder::BuildGeoGrid(config);
    std::cout << "Building schools" << std::endl;
    vector<shared_ptr<School>> schools = builder::BuildSchools(config, geogrid);
    std::cout << "Building universities" << std::endl;
    vector<shared_ptr<University>> universities = builder::BuildUniversities(config, geogrid);
    std::cout << "Building workplaces" << std::endl;
    vector<shared_ptr<WorkPlace>> workplaces = builder::BuildWorkplaces(config, geogrid);
    std::cout << "Building communities" << std::endl;
    vector<shared_ptr<Community>> communities = builder::BuildCommunities(config, geogrid);

    // Write
    auto population_file = make_shared<PopulationFile>(
        config,
        population
    );
    population_file->Write();

    auto geo_grid_file = make_shared<GeoGridFile>(
        config,
        geogrid
    );
    geo_grid_file->Write();

    auto school_file = make_shared<SchoolFile>(
        config,
        vector<GenStruct::ptr>(schools.begin(), schools.end()),
        geogrid
    );
    school_file->Write();

    auto university_file = make_shared<UniversityFile>(
        config,
        vector<GenStruct::ptr>(universities.begin(), universities.end()),
        geogrid
    );
    university_file->Write();

    auto workplace_file = make_shared<WorkplaceFile>(
        config,
        vector<GenStruct::ptr>(workplaces.begin(), workplaces.end()),
        geogrid
    );
    workplace_file->Write();

    auto community_file = make_shared<CommunityFile>(
        config,
        vector<GenStruct::ptr>(communities.begin(), communities.end()),
        geogrid
    );
    community_file->Write();

    // Initialize
    dir.initialize(population_file, geo_grid_file, school_file, university_file, workplace_file, community_file);
}

} // namespace geogen
} // namespace gen
} // namespace stride
