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
private:
    const std::string m_file_name = "Schools.csv";
    const std::initializer_list<std::string> m_labels = {"id","latitude","longitude","band"}; 

    shared_ptr<GenStruct> getStruct(CSVRow const & row) 
    {
        auto school = make_shared(School(
            row.getValue<unsigned int>("id"),  
            util::GeoCoordinate(
                row.getValue<double>("latitude"),
                row.getValue<double>("longitude")
                )
            )
        );
        return school;
    }

    std::vector<std::string> getValues(shared_ptr<School> school) 
    {
        std::vector<std::string> values;
        values.push_back(to_string(school->id));
        values.push_back(to_string(school->coordinate.m_latitude));
        values.push_back(to_string(school->coordinate.m_longitude));
        return values;
    }
};

} // namespace gen
} // namespace stride
