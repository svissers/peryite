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
    std::cout << Geogrid->getTotalPopulation()<< std::endl;

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
    // std::cout << Schools->size() << std::endl;
    writefiles(Geogrid, Geogrid, "./output/Geogrid.csv");
    writefiles(Communities, Geogrid, "./output/Communities.csv");
    writefiles(Schools, Geogrid, "./output/Schools.csv");
    writefiles(Universities, Geogrid, "./output/Universities.csv");
    writefiles(Workplaces, Geogrid, "./output/Workplaces.csv");




}


} // namespace gen
} // namespace stride
