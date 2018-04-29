#pragma once
#include "GenFile.h"
#include "../structs/University.h"

namespace stride {
namespace gen {
namespace files {

/**
 *
 */
class UniversityFile : public GenFile
{
public:
    UniversityFile(GenConfiguration& config) : GenFile(config) {}

    UniversityFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo)
    : GenFile(config, structs, geo)
    {
        m_file_name = "Universities.csv";
        m_labels = {"id", "urban_id", "latitude","longitude","band"};
    }

private:
    std::shared_ptr<GenStruct> GetStruct(util::CSVRow const & row)
    {
        auto university = std::make_shared<University>(University(
            row.GetValue<unsigned int>("id"),
            row.GetValue<unsigned int>("urban_id"),
            util::GeoCoordinate(
                row.GetValue<double>("latitude"),
                row.GetValue<double>("longitude")
                )
            )
        );
        return university;
    }

    std::vector<std::string> GetValues(std::shared_ptr<GenStruct> g_struct)
    {
        std::shared_ptr<University> university = std::static_pointer_cast<University>(g_struct);
        std::vector<std::string> values = {
            std::to_string(university->id),
            std::to_string(university->urban_id),
            std::to_string(university->coordinate.m_latitude),
            std::to_string(university->coordinate.m_longitude)
        };
        return values;
    }
};

typedef std::shared_ptr<UniversityFile> UniversityFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
