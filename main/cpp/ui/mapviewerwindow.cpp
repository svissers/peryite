#include "mapviewerwindow.h"
#include "ui_mapviewerwindow.h"
#include "geogridlocation.h"
#include "util.h"

#include <QDir>
#include <QGraphicsPixmapItem>
#include <QFile>
#include <QtDebug>
#include <QCloseEvent>

using namespace stride;

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
}

MapViewerWindow::~MapViewerWindow()
{
    delete ui;
}

void MapViewerWindow::createPopView(const std::shared_ptr<Population> population) {
    circles = new QList<VisualizationCircle *>();

    // for (int i = 0; i < population->size(); i++) {
    //     qDebug() << population->at(i).GetId();
    //     qDebug() << population->at(i).GetCoordinate().m_latitude << " | " << population->at(i).GetCoordinate().m_longitude;
    // }

    addCircle(new VisualizationCircle(QPointF(100, 100), 10));

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
    for (int i = 0; i < circles->size(); i++) {
        if (circles->at(i)->containsPoint(mousePos)) {
            hoverCircle(circles->at(i));
            return;
        }
    }

    // Nothing selected
    noHover();
}

void MapViewerWindow::draw() {
    // Load pixmap from image
    QPixmap pixmap = QPixmap::fromImage(*image);

    // Draw circles on the pixmap
    for (int i = 0; i < circles->size(); i++) {
        VisualizationCircle *c = circles->at(i);
        drawCircle(&pixmap, c->position, c->radius, (c == selected));
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
