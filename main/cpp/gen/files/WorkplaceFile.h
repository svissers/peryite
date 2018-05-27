#pragma once
#include "GenFile.h"
#include "../structs/Workplace.h"

namespace stride {
namespace gen {
namespace files {

/**
 * A class that can read and write workplace data files.
 */
class WorkplaceFile : public GenFile
{
public:
    /// Constructor. Constructs the WorkplaceFile object using the config.
    /// @param config           The geopop configuration to be used.
    /// @param suffix           The suffix to use for the file name.
    WorkplaceFile(GenConfiguration& config, std::string suffix) : GenFile(config, suffix) {}

    /// Constructor. Constructs the WorkplaceFile object with an existing vector of workplaces.
    /// @param config           The geopop configuration to be used.
    /// @param structs          The workplaces that the file will contain.
    /// @param geo              The geogrid that was used to build the workplaces.
    /// @param suffix           The suffix to use for the file name.
    WorkplaceFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo, std::string suffix)
    : GenFile(config, structs, geo, suffix)
    {
        m_file_name = "Workplaces";
        m_labels = {"id","latitude","longitude","band"};
    }

private:
    /// Returns the workplace associated with a row in the csv data.
    /// @param row              The row that describes the workplace.
    /// @return                 The workplace associated with the csv row.
    std::shared_ptr<GenStruct> GetStruct(util::CSVRow const & row)
    {
        auto workplace = std::make_shared<WorkPlace>(WorkPlace(
            row.GetValue<unsigned int>("id"),
            util::spherical_point(
                row.GetValue<double>("latitude"),
                row.GetValue<double>("longitude")
                )
            )
        );
        return workplace;
    }

    /// Returns the values of a workplace in string format.
    /// @param g_struct         The workplace that contains the values.
    /// @return                 Vector of the values of the workplace in string format.
    std::vector<std::string> GetValues(std::shared_ptr<GenStruct> g_struct)
    {
        std::shared_ptr<WorkPlace> workplace = std::static_pointer_cast<WorkPlace>(g_struct);
        std::vector<std::string> values = {
            std::to_string(workplace->id),
            std::to_string(workplace->coordinate.get<0>()),
            std::to_string(workplace->coordinate.get<1>())
        };
        return values;
    }
};

typedef std::shared_ptr<WorkplaceFile> WorkplaceFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
