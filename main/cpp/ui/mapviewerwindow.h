#ifndef MAPVIEWERWINDOW_H
#define MAPVIEWERWINDOW_H

#include "pop/Population.h"
#include "ui/visualizationcircle.h"
#include "util/StatisticsRetriever.h"
#include "pool/ContactPoolSys.h"

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

    // MapViewer calls
    void createPopView(const std::shared_ptr<Population> population);
    void updateInfected(const std::shared_ptr<Population> population);
    void updateDaysLabel(int day);
    void focusFlanders();
    void setupStatisticsRetriever(std::shared_ptr<Population> population);

private slots:
    void on_getExtraStats_clicked();

private:
    Ui::MapViewerWindow *ui;
    QImage *image;
    QTimer *timer;
    QList<VisualizationCircle *> *circles;
    VisualizationCircle *selected;
    QGraphicsScene *gfxScene;
    QGraphicsPixmapItem *gfxItem;
    util::StatisticsRetriever *statsRetriever;
    std::shared_ptr<Population> m_pop;

    void closeEvent(QCloseEvent *event);
    void loadIcon();

    // Update and drawing
    void update();
    void updateSelection(QPointF mousePos);
    void draw();
    void drawCircle(QPixmap *pm, VisualizationCircle *circle);
    void drawLines(QPixmap *pm, VisualizationCircle *circle);
    void hoverCircle(VisualizationCircle *c);
    void noHover();
    VisualizationCircle* getClosestCircle(QPointF mousePos);

    // Parsing
    void addCircle(VisualizationCircle *c);
    VisualizationCircle* findCircle(util::spherical_point coord);
    util::spherical_point findCommutingCoordinate(ContactPoolSys &cps, Person &person);
    int getMinimumPop();
    int getMaximumPop();
};

#endif // MAPVIEWERWINDOW_H
