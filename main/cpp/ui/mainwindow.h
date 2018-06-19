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
    /// Deprecated
    void setOutputFolder(QString path);

    /// Sets the config file
    void setConfigFile(QString path);

    /// Loads the logo for the main menu
    void loadLogo();

    /// Loads the icon for the window
    void loadIcon();

    /// Tooltips appear when a user hovers over a UI element
    void setTooltips();

    /// Updates buttons to be enabled/disabled depending on the ui state
    void updateButtons();

    Ui::MainWindow *ui;                     /// Link to ui
    PeryiteUIData *data;                    /// Not really used anymore. We now use our GenDirectory.
    stride::GuiController *guiController;   /// Link to guicontroller
};

#endif // MAINWINDOW_H
