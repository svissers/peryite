#pragma once
#include "GenFile.h"
#include "../structs/School.h"

namespace stride {
namespace gen {

/**
 *
 */
class SchoolFile : public GenFile
{
public:
    SchoolFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>>& structs, std::shared_ptr<GeoGrid> geo)
    : GenFile(config, structs, geo)
    {
        m_file_name = "Schools.csv";
        m_labels = {"id","latitude","longitude","band"};
    }

private:
    std::shared_ptr<GenStruct> getStruct(util::CSVRow const & row)
    {
        auto school = std::make_shared<School>(School(
            row.getValue<unsigned int>("id"),
            util::GeoCoordinate(
                row.getValue<double>("latitude"),
                row.getValue<double>("longitude")
                )
            )
        );
        return school;
    }

    std::vector<std::string> getValues(std::shared_ptr<GenStruct> g_struct)
    {
        std::shared_ptr<School> school = std::static_pointer_cast<School>(g_struct);
        std::vector<std::string> values;

        values.push_back(std::to_string(school->id));
        values.push_back(std::to_string(school->coordinate.m_latitude));
        values.push_back(std::to_string(school->coordinate.m_longitude));
        return values;
    }
};

} // namespace gen
} // namespace stride
