#pragma once
#include "GenFile.h"
#include "../structs/School.h"

namespace stride {
namespace gen {
namespace files {

/**
 *
 */
class SchoolFile : public GenFile
{
public:
    SchoolFile(GenConfiguration& config) : GenFile(config) {}

    SchoolFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo)
    : GenFile(config, structs, geo)
    {
        m_file_name = "Schools.csv";
        m_labels = {"id","latitude","longitude","band"};
    }

private:
    std::shared_ptr<GenStruct> GetStruct(util::CSVRow const & row)
    {
        auto school = std::make_shared<School>(School(
            row.GetValue<unsigned int>("id"),
            util::GeoCoordinate(
                row.GetValue<double>("latitude"),
                row.GetValue<double>("longitude")
                )
            )
        );
        return school;
    }

    std::vector<std::string> GetValues(std::shared_ptr<GenStruct> g_struct)
    {
        std::shared_ptr<School> school = std::static_pointer_cast<School>(g_struct);
        std::vector<std::string> values = {
            std::to_string(school->id),
            std::to_string(school->coordinate.m_latitude),
            std::to_string(school->coordinate.m_longitude)
        };
        return values;
    }
};

typedef std::shared_ptr<SchoolFile> SchoolFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
