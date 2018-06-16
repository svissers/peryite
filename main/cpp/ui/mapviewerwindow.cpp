#include "ui/mapviewerwindow.h"
#include "ui/ui_mapviewerwindow.h"
#include "ui/geogridlocation.h"
#include "util/GeometryGeoCoord.h"
#include "util.h"

#include <QDir>
#include <QGraphicsPixmapItem>
#include <QFile>
#include <QTimer>
#include <QtDebug>
#include <QCloseEvent>
#include <QIcon>
#include <QScrollBar>
#include <iostream>

using namespace stride;
using namespace std;

MapViewerWindow::MapViewerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapViewerWindow)
{
    ui->setupUi(this);

    this->setFixedSize(QSize(1000, 600));
    loadIcon();

    // Background color
    setStyleSheet("QWidget { background-color: #eaeaea; }");

    // Load background image into image
    QString filename = QDir(QCoreApplication::applicationDirPath()).cleanPath("./ui/map.png");
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

void MapViewerWindow::updateInfected(const std::shared_ptr<Population> population) {
    // Reset infected data
    for (int i = 0; i < circles->size(); i++) {
        circles->at(i)->resetInfected();
    }

    // Rebuild infected data
    for (auto i = 0U; i < population->size(); i++) {
        Person person = population->at(i);

        if (person.GetHealth().IsInfected() || person.GetHealth().IsRecovered()) {
            util::spherical_point coord = person.GetCoordinate();
            VisualizationCircle *circle = findCircle(coord);

            if (circle != nullptr) {
                circle->increaseInfected(1);
            } else {
                cerr << "Circle not found in updateInfected. This shouldn't happen." << endl;
            }
        }
    }
}

void MapViewerWindow::update() {
    // Get cursor position and add scrolling offset to it
    QPoint mousePos = mapFromGlobal(QCursor::pos());
    QPoint offset = QPoint(ui->FlandersMap->horizontalScrollBar()->value(), ui->FlandersMap->verticalScrollBar()->value());

    // Check hover selection for circles
    updateSelection(mousePos + offset);

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
        drawCircle(&pixmap, c);
    }

    // Set pixmap pixmap
    gfxItem->setPixmap(pixmap);

    // Display the scene
    ui->FlandersMap->setScene(gfxScene);
}

void MapViewerWindow::addCircle(VisualizationCircle *c) {
    *circles << c;
}

void MapViewerWindow::drawCircle(QPixmap *pm, VisualizationCircle *circle) {
    // Get params
    qreal radius = circle->getRadius();
    QPointF point = circle->position;

    // QPainter
    QPainter painter(pm);
    painter.scale(1, 1);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(circle->getColor(circle == selected));
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

    // Title
    QString title = "Location Info";
    ui->CircleInfoTitle->setText(title);

    // Info text
    QString text = "Population: " + Util::formatInt(c->geoGridLocation->population);
    text += "\nInfected: " + Util::formatInt(c->geoGridLocation->infected) + " (" + Util::formatInt(c->getInfectedPercent()) + "\%)";
    ui->CircleInfoText->setText(text);
}

void MapViewerWindow::noHover() {
    selected = NULL;
    ui->CircleInfoTitle->setText("");
    ui->CircleInfoText->setText("");
}

void MapViewerWindow::updateDaysLabel(int day) {
    ui->DaysLabel->setText("Day " + QString::number(day));
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


void MapViewerWindow::loadIcon()
{
    setWindowIcon(QIcon("./ui/logo.png"));
}

void MapViewerWindow::focusFlanders()
{
    // Set scrollbars starting value to focus on flanders
    ui->FlandersMap->horizontalScrollBar()->setSliderPosition(781);
    ui->FlandersMap->verticalScrollBar()->setSliderPosition(809);
}