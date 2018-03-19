#include "visualizationcircle.h"
#include <cmath>
#include <QDebug>

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
    radius = std::sqrt((float)(geoGridLocation->population) / 750);
    position = GCSToQPointF(geoGridLocation->longitude, geoGridLocation->latitude);
}

bool VisualizationCircle::containsPoint(QPointF point) {
    float sqrDistance = std::pow(position.x() - point.x(), 2) + std::pow(position.y() - point.y(), 2);

    return (sqrDistance <= radius * radius);
}

QPointF VisualizationCircle::GCSToQPointF(float longitude, float latitude) {
    // These conversions are specific to our png image, calculated by hand.
    // Not sure if there is a better way to go about this.
    float x = 263 * longitude - 606.7;
    float y = -475 * latitude + 24465;

    return QPointF(x, y);
}
