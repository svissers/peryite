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

    // Update and drawing
    void update();
    void updateSelection(QPointF mousePos);
    void draw();
    void drawCircle(QPixmap *pm, VisualizationCircle *c);    
    void hoverCircle(VisualizationCircle *c);
    void noHover();

    // Parsing
    void parseGeoGrid(GeoGenData *data);
    void parseCommunities(GeoGenData *data);
    void parseSchools(GeoGenData *data);
    void parseUniversities(GeoGenData *data);
    void parseWorkplaces(GeoGenData *data);
    void addCircle(VisualizationCircle *c);
    VisualizationCircle* findCircle(float longitude, float latitude);
    void addCommunity(QString csvLine);
    void addSchool(QString csvLine);
    void addUniversity(QString csvLine);
    void addWorkplace(QString csvLine);
};

#endif // GEOGENVISUALIZATION_H
