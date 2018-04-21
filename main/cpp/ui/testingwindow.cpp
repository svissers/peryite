#include "testingwindow.h"
#include "ui_testingwindow.h"
#include "sim/SimRunner.h"
#include "sim/Simulator.h"
#include "pop/Population.h"
#include "util/FileSys.h"
#include "util/ConfigInfo.h"
#include "util/LogUtils.h"
#include "ui/testingscattergraph.h"

#include <algorithm>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QCheckBox>
#include <QtCharts/QScatterSeries>
#include <QFile>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;
using namespace stride;
using namespace stride::util;
using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;
using namespace QtCharts;

TestingWindow::TestingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestingWindow)
{
    ui->setupUi(this);
    ui->configInput->setText("run_default.xml");
    ui->engineInput->addItem("lcg64");
    ui->engineInput->addItem("lcg64_shift");
    ui->engineInput->addItem("mrg2");
    ui->engineInput->addItem("mrg3");
    ui->engineInput->addItem("yarn2");
    ui->engineInput->addItem("yarn3");
    ui->varySeedInput->setChecked(true);
    ui->printInput->setChecked(true);

    running = false;
}

TestingWindow::~TestingWindow()
{
    delete ui;
}

void TestingWindow::on_startButton_clicked()
{
    // User can't run the simulation when it's already running
    if (running) {
        return;
    } else {
        running = true;
    }

    if (ui->configInput->text() == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a config file.");
        return;
    }

    // Set parameters from our UI
    QString configFile = ui->configInput->text();
    int rngSeed = ui->seedInput->value();
    QString rngType = ui->engineInput->currentText();
    bool printResults = ui->printInput->isChecked();
    int runs = ui->runsInput->value();

    // We save the results in a list first so we can sort them before adding them to our QScatterSeries object
    QList<int> results;

    // -----------------------------------------------------------------------------------------
    // Start running tests
    // -----------------------------------------------------------------------------------------
    if (printResults) {
        cout << "Starting Tests." << endl;
    }


    for (int i = 0; i < runs; i++) {
        // Get progress
        ui->startButton->setText("Running... " + QString::number(i) + " / " + QString::number(runs));
        ui->startButton->repaint();

        // Set random rngType if requested
        if (ui->varyEngineInput->isChecked()) {
            QStringList types;
            types << "lcg64" << "lcg64_shift" << "mrg2" << "mrg3" << "yarn2" << "yarn3";
            int index = qrand() % types.size();
            rngType = types[index];
        }

        // Set random rngSeed if requested
        if (ui->varySeedInput->isChecked()) {
            rngSeed = qrand();
        }

        // -----------------------------------------------------------------------------------------
        // Create config ptree
        // -----------------------------------------------------------------------------------------
        auto config = configFile.toStdString();
        ptree config_pt;

        const boost::filesystem::path configPath = FileSys::GetConfigDir() /= config;

        config_pt = FileSys::ReadPtreeFile(configPath);
        config_pt.put("run.rng_seed", rngSeed);
        config_pt.put("run.rng_type", rngType.toStdString());

        // -----------------------------------------------------------------------------------------
        // Instantiate SimRunner & register viewers & setup+execute the run.
        // -----------------------------------------------------------------------------------------
        auto runner = SimRunner::Create();

        // Respond to events so OS doesn't think the program is unresponsive
        QCoreApplication::processEvents();

        // -----------------------------------------------------------------------------------------
        // Register viewers do runner setup and the execute.
        // -----------------------------------------------------------------------------------------
        // RegisterViewers(runner);
        runner->Setup(config_pt);
        runner->Run();

        int result = runner->GetSim()->GetPopulation()->GetInfectedCount();
        results << result;

        if (printResults) {
            cout << "Test Instance " << i + 1 << "/" << runs << " completed. " << endl;
            cout << "Infected: " << result << endl;
        }

        // -----------------------------------------------------------------------------------------
        // Done.
        // -----------------------------------------------------------------------------------------
        spdlog::drop_all();
        running = false;

        // Respond to events so OS doesn't think the program is unresponsive
        QCoreApplication::processEvents();
    }

    if (printResults) {
        cout << "Tests completed!" << endl;
    }

    ui->startButton->setText("Start Tests");

    // -----------------------------------------------------------------------------------------
    // Create QScatterSeries for graph
    // This is where the results will be saved
    // -----------------------------------------------------------------------------------------
    QScatterSeries *scatterSeries = new QScatterSeries();

    // Sort our results list
    sort(results.begin(), results.end());

    // Constructing the name
    QString name = "Total infected count, " + QString::number(runs) + " sorted runs";
    // Add Config name
    name += " (config: " + configFile + ")";
    // Mention random seed
    name += (ui->varySeedInput->isChecked()) ? " (random seed)" : (" (seed: " + QString::number(rngSeed));
    // Mention random seed
    name += (ui->varyEngineInput->isChecked()) ? " (random rng engine)" : (" (rng engine: " + rngType + ")");
    // Mention spread (+percentage)
    int spread = results.last() - results.first();
    float spreadPercentage = 100 * (float)(spread) / results.last();
    name += " - " + QString::number(spread) + "(" +  QString::number(spreadPercentage) + "\%) spread";

    // Set the series name, marker
    scatterSeries->setName(name);
    scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    scatterSeries->setMarkerSize(10);

    // Add our items from the sorted list to the scatterseries
    for (int i = 0; i < results.size(); i++) {
        scatterSeries->append(i, results[i]);
    }

    // -----------------------------------------------------------------------------------------
    // Write results to file
    // -----------------------------------------------------------------------------------------
    QString filename = "TestResults_" + configFile.split(".").at(0) + ".txt";
    QFile file(filename);

    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << name << endl;
        for (int i = 0; i < results.size(); i++) {
            stream << QString::number(results[i]) << endl;
        }
    }

    file.close();

    // -----------------------------------------------------------------------------------------
    // Create new window for graph
    // -----------------------------------------------------------------------------------------
    TestingScatterGraph *wdg = new TestingScatterGraph;
    wdg->createGraph(scatterSeries);
    wdg->show();
}
