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
    void resetInfected();
    void increaseInfected(int amount);
    qreal getInfectedPercent();

    int id;
    float latitude;
    float longitude;
    QString name;
    int province;
    int population;
    int infected;
    int communities;
    int schools;
    int universities;
    int workplaces;
};

#endif // GEOGRIDLOCATION_H
