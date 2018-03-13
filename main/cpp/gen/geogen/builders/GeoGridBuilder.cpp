#include "GeoGridBuilder.h"
#include "../structs/UrbanCenter.h"
#include "util/CSV.h"
#include "geo/GeoCoordinate.h"
namespace stride {
namespace gen {

using namespace std;

shared_ptr<GeoGrid> GeoGridBuilder::build(const GeoConfiguration& config)
{
        geo_grid = make_shared<GeoGrid>();
        // Construct the urban centers from the city data
        util::CSV cities_data = util::CSV(config.getCitiesFileName());
        maxLong = 0;
        minLong = 90;
        for (util::CSVRow& row : cities_data) {
                auto id         = row.getValue<unsigned int>("city_id");
                auto population = row.getValue<unsigned int>("population");
                auto name       = row.getValue<string>("city_name");
                auto province   = row.getValue<unsigned int>("province");
                auto longitude  = row.getValue<double>("longitude");
                auto latitude   = row.getValue<double>("latitude");
                UrbanCenter center = UrbanCenter(id, population, name, province, util::GeoCoordinate(longitude, latitude));
                geo_grid->push_back(center);
                if(longitude > maxLong)
                        maxLong = longitude;
                if(longitude < minLong)
                        minLong = longitude;
        }
        LongitudeBandWidth = (maxLong - minLong)/AMOUNTOFBANDS;
        return geo_grid;
}

    void GeoGridBuilder::write(std::string GeogridFile) {
               std::vector<std::vector<UrbanCenter> > sortedCenters;
        for(unsigned int it = geo_grid->begin(); it < geo_grid->end();it++){
                for(unsigned int i=0; i < AMOUNTOFBANDS; i++){
                        if(it->coordinate.m_longitude<minLong+((i+1)*LongitudeBandWidth)){
                                for(unsigned int j=0; j<sortedCenters[i].size(); j++){
                                        if(sortedCenters[i][j].coordinate.m_latitude> it->coordinate.m_latitude){
                                                j--;
                                                break;
                                        }
                                }
                                sortedCenters[i].insert(sortedCenters.begin()+j, *it);
                                break;
                        }
                }
        }
            ofstream my_file{GeogridFile};
            if(my_file.is_open()){
                    for(unsigned int i = 0; i < sortedCenters.size(); i++){
                            for (unsigned int j = 0; j < sortedCenters[i].size(); j++){
                                    my_file<< "||center|| ID: " << sortedCenters[i][j].id << " , "
                                           << sortedCenters[i][j].coordinate << " , Name: " << sortedCenters[i][j].name << " , Province: "
                                           << sortedCenters[i][j].province << " , Pop: " << sortedCenters[i][j].population << " ";
                            }
                            my_file<< std::endl;
                    }
            }

    }

} // namespace gen
} // namespace stride
