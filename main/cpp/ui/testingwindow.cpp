#include "testingwindow.h"
#include "ui_testingwindow.h"
#include "sim/SimRunner.h"
#include "sim/Simulator.h"
#include "pop/Population.h"
#include "util/FileSys.h"
#include "util/ConfigInfo.h"
#include "util/LogUtils.h"

#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QCheckBox>
#include <boost/property_tree/xml_parser.hpp>

using namespace std;
using namespace stride;
using namespace stride::util;
using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;

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

    // Process name and arguments
    QString configFile = ui->configInput->text();
    int rngSeed = ui->seedInput->value();
    QString rngType = ui->engineInput->currentText();
    bool printResults = ui->printInput->isChecked();

    int runs = ui->runsInput->value();

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

        if (printResults) {
            cout << "Test Instance " << i + 1 << "/" << runs << " completed. " << endl;
            cout << "Infected: " << runner->GetSim()->GetPopulation()->GetInfectedCount() << endl;
        }

        // -----------------------------------------------------------------------------------------
        // Done.
        // -----------------------------------------------------------------------------------------
        spdlog::drop_all();
        running = false;s

        // Respond to events so OS doesn't think the program is unresponsive
        QCoreApplication::processEvents();
    }

    if (printResults) {
        cout << "Tests completed!" << endl;
    }

    ui->startButton->setText("Start Tests");
}
