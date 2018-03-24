#pragma once
#include "GenFile.h"

namespace stride {
namespace gen {
namespace files {

/**
 *
 */
class GeoGridFile : public GenFile
{
public:
    GeoGridFile(GenConfiguration& config) : GenFile(config) {}

    GeoGridFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo)
    : GenFile(config, structs, geo)
    {
        m_file_name = "Geogrid.csv";
        m_labels = {"id","latitude","longitude","name","province","population","band"};
    }
private:
    std::shared_ptr<GenStruct> getStruct(util::CSVRow const & row)
    {
        auto center = std::make_shared<UrbanCenter>(UrbanCenter(
            row.getValue<unsigned int>("id"),
            row.getValue<unsigned int>("population"),
            row.getValue<std::string>("name"),
            row.getValue<int>("province"),
            util::GeoCoordinate(
                row.getValue<double>("latitude"),
                row.getValue<double>("longitude")
                )
            )
        );
        return center;
    }

    std::vector<std::string> getValues(std::shared_ptr<GenStruct> g_struct)
    {
        std::shared_ptr<UrbanCenter> center = std::static_pointer_cast<UrbanCenter>(g_struct);
        std::vector<std::string> values = {
            std::to_string(center->id),
            std::to_string(center->coordinate.m_latitude),
            std::to_string(center->coordinate.m_longitude),
            center->name,
            std::to_string(center->province),
            std::to_string(center->population)
        };
        return values;
    }
};

typedef std::shared_ptr<GeoGridFile> GeoGridFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
