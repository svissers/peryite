#ifndef GEOGRIDLOCATION_H
#define GEOGRIDLOCATION_H

#include "util/GeometryGeoCoord.h"

#include <QString>

class GeoGridLocation
{
public:
    /// Constructors
    GeoGridLocation();
    GeoGridLocation(QString csvLine);
    GeoGridLocation(stride::util::spherical_point coord);
    GeoGridLocation(stride::util::spherical_point coord, int pop, std::string ucname);

    /// Increases the population by a certain amount
    void increasePop(int amount);

    /// Resets the amount of infected people to 0
    void resetInfected();

    /// Increases the amount of infected people by a given amount
    void increaseInfected(int amount);

    /// Returns the percentage (0 - 100) of infected people compared to the total population
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
