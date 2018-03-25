#ifndef VISUALIZATIONGRAPHBAR_H
#define VISUALIZATIONGRAPHBAR_H

#include <QPointF>

class VisualizationGraphBar
{
public:
    VisualizationGraphBar();

    VisualizationGraphBar(QPointF pos, qreal w, qreal h);

    QPointF position; // Position is bottom left of bar
    qreal width;
    qreal height;

    bool containsPoint(QPointF point);
};

#endif // VISUALIZATIONGRAPHBAR_H
