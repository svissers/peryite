#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <peryiteuidata.h>
#include <geogenvisualization.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Geo_geoGenFileSelect_clicked();

    void on_Geo_configFileSelect_clicked();

    void on_Geo_generateGeoGen_clicked();

    void on_Geo_visualizeGeoGen_clicked();

    void on_Pop_configFileSelect_clicked();

    void on_Pop_popGenFileSelect_clicked();

private:
    Ui::MainWindow *ui;
    PeryiteUIData *data;

    void setGeoGenFolder(QString filename);
    void setConfigFile(QString filename);
    void setPopGenFile(QString filename);
};

#endif // MAINWINDOW_H
