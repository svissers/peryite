#ifndef PERYITEUIDATA_H
#define PERYITEUIDATA_H

#include <QStringList>
#include "geogendata.h"
#include "popgendata.h"

class PeryiteUIData
{
public:
    PeryiteUIData();

    bool setGeoGenData(QString folder, QStringList &missingFiles);
    bool setPopGenData(QString folder, QStringList &missingFiles);

    QString configFile;
    QString outputFolder;
    GeoGenData *geogenData;
    PopGenData *popgenData;
};

#endif // PERYITEUIDATA_H
