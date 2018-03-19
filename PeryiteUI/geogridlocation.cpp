#include "geogridlocation.h"
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>

GeoGridLocation::GeoGridLocation()
{

}

GeoGridLocation::GeoGridLocation(QString csvLine) {
    // Remove the newline character and split into strings
    csvLine.truncate(csvLine.lastIndexOf(QChar('\n')));
    QStringList list = csvLine.split(QRegularExpression(", "));

    id = list[0].toInt();
    latitude = list[1].toFloat();
    longitude = list[2].toFloat();
    name = list[3]; // TODO: correct capitalizaton
    province = list[4].toInt();
    population = list[5].toInt();
}
