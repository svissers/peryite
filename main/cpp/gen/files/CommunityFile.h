#pragma once
#include "GenFile.h"
#include "../structs/Community.h"

namespace stride {
namespace gen {
namespace files {

/**
 * A class that can read and write community data files.
 */
class CommunityFile : public GenFile
{
public:

    /// Constructor. Constructs the CommunityFile object using the config.
    /// @param config           The geopop configuration to use.
    CommunityFile(GenConfiguration& config) : GenFile(config) {}

    /// Constructor. Initializes the CommunityFile object with a vector of communities.
    /// @param config           The configuration property tree to use.
    /// @param structs          The communities that the file will contain.
    /// @param geo              The geogrid that was used to build the communities.
    CommunityFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo)
    : GenFile(config, structs, geo)
    {
        m_file_name = "Communities.csv";
        m_labels = {"id","latitude","longitude","primary", "band"};
    }

private:
    /// Returns the community associated with a row in the csv data.
    /// @param row              The row that describes the community.
    /// @return                 The community associated with the csv row.
    std::shared_ptr<GenStruct> GetStruct(util::CSVRow const & row)
    {
        auto community = std::make_shared<Community>(Community(
            row.GetValue<unsigned int>("id"),
            row.GetValue<bool>("primary"),
            util::spherical_point(
                row.GetValue<double>("latitude"),
                row.GetValue<double>("longitude")
            )
        ));
        return community;
    }

    /// Returns the values of a community in string format.
    /// @param g_struct         The community that contains the values.
    /// @return                 Vector of the values of the community in string format.
    std::vector<std::string> GetValues(std::shared_ptr<GenStruct> g_struct)
    {
        std::shared_ptr<Community> community = std::static_pointer_cast<Community>(g_struct);
        std::vector<std::string> values = {
            std::to_string(community->id),
            std::to_string(community->coordinate.get<0>()),
            std::to_string(community->coordinate.get<1>()),
            std::to_string(community->is_primary)
        };
        return values;
    }
};

typedef std::shared_ptr<CommunityFile> CommunityFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
