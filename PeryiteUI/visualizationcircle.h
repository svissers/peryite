#ifndef VISUALIZATIONCIRCLE_H
#define VISUALIZATIONCIRCLE_H

#include<QPointF>

class VisualizationCircle
{
public:
    VisualizationCircle();
    VisualizationCircle(QPointF pos, qreal r);

    QPointF position;
    qreal radius;

    bool containsPoint(QPointF point);
};

#endif // VISUALIZATIONCIRCLE_H
