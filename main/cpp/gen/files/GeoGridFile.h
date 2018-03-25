#pragma once
#include "GenFile.h"
#include "../GeoGrid.h"

namespace stride {
namespace gen {
namespace files {

/**
 *
 */
class GeoGridFile : public GenFile
{
private:
    GeoGrid m_grid;
public:
    GeoGridFile(GenConfiguration& config) : GenFile(config) {}

    GeoGridFile(GenConfiguration& config, GeoGrid grid)
    : GenFile(config)
    {
        m_grid = grid;
        m_file_name = "Geogrid.csv";
        m_labels = {"id","latitude","longitude","name","province","population"};
    }

    std::vector<std::vector<std::shared_ptr<GenStruct>>> read()
    { return std::vector<std::vector<std::shared_ptr<GenStruct>>>(); }

    // TODO: move the method definitions into a cpp file.
    void write()
    {
        if (m_grid.size() == 0)
            return;
        std::string file_path = m_out_dir.string()+"/"+m_file_name;
        std::ofstream my_file{file_path};
        if(my_file.is_open()) {
            my_file << boost::algorithm::join(m_labels,",") << "\n";
                for (auto center : m_grid) {
                    my_file << boost::algorithm::join(getValues(center),",") << "\n";
                }
            my_file.close();
        }
    }

    GeoGrid readGrid()
    {
        if (m_grid.size() != 0)
            return m_grid;
        // Populate the geogrid and return it.
        std::string file_path = m_out_dir.string()+"/"+m_file_name;
        util::CSV struct_data(file_path);
        for (util::CSVRow const & row : struct_data) {
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
            m_grid.push_back(center);
        }
        return m_grid;
    }

private:

    std::shared_ptr<GenStruct> getStruct(util::CSVRow const & row)
    {
        return std::shared_ptr<GenStruct>();
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
