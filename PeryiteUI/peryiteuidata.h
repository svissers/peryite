#ifndef PERYITEUIDATA_H
#define PERYITEUIDATA_H

#include <QString>
#include "geogendata.h"

class PeryiteUIData
{
public:
    PeryiteUIData();

    void setGeoGenData(QString folder);

    QString configFile;
    QString geoGenFolder;
    QString popGenFile;
    GeoGenData *geogenData;
};

#endif // PERYITEUIDATA_H
