#pragma once
#include "GenFile.h"
#include "../structs/Workplace.h"

namespace stride {
namespace gen {

/**
 *
 */
class WorkplaceFile : public GenFile
{
private:
    const std::string m_file_name = "Workplaces.csv";
    const std::initializer_list<std::string> m_labels = {"id","latitude","longitude","band"};

    std::shared_ptr<GenStruct> getStruct(util::CSVRow const & row)
    {
        auto workplace = std::make_shared<WorkPlace>(WorkPlace(
            row.getValue<unsigned int>("id"),
            util::GeoCoordinate(
                row.getValue<double>("latitude"),
                row.getValue<double>("longitude")
                )
            )
        );
        return workplace;
    }

    std::vector<std::string> getValues(std::shared_ptr<WorkPlace> workplace)
    {
        std::vector<std::string> values;
        values.push_back(std::to_string(workplace->id));
        values.push_back(std::to_string(workplace->coordinate.m_latitude));
        values.push_back(std::to_string(workplace->coordinate.m_longitude));
        return values;
    }
};

} // namespace gen
} // namespace stride
