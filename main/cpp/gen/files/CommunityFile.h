#pragma once
#include "GenFile.h"
#include "../structs/Community.h"

namespace stride {
namespace gen {

/**
 *
 */
class CommunityFIle : public GenFile
{
private:
    const std::string m_file_name = "Communities.csv";
    const std::initializer_list<std::string> m_labels = {"id","latitude","longitude","primary", "band"}; 

    shared_ptr<GenStruct> getStruct(CSVRow const & row) 
    {
        auto community = make_shared(Community(
            row.getValue<unsigned int>("id"),  
            util::GeoCoordinate(
                row.getValue<double>("latitude"),
                row.getValue<double>("longitude")
                )
            ),
            row.getValue<bool>("primary"),  
        );
        return community;
    }

    std::vector<std::string> getValues(shared_ptr<Community> community) 
    {
        std::vector<std::string> values;
        values.push_back(to_string(community->id));
        values.push_back(to_string(community->coordinate.m_latitude));
        values.push_back(to_string(community->coordinate.m_longitude));
        values.push_back(to_string(community->is_primary));
        return values;
    }
};

} // namespace gen
} // namespace stride
