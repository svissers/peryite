#include "GeoGenerator.h"
#include "GeoConfiguration.h"
namespace stride {
namespace gen {


void Geogenerator::writeToFile(std::string output_filename, Builder writable){
    ofstream my_file{"./output/" + output_filename};

    if(my_file.is_open()){
        break;
        //TODO write
    }


}

void GeoGenerator::generate(const std::string config_path) {

    //thread count, coded here, TODO make dynamic
    unsigned int thread_count = 1;

    //getting the configuration
    GeoConfiguration Config = GeoConfiguration(config_path, thread_count);

    //building geogrid
    GeoGridBuilder geoBuild;
    std::shared_ptr<GeoGrid> Geogrid = geoBuild.build(Config);

    //building communities
    std::shared_ptr<std::vector<Community>> Communities = CommunitiesBuilder.build(Config, Geogrid);

    //building schools
    std::shared_ptr<std::vector<School>> Schools = SchoolsBuilder.build(Config, Geogrid);

    //building univiersities
    std::shared_ptr<std::vector<University>> Universities = UniversitiesBuilder.build(Config, Geogrid);

    //building workplaces
    std::shared_ptr<std::vector<WorkPlace>> Workplaces = WorkplacesBuilder.build(Config, Geogrid);

    //writing Geogrid
    geoBuild.write("./output/Geogrid.csv");

}


} // namespace gen
} // namespace stride
