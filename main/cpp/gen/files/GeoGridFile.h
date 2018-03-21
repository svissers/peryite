#pragma once
#include "GenFile.h"
#include "../structs/GeoGrid.h"

namespace stride {
namespace gen {

/**
 *
 */
class GeoGridFile : public GenFile
{
private:
    const std::string m_file_name = "Geogrid.csv";
    const std::initializer_list<std::string> m_labels = {"id","latitude","longitude","name","province","population","band"}; 

    shared_ptr<GenStruct> getStruct(CSVRow const & row) 
    {
        auto center = make_shared(UrbanCenter(
            row.getValue<unsigned int>("id"),  
            row.getValue<unsigned int>("pop"),
            row.getValue<string>("name"),
            row.getValue<int>("province"),
            util::GeoCoordinate(
                row.getValue<double>("latitude"),
                row.getValue<double>("longitude")
                )
            )
            
        );
        return center;
    }

    std::vector<std::string> getValues(shared_ptr<UrbanCenter> center) 
    {
        std::vector<std::string> values;
        values.push_back(to_string(center->id));
        values.push_back(to_string(center->coordinate.m_latitude));
        values.push_back(to_string(center->coordinate.m_longitude));
        values.push_back(to_string(center->name));
        values.push_back(to_string(center->province));
        values.push_back(to_string(center->population));
        return values;
    }
};

} // namespace gen
} // namespace stride
