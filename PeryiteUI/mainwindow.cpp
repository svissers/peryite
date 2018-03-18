#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geogenvisualization.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QProcess>
#include<iostream>
#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    data(new PeryiteUIData())
{
    ui->setupUi(this);

    // Background color
    setStyleSheet("QMainWindow { background-color: #f7f7f7; }");
    ui->GeoGenGroup->setStyleSheet("QLabel { background-color: #626a77; color: #ffffff; border-radius: 2px; border: 1px solid black; }");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Main functions (non-slots)
 */

void MainWindow::setGeoGenFolder(QString filename)
{
    ui->geoGenFileLabel->setText(filename);
    data->geoGenFolder = filename;
    data->geogenData->setFilenames(data->geoGenFolder);

    /*
     * We can check if the geogen folder actually contains the required files, by letting setFilenames return a bool.
     * We could then give a MessageBox::warning here
     */
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
    QString filename = QFileDialog::getExistingDirectory(
                this,
                tr("Select the GeoGen output folder."),
                "../../cmake-build-release/installed/output",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (filename == "") {
        QMessageBox::warning(this, tr("No folder selected"), "You have not selected a GeoGen output folder.");
        return;
    }

    setGeoGenFolder(filename);
}

void MainWindow::on_configFileSelect_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
                this,
                tr("Select the config file."),
                "../../cmake-build-release/installed/config",
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
    QStringList path = data->configFile.split(QRegularExpression("/"));
    QString configFile = path.last();
    QStringList arguments;
    arguments << "../GeoGen.sh" << configFile;

    // Starting the process and waiting for it to finish
    QProcess *process = new QProcess(this);
    process->start("/bin/bash", arguments);
    process->waitForFinished(); // Default waits max 30s, TODO: see if that's enough.

    // Construct the path of the output folder
    QString installedFolder = "";

    path.pop_front(); // First is an empty string because string starts with "/"

    while (path.length() > 2) {
        QString folder = path.first();
        installedFolder += "/" + folder;
        path.pop_front();
    }

    // Set folder data and update geogendata
    setGeoGenFolder(installedFolder + "/output/" + configFile.split(QRegularExpression("\\.")).first() + "/");

    // Message when done
    QMessageBox::information(this, tr("Done"), "GeoGen completed!");
}

void MainWindow::on_visualizeGeoGen_clicked()
{
    GeoGenVisualization *wdg = new GeoGenVisualization;
    wdg->show();
}
