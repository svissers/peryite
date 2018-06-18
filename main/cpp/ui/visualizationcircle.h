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

private:
    static qreal minimumRadius;
    static qreal maximumRadius;
};

#endif // VISUALIZATIONCIRCLE_H
