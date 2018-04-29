#pragma once
#include "../structs/GenStruct.h"
#include "../GenConfiguration.h"
#include "../GeoGrid.h"
#include "util/CSV.h"

namespace stride {
namespace gen {
namespace files {

/**
 * A class that can read and write generator data files.
 */
class GenFile
{
public:
    /// Constructor. Constructs the GenFile object using the config.
    /// @param config           The geopop configuration to use.
    GenFile(GenConfiguration& config);

    /// Constructor. Initializes the GenFile object with a vector of communities.
    /// @param config           The geopop configuration to use.
    /// @param structs          The genstructs that the file will contain.
    /// @param geo              The geogrid that was used to build the genstructs.
    GenFile(GenConfiguration& config, std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo);

    /// Writes the genstruct data that the file contains to a file.
    virtual void Write();

    /// Reads the genstruct data from a file or returns it if it already exists.
    /// @return                 The genstructs that the file contains.
    virtual std::vector<std::vector<std::shared_ptr<GenStruct>>> Read();

protected:
    /// The labels for the first line in a csv file.
    std::vector<std::string> m_labels;
    /// The path to the file.
    boost::filesystem::path m_file_path;
    /// The prefix (directory name or prefix to filename) used in the file path.
    std::string m_output_prefix;
    /// The name of the file
    std::string m_file_name;
    /// The struct data the file contains.
    std::vector<std::vector<std::shared_ptr<GenStruct>>> m_sorted_structs;

    /// Returns the genstruct associated with a row in the csv data.
    /// @param row              The row that describes the genstruct.
    /// @return                 The genstruct associated with the csv row.
    virtual std::shared_ptr<GenStruct> GetStruct(util::CSVRow const & row) =0;

    /// Returns the values of a genstruct in string format.
    /// @param g_struct         The genstruct that contains the values.
    /// @return                 Vector of the values of the genstruct in string format.
    virtual std::vector<std::string> GetValues(std::shared_ptr<GenStruct> g_struct) =0;

    /// Sorts the struct data into latitude bands and sets m_sorted_structs.
    /// @param structs          The structs to be inserted.
    /// @param geo              The geogrid that was used to build the genstructs.
    void insertStructs(std::vector<std::shared_ptr<GenStruct>> structs, GeoGrid& geo);
};

} // namespace files
} // namespace gen
} // namespace stride
