#include "testingwindow.h"
#include "ui_testingwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QCheckBox>

TestingWindow::TestingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestingWindow)
{
    ui->setupUi(this);
    ui->configInput->setText("run_config_default.xml");
    ui->engineInput->addItem("lcg64");
    ui->engineInput->addItem("lcg64_shift");
    ui->engineInput->addItem("mrg2");
    ui->engineInput->addItem("mrg3");
    ui->engineInput->addItem("yarn2");
    ui->engineInput->addItem("yarn3");
}

TestingWindow::~TestingWindow()
{
    delete ui;
}

void TestingWindow::on_startButton_clicked()
{
    if (ui->configInput->text() == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a config file.");
        return;
    }

    // Process name and arguments
    QString configFile = ui->configInput->text();
    QString rngSeed = ui->seedInput->text();
    QString rngType = ui->engineInput->currentText();

    int runs = ui->runsInput->value();

    for (int i = 0; i < runs; i++) {
        // Get progress
        ui->startButton->setText(QString::number(i) + " / " + QString::number(runs));
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
            rngSeed = QString::number(qrand());
        }

        QStringList arguments;
        arguments << "../StrideTest.sh" << configFile << rngSeed << rngType;

        // Starting the process and waiting for it to finish
        QProcess *process = new QProcess(this);
        process->start("/bin/bash", arguments);
        process->waitForFinished(180000); // Max 3 mins per case
    }

    ui->startButton->setText("Start Tests");
}
