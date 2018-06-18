#ifndef POPGENVISUALIZATION_H
#define POPGENVISUALIZATION_H

#include "visualizationgraphbar.h"
#include "popgendata.h"
#include "sim/GuiController.h"
#include "gen/files/GenDirectory.h"

#include <QWidget>
#include <QGraphicsPixmapItem>

namespace Ui {
class PopGenVisualization;
}

class PopGenVisualization : public QWidget
{
    Q_OBJECT

public:
    explicit PopGenVisualization(stride::GuiController *guiCtrl, QWidget *parent = 0);
    ~PopGenVisualization();

    void parseData();

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
    stride::GuiController* guiController;

    void closeEvent(QCloseEvent *event);
    void loadIcon();

    void update();
    void draw();
    void updateSelection(QPointF mousePos);
    void hoverBar(VisualizationGraphBar *b);
    void noHover();
    void drawBar(QPixmap *pm, QPointF point, float width, float height, bool selected);
    void addBar(VisualizationGraphBar *b);
    int getLargestAgeCategorySize();
    void createAgeDistributionGraph();

    void addAge(int age);
};

#endif // POPGENVISUALIZATION_H
