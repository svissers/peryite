#include "peryiteuidata.h"

PeryiteUIData::PeryiteUIData()
{
    configFile = "";
    geoGenFolder = "";
    geogenData = new GeoGenData();
}

void PeryiteUIData::setGeoGenData(QString folder) {
    geogenData->setFilenames(folder);
}
