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
    /// Constructor & Destructor
    explicit PopGenVisualization(stride::GuiController *guiCtrl, QWidget *parent = 0);
    ~PopGenVisualization();
    
    /// Parses data from guiController and creates all visualization objects
    void parseData();

private:
    static const int maxAge = 99;

    /// Close event
    void closeEvent(QCloseEvent *event);
    /// Loads the icon for the window
    void loadIcon();

    /// Update is called by the timer
    void update();

    /// Called by update
    void draw();

    /// Called by update
    void updateSelection(QPointF mousePos);

    /// Hover over a bar in the graph
    void hoverBar(VisualizationGraphBar *b);
    
    /// Hover over nothing
    void noHover();

    /// Draws a bar with given dimensions on the pixmap
    void drawBar(QPixmap *pm, QPointF point, float width, float height, bool selected);

    /// Adds a bar to the list of bars
    void addBar(VisualizationGraphBar *b);

    /// Returns the size of the category with the largest size
    int getLargestAgeCategorySize();

    /// Creates the age distribution graph
    void createAgeDistributionGraph();

    /// Increment the distribution for the given age
    void addAge(int age);

    Ui::PopGenVisualization *ui;            /// Link to the ui
    int ageDistribution[maxAge + 1];        /// Age distribution
    QImage *image;                          /// Image used to draw the graph
    QTimer *timer;                          /// Timer calls update
    QList<VisualizationGraphBar *> *bars;   /// List of all bars that will be drawn
    VisualizationGraphBar *selected;        /// The bar that the user is currently hovering over
    QGraphicsScene *gfxScene;               /// Graphics Scene (Qt)
    QGraphicsPixmapItem *gfxItem;           /// Graphics Item (Qt)
    stride::GuiController* guiController;   /// Link to guicontroller
};

#endif // POPGENVISUALIZATION_H
