#include "visualizationcircle.h"
#include <cmath>

VisualizationCircle::VisualizationCircle()
{

}

VisualizationCircle::VisualizationCircle(QPointF pos, qreal r) :
    position(pos),
    radius(r)
{

}

VisualizationCircle::VisualizationCircle(GeoGridLocation *gloc) :
    geoGridLocation(gloc)
{
    radius = (float)(geoGridLocation->population) / 3000;
    position = GCSToQPointF(geoGridLocation->latitude, geoGridLocation->longitude);
}

bool VisualizationCircle::containsPoint(QPointF point) {
    float sqrDistance = std::pow(position.x() - point.x(), 2) + std::pow(position.y() - point.y(), 2);

    return (sqrDistance <= radius * radius);
}

QPointF VisualizationCircle::GCSToQPointF(float longitude, float latitude) {
    float x = longitude;
    float y = latitude;

    return QPointF(x, y);
}
