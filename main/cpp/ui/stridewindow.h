#ifndef STRIDEWINDOW_H
#define STRIDEWINDOW_H

#include "sim/GuiController.h"

#include <QWidget>
#include <QtCharts/QScatterSeries>
#include <QList>
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
    void on_runButton_one_clicked();
    void on_runButton_all_clicked();
    void on_runButton_multi_clicked();

private:
    Ui::StrideWindow *ui;
    bool running;
    stride::GuiController *guiController;
    QList<int> results;

    // Run related parameters
    QString m_run_configFile;
    int m_run_rngSeed;
    QString m_run_rngType;
    int m_runs;

    // Extra Options
    bool m_run_showGraph;
    bool m_run_mapViewer;

    // Running
    void runAll();
    void run(int steps);

    bool checkConfigFile();
    boost::property_tree::ptree createConfigPTree(QString configFile);
    QtCharts::QScatterSeries* createResultsScatterSeries(QList<int> resultsList, int totalRuns, QString file, int seed, QString rngEngineType);
    void setStatus(QString status);
    void setInitialParameters();
    void setRunning(bool isRunning);
    bool setupRun();
    void setExtraOptions();
    void recordResults();
    void createScatterGraph();
    void endOfRun();
    void setRunButtonsEnabled(bool enabled);
};

#endif // STRIDEWINDOW_H
