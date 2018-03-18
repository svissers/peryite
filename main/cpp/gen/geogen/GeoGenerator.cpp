#include "GeoGenerator.h"
#include "GeoConfiguration.h"
#include "builders/SchoolsBuilder.h"
#include "builders/WorkplacesBuilder.h"
#include <boost/filesystem.hpp>

using namespace boost::filesystem;
namespace stride {
namespace gen {

using namespace util;

void GeoGenerator::generate(const std::string config_path, unsigned int thread_count)
{
    GeoConfiguration config = GeoConfiguration(config_path, thread_count);

    // Build
    std::shared_ptr<GeoGrid> geogrid = GeoGridBuilder::build(config);

    std::shared_ptr<std::vector<School>> schools = SchoolsBuilder::build(config, geogrid);

    std::shared_ptr<std::vector<Community>> communities = CommunitiesBuilder::build(config, geogrid);

    std::shared_ptr<std::vector<University>> universities = UniversitiesBuilder::build(config, geogrid);

    std::shared_ptr<std::vector<WorkPlace>> workplaces = WorkplacesBuilder::build(config, geogrid);

    // Write
    boost::filesystem::path out_dir = "output/"+config_path.substr(0, config_path.find_last_of("."));
    try {
            create_directories(out_dir);
    } catch (std::exception& e) {
            std::cout << "CliController::Go> Exception while creating output directory:  {}", e.what();
            throw;
    }
    writefiles(geogrid, geogrid, out_dir.string()+"/Geogrid.csv");
    writefiles(communities, geogrid, out_dir.string()+"/Communities.csv");
    writefiles(schools, geogrid, out_dir.string()+"/Schools.csv");
    writefiles(universities, geogrid, out_dir.string()+"/Universities.csv");
    writefiles(workplaces, geogrid, out_dir.string()+"/Workplaces.csv");
}


} // namespace gen
} // namespace stride
