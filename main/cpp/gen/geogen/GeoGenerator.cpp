#include "GeoGenerator.h"
#include "GeoConfiguration.h"
#include "builders/SchoolsBuilder.h"
#include "builders/WorkplacesBuilder.h"

namespace stride {
namespace gen {

using namespace util;

void GeoGenerator::generate(const std::string config_path, unsigned int thread_count)
{
    GeoConfiguration config = GeoConfiguration(config_path, thread_count);

    cout << "Building" << endl;
    // Build
    std::shared_ptr<GeoGrid> geogrid = GeoGridBuilder::build(config);
    cout << "GeoGrid done" << endl;

    std::shared_ptr<std::vector<School>> schools = SchoolsBuilder::build(config, geogrid);
    cout << "Schools done" << endl;

    std::shared_ptr<std::vector<Community>> communities = CommunitiesBuilder::build(config, geogrid);
    cout << "Communities done" << endl;

    std::shared_ptr<std::vector<University>> universities = UniversitiesBuilder::build(config, geogrid);
    cout << "Universities done" << endl;

    std::shared_ptr<std::vector<WorkPlace>> workplaces = WorkplacesBuilder::build(config, geogrid);
    cout << "Done Building" << endl;

    // Write
    writefiles(geogrid, geogrid, "./output/Geogrid.csv");
    writefiles(communities, geogrid, "./output/Communities.csv");
    writefiles(schools, geogrid, "./output/Schools.csv");
    writefiles(universities, geogrid, "./output/Universities.csv");
    writefiles(workplaces, geogrid, "./output/Workplaces.csv");
}


} // namespace gen
} // namespace stride
