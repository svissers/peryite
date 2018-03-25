#include "peryiteuidata.h"

PeryiteUIData::PeryiteUIData()
{
    configFile = "";
    outputFolder = "";
    geogenData = new GeoGenData();
}

bool PeryiteUIData::setGeoGenData(QString folder, QStringList &missingFiles) {
    return geogenData->setFilenames(folder, missingFiles);
}
