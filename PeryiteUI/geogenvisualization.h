#ifndef GEOGENVISUALIZATION_H
#define GEOGENVISUALIZATION_H

#include <QWidget>
#include <QImage>
#include <QList>
#include <QGraphicsPixmapItem>
#include "geogendata.h"
#include "visualizationcircle.h"

namespace Ui {
class GeoGenVisualization;
}

class GeoGenVisualization : public QWidget
{

public:
    explicit GeoGenVisualization(QWidget *parent = 0);
    ~GeoGenVisualization();

    void parseData(GeoGenData *data);

private slots:

private:
    Ui::GeoGenVisualization *ui;
    QImage *image;
    QTimer *timer;
    QList<VisualizationCircle *> *circles;
    VisualizationCircle *selected;
    QGraphicsScene *gfxScene;
    QGraphicsPixmapItem *gfxItem;

    void closeEvent(QCloseEvent *event);

    void update();
    void updateSelection(QPointF mousePos);
    void draw();
    void drawCircle(QPixmap *pm, QPointF point, float radius, bool selected);
    void addCircle(VisualizationCircle *c);
    void hoverCircle(VisualizationCircle *c);
    void noHover();

    static QString formatInt(int number);
};

#endif // GEOGENVISUALIZATION_H
