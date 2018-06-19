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
    /// Constructor and Destructor
    explicit StrideWindow(stride::GuiController *guiCtrl, QWidget *parent = 0);
    ~StrideWindow();

    /// Returns the name of the config file
    QString getConfigFileName();

    /// Returns the config tree
    boost::property_tree::ptree* getConfigPTree();

private slots:
    /// Qt Slots
    void on_runButton_batch_clicked();
    void on_runButton_all_clicked();
    void on_runButton_one_clicked();
    void on_runButton_multi_clicked();
    void on_configInput_editingFinished();

private:
    /// Running
    void runAll(bool batch);
    void run(int steps);

    /// Checks if the config file exists and is correct
    bool checkConfigFile();

    /// Creates a ptree from the configfile
    boost::property_tree::ptree createConfigPTree(QString configFile);

    /// Creates the scatter series results after the batch
    QtCharts::QScatterSeries* createResultsScatterSeries(QList<int> resultsList, int totalRuns, QString file, int seed, QString rngEngineType);

    /// Sets the status indicator on the screen
    void setStatus(QString status);

    /// Sets initial override parameters
    void setInitialParameters();

    /// Sets the running status
    void setRunning(bool isRunning);

    /// Setup a run
    bool setupRun();

    /// Setup a batch (multiple runs)
    void setupBatch();

    /// Set the extra options group
    void setExtraOptions();

    /// Records the results of a run
    void recordResults();

    /// Creates the scatter graph from a series
    void createScatterGraph();

    /// Called at the end of a run
    void endOfRun(bool continueBatch);

    /// Enables/disables run buttons
    void setRunButtonsEnabled(bool enabled);
    
    /// Tooltips appear when a user hovers over a ui item
    void setTooltips();

    /// Sets the icon for the window
    void loadIcon();

    Ui::StrideWindow *ui;                   /// Link to the ui
    bool running;                           /// Is the sim running?
    bool batchRunning;                      /// Is a batch running?
    bool filenameCorrect;                   /// Is the config filename correct? (deprecated)
    stride::GuiController *guiController;   /// Link to guicontroller
    QList<int> results;                     /// List of results (will be used to create scatter series)
    boost::property_tree::ptree config_pt;  /// Temporary ptree (deprecated)

    // -----------------------------------------------------------------------------------------
    // Run related parameters
    // -----------------------------------------------------------------------------------------
    QString m_run_configFile;               /// Config file
    int m_run_rngSeed;                      /// Seed override
    QString m_run_rngType;                  /// Rngtype override
    bool m_run_log;                         /// Log parameter
    int m_runs;                             /// Amount of runs
    int m_currentRun;                       /// Current run number
    bool m_run_mapViewer;                   /// Mapviewer parameter
    bool m_run_showGraph;                   /// Scattergraph parameter

};

#endif // STRIDEWINDOW_H
