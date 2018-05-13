#ifndef STRIDEWINDOW_H
#define STRIDEWINDOW_H

#include "sim/GuiController.h"

#include <QWidget>
#include <QtCharts/QScatterSeries>
#include <boost/property_tree/xml_parser.hpp>

namespace Ui {
class StrideWindow;
}

class StrideWindow : public QWidget
{
    Q_OBJECT

public:
    explicit StrideWindow(stride::GuiController *guiCtrl, QWidget *parent = 0);
    ~StrideWindow();

private slots:
    void on_runButton_all_clicked();

private:
    Ui::StrideWindow *ui;
    bool running;
    stride::GuiController *guiController;

    bool checkConfigFile();
    boost::property_tree::ptree createConfigPTree(QString configFile);
    QtCharts::QScatterSeries* createResultsScatterSeries(QList<int> resultsList, int totalRuns, QString file, int seed, QString rngEngineType);
    void setStatus(QString status);
    void setInitialParameters();
    void setRunning(bool isRunning);
};

#endif // STRIDEWINDOW_H
