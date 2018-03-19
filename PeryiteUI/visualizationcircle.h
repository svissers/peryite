#ifndef VISUALIZATIONCIRCLE_H
#define VISUALIZATIONCIRCLE_H

#include <QPointF>
#include <QString>
#include "geogridlocation.h"

class VisualizationCircle
{
public:
    VisualizationCircle();
    VisualizationCircle(QPointF pos, qreal r); // For debugging only
    VisualizationCircle(GeoGridLocation *gloc);

    QPointF position;
    qreal radius;
    GeoGridLocation *geoGridLocation;

    bool containsPoint(QPointF point);

    static QPointF GCSToQPointF(float longitude, float latitude);
};

#endif // VISUALIZATIONCIRCLE_H
