#pragma once
#include "GenFile.h"
#include "../structs/Community.h"

namespace stride {
namespace gen {
namespace files {

/**
 *
 */
class CommunityFile : public GenFile
{
public:
    CommunityFile(GenConfiguration& config) : GenFile(config) {}

    CommunityFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo)
    : GenFile(config, structs, geo)
    {
        m_file_name = "Communities.csv";
        m_labels = {"id","latitude","longitude","primary", "band"};
    }

private:
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

    std::vector<std::string> getValues(std::shared_ptr<GenStruct> g_struct)
    {
        std::shared_ptr<Community> community = std::static_pointer_cast<Community>(g_struct);
        std::vector<std::string> values = {
            std::to_string(community->id),
            std::to_string(community->coordinate.m_latitude),
            std::to_string(community->coordinate.m_longitude),
            std::to_string(community->is_primary)
        };
        return values;
    }
};

typedef std::shared_ptr<CommunityFile> CommunityFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
