#include "visualizationgraphbar.h"

VisualizationGraphBar::VisualizationGraphBar()
{

}

VisualizationGraphBar::VisualizationGraphBar(QPointF pos, qreal w, qreal h) :
    position(pos),
    width(w),
    height(h)
{

}

bool VisualizationGraphBar::containsPoint(QPointF point) {
    return (
        (point.x() >= position.x())
     && (point.x() <= position.x() + width)
     && (point.y() <= 400)
     && (point.y() >= 0)
    );

    // Y check is not for the actual rectangle, but for the complete graphicsview, because it's easier for the user that way.
}
