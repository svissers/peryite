#ifndef VISUALIZATIONGRAPHBAR_H
#define VISUALIZATIONGRAPHBAR_H

#include <QPointF>

class VisualizationGraphBar
{
public:
    VisualizationGraphBar();

    VisualizationGraphBar(QPointF pos, qreal w, qreal h, int dist_age, int dist_amount);

    QPointF position; // Position is bottom left of bar
    qreal width;
    qreal height;
    int age;
    int amount;

    bool containsPoint(QPointF point);
};

#endif // VISUALIZATIONGRAPHBAR_H
