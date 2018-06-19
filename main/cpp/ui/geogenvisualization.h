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

    /// Parses data from guiController and creates all visualization objects
    void parseData();

private slots:

private:
    /// Close event
    void closeEvent(QCloseEvent *event);

    /// Load the window icon
    void loadIcon();

    // -----------------------------------------------------------------------------------------
    // Update and drawing
    // -----------------------------------------------------------------------------------------
    /// Called by a timer
    void update();

    /// Called during update()
    void updateSelection(QPointF mousePos);

    /// Called during update()
    void draw();

    /// Draws a given circle on a given pixmap
    void drawCircle(QPixmap *pm, VisualizationCircle *c);

    /// Function for hovering over a circle (selecting it)
    void hoverCircle(VisualizationCircle *c);

    /// Hovering over nothing
    void noHover();

    /// Set the scrollbars to focus on flanders
    void focusFlanders();

    // -----------------------------------------------------------------------------------------
    // Parsing functions
    // -----------------------------------------------------------------------------------------
    void parseGeoGrid(std::map<unsigned int, stride::gen::files::GeoGridFilePtr> &geo_file, int amountOfRegions);
    void parseCommunities(std::map<unsigned int, stride::gen::files::CommunityFilePtr> &community_file, int amountOfRegions);
    void parseSchools(std::map<unsigned int, stride::gen::files::SchoolFilePtr> &school_file, int amountOfRegions);
    void parseUniversities(std::map<unsigned int, stride::gen::files::UniversityFilePtr> &university_file, int amountOfRegions);
    void parseWorkplaces(std::map<unsigned int, stride::gen::files::WorkplaceFilePtr> &workplace_file, int amountOfRegions);
    void addCircle(VisualizationCircle *c);
    void addCommunity(std::shared_ptr<stride::gen::Community> &community);
    void addSchool(std::shared_ptr<stride::gen::School> &school);
    void addUniversity(std::shared_ptr<stride::gen::University> &university);
    void addWorkplace(std::shared_ptr<stride::gen::WorkPlace> &workplace);

    /// Finds the circle with a given latitude / longitude
    VisualizationCircle* findCircle(float longitude, float latitude);

    Ui::GeoGenVisualization *ui;            /// Link to the ui
    QImage *image;                          /// PNG of the map that is drawn on
    QTimer *timer;                          /// Timer used for the update function
    QList<VisualizationCircle *> *circles;  /// List of all circles that can be drawn
    VisualizationCircle *selected;          /// Reference to circle that is being hovered by the mouse
    QGraphicsScene *gfxScene;               /// Graphics Scene (Qt)
    QGraphicsPixmapItem *gfxItem;           /// Graphics Item (Qt)
    stride::GuiController* guiController;   /// Link to GuiController
};

#endif // GEOGENVISUALIZATION_H
