#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "peryiteuidata.h"
#include "geogenvisualization.h"
#include "sim/GuiController.h"

#include <QMainWindow>
#include <boost/property_tree/ptree.hpp>

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
    void on_General_configFileSelect_clicked();

    void on_General_editConfigFile_clicked();

    void on_Geo_generateGeoGen_clicked();

    void on_Geo_visualizeGeoGen_clicked();

    void on_Pop_generatePopGen_clicked();

    void on_Pop_visualizePopGen_clicked();

    void on_openStrideWindow_clicked();

private:
    Ui::MainWindow *ui;
    PeryiteUIData *data;
    stride::GuiController *guiController;

    void setOutputFolder(QString path);
    void setConfigFile(QString path);
    void loadLogo();
    void loadIcon();
    void setTooltips();
    void updateButtons();
};

#endif // MAINWINDOW_H
