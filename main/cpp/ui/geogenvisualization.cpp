#include "geogenvisualization.h"
#include "ui_geogenvisualization.h"
#include "geogridlocation.h"
#include "util.h"
#include <QtDebug>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QCloseEvent>
#include <QFile>

GeoGenVisualization::GeoGenVisualization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeoGenVisualization)
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
    connect(timer, &QTimer::timeout, this, &GeoGenVisualization::update);
    timer->start(1000 / 30); // 30 fps
}

GeoGenVisualization::~GeoGenVisualization()
{
    delete ui;
    delete timer;
    delete image;
    delete gfxScene;
    delete gfxItem;
}

void GeoGenVisualization::update() {
    // Get cursor position
    QPoint mousePos = mapFromGlobal(QCursor::pos());

    // Check hover selection for circles
    updateSelection(mousePos);

    // Draw
    draw();
}

void GeoGenVisualization::updateSelection(QPointF mousePos) {
    for (int i = 0; i < circles->size(); i++) {
        if (circles->at(i)->containsPoint(mousePos)) {
            hoverCircle(circles->at(i));
            return;
        }
    }

    // Nothing selected
    noHover();
}

void GeoGenVisualization::draw() {
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

void GeoGenVisualization::addCircle(VisualizationCircle *c) {
    *circles << c;
}

void GeoGenVisualization::drawCircle(QPixmap *pm, QPointF point, float radius, bool selected) {
    // QPainter
    QPainter painter(pm);
    painter.scale(1, 1);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(selected ? QColor("#3c6382") : QColor("#78e08f"));
    painter.setPen(QColor(0, 0, 0, 0));

    // Draw
    painter.drawEllipse(point, radius, radius);
}

void GeoGenVisualization::closeEvent(QCloseEvent *event) {
    timer->stop();
    event->accept();
}

void GeoGenVisualization::hoverCircle(VisualizationCircle *c) {
    selected = c;

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
}

void GeoGenVisualization::noHover() {
    selected = NULL;
    ui->CircleInfoTitle->setText("");
    ui->CircleInfoText->setText("");
}

void GeoGenVisualization::parseData(GeoGenData *data) {
    // Create list of circles
    circles = new QList<VisualizationCircle *>();
    selected = NULL;

    // Parse data
    parseGeoGrid(data);
    parseCommunities(data);
    parseSchools(data);
    parseUniversities(data);
    parseWorkplaces(data);
}

VisualizationCircle* GeoGenVisualization::findCircle(float longitude, float latitude) {
    for (int i = 0; i < circles->length(); i++) {
        float lng = circles->at(i)->geoGridLocation->longitude;
        float lat = circles->at(i)->geoGridLocation->latitude;

        if (lng == longitude && lat == latitude) {
            return circles->at(i);
        }
    }

    qDebug() << "Could not findCircle() with longitude: " << longitude << ", latitude: " << latitude;
    return NULL;
}

void GeoGenVisualization::addCommunity(QString csvLine) {
    QStringList list = Util::parseCSVLine(csvLine);

    // Read data from CSV and find corresponding circle
    float latitude = list[1].toFloat();
    float longitude = list[2].toFloat();
    VisualizationCircle* circle = findCircle(longitude, latitude);

    // Nullcheck
    if (circle == NULL) {
        qDebug() << "addCommunity did not find circle. Did not add community.";
        return;
    }

    circle->geoGridLocation->communities++;
}

void GeoGenVisualization::addSchool(QString csvLine) {
    QStringList list = Util::parseCSVLine(csvLine);

    // Read data from CSV and find corresponding circle
    float latitude = list[1].toFloat();
    float longitude = list[2].toFloat();
    VisualizationCircle* circle = findCircle(longitude, latitude);

    // Nullcheck
    if (circle == NULL) {
        qDebug() << "addSchool did not find circle. Did not add school.";
        return;
    }

    circle->geoGridLocation->schools++;
}

void GeoGenVisualization::addUniversity(QString csvLine) {
    QStringList list = Util::parseCSVLine(csvLine);

    // Read data from CSV and find corresponding circle
    float latitude = list[2].toFloat();
    float longitude = list[3].toFloat();
    VisualizationCircle* circle = findCircle(longitude, latitude);

    // Nullcheck
    if (circle == NULL) {
        qDebug() << "addUniversity did not find circle. Did not add university.";
        return;
    }

    circle->geoGridLocation->universities++;
}

void GeoGenVisualization::addWorkplace(QString csvLine) {
    QStringList list = Util::parseCSVLine(csvLine);

    // Read data from CSV and find corresponding circle
    float latitude = list[1].toFloat();
    float longitude = list[2].toFloat();
    VisualizationCircle* circle = findCircle(longitude, latitude);

    // Nullcheck
    if (circle == NULL) {
        qDebug() << "addWorkplace did not find circle. Did not add workplace.";
        return;
    }

    circle->geoGridLocation->workplaces++;
}

void GeoGenVisualization::parseGeoGrid(GeoGenData *data) {
    QFile geogridFile(data->geogridFile);

    if (!geogridFile.open(QIODevice::ReadOnly)) {
        qDebug() << geogridFile.errorString();
        return;
    }

    while (!geogridFile.atEnd()) {
        QString line = geogridFile.readLine();

        // Ignore header lines
        if (line.startsWith("id")) { continue; }

        addCircle(new VisualizationCircle(new GeoGridLocation(line)));
    }

    geogridFile.close();
}

void GeoGenVisualization::parseCommunities(GeoGenData *data) {
    QFile communitiesFile(data->communitiesFile);

    if (!communitiesFile.open(QIODevice::ReadOnly)) {
        qDebug() << communitiesFile.errorString();
        return;
    }

    while (!communitiesFile.atEnd()) {
        QString line = communitiesFile.readLine();

        // Ignore header lines
        if (line.startsWith("id")) { continue; }

        addCommunity(line);
    }

    communitiesFile.close();
}

void GeoGenVisualization::parseSchools(GeoGenData *data) {
    QFile schoolsFile(data->schoolsFile);

    if (!schoolsFile.open(QIODevice::ReadOnly)) {
        qDebug() << schoolsFile.errorString();
        return;
    }

    while (!schoolsFile.atEnd()) {
        QString line = schoolsFile.readLine();

        // Ignore header lines
        if (line.startsWith("id")) { continue; }

        addSchool(line);
    }

    schoolsFile.close();
}

void GeoGenVisualization::parseUniversities(GeoGenData *data) {
    QFile universitiesFile(data->universitiesFile);

    if (!universitiesFile.open(QIODevice::ReadOnly)) {
        qDebug() << universitiesFile.errorString();
        return;
    }

    while (!universitiesFile.atEnd()) {
        QString line = universitiesFile.readLine();

        // Ignore header lines
        if (line.startsWith("id")) { continue; }

        addUniversity(line);
    }

    universitiesFile.close();
}

void GeoGenVisualization::parseWorkplaces(GeoGenData *data) {
    QFile workplacesFile(data->workplacesFile);

    if (!workplacesFile.open(QIODevice::ReadOnly)) {
        qDebug() << workplacesFile.errorString();
        return;
    }

    while (!workplacesFile.atEnd()) {
        QString line = workplacesFile.readLine();

        // Ignore header lines
        if (line.startsWith("id")) { continue; }

        addWorkplace(line);
    }

    workplacesFile.close();
}
