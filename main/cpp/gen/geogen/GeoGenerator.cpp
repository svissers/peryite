#include "GeoGenerator.h"
#include "GeoConfiguration.h"
namespace stride {
namespace gen {

void GeoGenerator::generate(const std::string config_path) {

    //thread count, coded here, TODO make dynamic
    unsigned int thread_count = 1;

    //getting the configuration
    GeoConfiguration Config = GeoConfiguration(config_path, thread_count);

    //building geogrid
    std::shared_ptr<GeoGrid> Geogrid = GeoGridBuilder.build(Config);

    //building communities
    std::shared_ptr<std::vector<Community>> Communities = CommunitiesBuilder.build(Config, Geogrid);

    //building schools
    std::shared_ptr<std::vector<School>> Schools = SchoolsBuilder.build(Config, Geogrid);

    //building univiersities
    std::shared_ptr<vector<University>> Universities = UniversitiesBuilder.build(Config, Geogrid);

    //building workplaces
    std::shared_ptr<std::vector<WorkPlace>> Workplaces = WorkplacesBuilder.build(Config, Geogrid);



}


} // namespace gen
} // namespace stride
