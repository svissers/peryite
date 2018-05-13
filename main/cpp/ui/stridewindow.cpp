#include "stridewindow.h"
#include "ui_stridewindow.h"
#include "sim/SimRunner.h"
#include "sim/Sim.h"
#include "pop/Population.h"
#include "util/FileSys.h"
#include "util/ConfigInfo.h"
#include "util/LogUtils.h"
#include "ui/stridescattergraph.h"

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
    ui->configInput->setText("run_geopop.xml");
    ui->engineInput->addItem("lcg64");
    ui->engineInput->addItem("lcg64_shift");
    ui->engineInput->addItem("mrg2");
    ui->engineInput->addItem("mrg3");
    ui->engineInput->addItem("yarn2");
    ui->engineInput->addItem("yarn3");
    ui->varySeedInput->setChecked(true);

    running = false;
}

StrideWindow::~StrideWindow()
{
    delete ui;
}

void StrideWindow::on_startButton_clicked()
{
    // -----------------------------------------------------------------------------------------
    // User can't run the simulation when it's already running
    // -----------------------------------------------------------------------------------------
    if (running) { return; }
    else { running = true; }

    // -----------------------------------------------------------------------------------------
    // Check input config file
    // -----------------------------------------------------------------------------------------
    if (!checkConfigFile()) { return; }

    // -----------------------------------------------------------------------------------------
    // Set parameters from our UI
    // -----------------------------------------------------------------------------------------
    QString configFile = ui->configInput->text();
    int rngSeed = ui->seedInput->value();
    QString rngType = ui->engineInput->currentText();
    int runs = ui->runsInput->value();

    // -----------------------------------------------------------------------------------------
    // We save the results in a list first so we can sort them,
    // before adding them to our QScatterSeries object.
    // -----------------------------------------------------------------------------------------
    QList<int> results;

    // -----------------------------------------------------------------------------------------
    // Start running tests
    // -----------------------------------------------------------------------------------------

    for (int i = 0; i < runs; i++) {
        // -----------------------------------------------------------------------------------------
        // Set progress text in UI
        // -----------------------------------------------------------------------------------------
        ui->startButton->setText("Running... " + QString::number(i) + " / " + QString::number(runs));
        ui->startButton->repaint();

        // -----------------------------------------------------------------------------------------
        // Set random rngType if requested
        // -----------------------------------------------------------------------------------------
        if (ui->varyEngineInput->isChecked()) {
            QStringList types;
            types << "lcg64" << "lcg64_shift" << "mrg2" << "mrg3" << "yarn2" << "yarn3";
            int index = qrand() % types.size();
            rngType = types[index];
        }

        // -----------------------------------------------------------------------------------------
        // Set random rngSeed if requested
        // -----------------------------------------------------------------------------------------
        if (ui->varySeedInput->isChecked()) {
            rngSeed = qrand();
        }

        // -----------------------------------------------------------------------------------------
        // Create config ptree
        // -----------------------------------------------------------------------------------------
        ptree config_pt = createConfigPTree(configFile);
        config_pt.put("run.rng_seed", rngSeed);
        config_pt.put("run.rng_type", rngType.toStdString());

        // Respond to events so OS doesn't think the program is unresponsive
        QCoreApplication::processEvents();

        // -----------------------------------------------------------------------------------------
        // Assign the ptree and run the simulation.
        // -----------------------------------------------------------------------------------------
        guiController->AssignPTree(config_pt);
        guiController->RunStride();

        // -----------------------------------------------------------------------------------------
        // Record results for graph
        // -----------------------------------------------------------------------------------------
        int result = guiController->GetRunner()->GetSim()->GetPopulation()->GetInfectedCount();
        results << result;

        // -----------------------------------------------------------------------------------------
        // Done. Respond to events so OS doesn't think the program is unresponsive.
        // -----------------------------------------------------------------------------------------
        QCoreApplication::processEvents();
    }

    // -----------------------------------------------------------------------------------------
    // Reset UI state
    // -----------------------------------------------------------------------------------------
    ui->startButton->setText("Start Tests");
    running = false;

    // -----------------------------------------------------------------------------------------
    // Create QScatterSeries for graph
    // -----------------------------------------------------------------------------------------
    QScatterSeries *scatterSeries = createResultsScatterSeries(results, runs, configFile, rngSeed, rngType);

    // -----------------------------------------------------------------------------------------
    // Write results to file
    // -----------------------------------------------------------------------------------------
    QString filename = "TestResults_" + configFile.split(".").at(0) + ".txt";
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
