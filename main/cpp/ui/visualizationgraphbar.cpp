#include "visualizationgraphbar.h"

VisualizationGraphBar::VisualizationGraphBar()
{

}

VisualizationGraphBar::VisualizationGraphBar(QPointF pos, qreal w, qreal h, int dist_age, int dist_amount) :
    position(pos),
    width(w),
    height(h),
    age(dist_age),
    amount(dist_amount)
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
