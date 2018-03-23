#include "GeoGenerator.h"
#include "../GenConfiguration.h"
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
using namespace util;

void GeoGenerator::generate(const string config_path, unsigned int thread_count)
{
    GenConfiguration config(config_path, thread_count);

    // Build
    shared_ptr<GeoGrid> geogrid = GeoGridBuilder::build(config);

    vector<shared_ptr<School>> schools = SchoolsBuilder::build(config, geogrid);

    vector<shared_ptr<Community>> communities = CommunitiesBuilder::build(config, geogrid);

    vector<shared_ptr<University>> universities = UniversitiesBuilder::build(config, geogrid);

    vector<shared_ptr<WorkPlace>> workplaces = WorkplacesBuilder::build(config, geogrid);

    vector<shared_ptr<GenStruct>> nschools (schools.begin(), schools.end());

    // Write
    std::cout << "Reached" << std::endl;
    SchoolFile school_file(config, nschools, geogrid);
    std::cout << "Reached1" << std::endl;
    school_file.write();
    std::cout << "Reached2" << std::endl;
}


} // namespace gen
} // namespace stride
