#ifndef MAPVIEWERWINDOW_H
#define MAPVIEWERWINDOW_H

#include "pop/Population.h"
#include "ui/visualizationcircle.h"

#include <QWidget>
#include <QImage>
#include <QGraphicsPixmapItem>

using namespace stride;

namespace Ui {
class MapViewerWindow;
}

class MapViewerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MapViewerWindow(QWidget *parent = 0);
    ~MapViewerWindow();

    void createPopView(const std::shared_ptr<Population> population);

private:
    Ui::MapViewerWindow *ui;
    QImage *image;
    QTimer *timer;
    QList<VisualizationCircle *> *circles;
    VisualizationCircle *selected;
    QGraphicsScene *gfxScene;
    QGraphicsPixmapItem *gfxItem;

    void closeEvent(QCloseEvent *event);

    // Update and drawing
    void update();
    void updateSelection(QPointF mousePos);
    void draw();
    void drawCircle(QPixmap *pm, VisualizationCircle *circle);
    void hoverCircle(VisualizationCircle *c);
    void noHover();
    VisualizationCircle* getClosestCircle(QPointF mousePos);

    // Parsing
    void addCircle(VisualizationCircle *c);
    VisualizationCircle* findCircle(util::spherical_point coord);
    int getMinimumPop();
    int getMaximumPop();
};

#endif // MAPVIEWERWINDOW_H
