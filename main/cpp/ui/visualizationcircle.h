#ifndef VISUALIZATIONCIRCLE_H
#define VISUALIZATIONCIRCLE_H

#include "geogridlocation.h"
#include "util/GeometryGeoCoord.h"

#include <QPointF>
#include <QString>
#include <QColor>

class VisualizationCircle
{
public:
    static int minimumRadiusPopulation;
    static int maximumRadiusPopulation;

    VisualizationCircle();
    VisualizationCircle(QPointF pos, int r); // For debugging and testing purposes only
    VisualizationCircle(GeoGridLocation *gloc);
    VisualizationCircle(stride::util::spherical_point coord);

    QPointF position;
    GeoGridLocation *geoGridLocation;

    bool containsPoint(QPointF point, qreal margin = 0.0f);
    void increasePop(int amount);
    void resetInfected();
    void increaseInfected(int amount);
    qreal getRadius();
    qreal sqrDistanceToPoint(QPointF point);
    QColor getColor(bool selected);
    qreal getInfectedPercent();
    int getPopulation();

    /// Commuting
    void addCommuter(VisualizationCircle* destination);
    unsigned int getCommuters(VisualizationCircle* destination);

    /// Stores pointers to all other circles (key), where >0 persons commute from this circle to the other circle.
    /// value is the amount of people commuting over this connection.
    std::map<VisualizationCircle *, unsigned int> commutingConnections;

private:
    static qreal minimumRadius;
    static qreal maximumRadius;
};

#endif // VISUALIZATIONCIRCLE_H
