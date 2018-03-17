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

VisualizationCircle::VisualizationCircle(QPointF pos, qreal r, QString i) :
    position(pos),
    radius(r),
    info(i)
{

}

bool VisualizationCircle::containsPoint(QPointF point) {
    float sqrDistance = std::pow(position.x() - point.x(), 2) + std::pow(position.y() - point.y(), 2);

    return (sqrDistance <= radius * radius);
}
