#include "peryiteuidata.h"

PeryiteUIData::PeryiteUIData()
{
    configFile = "";
    outputFolder = "";
    geogenData = new GeoGenData();
    popgenData = new PopGenData();
}

bool PeryiteUIData::setGeoGenData(QString folder, QStringList &missingFiles) {
    return geogenData->setFilenames(folder, missingFiles);
}

bool PeryiteUIData::setPopGenData(QString folder, QStringList &missingFiles) {
    return popgenData->setFilenames(folder, missingFiles);
}
