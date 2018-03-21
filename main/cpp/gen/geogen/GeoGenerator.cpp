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

using namespace util;

void GeoGenerator::generate(const std::string config_path, unsigned int thread_count)
{
    GenConfiguration config(config_path, thread_count);

    // Build
    std::shared_ptr<GeoGrid> geogrid = GeoGridBuilder::build(config);

    std::shared_ptr<std::vector<School>> schools = SchoolsBuilder::build(config, geogrid);

    std::shared_ptr<std::vector<Community>> communities = CommunitiesBuilder::build(config, geogrid);

    std::shared_ptr<std::vector<University>> universities = UniversitiesBuilder::build(config, geogrid);

    std::shared_ptr<std::vector<WorkPlace>> workplaces = WorkplacesBuilder::build(config, geogrid);

    // Write
    SchoolFile school_file(config, schools, geogrid);
    SchoolFile university_file(config, universities, geogrid);
    WorkplaceFile workplace_file(config, workplaces, geogrid);
    SchoolFile community_file(config, communities, geogrid);

    /*
    writefiles(geogrid, geogrid, out_dir.string()+"/Geogrid.csv");
    writefiles(communities, geogrid, out_dir.string()+"/Communities.csv");
    writefiles(schools, geogrid, out_dir.string()+"/Schools.csv");
    writefiles(universities, geogrid, out_dir.string()+"/Universities.csv");
    writefiles(workplaces, geogrid, out_dir.string()+"/Workplaces.csv");
    */
}


} // namespace gen
} // namespace stride
