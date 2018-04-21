#include "visualizationcircle.h"
#include "util.h"
#include <cmath>
#include <QDebug>

VisualizationCircle::VisualizationCircle()
{

}

VisualizationCircle::VisualizationCircle(QPointF pos, qreal r) :
    position(pos),
    radius(r)
{
    geoGridLocation = new GeoGridLocation();
}

VisualizationCircle::VisualizationCircle(GeoGridLocation *gloc) :
    geoGridLocation(gloc)
{
    radius = std::max(std::sqrt((float)(geoGridLocation->population) / 650), 3.0f);
    position = Util::GCSToQPointF(geoGridLocation->longitude, geoGridLocation->latitude);
}

bool VisualizationCircle::containsPoint(QPointF point) {
    float sqrDistance = std::pow(position.x() - point.x(), 2) + std::pow(position.y() - point.y(), 2);

    return (sqrDistance <= radius * radius);
}