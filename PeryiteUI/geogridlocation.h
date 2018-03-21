#ifndef GEOGRIDLOCATION_H
#define GEOGRIDLOCATION_H

#include <QString>

class GeoGridLocation
{
public:
    GeoGridLocation();
    GeoGridLocation(QString csvLine);

    int id;
    float latitude;
    float longitude;
    QString name;
    int province;
    int population;
    int communities;
    int schools;
    int universities;
    int workplaces;
};

#endif // GEOGRIDLOCATION_H
