#include "popgenvisualization.h"
#include "ui_popgenvisualization.h"
#include "util.h"
#include "visualizationgraphbar.h"
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QCloseEvent>

PopGenVisualization::PopGenVisualization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PopGenVisualization)
{
    ui->setupUi(this);

    // Background color
    setStyleSheet("QWidget { background-color: #626a77; }");

    // Load background image into image
    // QString filename = QDir(QCoreApplication::applicationDirPath()).cleanPath("../vlaanderen.png");
    image = new QImage(1000, 600, QImage::Format_RGB32);
    image->fill(QColor(247, 247, 247));

    // Setup GraphicsScene
    gfxScene = new QGraphicsScene();
    gfxItem = new QGraphicsPixmapItem();
    gfxScene->addItem(gfxItem);

    // Set timer interval for draw update
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PopGenVisualization::update);
    timer->start(1000/60 * 2); // 30 fps
}

PopGenVisualization::~PopGenVisualization()
{
    delete ui;
    delete timer;
    delete image;
    delete gfxScene;
    delete gfxItem;
}

void PopGenVisualization::closeEvent(QCloseEvent *event) {
    timer->stop();
    event->accept();
}

void PopGenVisualization::parseData(PopGenData *data) {
    // Make sure all distribution data is set to 0 first
    for (int i = 0; i <= maxAge; i++) {
        ageDistribution[i] = 0;
    }

    // Create list of bars
    bars = new QList<VisualizationGraphBar*>();
    selected = NULL;

    // Parse the popgen file
    QFile popgenFile(data->popFile);

    if (!popgenFile.open(QIODevice::ReadOnly)) {
        qDebug() << popgenFile.errorString();
        return;
    }

    while (!popgenFile.atEnd()) {
        QString line = popgenFile.readLine();

        // Ignore header lines
        if (line.startsWith("\"age\"")) { continue; }

        // Get the line
        QStringList list = Util::parseCSVLine(line);

        addAge(list[0].toInt());
    }

    popgenFile.close();

    createAgeDistributionGraph();
}

void PopGenVisualization::addAge(int age) {
    if (age > maxAge) {
        qDebug() << "age too large: " + age;
        return;
    }

    if (age < 0) {
        qDebug() << "age too small " + age;
        return;
    }

    ageDistribution[age]++;
}

void PopGenVisualization::update() {
    // Get cursor position
    QPoint mousePos = mapFromGlobal(QCursor::pos());

    // Check hover selection for bars
    updateSelection(mousePos);

    // Draw
    draw();
}

void PopGenVisualization::draw() {
    // Load pixmap into image
    QPixmap pixmap = QPixmap::fromImage(*image);

    // Draw bars on the pixmap
    for (int i = 0; i < bars->size(); i++) {
        VisualizationGraphBar *b = bars->at(i);
        drawBar(&pixmap, b->position, b->width, b->height, (b == selected));
    }

    // Set pixmap pixmap
    gfxItem->setPixmap(pixmap);

    // Display the scene
    ui->GraphArea->setScene(gfxScene);
}

void PopGenVisualization::drawBar(QPixmap *pm, QPointF point, float width, float height, bool selected) {
    // QPainter
    QPainter painter(pm);
    painter.setRenderHint(QPainter::Antialiasing);

    // Gradient Gradient
    QLinearGradient gradient(QPointF(point.x(), point.y() - height), QPointF(point.x(), point.y()));
    gradient.setColorAt(0.0, selected ? QColor(130, 150, 220) : QColor(130, 140, 150));
    gradient.setColorAt(1.0, selected ? QColor(90, 110, 180) : QColor(90, 100, 110));
    painter.setBrush(gradient);
    painter.setPen(QColor(0, 0, 0, 0));

    // Draw
    painter.drawRect(point.x(), point.y() - height, width, height);
}

void PopGenVisualization::addBar(VisualizationGraphBar *b) {
    *bars << b;
}

void PopGenVisualization::updateSelection(QPointF mousePos) {
    for (int i = 0; i < bars->size(); i++) {
        if (bars->at(i)->containsPoint(mousePos)) {
            hoverBar(bars->at(i));
            return;
        }
    }

    // Nothing selected
    noHover();
}

void PopGenVisualization::hoverBar(VisualizationGraphBar *b) {
    selected = b;

//    // Title
//    QString title = c->geoGridLocation->name;
//    ui->CircleInfoTitle->setText(title);

//    // Info text
//    QString text = "Population: " + Util::formatInt(c->geoGridLocation->population);
//    text += "\nCommunities: " + Util::formatInt(c->geoGridLocation->communities);
//    text += "\nSchools: " + Util::formatInt(c->geoGridLocation->schools);
//    text += "\nUniversities: " + Util::formatInt(c->geoGridLocation->universities);
//    text += "\nWorkplaces: " + Util::formatInt(c->geoGridLocation->workplaces);
//    ui->CircleInfoText->setText(text);
}

void PopGenVisualization::noHover() {
    selected = NULL;
//    ui->CircleInfoTitle->setText("");
//    ui->CircleInfoText->setText("");
}

int PopGenVisualization::getLargestAgeCategorySize() {
    int max = 0;

    // Find largest value
    for (int i = 0; i <= maxAge; i++) {
        if (ageDistribution[i] > max) {
            max = ageDistribution[i];
        }
    }

    return max;
}

void PopGenVisualization::createAgeDistributionGraph() {
    const float maxHeight = 320;
    int maxAgeCategorySize = getLargestAgeCategorySize();

    // Create bars from ageDistribution
    for (int i = 0; i <= maxAge; i++) {
        qreal width = 10;
        QPointF pos = QPointF(width * i, 401);
        qreal height = maxHeight * ageDistribution[i] / maxAgeCategorySize;

        addBar(new VisualizationGraphBar(pos, width, height));
    }
}
