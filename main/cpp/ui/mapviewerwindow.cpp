#include "mapviewerwindow.h"
#include "ui_mapviewerwindow.h"

#include <QDir>
#include <QGraphicsPixmapItem>
#include <QFile>

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

    // Load pixmap from image
    QPixmap pixmap = QPixmap::fromImage(*image);
    gfxItem->setPixmap(pixmap);

    // Display the scene
    ui->FlandersMap->setScene(gfxScene);
}

MapViewerWindow::~MapViewerWindow()
{
    delete ui;
}
