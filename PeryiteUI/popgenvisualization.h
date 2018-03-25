#ifndef POPGENVISUALIZATION_H
#define POPGENVISUALIZATION_H

#include "visualizationgraphbar.h"
#include <QWidget>
#include <QGraphicsPixmapItem>

namespace Ui {
class PopGenVisualization;
}

class PopGenVisualization : public QWidget
{
    Q_OBJECT

public:
    explicit PopGenVisualization(QWidget *parent = 0);
    ~PopGenVisualization();

    void parseData(QString popgenFilePath);

private:
    static const int maxAge = 99;

    Ui::PopGenVisualization *ui;
    int ageDistribution[maxAge + 1];
    QImage *image;
    QTimer *timer;
    QList<VisualizationGraphBar *> *bars;
    VisualizationGraphBar *selected;
    QGraphicsScene *gfxScene;
    QGraphicsPixmapItem *gfxItem;

    void closeEvent(QCloseEvent *event);

    void update();
    void draw();
    void updateSelection(QPointF mousePos);
    void hoverBar(VisualizationGraphBar *b);
    void noHover();
    void drawBar(QPixmap *pm, QPointF point, float width, float height, bool selected);
    void addBar(VisualizationGraphBar *b);

    void addAge(int age);
};

#endif // POPGENVISUALIZATION_H
