#include "geogendata.h"
#include "util.h"
#include <QStringList>
#include <QMessageBox>
#include <QObject>

GeoGenData::GeoGenData()
{

}

bool GeoGenData::setFilenames(QString path, QStringList &missingFiles) {
    QStringList requiredFiles;
    requiredFiles << "Communities.csv" << "Geogrid.csv" << "Schools.csv" << "Universities.csv" << "Workplaces.csv";
    missingFiles = getMissingFiles(path, requiredFiles);

    // Stop if data is missing
    if (!missingFiles.isEmpty()) { return false; }

    // Set the data
    communitiesFile = path + "/" + requiredFiles[0];
    geogridFile = path + "/" + requiredFiles[1];
    schoolsFile = path + "/" + requiredFiles[2];
    universitiesFile = path + "/" + requiredFiles[3];
    workplacesFile = path + "/" + requiredFiles[4];

    return true;
}

QStringList GeoGenData::getMissingFiles(QString path, QStringList requiredFiles) {
    // Check if required files exist
    QStringList missingFiles;

    for(int i = 0; i < requiredFiles.size(); i++) {
        if (!Util::fileExists(path + "/" + requiredFiles[i])) {
            missingFiles.append(requiredFiles[i]);
        }
    }

    return missingFiles;
}
