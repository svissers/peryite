#include "GeoGenerator.h"
#include "builders/SchoolsBuilder.h"
#include "builders/WorkplacesBuilder.h"
#include "../files/GeoGridFile.h"
#include "../files/SchoolFile.h"
#include "../files/UniversityFile.h"
#include "../files/WorkplaceFile.h"
#include "../files/CommunityFile.h"

using namespace boost::filesystem;
namespace stride {
namespace gen {

using namespace std;
using namespace files;
using namespace util;

void GeoGenerator::generate(GenDirectory& dir, unsigned int thread_count)
{
    auto config = dir.getConfig();

    // Build
    GeoGrid geogrid = GeoGridBuilder::build(config);
    vector<shared_ptr<School>> schools = SchoolsBuilder::build(config, geogrid);
    vector<shared_ptr<University>> universities = UniversitiesBuilder::build(config, geogrid);
    vector<shared_ptr<WorkPlace>> workplaces = WorkplacesBuilder::build(config, geogrid);
    vector<shared_ptr<Community>> communities = CommunitiesBuilder::build(config, geogrid);

    // Write
    auto geo_grid_file = make_shared<GeoGridFile>(
        config,
        vector<GenStruct::ptr>(geogrid.begin(), geogrid.end()),
        geogrid
    );
    geo_grid_file->write();

    auto school_file = make_shared<SchoolFile>(
        config,
        vector<GenStruct::ptr>(schools.begin(), schools.end()),
        geogrid
    );
    school_file->write();

    auto university_file = make_shared<UniversityFile>(
        config,
        vector<GenStruct::ptr>(universities.begin(), universities.end()),
        geogrid
    );
    university_file->write();

    auto workplace_file = make_shared<WorkplaceFile>(
        config,
        vector<GenStruct::ptr>(workplaces.begin(), workplaces.end()),
        geogrid
    );
    workplace_file->write();

    auto community_file = make_shared<CommunityFile>(
        config,
        vector<GenStruct::ptr>(communities.begin(), communities.end()),
        geogrid
    );
    community_file->write();

    // Initialize
    dir.initialize(geo_grid_file, school_file, university_file, workplace_file, community_file);
}


} // namespace gen
} // namespace stride
