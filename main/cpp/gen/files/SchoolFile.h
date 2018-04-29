#pragma once
#include "GenFile.h"
#include "../structs/School.h"

namespace stride {
namespace gen {
namespace files {

/**
 * A class that can read and write school data files.
 */
class SchoolFile : public GenFile
{
public:
    /// Constructor. Constructs the SchoolFile object using the config.
    /// @param config           The geopop configuration to be used.
    SchoolFile(GenConfiguration& config) : GenFile(config) {}

    /// Constructor. Constructs the SchoolFile object with an existing vector of schools.
    /// @param config           The geopop configuration to be used.
    /// @param structs          The schools that the file will contain.
    /// @param geo              The geogrid that was used to build the schools.
    SchoolFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo)
    : GenFile(config, structs, geo)
    {
        m_file_name = "Schools.csv";
        m_labels = {"id","latitude","longitude","band"};
    }

private:
    /// Returns the school associated with a row in the csv data.
    /// @param row              The row that describes the school.
    /// @return                 The school associated with the csv row.
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

    /// Returns the values of a school in string format.
    /// @param g_struct         The school that contains the values.
    /// @return                 Vector of the values of the school in string format.
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
