#ifndef MAPVIEWERWINDOW_H
#define MAPVIEWERWINDOW_H

#include <QWidget>
#include <QImage>
#include <QGraphicsPixmapItem>

namespace Ui {
class MapViewerWindow;
}

class MapViewerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MapViewerWindow(QWidget *parent = 0);
    ~MapViewerWindow();

private:
    Ui::MapViewerWindow *ui;
    QImage *image;
    QGraphicsScene *gfxScene;
    QGraphicsPixmapItem *gfxItem;
};

#endif // MAPVIEWERWINDOW_H
