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

    QString getConfigFileName();
    boost::property_tree::ptree* getConfigPTree();

private slots:
    void on_runButton_batch_clicked();
    void on_runButton_all_clicked();
    void on_runButton_one_clicked();
    void on_runButton_multi_clicked();
    void on_editConfigButton_clicked();
    void on_configInput_editingFinished();

private:
    Ui::StrideWindow *ui;
    bool running;
    bool batchRunning;
    bool filenameCorrect;
    stride::GuiController *guiController;
    QList<int> results;
    boost::property_tree::ptree config_pt;

    // Run related parameters
    QString m_run_configFile;
    int m_run_rngSeed;
    QString m_run_rngType;
    bool m_run_log;
    int m_runs;
    int m_currentRun;
    bool m_run_mapViewer;
    bool m_run_showGraph;

    // Running
    void runAll(bool batch);
    void run(int steps);

    bool checkConfigFile();
    boost::property_tree::ptree createConfigPTree(QString configFile);
    QtCharts::QScatterSeries* createResultsScatterSeries(QList<int> resultsList, int totalRuns, QString file, int seed, QString rngEngineType);
    void setStatus(QString status);
    void setInitialParameters();
    void setRunning(bool isRunning);
    bool setupRun();
    void setupBatch();
    void setExtraOptions();
    void recordResults();
    void createScatterGraph();
    void endOfRun(bool continueBatch);
    void setRunButtonsEnabled(bool enabled);
    void setTooltips();
    void loadIcon();
};

#endif // STRIDEWINDOW_H
