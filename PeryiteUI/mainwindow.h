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
    void on_General_outputFolderSelect_clicked();

    void on_General_configFileSelect_clicked();

    void on_Geo_generateGeoGen_clicked();

    void on_Geo_visualizeGeoGen_clicked();

    void on_Pop_generatePopGen_clicked();

    void on_Pop_visualizePopGen_clicked();

    void on_openTestingWindow_clicked();

private:
    Ui::MainWindow *ui;
    PeryiteUIData *data;

    void setOutputFolder(QString path);
    void setConfigFile(QString path);
};

#endif // MAINWINDOW_H