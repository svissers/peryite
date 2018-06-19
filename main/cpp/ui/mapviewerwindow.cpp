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
#include <QMessageBox>
#include <iostream>

using namespace std;
using namespace stride;
using namespace stride::util;

MapViewerWindow::MapViewerWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapViewerWindow)
{
    ui->setupUi(this);

    this->setFixedSize(QSize(1000, 696));
    statsRetriever = nullptr;
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

void MapViewerWindow::on_getExtraStats_clicked() {
    // -----------------------------------------------------------------------------------------
    // Retrieve the stats
    // ----------------------------------------------------------------------------------------- 
    util::spherical_point center = spherical_point(ui->latitude->value(), ui->longitude->value());
    auto stats = statsRetriever->GetStatisticsOfArea(m_pop, center, ui->radius->value());

    // -----------------------------------------------------------------------------------------
    // Construct the string we will be displaying
    // -----------------------------------------------------------------------------------------
    QString statsStr = "";
    QString titleStr = "";
    
    titleStr += "Statistics for (" + QString::number(ui->latitude->value()) + ", " + QString::number(ui->longitude->value()) + "), ";
    titleStr += "radius " + QString::number(ui->radius->value()) + "km";

    statsStr += "Population in radius: " + QString::number(std::get<0>(stats)) + "\n";
    statsStr += "Infected in radius: " + QString::number(std::get<1>(stats)) + "\n";
    statsStr += "Schoolgoing in radius: " + QString::number(std::get<2>(stats)) + "\n";
    statsStr += "Sick and schoolgoing in radius: " + QString::number(std::get<3>(stats)) + "\n";
    statsStr += "Working in radius: " + QString::number(std::get<4>(stats)) + "\n";
    statsStr += "Sick and working in radius: " + QString::number(std::get<5>(stats)) + "\n";
    statsStr += "Unemployed in radius: " + QString::number(std::get<6>(stats)) + "\n";
    statsStr += "Sick and enemployed in radius: " + QString::number(std::get<7>(stats)) + "\n";

    /// Display the information
    QMessageBox::information(this, titleStr, statsStr);
}

void MapViewerWindow::createPopView(const std::shared_ptr<Population> population) {
    cout << "MapViewer enabled." << endl;
    
    // -----------------------------------------------------------------------------------------
    // Use the population to create a list of visualization circles.
    // Since we are in the mapviewer here, we also assign commuting connections.
    // -----------------------------------------------------------------------------------------
    circles = new QList<VisualizationCircle *>();
    m_pop = population;

    // -----------------------------------------------------------------------------------------
    // Start by creating our list of visualizationcircles
    // This ensures that, when we start adding commuting connections,
    // All the circles already exist.
    // -----------------------------------------------------------------------------------------
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

    // -----------------------------------------------------------------------------------------
    // Set the minimum and maximum population so the circles can resize themselves accordingly
    // -----------------------------------------------------------------------------------------
    VisualizationCircle::minimumRadiusPopulation = getMinimumPop();
    VisualizationCircle::maximumRadiusPopulation = getMaximumPop();

    // -----------------------------------------------------------------------------------------
    // Now we can add the commuting connections
    // -----------------------------------------------------------------------------------------
    ContactPoolSys contactPoolSys = population->GetContactPoolSys();

    for (auto i = 0U; i < population->size(); i++) {
        Person p = population->at(i);
        spherical_point commutingCoord = findCommutingCoordinate(contactPoolSys, p);

        spherical_point homeCoord = population->at(i).GetCoordinate();
        VisualizationCircle *homeCircle = findCircle(homeCoord);

        if (p.GetAge() >= 18) {
            VisualizationCircle *commutingCircle = findCircle(commutingCoord);

            homeCircle->addCommuter(commutingCircle);
        }
    }

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

    if (closest == nullptr) {
        noHover();
    } else if (closest->containsPoint(mousePos, 3)) {
        hoverCircle(closest);
    } else {
        noHover();
    }
}

void MapViewerWindow::draw() {
    // -----------------------------------------------------------------------------------------
    // Load pixmap from image
    // -----------------------------------------------------------------------------------------
    QPixmap pixmap = QPixmap::fromImage(*image);

    // -----------------------------------------------------------------------------------------
    // Draw circles on the pixmap
    // -----------------------------------------------------------------------------------------
    if (ui->displayLocation->isChecked()) {
        for (int i = 0; i < circles->size(); i++) {
            VisualizationCircle *c = circles->at(i);

            // Filters
            if (c->getPopulation() < ui->minPop->value()) { continue; }
            if (c->getInfectedPercent() < ui->minSick->value()) { continue; }

            drawCircle(&pixmap, c);
        }
    }

    // -----------------------------------------------------------------------------------------
    // Draw commuting lines on the pixmap
    // -----------------------------------------------------------------------------------------
    if (ui->displayCommuting->isChecked()) {
        for (int i = 0; i < circles->size(); i++) {
            VisualizationCircle *c = circles->at(i);

            // Only draw for the selected circle,
            // Drawing all lines for all circles would be...
            // ...intense
            if (c != selected) { continue; }

            drawLines(&pixmap, c);
        }
    }

    // -----------------------------------------------------------------------------------------
    // Set gfxitem pixmap to pixmap
    // -----------------------------------------------------------------------------------------
    gfxItem->setPixmap(pixmap);

    // -----------------------------------------------------------------------------------------
    // Display the scene
    // -----------------------------------------------------------------------------------------
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

void MapViewerWindow::drawLines(QPixmap *pm, VisualizationCircle *circle) {
    // QPainter
    QPainter painter(pm);
    painter.scale(1, 1);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(circle->getColor(true));

    // Iterate over all lines
    for (int i = 0; i < circles->size(); i++) {
        VisualizationCircle *c = circles->at(i);

        unsigned int commuters = circle->getCommuters(c);

        if (commuters > 0) {
            QPointF p1 = circle->position;
            QPointF p2 = c->position;
            painter.drawLine(p1, p2);
        }
    }
}

void MapViewerWindow::closeEvent(QCloseEvent *event) {
    // timer->stop();
    event->accept();
}

void MapViewerWindow::hoverCircle(VisualizationCircle *c) {
    selected = c;

    // Title
    QString title = "Location (" + QString::number(c->geoGridLocation->latitude) + ", " + QString::number(c->geoGridLocation->longitude) + ")";
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
    if (!ui->displayLocation->isChecked()) {
        return nullptr;
    }

    float closestDist = INT_MAX;
    VisualizationCircle *closest = nullptr;

    for (int i = 0; i < circles->size(); i++) {
        VisualizationCircle *c = circles->at(i);

        if (c->getPopulation() < ui->minPop->value()) { continue; }
        if (c->getInfectedPercent() < ui->minSick->value()) { continue; }

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

void MapViewerWindow::setupStatisticsRetriever(std::shared_ptr<Population> population) 
{
    if (statsRetriever != nullptr) {
        delete statsRetriever;
    }
    
    statsRetriever = new StatisticsRetriever(population);
}

spherical_point MapViewerWindow::findCommutingCoordinate(ContactPoolSys &cps, Person &person)
{
    double age = person.GetAge();
    ContactPoolType::Id type;
    int id;

    if (age <= 25) {
        type = ContactPoolType::Id::School;
    }
    else {
        type = ContactPoolType::Id::Work;
    }

    id = person.GetPoolId(type);

    return cps[type][id].GetCoordinate();
}