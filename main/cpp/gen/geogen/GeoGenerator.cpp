#include "GeoGenerator.h"
#include "GeoConfiguration.h"
#include "builders/SchoolsBuilder.h"
#include "builders/WorkplacesBuilder.h"

namespace stride {
namespace gen {

using namespace util;

void GeoGenerator::generate(const std::string config_path) {

    //thread count, coded here, TODO make dynamic
    unsigned int thread_count = 1;

    //getting the configuration
    GeoConfiguration Config = GeoConfiguration(config_path, thread_count);

    //building geogrid
    GeoGridBuilder geoBuild;
    std::shared_ptr<GeoGrid> Geogrid = geoBuild.build(Config);

    //building communities
    CommunitiesBuilder comBuild;
    std::shared_ptr<std::vector<Community>> Communities = comBuild.build(Config, Geogrid);

    //building schoolsui
    SchoolsBuilder schoolsBuilder;
    std::shared_ptr<std::vector<School>> Schools = schoolsBuilder.build(Config, Geogrid);

    //building univiersities
    UniversitiesBuilder uniBuild;
    std::shared_ptr<std::vector<University>> Universities = uniBuild.build(Config, Geogrid);

    //building workplaces
    WorkplacesBuilder workplacesBuilder;
    std::shared_ptr<std::vector<WorkPlace>> Workplaces = workplacesBuilder.build(Config, Geogrid);

    writefiles(Geogrid, Geogrid, "./output/Geogrid.csv");

        //writing Geogrid
    geoBuild.write("./output/Geogrid.csv");

    //writing communities
    comBuild.write("./output/Communities.csv");

    //writing universities
    uniBuild.write("./output/Universities.csv");


}


} // namespace gen
} // namespace stride
