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
    missingFiles = Util::getMissingFiles(path, requiredFiles);

    // -----------------------------------------------------------------------------------------
    // Set ui parameters
    // -----------------------------------------------------------------------------------------
    if (!missingFiles.isEmpty()) { return false; }

    // -----------------------------------------------------------------------------------------
    // Set the data
    // -----------------------------------------------------------------------------------------
    communitiesFile = path + "/" + requiredFiles[0];
    geogridFile = path + "/" + requiredFiles[1];
    schoolsFile = path + "/" + requiredFiles[2];
    universitiesFile = path + "/" + requiredFiles[3];
    workplacesFile = path + "/" + requiredFiles[4];

    return true;
}
