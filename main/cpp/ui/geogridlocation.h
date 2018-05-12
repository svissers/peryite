#ifndef GEOGRIDLOCATION_H
#define GEOGRIDLOCATION_H

#include "util/GeometryGeoCoord.h"

#include <QString>

class GeoGridLocation
{
public:
    GeoGridLocation();
    GeoGridLocation(QString csvLine);
    GeoGridLocation(stride::util::spherical_point coord);

    void increasePop(int amount);

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
