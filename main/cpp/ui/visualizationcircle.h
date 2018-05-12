#ifndef VISUALIZATIONCIRCLE_H
#define VISUALIZATIONCIRCLE_H

#include "geogridlocation.h"
#include "util/GeometryGeoCoord.h"

#include <QPointF>
#include <QString>

class VisualizationCircle
{
public:
    VisualizationCircle();
    VisualizationCircle(QPointF pos, qreal r); // For debugging and testing purposes only
    VisualizationCircle(GeoGridLocation *gloc);
    VisualizationCircle(stride::util::spherical_point coord);

    QPointF position;
    qreal radius;
    GeoGridLocation *geoGridLocation;

    bool containsPoint(QPointF point);
    void increasePop(int amount);
};

#endif // VISUALIZATIONCIRCLE_H
