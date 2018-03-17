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
    void on_geoGenFileSelect_clicked();

    void on_configFileSelect_clicked();

    void on_generateGeoGen_clicked();

    void on_visualizeGeoGen_clicked();

private:
    Ui::MainWindow *ui;
    PeryiteUIData *data;

    void setGeoGenFile(QString filename);
    void setConfigFile(QString filename);
};

#endif // MAINWINDOW_H
