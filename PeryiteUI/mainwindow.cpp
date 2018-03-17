#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geogenvisualization.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QProcess>
#include<iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    data(new PeryiteUIData())
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Main functions (non-slots)
 */

void MainWindow::setGeoGenFile(QString filename)
{
    ui->geoGenFileLabel->setText(filename);
    data->geoGenFile = filename;
}

void MainWindow::setConfigFile(QString filename)
{
    ui->configFileLabel->setText(filename);
    data->configFile = filename;
}

/*
 * Slot functions
 */

void MainWindow::on_geoGenFileSelect_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Select the GeoGen file."),
                ".",
                "CSV Files (*.csv)");

    if (filename == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a GeoGen file.");
        return;
    }

    setGeoGenFile(filename);
}

void MainWindow::on_configFileSelect_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Select the config file."),
                ".",
                "XML Files (*.xml)");

    if (filename == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a config file.");
        return;
    }

    setConfigFile(filename);
}

void MainWindow::on_generateGeoGen_clicked()
{
    if (data->configFile == "") {
        QMessageBox::warning(this, tr("No file selected"), "You have not selected a config file.");
        return;
    }

    // Process name and arguments
    QStringList arguments;
    arguments << "output.txt";

    // Starting the process and waiting for it to finish
    QProcess *process = new QProcess(this);
    process->start("touch", arguments);
    process->waitForFinished(); // Default waits max 30s, TODO: see if that's enough.

    // Message when done
    QMessageBox::information(this, tr("Done"), "GeoGen completed!");

    setGeoGenFile("GEOGEN FILE HERE");
}

void MainWindow::on_visualizeGeoGen_clicked()
{
    GeoGenVisualization *wdg = new GeoGenVisualization;
    wdg->show();
}
