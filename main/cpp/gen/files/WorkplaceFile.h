#pragma once
#include "GenFile.h"
#include "../structs/Workplace.h"

namespace stride {
namespace gen {
namespace files {

/**
 *
 */
class WorkplaceFile : public GenFile
{
public:
    WorkplaceFile(GenConfiguration& config) : GenFile(config) {}

    WorkplaceFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo)
    : GenFile(config, structs, geo)
    {
        m_file_name = "Workplaces.csv";
        m_labels = {"id","latitude","longitude","band"};
    }

private:
    std::shared_ptr<GenStruct> GetStruct(util::CSVRow const & row)
    {
        auto workplace = std::make_shared<WorkPlace>(WorkPlace(
            row.GetValue<unsigned int>("id"),
            util::GeoCoordinate(
                row.GetValue<double>("latitude"),
                row.GetValue<double>("longitude")
                )
            )
        );
        return workplace;
    }

    std::vector<std::string> GetValues(std::shared_ptr<GenStruct> g_struct)
    {
        std::shared_ptr<WorkPlace> workplace = std::static_pointer_cast<WorkPlace>(g_struct);
        std::vector<std::string> values = {
            std::to_string(workplace->id),
            std::to_string(workplace->coordinate.m_latitude),
            std::to_string(workplace->coordinate.m_longitude)
        };
        return values;
    }
};

typedef std::shared_ptr<WorkplaceFile> WorkplaceFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
