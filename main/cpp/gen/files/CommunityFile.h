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

    std::shared_ptr<GenStruct> getStruct(util::CSVRow const & row)
    {
        auto community = std::make_shared<Community>(Community(
            row.getValue<unsigned int>("id"),
            row.getValue<bool>("primary"),
            util::GeoCoordinate(
                row.getValue<double>("latitude"),
                row.getValue<double>("longitude")
            )
        ));
        return community;
    }

    std::vector<std::string> getValues(std::shared_ptr<Community> community)
    {
        std::vector<std::string> values;
        values.push_back(std::to_string(community->id));
        values.push_back(std::to_string(community->coordinate.m_latitude));
        values.push_back(std::to_string(community->coordinate.m_longitude));
        values.push_back(std::to_string(community->is_primary));
        return values;
    }
};

} // namespace gen
} // namespace stride
