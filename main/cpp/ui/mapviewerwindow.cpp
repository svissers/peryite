#include "mapviewerwindow.h"
#include "ui_mapviewerwindow.h"
#include "geogridlocation.h"
#include "util/GeometryGeoCoord.h"
#include "util.h"

#include <QDir>
#include <QGraphicsPixmapItem>
#include <QFile>
#include <QTimer>
#include <QtDebug>
#include <QCloseEvent>

using namespace stride;
using namespace std;

MapViewerWindow::MapViewerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapViewerWindow)
{
    ui->setupUi(this);

    // Background color
    setStyleSheet("QWidget { background-color: #eaeaea; }");

    // Load background image into image
    QString filename = QDir(QCoreApplication::applicationDirPath()).cleanPath("./ui/vlaanderen.png");
    image = new QImage(filename);

    // Setup GraphicsScene
    gfxScene = new QGraphicsScene();
    gfxItem = new QGraphicsPixmapItem();
    gfxScene->addItem(gfxItem);

    // Set timer interval for draw update
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MapViewerWindow::update);
    timer->start(1000 / 30); // 30 fps
}

MapViewerWindow::~MapViewerWindow()
{
    delete ui;
}

void MapViewerWindow::createPopView(const std::shared_ptr<Population> population) {
    circles = new QList<VisualizationCircle *>();

    cout << "MapViewer enabled." << endl;

    for (auto i = 0U; i < population->size(); i++) {
        util::spherical_point coord = population->at(i).GetCoordinate();
        VisualizationCircle *circle = findCircle(coord);

        // Create new circle for new locations
        if (circle == nullptr) {
            VisualizationCircle *newCircle = new VisualizationCircle(coord);
            addCircle(newCircle);
        }
        // Increase circle population for locations that already exist.
        else {
            circle->increasePop(1);
        }
    }

    // Set the minimum and maximum population so the circles can resize themselves accordingly
    VisualizationCircle::minimumRadiusPopulation = getMinimumPop();
    VisualizationCircle::maximumRadiusPopulation = getMaximumPop();

    update();
}

void MapViewerWindow::update() {
    // Get cursor position
    QPoint mousePos = mapFromGlobal(QCursor::pos());

    // Check hover selection for circles
    updateSelection(mousePos);

    // Draw
    draw();
}

void MapViewerWindow::updateSelection(QPointF mousePos) {
    VisualizationCircle *closest = getClosestCircle(mousePos);

    if (closest->containsPoint(mousePos, 3)) {
        hoverCircle(closest);
    } else {
        noHover();
    }
}

void MapViewerWindow::draw() {
    // Load pixmap from image
    QPixmap pixmap = QPixmap::fromImage(*image);

    // Draw circles on the pixmap
    for (int i = 0; i < circles->size(); i++) {
        VisualizationCircle *c = circles->at(i);
        drawCircle(&pixmap, c->position, c->getRadius(), (c == selected));
    }

    // Set pixmap pixmap
    gfxItem->setPixmap(pixmap);

    // Display the scene
    ui->FlandersMap->setScene(gfxScene);
}

void MapViewerWindow::addCircle(VisualizationCircle *c) {
    *circles << c;
}

void MapViewerWindow::drawCircle(QPixmap *pm, QPointF point, float radius, bool selected) {
    // QPainter
    QPainter painter(pm);
    painter.scale(1, 1);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(selected ? QColor("#3c6382") : QColor("#78e08f"));
    painter.setPen(QColor(0, 0, 0, 0));

    // Draw
    painter.drawEllipse(point, radius, radius);
}

void MapViewerWindow::closeEvent(QCloseEvent *event) {
    // timer->stop();
    event->accept();
}

void MapViewerWindow::hoverCircle(VisualizationCircle *c) {
    selected = c;

    /*
    // Title
    QString title = c->geoGridLocation->name;
    ui->CircleInfoTitle->setText(title);

    // Info text
    QString text = "Population: " + Util::formatInt(c->geoGridLocation->population);
    text += "\nCommunities: " + Util::formatInt(c->geoGridLocation->communities);
    text += "\nSchools: " + Util::formatInt(c->geoGridLocation->schools);
    text += "\nUniversities: " + Util::formatInt(c->geoGridLocation->universities);
    text += "\nWorkplaces: " + Util::formatInt(c->geoGridLocation->workplaces);
    ui->CircleInfoText->setText(text);
    */
}

void MapViewerWindow::noHover() {
    selected = NULL;
    ui->CircleInfoTitle->setText("");
    ui->CircleInfoText->setText("");
}

VisualizationCircle* MapViewerWindow::findCircle(util::spherical_point coord) {
    for (int i = 0; i < circles->length(); i++) {
        float lng = circles->at(i)->geoGridLocation->longitude;
        float lat = circles->at(i)->geoGridLocation->latitude;

        if ((float)coord.get<0>() == lat && (float)coord.get<1>() == lng) {
            return circles->at(i);
        }
    }

    return nullptr;
}

int MapViewerWindow::getMinimumPop() {
    int minimum = circles->at(0)->geoGridLocation->population;

    for (int i = 0; i < circles->size(); i++) {
        VisualizationCircle *c = circles->at(i);
        int pop = c->geoGridLocation->population;

        if (pop < minimum) {
            minimum = pop;
        }
    }

    return minimum;
}

int MapViewerWindow::getMaximumPop() {
    int maximum = circles->at(0)->geoGridLocation->population;

    for (int i = 0; i < circles->size(); i++) {
        VisualizationCircle *c = circles->at(i);
        int pop = c->geoGridLocation->population;

        if (pop > maximum) {
            maximum = pop;
        }
    }

    return maximum;
}

VisualizationCircle* MapViewerWindow::getClosestCircle(QPointF mousePos) {
    float closestDist = circles->at(0)->sqrDistanceToPoint(mousePos);
    VisualizationCircle *closest = circles->at(0);

    for (int i = 0; i < circles->size(); i++) {
        VisualizationCircle *c = circles->at(i);

        float dist = circles->at(i)->sqrDistanceToPoint(mousePos);

        if (dist < closestDist) {
            closestDist = dist;
            closest = c;
        }
    }

    return closest;
}
