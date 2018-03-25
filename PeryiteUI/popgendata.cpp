#include "popgendata.h"
#include "util.h"

PopGenData::PopGenData()
{

}

bool PopGenData::setFilenames(QString path, QStringList &missingFiles) {
    // TODO: Implement this once we know what files we need

//    QStringList requiredFiles;
//    requiredFiles << "Communities.csv" << "Geogrid.csv" << "Schools.csv" << "Universities.csv" << "Workplaces.csv";
//    missingFiles = Util::getMissingFiles(path, requiredFiles);

//    // Stop if data is missing
//    if (!missingFiles.isEmpty()) { return false; }

    // Set the data
    // popFile = path + "/" + requiredFiles[0];

    // PLACEHOLDER
    popFile = "/home/phalcon/Eindwerk/peryite/cmake-build-release/installed/data/pop_flanders600.csv";

    return true;
}
