#pragma once
#include "GenFile.h"
#include "../GeoGrid.h"
#include <string>
#include <boost/algorithm/string.hpp>

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
    /// Constructor. Constructs the GeoGridFile object using the config.
    /// @param config           The geopop configuration to be used.
    GeoGridFile(GenConfiguration& config) : GenFile(config) {}

    /// Constructor. Constructs the GeoGridFile object with an existing grid.
    /// @param config           The geopop configuration to be used.
    /// @param grid             The geogrid that the file will contain.
    GeoGridFile(GenConfiguration& config, GeoGrid grid)
    : GenFile(config)
    {
        m_grid = grid;
        m_file_name = "Geogrid.csv";
        m_labels = {"id","latitude","longitude","name","province","population", "is_fragmented", "fragmented_populations", "fragmented_lats", "fragmented_longs"};
    }

    /// Read should not be used, use ReadGrid instead.
    /// @return     A brand new vector of genstructs.
    std::vector<std::vector<std::shared_ptr<GenStruct>>> Read()
    { return std::vector<std::vector<std::shared_ptr<GenStruct>>>(); }

    /// Writes the geogrid that the file contains to a file.
    void write()
    {
        if (m_grid.size() == 0)
            return;
        std::ofstream my_file{m_file_path.string()};
        if(my_file.is_open()) {
            my_file << boost::algorithm::join(m_labels,",") << "\n";
                for (auto center : m_grid) {
                    my_file << boost::algorithm::join(GetValues(center),",") << "\n";
                }
            my_file.close();
        }
    }

    /// Reads the geogrid from a file or returns it if it already exists.
    /// @return     The geogrid that the file contains.
    GeoGrid ReadGrid()
    {
        if (m_grid.size() != 0)
            return m_grid;
        // Populate the geogrid and return it.
        m_grid.m_min_long = 90;
        m_grid.m_max_long = 0;
        util::CSV struct_data(m_file_path.string());
        for (util::CSVRow const & row : struct_data) {
            double longitude = row.GetValue<double>("longitude");
            auto center = std::make_shared<UrbanCenter>(UrbanCenter(
                row.GetValue<unsigned int>("id"),
                row.GetValue<unsigned int>("population"),
                row.GetValue<std::string>("name"),
                row.GetValue<int>("province"),
                util::GeoCoordinate(
                    row.GetValue<double>("latitude"),
                    longitude
                    )
                )
            );
            center->is_fragmented = row.GetValue<bool>("is_fragmented");
            std::vector<std::string> populations;
            auto pops_str = row.GetValue<std::string>("fragmented_populations");
            boost::split(
                populations,
                pops_str,
                boost::is_any_of(";")
            );
            for (const auto& pop : populations)
                center->fragmented_populations.push_back(std::stoul(pop));

            std::vector<std::string> lats;
            std::vector<std::string> longs;
            auto lats_str   = row.GetValue<std::string>("fragmented_lats");
            auto longs_str  = row.GetValue<std::string>("fragmented_longs");
            boost::split(lats, lats_str, boost::is_any_of(";"));
            boost::split(longs, longs_str, boost::is_any_of(";"));
            for (unsigned int i = 0; i < lats.size(); i++) {
                double lat = std::stod(lats[i]);
                double lon = std::stod(longs[i]);
                center->fragmented_coords.push_back(util::GeoCoordinate(lat,lon));
            }

            if(longitude > m_grid.m_max_long){
                m_grid.m_max_long = longitude;
            }
            else if(longitude < m_grid.m_min_long){
                m_grid.m_min_long = longitude;
            }
            m_grid.push_back(center);
        }
        return m_grid;
    }

private:

    /// GetStruct should not be used.
    /// @return     A brand new empty genstruct.
    std::shared_ptr<GenStruct> GetStruct(util::CSVRow const & row)
    {
        return std::shared_ptr<GenStruct>();
    }

    /// Returns the values of a urban center in string format.
    /// @param g_struct         The urban center that contains the values.
    /// @return                 Vector of the values of the urban center in string format.
    std::vector<std::string> GetValues(std::shared_ptr<GenStruct> g_struct)
    {
        std::shared_ptr<UrbanCenter> center = std::static_pointer_cast<UrbanCenter>(g_struct);
        std::vector<std::string> populations;
        for (const auto& pop : center->fragmented_populations) {
            populations.push_back(std::to_string(pop));
        }
        std::vector<std::string> lats;
        std::vector<std::string> longs;
        for (const auto& coord : center->fragmented_coords) {
            lats.push_back(std::to_string(coord.m_latitude));
            longs.push_back(std::to_string(coord.m_longitude));
        }
        std::vector<std::string> values = {
            std::to_string(center->id),
            std::to_string(center->coordinate.m_latitude),
            std::to_string(center->coordinate.m_longitude),
            center->name,
            std::to_string(center->province),
            std::to_string(center->population),
            std::to_string(center->is_fragmented),
            boost::algorithm::join(populations,";"),
            boost::algorithm::join(lats,";"),
            boost::algorithm::join(longs,";")
        };
        return values;
    }
};

typedef std::shared_ptr<GeoGridFile> GeoGridFilePtr;

} // namespace files
} // namespace gen
} // namespace stride
