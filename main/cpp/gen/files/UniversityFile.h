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

    std::shared_ptr<GenStruct> getStruct(util::CSVRow const & row)
    {
        auto university = std::make_shared<University>(University(
            row.getValue<unsigned int>("id"),
            util::GeoCoordinate(
                row.getValue<double>("latitude"),
                row.getValue<double>("longitude")
                )
            )
        );
        return university;
    }

    std::vector<std::string> getValues(std::shared_ptr<University> university)
    {
        std::vector<std::string> values;
        values.push_back(std::to_string(university->id));
        values.push_back(std::to_string(university->coordinate.m_latitude));
        values.push_back(std::to_string(university->coordinate.m_longitude));
        return values;
    }
};

} // namespace gen
} // namespace stride
