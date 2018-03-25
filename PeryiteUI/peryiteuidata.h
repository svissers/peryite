#ifndef PERYITEUIDATA_H
#define PERYITEUIDATA_H

#include <QStringList>
#include "geogendata.h"

class PeryiteUIData
{
public:
    PeryiteUIData();

    bool setGeoGenData(QString folder, QStringList &missingFiles);

    QString configFile;
    QString outputFolder;
    GeoGenData *geogenData;
};

#endif // PERYITEUIDATA_H
