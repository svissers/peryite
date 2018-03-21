#pragma once
#include "GenFile.h"
#include "../structs/University.h"

namespace stride {
namespace gen {

/**
 *
 */
class UniversityFile : public GenFile
{
private:
    const std::string m_file_name = "Universities.csv";
    const std::initializer_list<std::string> m_labels = {"id","latitude","longitude","band"}; 

    shared_ptr<GenStruct> getStruct(CSVRow const & row) 
    {
        auto university = make_shared(University(
            row.getValue<unsigned int>("id"),  
            util::GeoCoordinate(
                row.getValue<double>("latitude"),
                row.getValue<double>("longitude")
                )
            )
        );
        return university;
    }

    std::vector<std::string> getValues(shared_ptr<University> university) 
    {
        std::vector<std::string> values;
        values.push_back(to_string(university->id));
        values.push_back(to_string(university->coordinate.m_latitude));
        values.push_back(to_string(university->coordinate.m_longitude));
        return values;
    }
};

} // namespace gen
} // namespace stride
