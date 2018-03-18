#ifndef GEOGENDATA_H
#define GEOGENDATA_H

#include <QString>

class GeoGenData
{
public:
    GeoGenData();

    void setFilenames(QString folder);

    QString communitiesFile;
    QString geogridFile;
    QString schoolsFile;
    QString universitiesFile;
    QString workplacesFile;
};

#endif // GEOGENDATA_H
