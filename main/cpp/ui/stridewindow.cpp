#include "stridewindow.h"
#include "ui_stridewindow.h"
#include "sim/SimRunner.h"
#include "sim/Sim.h"
#include "pop/Population.h"
#include "util/FileSys.h"
#include "util/ConfigInfo.h"
#include "util/LogUtils.h"
#include "ui/stridescattergraph.h"
#include "ui/util.h"

#include <algorithm>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QCheckBox>
#include <QFile>

using namespace std;
using namespace stride;
using namespace stride::util;
using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;
using namespace QtCharts;

StrideWindow::StrideWindow(GuiController *guiCtrl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StrideWindow),
    guiController(guiCtrl)
{
    ui->setupUi(this);

    setInitialParameters();
    setTooltips();
    loadIcon();

    this->setFixedSize(QSize(480, 384));
}

StrideWindow::~StrideWindow()
{
    delete ui;
}

void StrideWindow::setupBatch() {
    // -----------------------------------------------------------------------------------------
    // Set parameters from our UI
    // -----------------------------------------------------------------------------------------
    m_run_configFile = ui->configInput->text();
    m_run_rngSeed = ui->seedInput->value();
    m_run_rngType = ui->engineInput->currentText();
    m_runs = ui->runsInput->value();

    // -----------------------------------------------------------------------------------------
    // We save the results in a list first so we can sort them,
    // before adding them to our QScatterSeries object. (if that option is enabled)
    // -----------------------------------------------------------------------------------------
    results = QList<int>();

    // -----------------------------------------------------------------------------------------
    // Setup batch bookkeeping
    // -----------------------------------------------------------------------------------------
    m_currentRun = 0;
    batchRunning = true;
}

bool StrideWindow::setupRun() {
    // -----------------------------------------------------------------------------------------
    // Set our running status and process events so buttons get disabled
    // -----------------------------------------------------------------------------------------
    setRunning(true);
    setStatus("Setup... Run " + QString::number(m_currentRun) + " / " + QString::number(m_runs));
    QCoreApplication::processEvents();

    // -----------------------------------------------------------------------------------------
    // Check input config file
    // -----------------------------------------------------------------------------------------
    if (!checkConfigFile()) {
        QMessageBox::warning(this, tr("Setup failed."), "Incorrect config file.");
        return false;
    }

    // -----------------------------------------------------------------------------------------
    // Set parameters from our UI
    // -----------------------------------------------------------------------------------------
    m_run_mapViewer = ui->mapViewerInput->isChecked();
    m_run_log = ui->logInput->isChecked();

    // -----------------------------------------------------------------------------------------
    // Set random rngType if requested
    // -----------------------------------------------------------------------------------------
    if (ui->varyEngineInput->isChecked()) {
        QStringList types;
        types << "lcg64" << "lcg64_shift" << "mrg2" << "mrg3" << "yarn2" << "yarn3";
        int index = qrand() % types.size();
        m_run_rngType = types[index];
    }

    // -----------------------------------------------------------------------------------------
    // Set random rngSeed if requested
    // -----------------------------------------------------------------------------------------
    if (ui->varySeedInput->isChecked()) {
        m_run_rngSeed = qrand();
    }

    // -----------------------------------------------------------------------------------------
    // Create config ptree
    // -----------------------------------------------------------------------------------------
    ptree config_pt = createConfigPTree(m_run_configFile);
    config_pt.put("run.rng_seed", m_run_rngSeed);
    config_pt.put("run.rng_type", m_run_rngType.toStdString());
    config_pt.put("run.output_map", m_run_mapViewer ? 1 : 0);
    config_pt.put("run.stride_log_level", m_run_log ? "info" : "None");

    // -----------------------------------------------------------------------------------------
    // Assign the ptree and setup the simulation
    // -----------------------------------------------------------------------------------------
    guiController->AssignPTree(config_pt);
    guiController->Setup();

    // -----------------------------------------------------------------------------------------
    // Assign the ptree and setup the simulation
    // -----------------------------------------------------------------------------------------
    m_currentRun++;

    return true;
}

void StrideWindow::setExtraOptions() {
    // -----------------------------------------------------------------------------------------
    // Set parameters from our UI
    // -----------------------------------------------------------------------------------------
    m_run_showGraph = ui->scatterGraphInput->isChecked();
}

void StrideWindow::on_runButton_batch_clicked() {
    // -----------------------------------------------------------------------------------------
    // Warn users against running this with mapviewer on. Because that's insane.
    // -----------------------------------------------------------------------------------------
    if (ui->mapViewerInput->isChecked() && ui->runsInput->value() > 1) {
        QMessageBox::StandardButton reply;
        QString text = "You are trying to execute a full batch with MapViewer enabled.\n";
        text += "This will open up a mapviewer for every run and probably freeze your pc.\n";
        text += "Disable MapViewer before running? (strongly recommended)";

        reply = QMessageBox::question(this, "Are you sure?", text, QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            ui->mapViewerInput->setChecked(false);
            runAll(true);
        } else if (reply == QMessageBox::No){
            runAll(true);
        }
    } else {
        runAll(true);
    }
}

void StrideWindow::on_runButton_all_clicked() {
    runAll(false);
}

void StrideWindow::on_runButton_one_clicked() {
    run(1);
}

void StrideWindow::on_runButton_multi_clicked() {
    run(ui->runMultiInput->value());
}


void StrideWindow::run(int steps) {
    // -----------------------------------------------------------------------------------------
    // Disable the buttons
    // -----------------------------------------------------------------------------------------
    setRunButtonsEnabled(false);

    // -----------------------------------------------------------------------------------------
    // Set up the batch
    // -----------------------------------------------------------------------------------------
    if (!batchRunning) {
        setupBatch();
    }

    // -----------------------------------------------------------------------------------------
    // Set up the run
    // -----------------------------------------------------------------------------------------
    if (!running) {
        setRunning(setupRun());
    }

    if (!running) {
        QMessageBox::warning(this, tr("Setup failed."), "Setup failed.");
        return;
    }

    // -----------------------------------------------------------------------------------------
    // Set the options that can be changed mid-run
    // -----------------------------------------------------------------------------------------
    setExtraOptions();

    // -----------------------------------------------------------------------------------------
    // Set progress text in UI
    // -----------------------------------------------------------------------------------------
    setStatus("Running " + Util::formatInt(steps) + " step(s)... Run " + QString::number(m_currentRun) + " / " + QString::number(m_runs));

    // -----------------------------------------------------------------------------------------
    // Run the simulation.
    // -----------------------------------------------------------------------------------------
    guiController->RunStride(steps);

    if (guiController->simulationDone()) {
        endOfRun(false);
    } else {
        setStatus("Paused. Day " + Util::formatInt(guiController->getCurrentDay()) + "/" + Util::formatInt(guiController->getTotalDays()));
        setRunButtonsEnabled(true);
    }
}

void StrideWindow::runAll(bool batch) {
    // -----------------------------------------------------------------------------------------
    // Set up the batch
    // -----------------------------------------------------------------------------------------
    if (!batchRunning) {
        setupBatch();
    }

    // -----------------------------------------------------------------------------------------
    // Set up the run
    // -----------------------------------------------------------------------------------------
    if (!running) {
        setRunning(setupRun());
    }

    if (!running) {
        QMessageBox::warning(this, tr("Setup failed."), "Setup failed.");
        return;
    }

    // -----------------------------------------------------------------------------------------
    // Set the options that can be changed mid-run
    // -----------------------------------------------------------------------------------------
    setExtraOptions();

    // -----------------------------------------------------------------------------------------
    // Set progress text in UI
    // -----------------------------------------------------------------------------------------
    setStatus("Running all... Run " + QString::number(m_currentRun) + " / " + QString::number(m_runs));

    // -----------------------------------------------------------------------------------------
    // Run the simulation.
    // -----------------------------------------------------------------------------------------
    guiController->RunStride();

    // -----------------------------------------------------------------------------------------
    // Done!
    // -----------------------------------------------------------------------------------------
    endOfRun(batch);
}

bool StrideWindow::checkConfigFile() {
    if (ui->configInput->text() == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a config file.");
        return false;
    } else {
        return true;
    }
}

ptree StrideWindow::createConfigPTree(QString configFile) {
    auto config = configFile.toStdString();
    ptree config_pt;

    const boost::filesystem::path configPath = FileSys::GetConfigDir() /= config;

    config_pt = FileSys::ReadPtreeFile(configPath);

    return config_pt;
}

QScatterSeries* StrideWindow::createResultsScatterSeries(QList<int> resultsList, int totalRuns, QString file, int seed, QString rngEngineType) {
    // Create scatter series
    QScatterSeries *series = new QScatterSeries();

    // Sort our results list
    sort(resultsList.begin(), resultsList.end());

    // Constructing the name
    QString name = "Total infected count, " + QString::number(totalRuns) + " sorted runs";
    // Add Config name
    name += " (config: " + file + ")";
    // Mention random seed
    name += (ui->varySeedInput->isChecked()) ? " (random seed)" : (" (seed: " + QString::number(seed));
    // Mention random seed
    name += (ui->varyEngineInput->isChecked()) ? " (random rng engine)" : (" (rng engine: " + rngEngineType + ")");
    // Mention spread (+percentage)
    int spread = resultsList.last() - resultsList.first();
    float spreadPercentage = 100 * (float)(spread) / resultsList.last();
    name += " - " + QString::number(spread) + "(" +  QString::number(spreadPercentage) + "\%) spread";

    // Set the series name, marker
    series->setName(name);
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series->setMarkerSize(10);

    // Add our items from the sorted list to the scatterseries
    for (int i = 0; i < resultsList.size(); i++) {
        series->append(i, resultsList[i]);
    }

    return series;
}

void StrideWindow::setStatus(QString status) {
    ui->statusLabel->setText("Status: " + status);

    // -----------------------------------------------------------------------------------------
    // Respond to events so OS doesn't think the program is unresponsive.
    // Since we would have to repaint here anyway, this is a good time to do it.
    // -----------------------------------------------------------------------------------------
    QCoreApplication::processEvents();
}

void StrideWindow::setRunning(bool isRunning) {
    running = isRunning;
    setRunButtonsEnabled(!isRunning);
}

void StrideWindow::setRunButtonsEnabled(bool enabled) {
    ui->runButton_batch->setEnabled(enabled);
    ui->runButton_all->setEnabled(enabled);
    ui->runButton_one->setEnabled(enabled);
    ui->runButton_multi->setEnabled(enabled);
}

void StrideWindow::setInitialParameters() {
    // -----------------------------------------------------------------------------------------
    // Set the initial values for checkboxes, scrollboxes, ...
    // -----------------------------------------------------------------------------------------
    ui->configInput->setText("run_geopop.xml");
    ui->engineInput->addItem("lcg64");
    ui->engineInput->addItem("lcg64_shift");
    ui->engineInput->addItem("mrg2");
    ui->engineInput->addItem("mrg3");
    ui->engineInput->addItem("yarn2");
    ui->engineInput->addItem("yarn3");
    ui->varySeedInput->setChecked(true);
    ui->mapViewerInput->setChecked(true);

    setRunning(false);
    setStatus("Idle");

    batchRunning = false;

    // -----------------------------------------------------------------------------------------
    // Disable buttons that are WIP
    // -----------------------------------------------------------------------------------------
    ui->editConfigButton->setEnabled(false);
}

void StrideWindow::setTooltips() {
    // Main Config
    ui->configInput->setToolTip("The Config file that will be used by Stride.");
    ui->configLabel->setToolTip("The Config file that will be used by Stride.");
    ui->editConfigButton->setToolTip("Edit the current config file.");
    ui->runsInput->setToolTip("The amount of runs in this batch. >1 runs is used for testing, mainly.");
    ui->runsLabel->setToolTip("The amount of runs in this batch. >1 runs is used for testing, mainly.");

    // Options
    ui->logInput->setToolTip("Should Stride log to the command line?");
    ui->logLabel->setToolTip("Should Stride log to the command line?");
    ui->mapViewerInput->setToolTip("Enable to show a live map with epidemiologic info during the simulation.");
    ui->mapViewerLabel->setToolTip("Enable to show a live map with epidemiologic info during the simulation.");
    ui->scatterGraphInput->setToolTip("Enable to show a scatter graph with results for all runs after completion of the batch.\nMainly used for testing.");
    ui->scatterGraphLabel->setToolTip("Enable to show a scatter graph with results for all runs after completion of the batch.\nMainly used for testing.");

    // RNG
    ui->engineInput->setToolTip("The RNG engine that will be used by Stride.");
    ui->engineLabel->setToolTip("The RNG engine that will be used by Stride.");
    ui->seedInput->setToolTip("The RNG seed that will be used by Stride.");
    ui->seedLabel->setToolTip("The RNG seed that will be used by Stride.");
    ui->varyEngineInput->setToolTip("If enabled, a random RNG engine will be used every run.");
    ui->varyEngineLabel->setToolTip("If enabled, a random RNG engine will be used every run.");
    ui->varySeedInput->setToolTip("If enabled, a random RNG seed will be used every run.");
    ui->varySeedLabel->setToolTip("If enabled, a random RNG seed will be used every run.");

    // Running
    ui->runButton_all->setToolTip("Run all (remaining) steps of the current simulation run.");
    ui->runButton_batch->setToolTip("Run all (remaining) runs in this batch.");
    ui->runButton_multi->setToolTip("Run X steps of the current simulation run. (Input X to the right)");
    ui->runButton_one->setToolTip("Run 1 step of the current simulation run.");
    ui->runMultiInput->setToolTip("The amount of steps that 'Run Multi' runs.");
}

void StrideWindow::recordResults() {
    if (m_run_showGraph) {
        int result = guiController->GetRunner()->GetSim()->GetPopulation()->GetInfectedCount();
        results << result;
    }
}

void StrideWindow::createScatterGraph() {
    // -----------------------------------------------------------------------------------------
    // Create QScatterSeries for graph
    // -----------------------------------------------------------------------------------------
    QScatterSeries *scatterSeries = createResultsScatterSeries(results, m_runs, m_run_configFile, m_run_rngSeed, m_run_rngType);

    // -----------------------------------------------------------------------------------------
    // Write results to file
    // -----------------------------------------------------------------------------------------
    QString filename = "TestResults_" + m_run_configFile.split(".").at(0) + ".txt";
    QFile file(filename);

    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << scatterSeries->name() << endl;
        for (int i = 0; i < results.size(); i++) {
            stream << QString::number(results[i]) << endl;
        }
    }

    file.close();

    // -----------------------------------------------------------------------------------------
    // Create new window for graph
    // -----------------------------------------------------------------------------------------
    StrideScatterGraph *wdg = new StrideScatterGraph;
    wdg->createGraph(scatterSeries);
    wdg->show();
}

void StrideWindow::endOfRun(bool continueBatch) {
    // -----------------------------------------------------------------------------------------
    // Reset a few things so we're ready for the next run.
    // -----------------------------------------------------------------------------------------
    spdlog::drop_all();
    setRunning(false);

    // -----------------------------------------------------------------------------------------
    // Record results for graph
    // -----------------------------------------------------------------------------------------
    recordResults();

    // -----------------------------------------------------------------------------------------
    // If we're done with the batch, reset UI state.
    // If not, continue if continueBatch (in other words, when user pressed run batch)
    // -----------------------------------------------------------------------------------------
    if (m_runs == m_currentRun) {
        setStatus("Idle");
        m_currentRun = 0;
        batchRunning = false;

        if (m_run_showGraph) {
            createScatterGraph();
        }
    } else {
        if (continueBatch) {
            runAll(true);
        } else {
            setStatus("Done with run " + Util::formatInt(m_currentRun) + "/" + Util::formatInt(m_runs));
        }
    }
}

void StrideWindow::loadIcon()
{
    setWindowIcon(QIcon("./ui/logo.png"));
}
