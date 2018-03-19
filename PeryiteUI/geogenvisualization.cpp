#include "geogenvisualization.h"
#include "ui_geogenvisualization.h"
#include "geogridlocation.h"
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
    setStyleSheet("QWidget { background-color: #626a77; }");

    // Load background image into image
    QString filename = QDir(QCoreApplication::applicationDirPath()).cleanPath("../vlaanderen.png");
    image = new QImage(filename);

    // Setup GraphicsScene
    gfxScene = new QGraphicsScene();
    gfxItem = new QGraphicsPixmapItem();
    gfxScene->addItem(gfxItem);

    // Set timer interval for draw update
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GeoGenVisualization::update);
    timer->start(1000/60 * 2); // 30 fps
}

GeoGenVisualization::~GeoGenVisualization()
{
    delete ui;
    delete timer;
    delete image;
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
    // Load pixmap into image
    QPixmap pixmap = QPixmap::fromImage(*image);

    // Draw circles oselected = circles->at(i);n the pixmap
    for (int i = 0; i < circles->size(); i++) {
        VisualizationCircle *c = circles->at(i);
        drawCircle(&pixmap, c->position, c->radius, (c == selected));
    }

    // Set pixmap pixmap
    gfxItem->setPixmap(pixmap);

    // Display the scene
    ui->FlandersMap->setScene(gfxScene);
    ui->FlandersMap->fitInView( gfxScene->itemsBoundingRect(), Qt::IgnoreAspectRatio);
    // gfxScene->addItem(gfxItem);
}

void GeoGenVisualization::addCircle(VisualizationCircle *c) {
    *circles << c;
}

void GeoGenVisualization::drawCircle(QPixmap *pm, QPointF point, float radius, bool selected) {
    // QPainter
    QPainter painter(pm);
    painter.setRenderHint(QPainter::Antialiasing);

    // Gradient
    QLinearGradient gradient(QPointF(point.x(), point.y() - radius), QPointF(point.x(), point.y() + radius));
    gradient.setColorAt(0.0, selected ? QColor(130, 150, 220) : QColor(130, 140, 150));
    gradient.setColorAt(1.0, selected ? QColor(90, 110, 180) : QColor(90, 100, 110));
    painter.setBrush(gradient);
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
    QString text = "Population: " + formatInt(c->geoGridLocation->population);
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

    // Start with geogrid
    QFile geogridFile(data->geogridFile);

    qDebug() << data->geogridFile;

    if (!geogridFile.open(QIODevice::ReadOnly)) {
        qDebug() << geogridFile.errorString();
        return;
    }

    while (!geogridFile.atEnd()) {
        QString line = geogridFile.readLine();
        addCircle(new VisualizationCircle(new GeoGridLocation(line)));
    }

    geogridFile.close();
}

QString GeoGenVisualization::formatInt(int n) {
    return QLocale(QLocale::English).toString(n);
}
