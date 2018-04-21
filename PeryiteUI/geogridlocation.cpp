#include "geogridlocation.h"
#include "util.h"
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>

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