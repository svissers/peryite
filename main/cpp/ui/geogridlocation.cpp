#include "geogridlocation.h"
#include "util.h"
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>

using namespace stride;

GeoGridLocation::GeoGridLocation()
{

}

GeoGridLocation::GeoGridLocation(QString csvLine) {
    QStringList list = Util::parseCSVLine(csvLine);

    // Read data from CSV
    id = list[0].toInt();
    latitude = list[1].toFloat();
    longitude = list[2].toFloat();
    name = list[3]; // TODO: correct capitalizaton
    province = list[4].toInt();
    population = list[5].toInt();

    // Init all communities, ... to 0 because we will add them when we read the other files
    communities = 0;
    schools = 0;
    universities = 0;
    workplaces = 0;
}

GeoGridLocation::GeoGridLocation(util::spherical_point coord) {
    latitude = coord.get<0>();
    longitude = coord.get<1>();
    population = 1;
    infected = 0;
}

void GeoGridLocation::increasePop(int amount) {
    population += amount;
}

void GeoGridLocation::resetInfected() {
    infected = 0;
}

void GeoGridLocation::increaseInfected(int amount) {
    infected += amount;
}

qreal GeoGridLocation::getInfectedPercent() {
    return 100 * infected / population;
}
