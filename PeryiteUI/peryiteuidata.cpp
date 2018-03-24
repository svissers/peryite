#include "peryiteuidata.h"

PeryiteUIData::PeryiteUIData()
{
    configFile = "";
    geoGenFolder = "";
    popGenFile = "";
    geogenData = new GeoGenData();
}

void PeryiteUIData::setGeoGenData(QString folder) {
    geogenData->setFilenames(folder);
}
