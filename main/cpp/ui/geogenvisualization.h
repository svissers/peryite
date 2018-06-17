#ifndef GEOGENVISUALIZATION_H
#define GEOGENVISUALIZATION_H

#include "geogendata.h"
#include "visualizationcircle.h"
#include "sim/GuiController.h"
#include "gen/files/GenDirectory.h"
#include "gen/structs/Community.h"
#include "gen/structs/School.h"
#include "gen/structs/University.h"
#include "gen/structs/Workplace.h"

#include <QWidget>
#include <QImage>
#include <QList>
#include <QGraphicsPixmapItem>

namespace Ui {
class GeoGenVisualization;
}

class GeoGenVisualization : public QWidget
{

public:
    explicit GeoGenVisualization(stride::GuiController *guiCtrl, QWidget *parent = 0);
    ~GeoGenVisualization();

    void parseData();

private slots:

private:
    Ui::GeoGenVisualization *ui;
    QImage *image;
    QTimer *timer;
    QList<VisualizationCircle *> *circles;
    VisualizationCircle *selected;
    QGraphicsScene *gfxScene;
    QGraphicsPixmapItem *gfxItem;
    stride::GuiController* guiController;

    void closeEvent(QCloseEvent *event);
    void loadIcon();

    // Update and drawing
    void update();
    void updateSelection(QPointF mousePos);
    void draw();
    void drawCircle(QPixmap *pm, VisualizationCircle *c);
    void hoverCircle(VisualizationCircle *c);
    void noHover();
    void focusFlanders();

    // Parsing
    void parseGeoGrid(std::map<unsigned int, stride::gen::files::GeoGridFilePtr> &geo_file, int amountOfRegions);
    void parseCommunities(std::map<unsigned int, stride::gen::files::CommunityFilePtr> &community_file, int amountOfRegions);
    void parseSchools(std::map<unsigned int, stride::gen::files::SchoolFilePtr> &school_file, int amountOfRegions);
    void parseUniversities(std::map<unsigned int, stride::gen::files::UniversityFilePtr> &university_file, int amountOfRegions);
    void parseWorkplaces(std::map<unsigned int, stride::gen::files::WorkplaceFilePtr> &workplace_file, int amountOfRegions);
    void addCircle(VisualizationCircle *c);
    VisualizationCircle* findCircle(float longitude, float latitude);
    void addCommunity(std::shared_ptr<stride::gen::Community> &community);
    void addSchool(std::shared_ptr<stride::gen::School> &school);
    void addUniversity(std::shared_ptr<stride::gen::University> &university);
    void addWorkplace(std::shared_ptr<stride::gen::WorkPlace> &workplace);
};

#endif // GEOGENVISUALIZATION_H
