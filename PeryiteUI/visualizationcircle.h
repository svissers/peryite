#ifndef VISUALIZATIONCIRCLE_H
#define VISUALIZATIONCIRCLE_H

#include<QPointF>
#include<QString>

class VisualizationCircle
{
public:
    VisualizationCircle();
    VisualizationCircle(QPointF pos, qreal r);
    VisualizationCircle(QPointF pos, qreal r, QString info);

    QPointF position;
    qreal radius;
    QString info; // Placeholder until we have the input files and format

    bool containsPoint(QPointF point);
};

#endif // VISUALIZATIONCIRCLE_H
